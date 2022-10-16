#include <QMC5883LCompass.h>
#include <ESP8266WiFi.h>
///#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

// change your config-values here
#include <myconfig.h>

QMC5883LCompass compass;

int a;
int x, y, z;
float b, last_b; // magnetic induction
float min_b, max_b, diff_b;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);
  compass.init();

  WiFi.hostname(deviceName);
  // Optional set fixed IP
  /*   WiFi.config(staticIP, gateway, subnet, dns); */
  // if (!WiFi.config(staticIP, gateway, subnet, dns)) {
  //  Serial.println("STA Failed to configure");
  // }
  WiFi.begin(wifi_ssid, wifi_password);
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(50);
  }

  String mac = WiFi.macAddress();
  mac.remove(14, 1);
  mac.remove(11, 1);
  mac.remove(8, 1);
  mac.remove(5, 1);
  mac.remove(2, 1);
  topic = topic + mac + "-gas/";

  if (Serial)
  {
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
  }
  client.setServer(mqttserver, 1883);

  min_b = 100000;
}

void serial_out(String topic, String msg)
{
  if (Serial)
  {
    Serial.print("top: ");
    Serial.print(topic);
    Serial.print(" msg : ");
    Serial.println(msg);
  }
}

void sendmqtt()
{
  String top = topic + "magfield";
  String msg_out = String(b);
  client.connect(deviceName);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "x";
  msg_out = String(x);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "y";
  msg_out = String(y);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "z";
  msg_out = String(z);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "step";
  msg_out = String(step);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "min_b";
  msg_out = String(min_b);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "max_b";
  msg_out = String(max_b);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  top = topic + "diff_b";
  msg_out = String(diff_b);
  client.publish(top.c_str(), msg_out.c_str());
  serial_out(top, msg_out);
  top = "";
  msg_out = "";

  client.disconnect();
}

void loop()
{

  // Read compass values
  compass.read();
  // Return Azimuth reading
  a = compass.getAzimuth();
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();

  b = sqrt((float)(x * x + y * y + z * z)); // calculate magnetic fiel value

  if (b < min_b)
  {
    min_b = b;
  }
  if (b > max_b)
  {
    max_b = b;
/*     if (max_b != min_b)
    {
      max_diff = (max_b - min_b) * 2 / 3; // calculate treshold with maxima/minima 
    } */
  }

  if (b > (last_b + max_diff))
  {
    step = 0.01; // update + 1 step
    diff_b = b - last_b;
    last_b = b;
    sendmqtt();
  }
  if (b < (last_b - max_diff))
  {
    step = 0; // Lower send just for information of the switch-state
    diff_b = last_b - b;
    last_b = b;
    sendmqtt();
  }
  delay(1000);
}