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

#include "login_message.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>
#include <easylogging++.h>

using namespace std;
using namespace roa;

login_message::login_message(std::string username, std::string password) noexcept
        : username(username), password(password) {}

login_message::~login_message() {

}

std::string login_message::serialize() {
    stringstream ss;
    ss << (char)LOGIN_MESSAGE_TYPE;
    {
        cereal::BinaryOutputArchive archive(ss);

        archive(this->username, this->password);
    }

    return ss.str();
}