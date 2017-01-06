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

#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#define DOCTEST_CONFIG_USE_IOSFWD
#include <doctest.h>
#include <iostream>
#include <sstream>
#include <cereal/include/cereal/details/helpers.hpp>
#include "messages.h"
#include "exceptions.h"

using namespace std;
using namespace roa;

TEST_SUITE("message serialization/deserialization");

TEST_CASE("serialize/deserialize login_message happy flow") {
    login_message message("user", "pass");
    auto serialized_message = message.serialize();
    FAST_REQUIRE_GT(serialized_message.length(), 0);
    auto deserialized_message = message::deserialize(serialized_message);
    FAST_REQUIRE_NE(deserialized_message, nullptr);
    auto new_message = dynamic_cast<login_message*>(deserialized_message.get());
    FAST_REQUIRE_NE(new_message, nullptr);
    FAST_CHECK_EQ(new_message->username, "user");
    FAST_CHECK_EQ(new_message->password, "pass");
}

TEST_CASE("serialize/deserialize login_message errors") {
    CHECK_THROWS_AS(message::deserialize(""), serialization_exception&);
    CHECK_THROWS_AS(message::deserialize("garbage"), serialization_exception&);

    stringstream ss;
    ss << (char)/*LOGIN_MESSAGE_TYPE*/0;
    ss << "garbage";
    CHECK_THROWS_AS(message::deserialize(ss.str()), cereal::Exception&);
}

TEST_SUITE_END();