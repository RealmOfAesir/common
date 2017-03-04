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
#include <messages/message_callback_component.h>
#include "test_helpers/test_message.h"

using namespace std;
using namespace roa;


TEST_CASE("Register/deregister should work") {
    message_callback_component<true> handler;
    message_sender sender(true, 1);
    json_test_message t_msg(sender);
    uint32_t callbackCalled = 0;
    handler.register_callback(json_test_message::id, [&](json_message const * const msg) -> void {
        callbackCalled++;
    });

    REQUIRE(callbackCalled == 0);

    handler.call_callbacks_for(json_test_message::id, &t_msg);

    REQUIRE(callbackCalled == 1);

    handler.deregister_callback(json_test_message::id);

    handler.call_callbacks_for(json_test_message::id, &t_msg);

    REQUIRE(callbackCalled == 1);
}