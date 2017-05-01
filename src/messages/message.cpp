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

#include "message.h"
#include "user_access_control/login_message.h"
#include "exceptions.h"
#include "user_access_control/login_response_message.h"
#include "misc.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <sstream>
#include <easylogging++.h>
#include <admin_messages/quit_message.h>
#include <messages/user_access_control/register_message.h>
#include <messages/user_access_control/register_response_message.h>
#include <messages/chat/chat_receive_message.h>
#include <messages/chat/chat_send_message.h>

using namespace std;
using namespace roa;

template <bool UseJson>
message<UseJson>::message(message_sender sender) : sender(sender) {

}

template <bool UseJson>
template <bool UseJsonAsReturnType>
tuple<uint32_t, unique_ptr<message<UseJsonAsReturnType> const>> message<UseJson>::deserialize(std::string buffer) {
    if(buffer.empty() || buffer.length() < 4) {
        LOG(WARNING) << "[message] deserialize encountered empty buffer";
        throw serialization_exception("empty buffer " + to_string(buffer.length()));
    }

    uint32_t type;
    message_sender sender;
    stringstream ss;
    ss << buffer;
    ss.flush();
    typename conditional<UseJson, cereal::JSONInputArchive, cereal::BinaryInputArchive>::type archive(ss);
    archive(type, sender);

    if(UseJson) {
        LOG(INFO) << "original json message: " << buffer;
    }

    LOG(INFO) << "[message] type " << type << " with length " << buffer.size();

    switch(type) {
        case login_message<UseJson>::id:
            {
                std::string username;
                std::string password;
                archive(username, password);
                return make_tuple(type, make_unique<login_message<UseJsonAsReturnType>>(sender, username, password));
            }
        case login_response_message<UseJson>::id:
            {
                int8_t admin_status;
                int error;
                std::string error_str;
                archive(admin_status, error, error_str);
                return make_tuple(type, make_unique<login_response_message<UseJsonAsReturnType>>(sender, admin_status, error, error_str));
            }
        case register_message<UseJson>::id:
            {
                std::string username;
                std::string password;
                std::string email;
                archive(username, password, email);
                return make_tuple(type, make_unique<register_message<UseJsonAsReturnType>>(sender, username, password, email));
            }
        case register_response_message<UseJson>::id:
            {
                int8_t admin_status;
                int error;
                std::string error_str;
                archive(admin_status, error, error_str);
                return make_tuple(type, make_unique<register_response_message<UseJsonAsReturnType>>(sender, admin_status, error, error_str));
            }
        case chat_send_message<UseJson>::id:
            {
                string from_username;
                string target;
                string message;
                archive(from_username, target, message);
                return make_tuple(type, make_unique<chat_send_message<UseJsonAsReturnType>>(sender, from_username, target, message));
            }
        case chat_receive_message<UseJson>::id:
            {
                string from_username;
                string target;
                string message;
                archive(from_username, target, message);
                return make_tuple(type, make_unique<chat_receive_message<UseJsonAsReturnType>>(sender, from_username, target, message));
            }

        // ---- admin messages ----

        case quit_message<UseJson>::id:
            return make_tuple(type, make_unique<quit_message<UseJsonAsReturnType>>(sender));
        default:
            LOG(WARNING) << "[message] deserialize encountered unknown message type: " << type;

            throw serialization_exception("unknown message type " + to_string(type));
    }
}

template class message<false>;
template class message<true>;
template tuple<uint32_t, unique_ptr<message<false> const>> message<false>::deserialize<false>(std::string buffer);
template tuple<uint32_t, unique_ptr<message<false> const>> message<true>::deserialize<false>(std::string buffer);
template tuple<uint32_t, unique_ptr<message<true> const>> message<false>::deserialize<true>(std::string buffer);
template tuple<uint32_t, unique_ptr<message<true> const>> message<true>::deserialize<true>(std::string buffer);