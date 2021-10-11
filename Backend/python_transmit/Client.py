import time
from paho.mqtt import client as mqtt_client

class Client():
    # Instance variables
    broker = ""
    port = 0
    client_id = ""
    username = ""
    passward = ""
    protocal = ""
    protocal_port = {"tcp" : 1883, "websockets" : 8083, "websocketss" : 8084}
    topic_sub = ""
    topic_pub = ""
    flag_connected = 0

    # Constructor
    def __init__(self) -> None:
        pass

    # device_type = ["rgb-control","env-percept","illegal-detect"]
    # info = [broker, protocal, username, passward, type]
    def set_client(self, info):
        self.broker = info[0]
        self.protocal = info[1]
        self.port = self.protocal_port[info[1]]
        self.username = info[2]
        self.passward = info[3]
        self.client_id = "python-mqtt_" + info[4]
        self.topic_sub = info[4] + "/pub"
        self.topic_pub = info[4]
    
    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.flag_connected = 1
            print("Connected to MQTT Broker!")
            print("Subscribing topic:",self.topic_sub," complete")
            client.subscribe(self.topic_sub)
        else:
            print("Failed to connect, return code {rc}".format(rc = rc), )

    def connect_mqtt(self):
        client = mqtt_client.Client(self.client_id, transport=self.protocal)
        client.username_pw_set(self.username, self.passward)
        client.on_connect = self.on_connect
        client.on_message = self.on_message
        client.connect(self.broker, self.port)
        return client
    
    def on_message(self, client, userdata, msg):
        print("Received `{payload}` from `{topic}` topic".format(
            payload=msg.payload.decode(), topic=msg.topic))
        msg_receive = msg.payload.decode()
        self.publish(client, msg_receive)
    
    def msg_generate(self, msg) -> str:
        pass

    def publish(self, client, msg_receive):
        msg_pub = self.msg_generate(msg_receive)
        result = client.publish(self.topic_pub, msg_pub)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print("Send `{msg}` to topic `{topic}`".format(msg = msg_pub, topic = self.topic_pub))
        else:
            print("Failed to send message to topic {topic}".format(topic = self.topic_pub))
        time.sleep(1)
    
    def run(self):
        client = self.connect_mqtt()
        client.loop_forever()

    