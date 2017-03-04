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

namespace roa {
    class message_sender {
    public:
        message_sender();
        message_sender(bool is_client, uint32_t id);
        message_sender(message_sender&&) = default;
        constexpr message_sender(message_sender const &) = default;

        template<class Archive>
        void serialize(Archive & archive);

        bool is_client;
        uint32_t id;
    };
}