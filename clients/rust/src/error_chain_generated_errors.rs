use std;
use protobuf;
use serde_json;

error_chain! {
    types {
        Error, ErrorKind, ResultExt, Result;
    }

    foreign_links {
        ProtobufError(protobuf::ProtobufError);
        Io(std::io::Error);
        JsonSerde(serde_json::error::Error);
    }

    errors {
        SysadminConnectionError(t: String) {
            description("failed during connection attempt")
            display("Network Error: {:?}", t)
        }
        SysadminProtocolError(t: String) {
            description("Error during serialization")
            display("Sysadmin Protocol Error: {:?}", t)
        }
    }
}
