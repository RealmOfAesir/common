#![feature(custom_derive, plugin)]
#![plugin(serde_macros)]

extern crate serde;
extern crate serde_json;

use serde::Deserialize;

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

pub fn deserialize_message<'a, T: Deserialize>(msg: &'a str) -> T {
    serde_json::from_str(msg).unwrap()
}
