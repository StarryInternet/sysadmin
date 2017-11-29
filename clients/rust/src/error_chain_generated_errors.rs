use std;
use protobuf::ProtobufError;

error_chain! {
    types {
        Error, ErrorKind, ResultExt, Result;
    }

    foreign_links {
        ProtobufError(ProtobufError);
        Io(std::io::Error);

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
