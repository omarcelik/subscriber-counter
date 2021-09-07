import urllib.request
import json

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    client.subscribe("brude/kanal", qos=0)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if "https://www.youtube.com/user/" in str(msg.payload):

        temp = str(msg.payload)
        temp = temp[:0] + '' + temp[2:-1]
        temp = temp[29:]

        key = "AIzaSyDxJZ0m1dC3T6Pmw373QIAZbq0yqGb_XHo"
        name = temp

        data = urllib.request.urlopen("https://www.googleapis.com/youtube/v3/channels?part=snippet,statistics&forUsername=" + name + "&key=" + key).read()

        subs = json.loads(data)["items"][0]["statistics"]["subscriberCount"]
        videos = json.loads(data)["items"][0]["statistics"]["videoCount"]
        views = json.loads(data)["items"][0]["statistics"]["viewCount"]
        datum = json.loads(data)["items"][0]["snippet"]["publishedAt"]
        title = json.loads(data)["items"][0]["snippet"]["title"]

        country = json.loads(data)["items"][0]["snippet"]
        if "country" in country:
            country = json.loads(data)["items"][0]["snippet"]["country"]
        else:
            country = "-"

        datum = datum[:10]
        datum = datum.replace('-', ".")
        datum = datum[8:] + "." + datum[5:-3] + "." + datum[0:-6]

        poruka = title + "*" +  subs + "*" + videos + "*" + views + "*" + country + "*" + datum
        client.publish("brude/info", poruka)

    elif "https://www.youtube.com/channel/" in str(msg.payload):

        temp = str(msg.payload)
        temp = temp[:0] + '' + temp[2:-1]
        temp = temp[32:]

        name = temp
        key = "AIzaSyDxJZ0m1dC3T6Pmw373QIAZbq0yqGb_XHo"
        data = urllib.request.urlopen("https://www.googleapis.com/youtube/v3/channels?part=snippet,statistics&id=" + name + "&key=" + key).read()
        subs = json.loads(data)["items"][0]["statistics"]["subscriberCount"]
        videos = json.loads(data)["items"][0]["statistics"]["videoCount"]
        views = json.loads(data)["items"][0]["statistics"]["viewCount"]
        datum = json.loads(data)["items"][0]["snippet"]["publishedAt"]
        title = json.loads(data)["items"][0]["snippet"]["title"]

        country = json.loads(data)["items"][0]["snippet"]
        if "country" in country:
            country = json.loads(data)["items"][0]["snippet"]["country"]
        else:
            country = "-"

        datum = datum[:10]
        datum = datum.replace('-', ".")
        datum = datum[8:] + "." + datum[5:-3] + "." + datum[0:-6]

        poruka = title + "*" + subs + "*" + videos + "*" + views + "*" + country + "*" + datum
        client.publish("brude/info", poruka)

    else:
        print ("Nista od navedenog.")



client = mqtt.Client()
client.connect("broker.hivemq.com", 1883)
client.on_message = on_message
client.on_connect = on_connect



client.loop_forever()
client.loop_start()

