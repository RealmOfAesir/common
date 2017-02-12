/*
    Realm of Aesir backend
    Copyright (C) 2016  Michael de Lang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <catch.hpp>
#include <iostream>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include "messages/message.h"
#include "messages/login_message.h"
#include "messages/login_response_message.h"
#include "exceptions.h"

using namespace std;
using namespace roa;

//TEST_SUITE("message serialization/deserialization");

TEST_CASE("serialize/deserialize login_message happy flow") {
    login_message<false> login_msg("user", "pass");
    auto serialized_message = login_msg.serialize();
    REQUIRE(serialized_message.length() > 0);
    auto deserialized_message = message<false>::deserialize(serialized_message);
    REQUIRE(deserialized_message != nullptr);
    auto new_message = dynamic_cast<login_message<false>*>(deserialized_message.get());
    REQUIRE(new_message != nullptr);
    REQUIRE(new_message->username == "user");
    REQUIRE(new_message->password == "pass");
}

TEST_CASE("deserialize garbage") {
    REQUIRE_THROWS_AS(message<false>::deserialize(""), serialization_exception&);
    REQUIRE_THROWS_AS(message<false>::deserialize("garbage"), serialization_exception&);
}

TEST_CASE("serialize/deserialize login_message errors") {
    stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive((uint32_t)LOGIN_MESSAGE_TYPE, string("garbage"));
    ss.flush();
    REQUIRE_THROWS_AS(message<false>::deserialize(ss.str()), cereal::Exception&);
}

TEST_CASE("serialize/deserialize login_response_message happy flow") {
    login_response_message<false> login_response_msg(1, "test");
    auto serialized_message = login_response_msg.serialize();
    REQUIRE(serialized_message.length() > 0);
    auto deserialized_message = message<false>::deserialize(serialized_message);
    REQUIRE(deserialized_message != nullptr);
    auto new_message = dynamic_cast<login_response_message<false>*>(deserialized_message.get());
    REQUIRE(new_message != nullptr);
    REQUIRE(new_message->error == 1);
    REQUIRE(new_message->error_str == "test");
}

TEST_CASE("serialize/deserialize login_response_message errors") {
    stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive((uint32_t)LOGIN_RESPONSE_MESSAGE_TYPE, string("garbage"));
    REQUIRE_THROWS_AS(message<false>::deserialize(ss.str()), bad_alloc&);
    ss.str("");
    ss.clear();
    archive((uint32_t)LOGIN_RESPONSE_MESSAGE_TYPE, (int)1);
    REQUIRE_THROWS_AS(message<false>::deserialize(ss.str()), cereal::Exception&);
}

//TEST_SUITE_END();