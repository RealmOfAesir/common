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

#include "message.h"
#include "login_message.h"
#include "exceptions.h"
#include "login_response_message.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>
#include <easylogging++.h>

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

    stringstream ss;
    switch(buffer[0]) {
        case LOGIN_MESSAGE_TYPE:
            LOG(INFO) << "[message] deserialize encountered LOGIN_MESSAGE_TYPE";
            ss << buffer.substr(1);
            ss.flush();
            {
                std::string username;
                std::string password;
                cereal::BinaryInputArchive archive(ss);
                archive(username, password);
                return make_unique<login_message>(username, password);
            }
        case LOGIN_RESPONSE_MESSAGE_TYPE:
            LOG(INFO) << "[message] deserialize encountered LOGIN_RESPONSE_MESSAGE_TYPE";
            ss << buffer.substr(1);
            ss.flush();
            {
                int error;
                std::string error_str;
                cereal::BinaryInputArchive archive(ss);
                archive(error, error_str);
                return make_unique<login_response_message>(error, error_str);
            }
        default:
            LOG(WARNING) << "[message] deserialize encountered unknown message type: " << buffer[0];

            throw serialization_exception();
    }
}

