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
#include <doctest.h>
#include <iostream>
#include "messages.h"

using namespace std;
TEST_CASE("serialize/deserialize login_message") {
    login_message message("user", "pass");
    auto serialized_message = message.serialize();
    auto deserialized_message = message::deserialize(serialized_message);
    auto new_message = dynamic_cast<login_message*>(deserialized_message.get());
    CHECK(new_message != nullptr);
    CHECK(new_message->username == "user");
    CHECK(new_message->password == "pass");
}