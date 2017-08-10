## MQTT with Arduino and Raspberry Pi

MQTT is a lightweight pub-sub framework that provides RabbitMQ-like
functionality on internet-connected microcontrollers like ESP8266.  Although
it's different from the AMQP protocol, RabbitMQ does have a MQTT plugin that
will allow MQTT clients to publish/subscribe to RabbitMQ.  This allows full
AMQP clients (like Raspberry Pi) to communicate with microcontrollers (like
ESP8266/Arduino) via RabbitMQ.

### Setting up the RabbitMQ Server

To install RabbitMQ on Rasperry Pi, first

    # apt-get install rabbitmq-server
    # rabbitmq-plugins enable rabbitmq_mqtt
    # service rabbitmq-server restart

then set up a new user (called `espuser`) we can use from our Arduino:

    # rabbitmqctl add_user espuser esppassword
    # rabbitmqctl set_permissions -p "/" espuser "^mqtt-subscription-ESP8266.*" ".*" ".*"

where `mqtt-subscription-XXXX` is the queue that is declared by the MQTT plugin
for RabbitMQ and `XXXX` is the clientID passed in as the first parameter to the
`client.connect()` routine in the [Arduino PubSubClient library].

    # rabbitmqctl add_user rpiuser rpipassword
    # rabbitmqctl set_permissions -p "/" rpiuser "^mqtt-subscription-ESP8266.*" ".*" ".*"

to connect, you would have to do (Arduino) `client.connect("ESP8266...", "espuser", "esppassword")`

Also add a user for the Raspberry Pi to use on the RabbitMQ user (called
`rpiuser`):

    # rabbitmqctl add_user rpiuser rpipassword
    # rabbitmqctl set_permissions -p "/" rpiuser "^mqtt-subscription-ESP8266.*" ".*" ".*"

### Connecting to RabbitMQ from Arduino

To connect to RabbitMQ from Arduino, you would issue something like

    client.connect("ESP8266...", "espuser", "esppassword")

Strangely, the `client.subscribe('XXX')` doesn't seem to do anything with
respect to `XXX`.  Haven't figured this out yet.

From the pub side, you really only need

    #!/usr/bin/env python
    import pika
    rmq_creds = pika.PlainCredentials('rpiuser', 'rpipassword')
    rmq_conn_params = pika.ConnectionParameters('localhost', 5672, '/', rmq_creds)
    conn = pika.BlockingConnection(rmq_conn_params)
    channel = conn.channel()
    channel.basic_publish(
        exchange='',
        routing_key='mqtt-subscription-ESP8266 Clientqos0',
        body='hello world')
    conn.close()

That is, no need to declare any queues or exchanges, and use the default
exchange.  Note the routing key; in the Arduino sketch, we declare the MQTT
client id as `ESP8266 Client` and use the qos=0.  The RabbitMQ MQTT plugin then
crafts a queue named `mqtt-subscription-ESP8266 Clientqos0` based on this
information when the Arduino declares its subscription:

    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP8266 Client", mqtt_user, mqtt_password)) {
        Serial.println("connected");
        client.subscribe(mqtt_topic, 1);
    } else {
        Serial.print("failed (");
        Serial.print(client.state());
        Serial.println(")");
    }

### Unresolved Mysteries

* I can delete the wifi password and it still works.  Does the router DHCP
reservation just shoo in my client after the initial authentication and lease?

### References

- https://www.rabbitmq.com/mqtt.html
- http://pubsubclient.knolleary.net/api.html
- https://www.baldengineer.com/mqtt-tutorial.html

[Arduino PubSubClient library]: https://pubsubclient.knolleary.net/
