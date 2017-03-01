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

#include "message.h"

namespace roa {
    template <bool UseJson>
    class login_message : public message<UseJson> {
    public:
        login_message(std::string username, std::string password) noexcept;

        ~login_message() override;

        std::string const serialize() const override;

        std::string username;
        std::string password;
    };

    template class login_message<false>;
    template class login_message<true>;
}