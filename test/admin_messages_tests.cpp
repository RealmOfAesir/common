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
#include <cereal/include/cereal/details/helpers.hpp>
#include "messages/message.h"
#include "admin_messages/quit_message.h"

using namespace std;
using namespace roa;

TEST_CASE("serialize/deserialize quit_message binary happy flow") {
    message_sender sender(true, 1);
    quit_message<false> quit_msg(sender);
    auto serialized_message = quit_msg.serialize();
    REQUIRE(serialized_message.length() > 0);
    auto deserialized_message = message<false>::template deserialize<false>(serialized_message);
    REQUIRE(get<0>(deserialized_message) == ADMIN_QUIT_MESSAGE_TYPE);
    REQUIRE(get<1>(deserialized_message) != nullptr);
    auto new_message = dynamic_cast<quit_message<false> const *>(get<1>(deserialized_message).get());
    REQUIRE(new_message != nullptr);
}

TEST_CASE("serialize/deserialize quit_message json happy flow") {
    message_sender sender(true, 1);
    quit_message<true> quit_msg(sender);
    auto serialized_message = quit_msg.serialize();
    REQUIRE(serialized_message.length() > 0);
    auto deserialized_message = message<true>::template deserialize<true>(serialized_message);
    REQUIRE(get<0>(deserialized_message) == ADMIN_QUIT_MESSAGE_TYPE);
    REQUIRE(get<1>(deserialized_message) != nullptr);
    auto new_message = dynamic_cast<quit_message<true> const*>(get<1>(deserialized_message).get());
    REQUIRE(new_message != nullptr);
}