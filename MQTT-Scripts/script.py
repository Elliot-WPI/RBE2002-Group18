import paho.mqtt.client as mqtt
print("hello python")
Sonar = 0
IMU = 0
IR = 0
##when it connects , send a message and subscribe to the sensors
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("team18/IR/dist")
    client.subscribe("team18/Sonar/dist")
    client.subscribe("team18/IMU/Zaxis")
    #when message is recieved update the solution

def on_message(client, userdata, msg):
    if "IR" in msg.topic:
        global IR
        IR = msg.payload.decode("utf-8")
    if  "Sonar" in msg.topic:
        global Sonar
        Sonar = msg.payload.decode("utf-8")
    if "imu" in msg.topic:
        global IMU
        IMU = msg.payload.decode("utf-8")
    print (IR, '\t',Sonar,'\t',IMU)
    print(msg.topic)
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("team18", password="mors0825")
client.connect("robomqtt.cs.wpi.edu",port = 1883,keepalive=60, bind_address="")
##client.publish("team18/test","hello")

client.loop_forever()