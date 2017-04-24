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
#include <memory>

#include "../messages/message.h"

namespace roa {
    template <bool UseJson>
    class quit_message : public message<UseJson> {
    public:
        quit_message(message_sender sender) noexcept;

        ~quit_message() override;

        std::string const serialize() const override;

        static constexpr uint32_t id = ADMIN_QUIT_MESSAGE_TYPE;
    };

    using json_quit_message = quit_message<true>;
    using binary_quit_message = quit_message<false>;
}
