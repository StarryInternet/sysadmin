//Copyright Starry Inc. 2018. All Rights Reserved.
//! Various `Error` types and `Error` related utilities.

/// `Error` type, using failure, used throughout rust client
pub type SysadminError = ::failure::Error;

/// `Result` type used throughout rust client
pub type SysadminResult<T> = ::std::result::Result<T, SysadminError>;

#[derive(Fail, Debug)]
pub enum SysadminErrorKind {
    #[fail(display = "Network Error: {}", _0)]
    SysadminConnectionError(String),
    #[fail(display = "Sysadmin Protocol Error: {}", _0)]
    SysadminProtocolError(String),
}
