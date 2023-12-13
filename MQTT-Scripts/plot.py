import matplotlib as plt
import paho.mqtt.client as mqtt
print("hello python")
x = []
y = []
ids = []
tags = []
##when it connects , send a message and subscribe to the sensors
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc)) #messsage, taken from online
    #all the subscritions
    client.subscribe("theIlluminati/tag0/x")
    client.subscribe("theIlluminati/tag0/y")
    client.subscribe("theIlluminati/tag0/id")
    client.subscribe("theIlluminati/tag1/x")
    client.subscribe("theIlluminati/tag1/y")
    client.subscribe("theIlluminati/tag1/id")
    client.subscribe("theIlluminati/tag2/x")
    client.subscribe("theIlluminati/tag2/y")
    client.subscribe("theIlluminati/tag2/id")
    client.subscribe("theIlluminati/tag3/x")
    client.subscribe("theIlluminati/tag3/y")
    client.subscribe("theIlluminati/tag0/id")
    #when message is recieved update the solutio


def on_message(client, userdata, msg):
    tagnumber = ''.join(c for c in msg.topic if c.isdigit())#found online eliminates all non numbers, leaving only tag number
    global tags
    ##puts each message into its own catagory filling the array of coordinates
    if("x" in msg.topic):
        tags[tagnumber][0] = msg.payload.decode("utf-8")
    if("y" in msg.topic):
        tags[tagnumber][1] = msg.payload.decode("utf-8")
    if("id" in msg.topic):
        tags[tagnumber][2] = msg.payload.decode("utf-8")
    #compiles the coordinates to thier own planes
    for i in tags:
        x[i] = tags[i][0]
        y[i] = tags[i][1]
        ids[i] = tags[i][2]
    #updates plot
    plt.plot(x,y)
    plt.axis([0,10,0,10])
    plt.text(ids)
    
        
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("team18", password="mors0825")
client.connect("robomqtt.cs.wpi.edu",port = 1883,keepalive=60, bind_address="")
client.loop_forever()