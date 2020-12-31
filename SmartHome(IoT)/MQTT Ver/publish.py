import paho.mqtt.client as mqtt
import json


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK")
    else:
        print("Bad connection Returned code=", rc)


def on_disconnect(client, userdata, flags, rc=0):
    print(str(rc))


def on_publish(client, userdata, mid):
    print("In on_pub callback mid= ", mid)


# 새로운 클라이언트 생성
client = mqtt.Client()

# 콜백 함수 설정 on_connect(브로커에 접속), on_disconnect(브로커에 접속중료), on_publish(메세지 발행)
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_publish = on_publish

# address : localhost, port: 8880(MY team port) 에 연결
client.connect('localhost', 8880)
client.loop_start()

# common topic 으로 메세지 발행
client.publish('common', json.dumps({"Connect port": "ok"}), 1)
client.loop_stop()
# 연결 종료
client.disconnect()