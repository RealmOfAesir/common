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

#define LOGIN_MESSAGE_TYPE 0
#define LOGIN_RESPONSE_MESSAGE_TYPE 1

#define ADMIN_QUIT_MESSAGE_TYPE 10000

namespace roa {
    template <bool UseJson>
    class message {
    public:
        virtual ~message() noexcept {};

        virtual std::string serialize() = 0;

        static std::unique_ptr<message> deserialize(std::string buffer);
    };

    template class message<false>;
    template class message<true>;
}