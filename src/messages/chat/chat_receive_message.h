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

#pragma once

#include <string>

#include "../message.h"

namespace roa {
    template <bool UseJson>
    class chat_receive_message : public message<UseJson> {
    public:
        chat_receive_message(message_sender sender, std::string from_username, std::string target, std::string message) noexcept;

        ~chat_receive_message() noexcept override;

        std::string const serialize() const override;

        std::string from_username;
        std::string target;
        std::string message;
        static constexpr uint32_t id = 5;
    };

    using json_chat_receive_message = chat_receive_message<true>;
    using binary_chat_receive_message = chat_receive_message<false>;
}