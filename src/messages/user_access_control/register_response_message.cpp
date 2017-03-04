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

#include "register_response_message.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <sstream>

using namespace std;
using namespace roa;

template <bool UseJson>
register_response_message<UseJson>::register_response_message(message_sender sender, int error, std::string error_str) noexcept
        : message<UseJson>(sender), error(error), error_str(error_str) {}

template <bool UseJson>
register_response_message<UseJson>::~register_response_message() {

}

template <bool UseJson>
string const register_response_message<UseJson>::serialize() const {
    stringstream ss;
    {
        typename conditional<UseJson, cereal::JSONOutputArchive, cereal::BinaryOutputArchive>::type archive(ss);

        archive(register_response_message<UseJson>::id, this->sender, this->error, this->error_str);
    }

    return ss.str();
}

template<bool UseJson> uint32_t constexpr register_response_message<UseJson>::id;
template class register_response_message<false>;
template class register_response_message<true>;