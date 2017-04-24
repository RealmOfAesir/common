/*
    Realm of Aesir
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
#include <easyloggingpp/src/easylogging++.h>
#include <messages/user_access_control/register_message.h>
#include <messages/user_access_control/register_response_message.h>
#include "messages/message.h"
#include "messages/user_access_control/login_message.h"
#include "messages/user_access_control/login_response_message.h"
#include "exceptions.h"
#include "test_helpers.h"

using namespace std;
using namespace roa;

TEST_CASE("serialize/deserialize login_message happy flow") {
    auto new_json_message = test_happy_flow<login_message, true, string, string>(json_login_message::id, string("user"), string("pass"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->username == "user");
    REQUIRE(new_json_message->password == "pass");

    auto new_binary_message = test_happy_flow<login_message, false, string, string>(binary_login_message::id, string("user"), string("pass"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->username == "user");
    REQUIRE(new_binary_message->password == "pass");
}

TEST_CASE("deserialize garbage") {
    REQUIRE_THROWS_AS(message<false>::template deserialize<false>(""), serialization_exception&);
    REQUIRE_THROWS_AS(message<false>::template deserialize<false>("garbage"), cereal::Exception&);
}

TEST_CASE("serialize/deserialize login_message errors") {
    {
        stringstream ss;
        cereal::BinaryOutputArchive archive(ss);
        archive((uint32_t) LOGIN_MESSAGE_TYPE, string("garbage"));
        ss.flush();
        REQUIRE_THROWS_AS(message<false>::template deserialize<false>(ss.str()), cereal::Exception &);
    }
    {
        stringstream ss;
        cereal::BinaryOutputArchive archive(ss);
        message_sender sender(true, 1, 2, 3);
        archive((uint32_t) LOGIN_MESSAGE_TYPE, sender, string("garbage"));
        ss.flush();
        REQUIRE_THROWS_AS(message<false>::template deserialize<false>(ss.str()), cereal::Exception &);
    }
}

TEST_CASE("serialize/deserialize login_response_message happy flow") {
    auto new_json_message = test_happy_flow<login_response_message, true, int, string>(json_login_response_message::id, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<login_response_message, false, int, string>(binary_login_response_message::id, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}

TEST_CASE("serialize/deserialize register_message json happy flow") {
    auto new_json_message = test_happy_flow<register_message, true, string, string, string>(json_register_message::id, string("user"), string("pass"), string("email"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->username == "user");
    REQUIRE(new_json_message->password == "pass");
    REQUIRE(new_json_message->email == "email");

    auto new_binary_message = test_happy_flow<register_message, false, string, string, string>(binary_register_message::id, string("user"), string("pass"), string("email"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->username == "user");
    REQUIRE(new_binary_message->password == "pass");
    REQUIRE(new_binary_message->email == "email");
}

TEST_CASE("serialize/deserialize register_response_message json happy flow") {
    auto new_json_message = test_happy_flow<register_response_message, true, int, string>(json_register_response_message::id, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<register_response_message, false, int, string>(binary_register_response_message::id, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}