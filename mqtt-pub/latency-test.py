from paho.mqtt import client as mqtt_client
import time
import logging
import ssl
import datetime
import pygame


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


pygame.init()
screen = pygame.display.set_mode((800, 480))
clock = pygame.time.Clock()

y_speed = 0.0
y_forward = True
x_speed = 0.0
x_right = True

running = True
while running:

    # Did the user click the window close button?
    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()

    if keys[pygame.K_w] :
        y_speed = 1.0
        y_forward = True
    
    if keys[pygame.K_s] :
        if (y_speed != 0.0) :
            y_speed = 0.0
        else :
            y_speed = 1.0
            y_forward = False
    
    if keys[pygame.K_a] :
        x_speed = 1.0
        x_right = False

        if (y_speed != 0.0) :
            y_speed = 0.5
    
    if keys[pygame.K_d] :
        if (x_speed != 0.0) :
            x_speed = 0.0
            if (y_speed != 0.0) :
                y_speed = 1.0
        else :
            x_speed = 1.0
            x_right = True

            if (y_speed != 0.0) :
                y_speed = 0.5


    print("Y speed: ", y_speed, ", dir: ", y_forward)
    print("Turn: ", x_speed, ", dir: ", x_right)

    y_speed = 0.0
    x_speed = 0.0

    # Fill the background with white
    screen.fill((255, 255, 255))

    # Flip the display
    pygame.display.flip()

    clock.tick(5)
    pygame.display.update()

# Done! Time to quit.
pygame.quit()


# client = connect_mqtt()
# subscribe(client)
# client.loop_start()


# # Publish 10 messages
# for i in range(10) :
#     time_now = datetime.datetime.now()
#     result = client.publish("car_data", "Test", 1, True, None)
#     status = result[0]
    
#     if status == 0:
#         pub_time = time_now
#         print(time_now, " : Message send")
#     else :
#         print("Failed to send message!!!")
    
#     time.sleep(10.0)

# client.loop_stop()

# print("Bye bye")
