import logging
import os

import paho.mqtt.client as paho
from paho import mqtt
from paho.mqtt import MQTTException, subscribe
from paho.mqtt.publish import single
from asyncio import sleep, run

logger = logging.getLogger("django")


class BoxMqttClient:
    """
    Usage:
    You should have a HIVEMQ_PASSWORD environment variable set
    >>> client = BoxMqttClient()
    >>> client.publish("test_queue", json.dumps({"message": "test"})) # Publish message
    >>> client.subscribe("test_queue", msg_count=1) # receive single message from the topic
    >>> client.subscribe("test_queue" msg_count=0) # Subscribe to topic and receive messages forever
    """

    def __init__(self):
        self._auth = {
            "username": "IoT_2022_drug_dealer",
            "password": os.environ["HIVEMQ_PASSWORD"],
        }
        self._connection_data = {
            "hostname": "4e2bde16ffff46d9a8f341015b213f81.s2.eu.hivemq.cloud",
            "port": 8883,
        }
        self._tls = {"tls_version": mqtt.client.ssl.PROTOCOL_TLS}
        self.topic_message = ''
    
    def publish(self, topic: str, message: str, qos: int = 0):
        try:
            single(
                topic,
                payload=message,
                qos=qos,
                auth=self._auth,
                **self._connection_data,
                protocol=paho.MQTTv5,
                tls=self._tls,
                retain=True
            )
            return True
        except MQTTException:
            logger.exception("Failed to publish message to topic %s", topic)
            return False

    def _on_message(self, client, userdata, message):
        logger.info("Received message %s on topic %s", message.payload, message.topic)
        self.topic_message = message.payload
        client.disconnect()
        return message.payload

    async def stop_after(self, delay, client):
        await sleep(delay)
        client.loop_stop()

    def subscribe(self, topic: str, qos: int = 1, msg_count: int = 1):
        client = paho.Client()
        client.username_pw_set(**self._auth)
        client.tls_set(**self._tls)
        client.on_message = self._on_message
        client.connect(
                self._connection_data["hostname"], self._connection_data["port"]
            )
        client.subscribe(topic, qos=qos)
        client.loop_start()
        run(self.stop_after(5, client))
        return self.topic_message
