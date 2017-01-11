#!/usr/bin/env python
#
#  Very simple pika producer to publish messages to the topic queue from which
#  the ESP8266Wifi+PubSubClient subscribes.
#

import pika

rmq_creds = pika.PlainCredentials('rpiuser', 'rpipassword')
rmq_conn_params = pika.ConnectionParameters( 'localhost', 5672, '/', rmq_creds )
conn = pika.BlockingConnection( rmq_conn_params )
channel = conn.channel()

# channel.queue_declare(queue='mqtt-subscription-ESP8266 Clientqos0')

# channel.exchange_declare( exchange='amq.topic', type='topic', auto_delete=False, durable=True )

# default exchange ('') "just works" with RabbitMQ
# mqtt-subscription-* is from RabbitMQ's MQTT bridge;
# qos[01] determined by consumer subscription (QOS=0 unreliable; QOS=1 reliable)
channel.basic_publish(
    exchange='',
    routing_key='mqtt-subscription-ESP8266 Clientqos1', 
    body='wtf')
conn.close()
