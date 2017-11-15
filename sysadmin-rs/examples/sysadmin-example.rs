extern crate sysadmin;
extern crate protobuf;

use std::time::Duration;
use std::net::{IpAddr, Ipv4Addr, SocketAddr};
use sysadmin::sysadminctl::{ConfigValue, Set, Get, Commit, CommitConfig, Response};


/// Available Payloads:
///     Set
///     Get
///     Commit
///     Drop
///     FireHooks
///     EraseKey
///     Rollback
///     Reset
///     DumpHooks
///     TriggerHook
///     Blame
///     InFlight

fn make_client() -> sysadmin::SysAdminClient {
    let default_timeout = Duration::new(2, 0);
    let socket = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1)), 4000_u16);
    let sysadminclient = sysadmin::SysAdminClient::new(socket, default_timeout, 0_u32, 1_u32);
    sysadminclient
}

fn make_set() -> Set {
    let mut set = Set::new();
    set.set_key("fizz".to_string());
    let mut cv = ConfigValue::new();
    cv.set_int32val(123_i32);
    set.set_value(cv);
    set
}


fn make_commit() -> Commit {
    let mut commit = Commit::new();
    commit.set_config(CommitConfig::DEFAULT);
    commit
}

fn make_get() -> Get {
    let mut get = Get::new();
    get.set_key("fizz".to_string());
    get
}



fn print_resp(resp: Result<Response, protobuf::error::ProtobufError>) {
    match resp {
        Ok(r) => println!("Response = {:?}", r),
        Err(e) => println!("Err = {:?}", e),
    }
}

fn main() {
    // Build the sysadmin server and run it with:
    //      ./build/src/sysadmin test/configs/config.yaml


    let mut sysadminclient = make_client();
    let set = make_set();
    let commit = make_commit();
    let get = make_get();

    let set_resp: Result<Response, protobuf::error::ProtobufError> = sysadminclient.sender(set);
    print_resp(set_resp);

    let commit_resp = sysadminclient.sender(commit);
    print_resp(commit_resp);


    let get_resp = sysadminclient.sender(get);

    let ref kvs = get_resp.unwrap().take_get().take_kvs()[0];
    assert!(kvs.get_value().get_int32val() == 123_i32);

}
