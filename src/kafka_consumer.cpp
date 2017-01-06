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

#include "kafka_consumer.h"
#include "exceptions.h"
#include "macros.h"

#include <easylogging++.h>

using namespace std;

#ifdef EXPERIMENTAL_OPTIONAL
using namespace experimental;
#endif

using namespace roa;

kafka_consumer::kafka_consumer(string broker_list, string group_id, vector<string> topics) {
        string errstr;
        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

        conf->set("group.id", group_id, errstr);
        conf->set("metadata.broker.list", broker_list, errstr);
        {
            RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

            conf->set("default_topic_conf", tconf, errstr);
            delete tconf;
        }

        {
            RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);
            if (!consumer) {
                LOG(ERROR) << "[kafka_consumer] Failed to create consumer: " << errstr;
                throw kafka_exception();
            }

            _consumer = consumer;
        }

        LOG(INFO) << "[kafka_consumer] Created consumer " << _consumer.value()->name();

        RdKafka::ErrorCode err = _consumer.value()->subscribe(topics);
        if (err) {
            LOG(ERROR) << "[kafka_consumer] Failed to subscribe to topics: " << RdKafka::err2str(err);
            this->close();
            throw kafka_exception();
        }
    }

optional<unique_ptr<message>> kafka_consumer::try_get_message(uint16_t ms_to_wait) {
    if(unlikely(!_consumer)) {
        return {};
    }

    RdKafka::Message *msg = _consumer.value()->consume(ms_to_wait);

    if(!msg) {
        return {};
    }

    if(msg->err() != RdKafka::ERR_NO_ERROR) {
        LOG(WARNING) << "[kafka_consumer] Message error: " << msg->errstr();
        return {};
    }

    return optional<unique_ptr<message>>();
}

void kafka_consumer::close() {
    if(_consumer) {
        delete _consumer.value();
        _consumer = {};
    }
}
