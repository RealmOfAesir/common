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

#include "messages.h"
#include "exceptions.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>
#include <easylogging++.h>

#define LOGIN_MESSAGE_TYPE 0

using namespace std;
using namespace roa;

std::unique_ptr<message> message::deserialize(std::string buffer) {
    if(buffer.empty() || buffer.length() < 2) {
        LOG(WARNING) << "[message] deserialize encountered empty buffer";
        throw serialization_exception();
    }

    if(buffer[0] > 2) {
        LOG(WARNING) << "[message] deserialize encountered unknown message type: " << buffer[0];
        throw serialization_exception();
    }

    if(buffer[0] == LOGIN_MESSAGE_TYPE) {
        LOG(INFO) << "[message] deserialize encountered LOGIN_MESSAGE_TYPE";
        stringstream ss;
        ss << buffer.substr(1);
        ss.flush();
        {
            std::string username;
            std::string password;
            cereal::BinaryInputArchive archive(ss);
            archive(username, password);
            return make_unique<login_message>(username, password);
        }
    }

    LOG(WARNING) << "[message] deserialize encountered unknown message type: " << buffer[0];

    throw serialization_exception();
}

login_message::login_message(std::string username, std::string password)
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