#![feature(custom_derive, plugin)]
#![plugin(serde_macros)]

extern crate serde;
extern crate serde_json;

use serde::{Deserialize, Serialize};
use serde_json::Error;

#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub enum Type {
    Register,
    Login
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct Message {
    pub msg_type: Type,
    pub version: i32,
    pub content: String
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct RegisterV1 {
    pub username: String,
    pub password: String
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct LoginV1 {
    pub username: String,
    pub password: String
}

pub fn deserialize_message<'a, T: Deserialize>(msg: &'a str) -> Result<T, Error> {
    serde_json::from_str(msg)
}

pub fn serialize_message<'a, T: Serialize>(data: T) -> Result<String, Error> {
    serde_json::to_string(&data)
}
