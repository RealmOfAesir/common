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

#include "message_callback_component.h"

#include <easylogging++.h>
#include <macros.h>

using namespace std;
using namespace roa;

template <bool UseJson>
message_callback_component<UseJson>::message_callback_component() : _callbacks() {

}

template <bool UseJson>
message_callback_component<UseJson>::~message_callback_component() {

}

template <bool UseJson>
void message_callback_component<UseJson>::register_callback(uint32_t id, function<void(message<UseJson> const * const)> callback) {
    _callbacks.emplace(id, callback);
}

template <bool UseJson>
void message_callback_component<UseJson>::call_callbacks_for(uint32_t id, message<UseJson> const * const msg) const {
    if(unlikely(msg == nullptr)) {
        return;
    }

    auto search = _callbacks.find(id);
    if(search != _callbacks.end()) {
        search->second(msg);
    }
}

template <bool UseJson>
void message_callback_component<UseJson>::deregister_callback(uint32_t id) {
    _callbacks.erase(id);
}

template class message_callback_component<false>;
template class message_callback_component<true>;