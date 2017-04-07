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

#pragma once

#include <string>
#include <memory>
#include <tuple>
#include <sstream>

#include "message_sender.h"

#define LOGIN_MESSAGE_TYPE 0
#define LOGIN_RESPONSE_MESSAGE_TYPE 1
#define REGISTER_MESSAGE_TYPE 2
#define REGISTER_RESPONSE_MESSAGE_TYPE 3

#define ADMIN_QUIT_MESSAGE_TYPE 10000

namespace roa {
    template <bool UseJson>
    class message {
    public:
        message(message_sender sender);
        virtual ~message() noexcept {};

        virtual std::string const serialize() const = 0;

        template <bool UseJsonAsReturnType>
        static std::tuple<uint32_t, std::unique_ptr<message<UseJsonAsReturnType> const>> deserialize(std::string buffer);

        message_sender sender;
    };

    using json_message = message<true>;
    using binary_message = message<false>;
}