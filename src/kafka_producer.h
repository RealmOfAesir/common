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

#include "messages/message.h"

#include <string>
#include <memory>
#include <rdkafkacpp.h>

namespace roa {
    class producer_event_callback : public RdKafka::EventCb {
    public:
        void event_cb (RdKafka::Event &event);
    };

    /* Use of this partitioner is pretty pointless since no key is provided
     * in the produce() call. */
    class producer_hash_partitioner_callback : public RdKafka::PartitionerCb {
    public:
        int32_t partitioner_cb (const RdKafka::Topic *topic, const std::string *key, int32_t partition_cnt, void *msg_opaque);

    private:
        static inline unsigned int djb_hash (const char *str, size_t len);
    };

    class producer_delivery_callback : public RdKafka::DeliveryReportCb {
    public:
        void dr_cb (RdKafka::Message &message);
    };

    class ikafka_producer {
    public:
        virtual ~ikafka_producer() = default;

        virtual void close() = 0;

        virtual void send_message(std::unique_ptr<message> msg, uint16_t ms_to_wait = 0) = 0;
        virtual bool is_queue_empty() = 0;
        virtual int poll(uint32_t ms_to_wait) = 0;
    };

    class kafka_producer : public ikafka_producer {
    public:
        kafka_producer(std::string broker_list, std::string topic_str, bool debug = false);

        ~kafka_producer();

        void close() override;

        void send_message(std::unique_ptr<message> msg, uint16_t ms_to_wait = 0) override;
        bool is_queue_empty() override;
        int poll(uint32_t ms_to_wait) override;
    private:
        bool _closing;
        std::unique_ptr<RdKafka::Producer> _producer;
        std::unique_ptr<RdKafka::Topic> _topic;
        producer_hash_partitioner_callback _hash_partitioner_callback;
        producer_delivery_callback _delivery_callback;
        producer_event_callback _event_callback;
    };
}