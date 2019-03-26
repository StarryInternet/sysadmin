extern crate failure;
extern crate protobuf;
extern crate sysadmin_bindings;

use sysadmin_bindings::{Commit, CommitConfig, Drop, DumpHooks, Get, Set, SysadminClient};

// Available Commands:
//     Set
//     Get
//     Commit
//     Drop
//     FireHooks
//     EraseKey
//     Rollback
//     Reset
//     DumpHooks
//     TriggerHook
//     Blame
//     InFlight

fn main() {
    // Build the sysadmin server:
    //      ./docker_control.sh -t
    // and run it with:
    //      ./docker_control.sh -s

    ::std::process::exit(match run() {
        Ok(_) => 0,
        Err(err) => {
            println!("{}", err);

            1
        }
    });
}

fn run() -> Result<(), failure::Error> {
    let mut client = SysadminClient::default();

    // init connection
    // if the connection fails we will report an error and exit
    client.connect("127.0.0.1:4000")?;

    // create command-specific struct which implement
    // a method for sending itself via a borrowed client
    // this returns a "response" struct
    let set_struct = Set::new("foo", 3_i32);
    let set_resp = set_struct.send_command(&mut client)?;
    println!("{:?}", set_resp);

    // until you commit changes, the 'foo' key won't be found
    let commit_struct = Commit::default();
    let com_resp = commit_struct.send_command(&mut client)?;
    assert_eq!(com_resp.status, sysadmin_bindings::StatusCode::SUCCESS);
    println!("{:?}", com_resp);

    let set_struct = Set::new("bar", 3);
    let bar_set_response = set_struct.send_command(&mut client)?;
    println!("{:?}", bar_set_response);

    // single-line send
    let drop_res = Drop::default().send_command(&mut client)?;
    println!("DROP RESULT {:?}", drop_res);

    // call commit without hooks
    let com_resp = Commit::new(CommitConfig::NO_HOOKS).send_command(&mut client)?;
    assert_eq!(com_resp.status, sysadmin_bindings::StatusCode::SUCCESS);
    println!("{:?}", com_resp);

    let get_resp = Get::new("foo").send_command(&mut client)?;
    println!("GetResponse.id {:?}", get_resp.id);
    println!("GetResponse.status {:?}", get_resp.status);
    get_resp.kvs.into_iter().for_each(|k| {
        println!(
            "GetResponse.kvs key={:?} value={:?}",
            k.key,
            k.value.unwrap()
        )
    });

    // this value was dropped before it was commit so it should not be found
    let get_bar_resp = Get::new("bar").send_command(&mut client)?;
    assert_eq!(
        get_bar_resp.status,
        sysadmin_bindings::StatusCode::KEY_NOT_FOUND
    );

    // it's possible to see hooks that have been set
    let dumphooks = DumpHooks::new().send_command(&mut client)?;
    println!("DUMPHOOKS RESULTS {:?}", dumphooks);

    // you are able to change xid and id, but it's usually not necessary
    client.set_xid(0_u32);

    Ok(())
}
