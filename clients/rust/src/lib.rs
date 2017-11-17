extern crate protobuf;
extern crate bufstream;
extern crate byteorder;
extern crate bytes;
pub mod sysadminctl;

use std::net::{TcpStream, SocketAddr};
use std::time::Duration;
use protobuf::Message;
use byteorder::{WriteBytesExt, LittleEndian};



pub struct SysAdminClient {
    addr: SocketAddr,
    timeout: Duration,
    xid: u32,
    id: u32,
    stream: Option<TcpStream>,
}


impl SysAdminClient {
    pub fn new(addr: SocketAddr, timeout: Duration, xid: u32, id: u32) -> SysAdminClient {
        SysAdminClient {
            addr: addr,
            timeout: timeout,
            xid: xid,
            id: id,
            stream: None,
        }
    }

    /// Init the connection. This happens when send it called but the connection
    /// persists.
    fn init_stream(&mut self) -> Result<(), protobuf::error::ProtobufError> {

        if self.stream.is_none() {
            let stream = TcpStream::connect(self.addr).map_err(|e| {
                protobuf::error::ProtobufError::from(e)
            })?;
            stream.set_write_timeout(Some(self.timeout.clone()))?;
            stream.set_read_timeout(Some(self.timeout.clone()))?;
            self.stream = Some(stream);
        }
        Ok(())
    }

    /// Makes the Command which will wrap the payload (e.g. Set or Commit)
    /// xid and id are inserted automatically.
    fn make_command(&self) -> sysadminctl::Command {
        let mut cmd = sysadminctl::Command::new();
        cmd.set_xid(self.xid.clone());
        cmd.set_id(self.id.clone());
        cmd
    }

    /// send the command via tcp
    fn send(
        &mut self,
        command: sysadminctl::Command,
    ) -> Result<sysadminctl::Response, protobuf::error::ProtobufError> {
        self.init_stream()?;
        assert!(command.is_initialized() == true);
        use std::io::Write;


        let size = command.compute_size();
        let bytes = command.write_to_bytes()?;
        assert!(size == bytes.len() as u32);

        // get BigEndian vec from u32
        let mut wtr = vec![];
        wtr.write_u32::<LittleEndian>(size).unwrap();

        {
            // write to socket
            let stream = self.stream.as_mut().unwrap();
            stream.write_all(&wtr)?;
            stream.write_all(&bytes)?;
            stream.flush()?;
        }

        let mut stream = self.stream.as_mut().unwrap();
        // receive response
        let mut cis = protobuf::CodedInputStream::new(&mut stream);
        let resp_size = cis.read_raw_little_endian32()?;
        let response_bytes = cis.read_raw_bytes(resp_size)?;
        let resp = protobuf::parse_from_bytes::<sysadminctl::Response>(&response_bytes);
        resp
    }


    #[allow(dead_code)]
    pub fn set_xid(mut self, xid: u32) -> Self {
        self.xid = xid;
        self
    }

    #[allow(dead_code)]
    pub fn set_id(mut self, id: u32) -> Self {
        self.id = id;
        self
    }

    /// Wraps any payload type in a Command and sends it via tcp
    pub fn sender<T: Payload>(
        &mut self,
        msg: T,
    ) -> Result<sysadminctl::Response, protobuf::error::ProtobufError> {
        let mut cmd = self.make_command();
        msg.set_payload(&mut cmd);
        self.send(cmd)
    }
}

pub trait Payload {
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





#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
