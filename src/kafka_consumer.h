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

#pragma once

#include "messages/message.h"

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <rdkafkacpp.h>

namespace roa {
    template <bool UseJson>
    class ikafka_consumer {
    public:
        virtual ~ikafka_consumer() = default;

        virtual void start(std::string broker_list, std::string group_id, std::vector<std::string> topics, bool debug = false) = 0;
        virtual void close() = 0;

        virtual std::tuple<uint32_t, std::unique_ptr<message<UseJson> const>> try_get_message(uint16_t ms_to_wait = 0) = 0;
        virtual bool is_queue_empty() = 0;
    };

    template <bool UseJson>
    class kafka_consumer : public ikafka_consumer<UseJson> {
    public:
        kafka_consumer();

        ~kafka_consumer();

        void start(std::string broker_list, std::string group_id, std::vector<std::string> topics, bool debug = false) override;
        void close() override;

        std::tuple<uint32_t, std::unique_ptr<message<UseJson> const>> try_get_message(uint16_t ms_to_wait = 0) override;
        bool is_queue_empty() override;
    private:
        bool _closing;
        std::unique_ptr<RdKafka::KafkaConsumer> _consumer;
    };
}