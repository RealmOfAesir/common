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

#include <cereal/include/cereal/archives/binary.hpp>
#include <cereal/include/cereal/archives/json.hpp>
#include "message_sender.h"

using namespace roa;
using namespace std;

message_sender::message_sender() : is_client(false), id(0) {

}

message_sender::message_sender(bool is_client, uint32_t id) : is_client(is_client), id(id) {

}

template<class Archive>
void message_sender::serialize(Archive &archive) {
    archive(this->is_client, this->id);
}

template void message_sender::serialize(cereal::BinaryInputArchive &archive);
template void message_sender::serialize(cereal::JSONInputArchive &archive);
template void message_sender::serialize(cereal::BinaryOutputArchive &archive);
template void message_sender::serialize(cereal::JSONOutputArchive &archive);

