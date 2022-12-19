#
# Copyright 2021 HiveMQ GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import time
import paho.mqtt.client as paho
from paho import mqtt
import os
import argparse

# setting callbacks for different events to see if it works, print the message etc.
def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)

# with this callback you can see if your publish was successful
def on_publish(client, userdata, mid, properties=None):
    print("mid: " + str(mid))

def prepare_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("reps", type=int, help="number of generated messages")
    parser.add_argument("topic", help="specify the topic name")
    parser.add_argument("message", help="message to send")
    parser.add_argument("-r", "--retain", help="retain messages, default False", action="store_true")
    parser.add_argument("-c", "--clear_topic", help="remove all retained messages", action="store_true")
    return parser.parse_args()

def main():

    user_input = prepare_args()

    reps = user_input.reps
    payload = user_input.message
    topic = user_input.topic

    # using MQTT version 5 here, for 3.1.1: MQTTv311, 3.1: MQTTv31
    # userdata is user defined data of any type, updated by user_data_set()
    # client_id is the given name of the client
    client = paho.Client(client_id="broker_1", userdata=None, protocol=paho.MQTTv5)
    client.on_connect = on_connect

    # enable TLS for secure connection
    client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
    
    # set username and password
    mqtt_passwd = os.environ["HIVEMQ_PASSWORD"]
    client.username_pw_set("IoT_2022_drug_dealer", mqtt_passwd)
    
    # connect to HiveMQ Cloud on port 8883 (default for MQTT)
    client.connect("4e2bde16ffff46d9a8f341015b213f81.s2.eu.hivemq.cloud", 8883)

    # setting callbacks, use separate functions like above for better visibility
    client.on_publish = on_publish

    if user_input.clear_topic:
        client.publish(topic, payload='', qos=1, retain=True)
        time.sleep(2)
    else:
        for i in range(reps):
            client.publish(topic, payload=payload, qos=1, retain=user_input.retain)
            time.sleep(2)
            
    client.disconnect()


if __name__ == '__main__':
    main()
    print('DONE')

