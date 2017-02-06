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

#include "kafka_producer.h"
#include "exceptions.h"
#include "macros.h"

#include <easylogging++.h>
#include <chrono>

using namespace std;
using namespace roa;

void producer_event_callback::event_cb (RdKafka::Event &event) {
    switch (event.type())
    {
        case RdKafka::Event::EVENT_ERROR:
            LOG(ERROR) << "[kafka_producer] ERROR (" << RdKafka::err2str(event.err()) << "): " << event.str();
            break;

        case RdKafka::Event::EVENT_STATS:
            LOG(ERROR) << "[kafka_producer] \"STATS\": " << event.str();
            break;

        case RdKafka::Event::EVENT_LOG:
            LOG(ERROR) << "[kafka_producer] LOG-"<< event.severity() << "-" << event.fac() << ": " << event.str();
            break;

        default:
            LOG(ERROR) << "[kafka_producer] EVENT " << event.type() << " (" << RdKafka::err2str(event.err()) << "): " << event.str();
            break;
    }
}

int32_t producer_hash_partitioner_callback::partitioner_cb (const RdKafka::Topic *topic, const std::string *key, int32_t partition_cnt, void *msg_opaque) {
    return djb_hash(key->c_str(), key->size()) % partition_cnt;
    //return 5381;
}

inline unsigned int producer_hash_partitioner_callback::djb_hash (const char *str, size_t len) {
    unsigned int hash = 5381;
    for (size_t i = 0 ; i < len ; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }
    return hash;
}

void producer_delivery_callback::dr_cb (RdKafka::Message &message) {
    if(message.err() != RdKafka::ERR_NO_ERROR) {
        LOG(ERROR) << "[kafka_producer] Message delivery for (" << message.len() << " bytes): " << message.errstr();
    }
    LOG(INFO) << "[kafka_producer] Message delivery for (" << message.len() << " bytes " << message.offset() << " offset): " << message.errstr();
}


kafka_producer::kafka_producer(std::string broker_list, std::string topic_str, bool debug)
    : _closing(), _producer(), _topic(), _hash_partitioner_callback(), _delivery_callback(), _event_callback() {
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf *topic_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    std::string errstr;

    if(broker_list.empty()) {
        LOG(ERROR) << "[kafka_producer] broker_list empty";
        throw kafka_exception("[kafka_producer] broker_list empty");
    }

    if(topic_str.empty()) {
        LOG(ERROR) << "[kafka_producer] topic_str empty";
        throw kafka_exception("[kafka_producer] topic_str empty");
    }

    if(conf->set("api.version.request", "true", errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] api.version.request " << errstr;
        throw kafka_exception("[kafka_producer] api.version.request");
    }

    if (conf->set("metadata.broker.list", broker_list, errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] metadata.broker.list " << errstr;
        throw kafka_exception("[kafka_producer] metadata.broker.list");
    }

    if (conf->set("event_cb", &_event_callback, errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] event_cb " << errstr;
        throw kafka_exception("[kafka_producer] event_cb");
    }

    if (conf->set("dr_cb", &_delivery_callback, errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] dr_cb " << errstr;
        throw kafka_exception("[kafka_producer] dr_cb");
    }

    if (conf->set("queue.buffering.max.ms", "100", errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] queue.buffering.max.ms " << errstr;
        throw kafka_exception("[kafka_producer] dr_cb");
    }

    if(debug) {
        if(conf->set("debug", "broker,topic,msg", errstr) != RdKafka::Conf::CONF_OK) {
            LOG(ERROR) << "[kafka_producer] debug " << errstr;
            throw kafka_exception("[kafka_producer] debug");
        }
    }

    if (topic_conf->set("partitioner_cb", &_hash_partitioner_callback, errstr) != RdKafka::Conf::CONF_OK) {
        LOG(ERROR) << "[kafka_producer] partitioner_cb " << errstr;
        throw kafka_exception("[kafka_producer] partitioner_cb");
    }

    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if(!producer) {
        LOG(ERROR) << "[kafka_producer] Failed to create producer: " << errstr;
        throw kafka_exception("[kafka_producer] Failed to create producer");
    }

    RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str, topic_conf, errstr);
    if (!topic) {
        LOG(ERROR) << "[kafka_producer] Failed to create topic: " << errstr;
        throw kafka_exception("[kafka_producer] Failed to create topic");
    }

    delete topic_conf;
    delete conf;

    LOG(INFO) << "[kafka_producer] created producer " << producer->name();

    _topic.reset(topic);
    _producer.reset(producer);
}

kafka_producer::~kafka_producer() {
    close();
}

void kafka_producer::send_message(unique_ptr<message> msg, uint16_t ms_to_wait) {
    if(unlikely(!_producer) || unlikely(!_topic)) {
        LOG(ERROR) << "[kafka_producer] No producer or topic";
        throw kafka_exception("[kafka_producer] No producer or topic");
    }

    if(unlikely(_closing)) {
        LOG(WARNING) << "[kafka_producer] is closing";
        return;
    }

    auto msg_str = msg->serialize();

    RdKafka::ErrorCode resp = _producer->produce(_topic.get(), RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY,
                                                 const_cast<char *>(msg_str.c_str()), msg_str.size(), NULL, NULL);

    if (resp != RdKafka::ERR_NO_ERROR /*&& resp != RdKafka::ERR__QUEUE_FULL*/) {
        LOG(ERROR) << "[kafka_producer] Produce failed: " << RdKafka::err2str(resp);
    }

    int ret = _producer->poll(ms_to_wait);
    LOG(ERROR) << "poll(): " << ret;
}

bool kafka_producer::is_queue_empty() {
    return _producer && _producer->outq_len() == 0;
}

int kafka_producer::poll(uint32_t ms_to_wait) {
    if(unlikely(!_producer)) {
        LOG(ERROR) << "[kafka_producer] No producer";
        throw kafka_exception("[kafka_producer] No producer");
    }

    return _producer->poll(ms_to_wait);
}

void kafka_producer::close() {
    if(unlikely(!_producer)) {
        return;
    }

    if(unlikely(_closing)) {
        throw new kafka_exception("[kafka_producer] already closing producer");
    }

    _closing = true;
    auto now = chrono::system_clock::now().time_since_epoch().count();
    auto wait_until = (chrono::system_clock::now() += 2000ms).time_since_epoch().count();

    while(!is_queue_empty() && now < wait_until) {
        _producer->poll(100);
        now = chrono::system_clock::now().time_since_epoch().count();
    }

    _topic.reset();
    _producer.reset();
    _closing = false;
}