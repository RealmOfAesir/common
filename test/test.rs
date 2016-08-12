#![feature(custom_derive, plugin)]
#![plugin(serde_macros)]

extern crate serde;
extern crate serde_json;
extern crate common;

use common::{serialize_message, deserialize_message, Message};

#[cfg(not(test))]
fn main() {
    println!("If you see this, the tests were not compiled nor ran!");
}

#[test]
fn serialization_test() {
    let msg: Message = Message { msg_type: common::Type::Login, version: 0, content: "test".to_string() };
    let msg_new: Message = deserialize_message(&serialize_message(&msg).unwrap()).unwrap();
    assert_eq!(msg_new, msg);
}

#[test]
#[should_panic]
fn error_test() {
    let msg_new: Message = deserialize_message("asdkjhaskjd").unwrap();
}
