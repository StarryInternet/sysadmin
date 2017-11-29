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
        value: SysadminValue::from(sysadminvalue.clone()),
    };
    let mut buf: sysadminctl::Set = set.into_buf();
    assert_eq!(buf.is_initialized(), true);
    let buf_val = buf.take_value();
    // see if configvalue matches
    assert_eq!(buf_val, sysadminctl::ConfigValue::from(sysadminvalue));
}

#[test]
fn test_client_set_get() {
    let mut client = SysAdminClient::new(Duration::from_secs(2_u64), 1_u32, 1_u32);
    let e = client.connect("127.0.0.1:9999").unwrap_err();
    assert_eq!(e.description(), "failed during connection attempt");
    assert_eq!(client.get_xid(), &1_u32);
    assert_eq!(client.get_id(), &1_u32);
    client.set_xid(2_u32);
    client.set_id(2_u32);
    assert_eq!(client.get_xid(), &2_u32);
    assert_eq!(client.get_id(), &2_u32);
}
