import paho.mqtt.client as mqtt
print("hello python")
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("team18", password="mors0825")
client.connect("robomqtt.cs.wpi.edu",port = 1883,keepalive=60, bind_address="")
client.publish("team18/test","hello")
client.subscribe("team18/IR/dist")
client.loop_forever()