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
#include <easylogging++.h>
#include <messages/user_access_control/register_message.h>
#include <messages/user_access_control/register_response_message.h>
#include <messages/user_access_control/logout_message.h>
#include <messages/user_access_control/create_character_message.h>
#include <messages/user_access_control/play_character_message.h>
#include <messages/user_access_control/create_character_response_message.h>
#include <messages/user_access_control/play_character_response_message.h>
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
        archive((uint32_t) json_login_message::id, string("garbage"));
        ss.flush();
        REQUIRE_THROWS_AS(message<false>::template deserialize<false>(ss.str()), cereal::Exception &);
    }
    {
        stringstream ss;
        cereal::BinaryOutputArchive archive(ss);
        message_sender sender(true, 1, 2, 3);
        archive((uint32_t) json_login_message::id, sender, string("garbage"));
        ss.flush();
        REQUIRE_THROWS_AS(message<false>::template deserialize<false>(ss.str()), cereal::Exception &);
    }
}

TEST_CASE("serialize/deserialize login_response_message happy flow") {
    auto new_json_message = test_happy_flow<login_response_message, true, int, int, string>(json_login_response_message::id, 2, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->admin_status == 2);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<login_response_message, false, int, int, string>(binary_login_response_message::id, 2, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->admin_status == 2);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}

TEST_CASE("serialize/deserialize register_message happy flow") {
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

TEST_CASE("serialize/deserialize register_response_message happy flow") {
    auto new_json_message = test_happy_flow<register_response_message, true, int, int, string>(json_register_response_message::id, 2, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->admin_status == 2);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<register_response_message, false, int, int, string>(binary_register_response_message::id, 2, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->admin_status == 2);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}

TEST_CASE("serialize/deserialize logout_message happy flow") {
    auto new_json_message = test_happy_flow<logout_message, true>(json_logout_message::id);
    REQUIRE(new_json_message != nullptr);

    auto new_binary_message = test_happy_flow<logout_message, false>(binary_logout_message::id);
    REQUIRE(new_binary_message != nullptr);
}

TEST_CASE("serialize/deserialize create_character_message happy flow") {
    auto new_json_message = test_happy_flow<create_character_message, true, string>(json_create_character_message::id, "player"s);
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->playername == "player"s);

    auto new_binary_message = test_happy_flow<create_character_message, false, string>(binary_create_character_message::id, "player"s);
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->playername == "player"s);
}

TEST_CASE("serialize/deserialize create_character_response_message happy flow") {
    auto new_json_message = test_happy_flow<create_character_response_message, true, int, string>(json_create_character_response_message::id, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<create_character_response_message, false, int, string>(binary_create_character_response_message::id, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}

TEST_CASE("serialize/deserialize play_character_message happy flow") {
    auto new_json_message = test_happy_flow<play_character_message, true, string>(json_play_character_message::id, "player"s);
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->playername == "player"s);

    auto new_binary_message = test_happy_flow<play_character_message, false, string>(binary_play_character_message::id, "player"s);
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->playername == "player"s);
}

TEST_CASE("serialize/deserialize play_character_response_message happy flow") {
    auto new_json_message = test_happy_flow<play_character_response_message, true, int, string>(json_play_character_response_message::id, 1, string("test"));
    REQUIRE(new_json_message != nullptr);
    REQUIRE(new_json_message->error_number == 1);
    REQUIRE(new_json_message->error_str == "test");

    auto new_binary_message = test_happy_flow<play_character_response_message, false, int, string>(binary_play_character_response_message::id, 1, string("test"));
    REQUIRE(new_binary_message != nullptr);
    REQUIRE(new_binary_message->error_number == 1);
    REQUIRE(new_binary_message->error_str == "test");
}