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

#include "message.h"

#include <type_traits>
#include <unordered_map>
#include <functional>

namespace roa {
    template <bool UseJson>
    class imessage_callback_component {
    public:
        virtual ~imessage_callback_component() = default;

        virtual void register_callback(uint32_t id, std::function<void(message<UseJson> const * const)> callback) = 0;
        virtual void deregister_callback(uint32_t id) = 0;

        virtual void call_callbacks_for(uint32_t id, message<UseJson> const * const message) const = 0;
    };

    template <bool UseJson>
    class message_callback_component : public imessage_callback_component<UseJson> {
    public:
        message_callback_component();
        ~message_callback_component();

        void register_callback(uint32_t id, std::function<void(message<UseJson> const * const)> callback) override;
        void deregister_callback(uint32_t id) override;

        void call_callbacks_for(uint32_t id, message<UseJson> const * const message) const override;

    private:
        std::unordered_map<uint32_t, std::function<void(message<UseJson> const * const)>> _callbacks;
    };
}