use super::*;
use std::time::Duration;

#[test]
fn test_str() {
    test_sysadminvalue(123_i32);
}
#[test]
fn test_i32() {
    test_sysadminvalue(String::from("123"));
}
#[test]
fn test_bool() {
    test_sysadminvalue(true);
}
#[test]
fn test_i32_list() {
    test_sysadminvalue(vec![1_i32, 2_i32, 3_i32]);
}
#[test]
fn test_str_list() {
    test_sysadminvalue(vec![
        String::from("1"),
        String::from("2"),
        String::from("3"),
    ]);
}
#[test]
fn test_bool_list() {
    test_sysadminvalue(vec![true, false, true]);
}

fn test_sysadminvalue<T: Into<SysadminValue>>(variant: T) {
    let sysadminvalue: SysadminValue = variant.into();
    let sv_for_convert = sysadminvalue.clone();
    let configvalue = sysadminctl::ConfigValue::from(sv_for_convert);
    let restored_sv = SysadminValue::from(configvalue);
    assert_eq!(sysadminvalue, restored_sv);
}

#[test]
fn test_set_str() {
    test_set_to_sysadminctl(123_i32);
}
#[test]
fn test_set_i32() {
    test_set_to_sysadminctl(String::from("123"));
}
#[test]
fn test_set_bool() {
    test_set_to_sysadminctl(true);
}
#[test]
fn test_set_i32_list() {
    test_set_to_sysadminctl(vec![1_i32, 2_i32, 3_i32]);
}
#[test]
fn test_set_str_list() {
    test_set_to_sysadminctl(vec![
        String::from("1"),
        String::from("2"),
        String::from("3"),
    ]);
}
#[test]
fn test_set_bool_list() {
    test_set_to_sysadminctl(vec![true, false, true]);
}

fn test_set_to_sysadminctl<T: Into<SysadminValue>>(v: T) {
    use std::clone::Clone;
    let k = String::from("key");
    let sysadminvalue: SysadminValue = v.into();
    let set = Set {
        key: k.clone(),
        value: SysadminValue::from(sysadminvalue.clone())
    };
    let mut buf: sysadminctl::Set = set.into_buf();
    assert_eq!(buf.is_initialized(), true);
    let buf_val = buf.take_value();
    // see if configvalue matches
    assert_eq!(buf_val, sysadminctl::ConfigValue::from(sysadminvalue));
}

#[test]
fn test_client_set_get() {
    let mut client = SysadminClient::new(Duration::from_secs(2_u64), 1_u32, 1_u32);
    let e = client.connect("127.0.0.1:9999").unwrap_err();
    assert_eq!(e.to_string(), "failed to connect to sysadmin");
    assert_eq!(client.get_xid(), &1_u32);
    assert_eq!(client.get_id(), &1_u32);
    client.set_xid(2_u32);
    client.set_id(2_u32);
    assert_eq!(client.get_xid(), &2_u32);
    assert_eq!(client.get_id(), &2_u32);
}

#[test]
fn test_send_command() {
    let mut client = SysadminClient::default();
    let set_struct = Set::new("bar", 3);
    let get_struct = Get::new("foo");
    let commit_struct = Commit::new(CommitConfig::NO_HOOKS);
    let error_string = "Sysadmin Connection Error: Command issued before connection was init";
    assert_eq!(
        set_struct
            .send_command(&mut client)
            .unwrap_err()
            .to_string(),
        error_string
    );
    assert_eq!(
        get_struct
            .send_command(&mut client)
            .unwrap_err()
            .to_string(),
        error_string
    );
    assert_eq!(
        commit_struct
            .send_command(&mut client)
            .unwrap_err()
            .to_string(),
        error_string
    );
}

#[test]
fn test_ser_and_deser() {
    let s_struct = Set::new("bar", vec![true, false]);

    let des = serde_json::to_string(&s_struct).unwrap();
    let ser: Set = serde_json::from_str(&des).unwrap();
    assert_eq!(s_struct, ser);

    let s_struct = Get::new("bar");

    let des = serde_json::to_string(&s_struct).unwrap();
    let ser: Get = serde_json::from_str(&des).unwrap();
    assert_eq!(s_struct, ser);

    let s_struct = Commit::new(CommitConfig::NO_HOOKS);

    let des = serde_json::to_string(&s_struct).unwrap();
    let ser: Commit = serde_json::from_str(&des).unwrap();
    assert_eq!(s_struct, ser);
}

macro_rules! test_into_buf {
    ($name:ident, $buf_type:ty, $sysadmin_type:ty) => {
        #[test]
        fn $name() {
            let sys_type: $sysadmin_type = ::std::default::Default::default();
            let pbuf: $buf_type = ::std::default::Default::default();
            let sys_into_buf = sys_type.into_buf();
            assert_eq!(sys_into_buf, pbuf);
        }
    };
}
test_into_buf!(test_no_arg_into_buf_drop, sysadminctl::Drop, Drop);
test_into_buf!(
    test_no_arg_into_buf_firehooks,
    sysadminctl::FireHooks,
    FireHooks
);
test_into_buf!(test_no_arg_into_buf_reset, sysadminctl::Reset, Reset);
test_into_buf!(
    test_no_arg_into_buf_dumphooks,
    sysadminctl::DumpHooks,
    DumpHooks
);
test_into_buf!(
    test_no_arg_into_buf_inflight,
    sysadminctl::InFlight,
    InFlight
);
