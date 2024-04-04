from paho.mqtt import client as mqtt_client
import time
import logging
import ssl
import datetime

broker = 'dfb0ec72cc864eddaee0fe147972f4af.s1.eu.hivemq.cloud'
port = 8883
topic = "time/ack"

# Alternatively you can use
# username = controller-test
# password = ControllerTest1

client_id = 'controller-python'
username = 'controller-python'
password = 'TestPython3'

pub_time = 0

FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

def on_disconnect(client, userdata, rc):
    logging.info("Disconnected with result code: %s", rc)
    reconnect_count, reconnect_delay = 0, FIRST_RECONNECT_DELAY
    while reconnect_count < MAX_RECONNECT_COUNT:
        logging.info("Reconnecting in %d seconds...", reconnect_delay)
        time.sleep(reconnect_delay)

        try:
            client.reconnect()
            logging.info("Reconnected successfully!")
            return
        except Exception as err:
            logging.error("%s. Reconnect failed. Retrying...", err)

        reconnect_delay *= RECONNECT_RATE
        reconnect_delay = min(reconnect_delay, MAX_RECONNECT_DELAY)
        reconnect_count += 1
    logging.info("Reconnect failed after %s attempts. Exiting...", reconnect_count)


def connect_mqtt():
    # def on_connect(client, userdata, flags, rc):
    # For paho-mqtt 2.0.0, you need to add the properties parameter.
    
    def on_connect(client, userdata, flags, rc, properties):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    

    # For paho-mqtt 2.0.0, you need to set callback_api_version.
    client = mqtt_client.Client(client_id=client_id, callback_api_version=mqtt_client.CallbackAPIVersion.VERSION2)

    client.username_pw_set(username, password)
    context = ssl._create_unverified_context()
    client.tls_set_context(context)
    client.tls_insecure_set(True)
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.connect(broker, port)
    
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        
        if msg.topic == topic :
            latency = datetime.datetime.now()
            latency = (latency - pub_time) / 2
            print("Latency is: ", latency)

    client.subscribe(topic)
    client.on_message = on_message

######################################################################################

client = connect_mqtt()
subscribe(client)
client.loop_start()


# Publish 10 messages
for i in range(10) :
    time_now = datetime.datetime.now()
    result = client.publish("car_data", "Test", 1, True, None)
    status = result[0]
    
    if status == 0:
        pub_time = time_now
        print(time_now, " : Message send")
    else :
        print("Failed to send message!!!")
    
    time.sleep(10.0)

client.loop_stop()

print("Bye bye")
