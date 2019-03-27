#[macro_use]
extern crate failure;
#[macro_use]
extern crate serde_derive;
extern crate bufstream;
extern crate byteorder;
extern crate bytes;
extern crate protobuf;
extern crate serde;
extern crate serde_json;

mod sysadminctl;

#[cfg(test)]
mod tests;

#[macro_use]
pub mod errors;

use std::convert::{From, Into};
use std::default::Default;
use std::net::{TcpStream, ToSocketAddrs};
use std::string::String;
use std::time::Duration;
use std::vec::Vec;

use byteorder::{LittleEndian, WriteBytesExt};
use protobuf::repeated::RepeatedField;
use protobuf::Message;

use errors::{SysadminErrorKind, SysadminResult};
use failure::ResultExt;

/// Lifted `Result` type used throughout this crate.
pub type Result<T> = errors::SysadminResult<T>;
/// Lifted `Error` type used throughout this crate.
pub type Error = errors::SysadminError;

const DEFAULT_COMMAND_TIMEOUT: u64 = 10;

/// SysadminClient manages the connection and provides methods
/// for sending specific commands. Each command returns a response in
/// the form of a struct specific to that command.
/// E.g. "SysadminClient.get" returns "GetResponse"
///
/// Note that the response structs used by the sysadmin lib are a close,
/// but not exact, mappings of the protobut defined responses.
/// For example the "id" and "status" from Response" are merged into
/// "[Get|Set|Whatever]Response" structs.
/// Errors are returned as ::failure::errors.

pub struct SysadminClient {
    timeout: Duration,
    xid: u32,
    id: u32,
    stream: Option<TcpStream>
}

impl SysadminClient {
    pub fn new(timeout: Duration, xid: u32, id: u32) -> SysadminClient {
        SysadminClient {
            timeout: timeout,
            xid: xid,
            id: id,
            stream: None
        }
    }

    /// Init the connection. You can call this method again to init
    /// a new connection.
    /// "address" will become a SocketAddr using the trait
    /// `[ToSocketAddrs]`: https://doc.rust-lang.org/std/net/trait.ToSocketAddrs.html
    pub fn connect<A: ToSocketAddrs>(&mut self, address: A) -> SysadminResult<()> {
        let stream = TcpStream::connect(address).context("failed to connect to sysadmin")?;
        stream
            .set_write_timeout(Some(self.timeout.clone()))
            .context("error setting write timeout")?;
        stream
            .set_read_timeout(Some(self.timeout.clone()))
            .context("error setting read timeout")?;
        self.stream = Some(stream);

        Ok(())
    }

    /// Set TCP stream timeout
    pub fn set_timeout(&mut self, timeout: Duration) -> SysadminResult<()> {
        if self.stream.is_none() {
            bail!(SysadminErrorKind::SysadminConnectionError(
                "Command issues before connection was initialized".to_owned()
            ));
        }

        self.timeout = timeout;

        if let Some(stream) = self.stream.as_mut() {
            stream
                .set_write_timeout(Some(self.timeout.clone()))
                .context("Error setting write timeout")?;
            stream
                .set_read_timeout(Some(self.timeout.clone()))
                .context("Error setting read timeout")?;
        }

        Ok(())
    }

    /// Reset TCP stream timeout to default (10 seconds).
    pub fn reset_timeout_to_default(&mut self) -> SysadminResult<()> {
        self.set_timeout(Duration::from_secs(DEFAULT_COMMAND_TIMEOUT))
    }

    /// Makes the Command which will wrap the payload (e.g. Set or Commit)
    /// xid and id are inserted automatically.
    fn make_command(&self) -> sysadminctl::Command {
        let mut cmd = sysadminctl::Command::new();
        cmd.set_xid(self.xid);
        cmd.set_id(self.id);
        cmd
    }

    /// send the command via tcp and get response
    fn send(&mut self, command: sysadminctl::Command) -> SysadminResult<sysadminctl::Response> {
        if self.stream.is_none() {
            bail!(SysadminErrorKind::SysadminConnectionError(
                "Command issued before connection was init".to_string(),
            ))
        }
        assert!(command.is_initialized() == true);
        use std::io::Write;

        let size = command.compute_size();
        let bytes = command.write_to_bytes()?;
        assert!(size == bytes.len() as u32);

        // get BigEndian vec from u32
        let mut wtr = vec![];
        wtr.write_u32::<LittleEndian>(size).unwrap();

        // write to socket
        let mut stream = self.stream.as_mut().unwrap();
        stream.write_all(&wtr)?;
        stream.write_all(&bytes)?;
        stream.flush()?;

        // XXX(KCS): TcpStream doesn't have a good way of checking for timeouts.
        // Attempt to peek at response data. If there's an error, command may have timed out.
        let mut buf = [0; 10];
        match stream.peek(&mut buf) {
            Err(e) => bail!(SysadminErrorKind::SysadminConnectionError(format!(
                "Unable to read response, command may have timed out: {}",
                e
            ))),
            _ => ()
        };

        // receive response
        let mut cis = protobuf::CodedInputStream::new(&mut stream);
        let resp_size = cis
            .read_raw_little_endian32()
            .context("error reading message size")?;
        let response_bytes = cis
            .read_raw_bytes(resp_size)
            .context("error reading response from sysadmin")?;
        let resp = protobuf::parse_from_bytes::<sysadminctl::Response>(&response_bytes)
            .context("error parsing response from sysadmin")?;
        Ok(resp)
    }

    pub fn set_xid(&mut self, xid: u32) {
        self.xid = xid;
    }

    pub fn set_id(&mut self, id: u32) {
        self.id = id;
    }

    pub fn get_xid(&self) -> &u32 {
        &self.xid
    }

    pub fn get_id(&self) -> &u32 {
        &self.id
    }

    /// Wraps any payload type in a Command and requests it via tcp
    fn request<T: Payload>(&mut self, msg: T) -> SysadminResult<sysadminctl::Response> {
        let mut cmd = self.make_command();
        msg.set_payload(&mut cmd);
        self.send(cmd)
    }
}

impl Default for SysadminClient {
    fn default() -> SysadminClient {
        SysadminClient::new(Duration::from_secs(DEFAULT_COMMAND_TIMEOUT), 1_u32, 1_u32)
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct kvs {
    pub key: String,
    pub value: Option<SysadminValue>
}

impl From<sysadminctl::MappedField> for kvs {
    fn from(mut m: sysadminctl::MappedField) -> kvs {
        let value = {
            if m.has_value() {
                Some(SysadminValue::from(m.take_value()))
            } else {
                None
            }
        };
        kvs {
            key: m.take_key(),
            value: value
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct GetResponse {
    pub id: u32,
    pub status: StatusCode,
    pub kvs: Vec<kvs>
}

impl From<sysadminctl::Response> for GetResponse {
    fn from(mut r: sysadminctl::Response) -> GetResponse {
        let mut get_resp = r.take_get();
        let ctl_vec = get_resp.take_kvs().to_vec();
        let mut kvs_vec: Vec<kvs> = Vec::new();
        ctl_vec.into_iter().for_each(|a| kvs_vec.push(kvs::from(a)));

        GetResponse {
            id: r.get_id(),
            status: r.get_status().into(),
            kvs: kvs_vec
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CommitResponse {
    pub id: u32,
    pub status: StatusCode,
    pub commit_id: Option<u32>
}

impl From<sysadminctl::Response> for CommitResponse {
    fn from(response: sysadminctl::Response) -> CommitResponse {
        let commit_resp = response.get_commit();
        let cid = {
            if commit_resp.has_commit_id() {
                Some(commit_resp.get_commit_id())
            } else {
                None
            }
        };

        CommitResponse {
            id: response.get_id(),
            status: response.get_status().into(),
            commit_id: cid
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Set {
    key: String,
    value: SysadminValue
}

impl Set {
    pub fn new<S: Into<String>, T: Into<SysadminValue>>(k: S, v: T) -> Set {
        Set {
            key: k.into(),
            value: v.into()
        }
    }
}

impl Set {
    fn into_buf(self) -> sysadminctl::Set {
        let mut set = sysadminctl::Set::new();
        set.set_key(self.key);
        let sv: SysadminValue = self.value;
        let cv: sysadminctl::ConfigValue = sysadminctl::ConfigValue::from(sv);
        set.set_value(cv);
        set
    }

    pub fn send_command(self, client: &mut SysadminClient) -> SysadminResult<GenericResponse> {
        let resp = client.request(self.into_buf())?;
        Ok(resp.into())
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Commit {
    #[serde(default)]
    config: CommitConfig
}

impl Commit {
    pub fn new(config: CommitConfig) -> Commit {
        Commit { config: config }
    }

    fn into_buf(self) -> sysadminctl::Commit {
        let mut commit = sysadminctl::Commit::new();
        let commitconfig = sysadminctl::CommitConfig::from(self.config);
        commit.set_config(commitconfig);
        commit
    }

    pub fn send_command(self, client: &mut SysadminClient) -> SysadminResult<CommitResponse> {
        let resp = client.request(self.into_buf())?;
        Ok(resp.into())
    }
}

impl Default for Commit {
    fn default() -> Commit {
        Commit::new(CommitConfig::default())
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum CommitConfig {
    DEFAULT = 0,
    TEMPLATE_ONLY = 1,
    NO_HOOKS = 3
}

impl Default for CommitConfig {
    fn default() -> CommitConfig {
        CommitConfig::DEFAULT
    }
}

impl From<CommitConfig> for sysadminctl::CommitConfig {
    fn from(c: CommitConfig) -> sysadminctl::CommitConfig {
        match c {
            CommitConfig::DEFAULT => sysadminctl::CommitConfig::DEFAULT,
            CommitConfig::TEMPLATE_ONLY => sysadminctl::CommitConfig::TEMPLATE_ONLY,
            CommitConfig::NO_HOOKS => sysadminctl::CommitConfig::NO_HOOKS
        }
    }
}

/// Constructs a command that takes no values.
/// The response type is also a param
macro_rules! no_arg_command {
    ($name:ident, $buf_type:ty, $return_type:ty) => {
        #[derive(Debug, Clone, PartialEq, Deserialize, Serialize)]
        pub struct $name {}

        impl $name {
            pub fn new() -> $name {
                $name {}
            }

            fn into_buf(self) -> $buf_type {
                let buf: $buf_type = ::std::default::Default::default();
                buf
            }

            pub fn send_command(
                self,
                client: &mut $crate::SysadminClient
            ) -> SysadminResult<$return_type> {
                let resp = client.request(self.into_buf())?;
                Ok(resp.into())
            }
        }
        impl Default for $name {
            fn default() -> $name {
                $name::new()
            }
        }
    };
}
no_arg_command!(Drop, sysadminctl::Drop, GenericResponse);
no_arg_command!(FireHooks, sysadminctl::FireHooks, GenericResponse);
no_arg_command!(Reset, sysadminctl::Reset, ResetResponse);
no_arg_command!(DumpHooks, sysadminctl::DumpHooks, DumpResponse);
no_arg_command!(InFlight, sysadminctl::InFlight, InFlightResponse);

/// constructs a cmd struct that takes a key and value.
/// The name of the key and type of the value are params.
/// The response type is also a param
macro_rules! single_arg_command {
    ($name:ident, $keyname:ident, $val_type:ty, $buf_type:ty, $set_cmd:ident, $return_type:ty ) => {
        #[derive(Debug, Clone, PartialEq, Deserialize, Serialize)]
        pub struct $name {
            $keyname: $val_type
        }

        impl $name {
            pub fn new<S: Into<$val_type>>(s: S) -> $name {
                $name { $keyname: s.into() }
            }

            fn into_buf(self) -> $buf_type {
                let mut buf: $buf_type = ::std::default::Default::default();
                buf.$set_cmd(self.$keyname);
                buf
            }

            pub fn send_command(
                self,
                client: &mut $crate::SysadminClient
            ) -> SysadminResult<$return_type> {
                let resp = client.request(self.into_buf())?;
                Ok(resp.into())
            }
        }
    };
}
single_arg_command!(
    EraseKey,
    key,
    String,
    sysadminctl::EraseKey,
    set_key,
    GenericResponse
);
single_arg_command!(
    TriggerHook,
    hook,
    String,
    sysadminctl::TriggerHook,
    set_hook,
    GenericResponse
);
single_arg_command!(
    Blame,
    key,
    String,
    sysadminctl::Blame,
    set_key,
    BlameResponse
);
single_arg_command!(Get, key, String, sysadminctl::Get, set_key, GetResponse);
single_arg_command!(
    Rollback,
    id,
    u32,
    sysadminctl::Rollback,
    set_id,
    GenericResponse
);

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ResetResponse {
    pub id: u32,
    pub status: StatusCode,
    pub commit_id: u32
}

impl From<sysadminctl::Response> for ResetResponse {
    fn from(mut r: sysadminctl::Response) -> ResetResponse {
        let reset_response = r.take_reset();
        ResetResponse {
            id: r.get_id(),
            status: r.get_status().into(),
            commit_id: reset_response.get_commit_id()
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct DumpResponse {
    pub id: u32,
    pub status: StatusCode,
    pub templatehooks: Vec<String>,
    pub servicehooks: Vec<String>
}

impl From<sysadminctl::Response> for DumpResponse {
    fn from(mut r: sysadminctl::Response) -> DumpResponse {
        let mut dump_response = r.take_dump(); // lol
        let temphook_resp = dump_response.take_templatehooks();
        let tmpl_vec = temphook_resp.to_vec();
        let servicehooks_resp = dump_response.take_servicehooks();
        let serv_vec = servicehooks_resp.to_vec();

        DumpResponse {
            id: r.get_id(),
            status: r.get_status().into(),
            templatehooks: tmpl_vec,
            servicehooks: serv_vec
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct BlameEntry {
    pub commit_id: u32,
    pub commit_time: String,
    pub val: SysadminValue
}

impl From<sysadminctl::BlameEntry> for BlameEntry {
    fn from(mut r: sysadminctl::BlameEntry) -> BlameEntry {
        BlameEntry {
            commit_id: r.get_commit_id(),
            commit_time: r.take_commit_time(),
            val: r.take_val().into()
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct BlameResponse {
    pub id: u32,
    pub status: StatusCode,
    pub entries: Vec<BlameEntry>
}

impl From<sysadminctl::Response> for BlameResponse {
    fn from(mut r: sysadminctl::Response) -> BlameResponse {
        let mut blame_response = r.take_blame();
        let entries = blame_response.take_entries().to_vec();
        let entries_vec = entries.into_iter().map(BlameEntry::from).collect();
        BlameResponse {
            id: r.get_id(),
            status: r.get_status().into(),
            entries: entries_vec
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct InFlightResponse {
    pub id: u32,
    pub status: StatusCode,
    pub kvs: Vec<kvs>
}

impl From<sysadminctl::Response> for InFlightResponse {
    fn from(mut r: sysadminctl::Response) -> InFlightResponse {
        let mut get_resp = r.take_get();
        let ctl_vec = get_resp.take_kvs().to_vec();
        let kvs_vec = ctl_vec.into_iter().map(kvs::from).collect();

        InFlightResponse {
            id: r.get_id(),
            status: r.get_status().into(),
            kvs: kvs_vec
        }
    }
}

/// this is an id and status but no payload
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct GenericResponse {
    pub id: u32,
    pub status: StatusCode
}

impl From<sysadminctl::Response> for GenericResponse {
    fn from(r: sysadminctl::Response) -> GenericResponse {
        GenericResponse {
            id: r.get_id(),
            status: r.get_status().into()
        }
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum StatusCode {
    SUCCESS,
    UNKNOWN_ERROR,
    TYPE_MISMATCH,
    COMMAND_TRANSLATION_ERROR,
    KEY_NOT_FOUND,
    EXTERNAL_PROCESS_ERROR,
    INVALID_KEY,
    FAILED_ROLLBACK,
    LOCKED_QUEUE,
    HOOK_NOT_FOUND,
    SUCCESS_KEY_CREATED,
    MESSAGE_SIZE_ERROR
}

impl From<StatusCode> for sysadminctl::StatusCode {
    fn from(c: StatusCode) -> sysadminctl::StatusCode {
        match c {
            StatusCode::SUCCESS => sysadminctl::StatusCode::SUCCESS,
            StatusCode::UNKNOWN_ERROR => sysadminctl::StatusCode::UNKNOWN_ERROR,
            StatusCode::TYPE_MISMATCH => sysadminctl::StatusCode::TYPE_MISMATCH,
            StatusCode::COMMAND_TRANSLATION_ERROR => {
                sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR
            },
            StatusCode::KEY_NOT_FOUND => sysadminctl::StatusCode::KEY_NOT_FOUND,
            StatusCode::EXTERNAL_PROCESS_ERROR => sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR,
            StatusCode::INVALID_KEY => sysadminctl::StatusCode::INVALID_KEY,
            StatusCode::FAILED_ROLLBACK => sysadminctl::StatusCode::FAILED_ROLLBACK,
            StatusCode::LOCKED_QUEUE => sysadminctl::StatusCode::LOCKED_QUEUE,
            StatusCode::HOOK_NOT_FOUND => sysadminctl::StatusCode::HOOK_NOT_FOUND,
            StatusCode::SUCCESS_KEY_CREATED => sysadminctl::StatusCode::SUCCESS_KEY_CREATED,
            StatusCode::MESSAGE_SIZE_ERROR => sysadminctl::StatusCode::MESSAGE_SIZE_ERROR
        }
    }
}

impl From<sysadminctl::StatusCode> for StatusCode {
    fn from(c: sysadminctl::StatusCode) -> StatusCode {
        match c {
            sysadminctl::StatusCode::SUCCESS => StatusCode::SUCCESS,
            sysadminctl::StatusCode::UNKNOWN_ERROR => StatusCode::UNKNOWN_ERROR,
            sysadminctl::StatusCode::TYPE_MISMATCH => StatusCode::TYPE_MISMATCH,
            sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR => {
                StatusCode::COMMAND_TRANSLATION_ERROR
            },
            sysadminctl::StatusCode::KEY_NOT_FOUND => StatusCode::KEY_NOT_FOUND,
            sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR => StatusCode::EXTERNAL_PROCESS_ERROR,
            sysadminctl::StatusCode::INVALID_KEY => StatusCode::INVALID_KEY,
            sysadminctl::StatusCode::FAILED_ROLLBACK => StatusCode::FAILED_ROLLBACK,
            sysadminctl::StatusCode::LOCKED_QUEUE => StatusCode::LOCKED_QUEUE,
            sysadminctl::StatusCode::HOOK_NOT_FOUND => StatusCode::HOOK_NOT_FOUND,
            sysadminctl::StatusCode::SUCCESS_KEY_CREATED => StatusCode::SUCCESS_KEY_CREATED,
            sysadminctl::StatusCode::MESSAGE_SIZE_ERROR => StatusCode::MESSAGE_SIZE_ERROR
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
#[serde(untagged)]
pub enum SysadminValue {
    Int32(i32),
    Strval(String),
    Bool(bool),
    Int32List(Vec<i32>),
    BoolList(Vec<bool>),
    StrvalList(Vec<String>)
}

impl From<i32> for SysadminValue {
    fn from(v: i32) -> Self {
        SysadminValue::Int32(v)
    }
}

impl From<String> for SysadminValue {
    fn from(v: String) -> Self {
        SysadminValue::Strval(v)
    }
}

impl<'a> From<&'a str> for SysadminValue {
    fn from(v: &str) -> Self {
        SysadminValue::Strval(String::from(v))
    }
}

impl From<bool> for SysadminValue {
    fn from(v: bool) -> Self {
        SysadminValue::Bool(v)
    }
}
impl From<Vec<i32>> for SysadminValue {
    fn from(v: Vec<i32>) -> Self {
        SysadminValue::Int32List(v)
    }
}
impl From<Vec<bool>> for SysadminValue {
    fn from(v: Vec<bool>) -> Self {
        SysadminValue::BoolList(v)
    }
}
impl From<Vec<String>> for SysadminValue {
    fn from(v: Vec<String>) -> Self {
        SysadminValue::StrvalList(v)
    }
}

impl From<SysadminValue> for sysadminctl::ConfigValue {
    fn from(v: SysadminValue) -> sysadminctl::ConfigValue {
        let mut cv = sysadminctl::ConfigValue::new();
        match v {
            SysadminValue::Int32(x) => cv.set_int32val(x),
            SysadminValue::Strval(x) => cv.set_strval(x),
            SysadminValue::Bool(x) => cv.set_boolval(x),
            SysadminValue::Int32List(x) => {
                let mut list = sysadminctl::Int32List::new();
                list.set_list(x);
                cv.set_int32list(list);
            },
            SysadminValue::BoolList(x) => {
                let mut list = sysadminctl::BoolList::new();
                list.set_list(x);
                cv.set_boollist(list);
            },
            SysadminValue::StrvalList(x) => {
                let mut list = sysadminctl::StringList::new();
                list.set_list(RepeatedField::from_vec(x));
                cv.set_stringlist(list);
            }
        }
        cv
    }
}

impl From<sysadminctl::ConfigValue> for SysadminValue {
    fn from(mut cv: sysadminctl::ConfigValue) -> SysadminValue {
        let sysadminvalue = match cv {
            ref mut v if v.has_int32val() => SysadminValue::Int32(v.get_int32val()),
            ref mut v if v.has_strval() => SysadminValue::Strval(v.take_strval()),
            ref mut v if v.has_boolval() => SysadminValue::Bool(v.get_boolval()),
            ref mut v if v.has_int32list() => {
                let mut list = v.take_int32list();
                SysadminValue::Int32List(list.take_list())
            },
            ref mut v if v.has_boollist() => {
                let mut list = v.take_boollist();
                SysadminValue::BoolList(list.take_list())
            },
            ref mut v if v.has_stringlist() => {
                let repeating = v.take_stringlist().take_list();
                let list = RepeatedField::into_vec(repeating);
                SysadminValue::StrvalList(list)
            },
            // TODO send an error back instead
            ref mut v => panic!(format!(
                "{} {} {:?}",
                "Error converting to SysadminValue", "from sysadminctl::ConfigValue:", v
            ))
        };
        sysadminvalue
    }
}

trait Payload {
    /// set_payload adds the payload to a command
    /// using the proper command for each payload type
    fn set_payload(self, command: &mut sysadminctl::Command);
}

impl Payload for sysadminctl::Set {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_set(self);
    }
}

impl Payload for sysadminctl::Get {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_get(self);
    }
}

impl Payload for sysadminctl::Commit {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_commit(self);
    }
}

impl Payload for sysadminctl::Drop {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_drop(self);
    }
}

impl Payload for sysadminctl::FireHooks {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_firehooks(self);
    }
}

impl Payload for sysadminctl::EraseKey {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_erase(self);
    }
}

impl Payload for sysadminctl::Rollback {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_rollback(self);
    }
}

impl Payload for sysadminctl::Reset {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_reset(self);
    }
}

impl Payload for sysadminctl::DumpHooks {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_dump(self);
    }
}

impl Payload for sysadminctl::TriggerHook {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_trigger(self);
    }
}

impl Payload for sysadminctl::Blame {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_blame(self);
    }
}

impl Payload for sysadminctl::InFlight {
    fn set_payload(self, command: &mut sysadminctl::Command) {
        command.set_inflight(self);
    }
}
