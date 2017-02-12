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

//TEST_SUITE("admin_message serialization/deserialization");

TEST_CASE("serialize/deserialize quit_message happy flow") {
    quit_message<false> quit_msg;
    auto serialized_message = quit_msg.serialize();
    REQUIRE(serialized_message.length() > 0);
    auto deserialized_message = message<false>::deserialize(serialized_message);
    REQUIRE(deserialized_message != nullptr);
    auto new_message = dynamic_cast<quit_message<false>*>(deserialized_message.get());
    REQUIRE(new_message != nullptr);
}
//TEST_SUITE_END();