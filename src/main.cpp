#include <QMC5883LCompass.h>
#include <ESP8266WiFi.h>

///#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <RunningAverage.h>

// change your config-values here
#include <myconfig.h> //copy example_config.h to myconfig...

QMC5883LCompass compass;

int a;
int x, y, z, counter;
float b, last_b; // magnetic induction
float min_b, max_b, diff_b;

float auto_treshold; // experimental

WiFiClient espClient;
PubSubClient client(espClient);

RunningAverage RunningAverage_b(496);

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
  client.setServer(mqttserver, port);

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
  client.connect(deviceName, username, password);
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

  top = topic + "mean_b";
  msg_out = String(RunningAverage_b.getAverage());
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
  RunningAverage_b.addValue(b);             // floating average

  if (b < min_b)
  {
    min_b = b;
  }
  if (b > max_b)
  {
    max_b = b;
    if (max_b != min_b)
    {
      auto_treshold = (max_b - min_b) * 0.75; // calculate treshold with maxima/minima
    }
  }

  if ((b > TL_TRESHOLD) && (last_b < TL_TRESHOLD)) // 17000 - this resultet from tests. // experimental use TL_TRESHOLD = max_b - auto_treshold
  {

    diff_b = b - last_b;
    last_b = b;
  }
  if ((b < LL_TRESHOLD) && (last_b > ( LL_TRESHOLD + 500 ))) // 3000 // experimental use LL_TRESHOLD = min_b + auto_treshold
  {
    step = step + 0.01; // update + 1 step
    diff_b = last_b - b;
    last_b = b;
  }

  if (counter == 0) // every minute 60.000 ms / 750 ms = 80 x
  {
    sendmqtt(); // send only once in a minute
    step = 0;
  }
  // send every minute a Ping / if you need to check for values more often - adapt the counter
  if (counter == _DELAY_COUNTER)
  {
    counter = 0;
  }
  else
  {
    counter++;
  }
  // if you miss to much values / the error goes up - you may need to have a shorter period to check on changes
  delay(_DELAY);
}