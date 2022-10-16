
/// Change here

// Difference the Magnet of the gas counter generates - you may need to adapt this to your needs
// My tests showed a mag-field of usual 12000-14000 - which goes up to 22000-24000 and then lowers to 300-500 to switch back to 12000.
// The maximum distance should aquire the needed "switch" of a number.
float max_diff = 15500; // this is very sensitive  - a magnet in 1m distance results in changes..
// you could try for auto-config - line 150-153 in main.cpp - but this tends to work after a while and is very speaky in the beginning

// Your Wifi SID
const char *wifi_ssid = "yourwifi";
// Your Wifi Password
const char *wifi_password = "yourpassword";

// your MQTT Server
IPAddress mqttserver(192, 168, 176, 6);

float step = 0.01; // @todo: It looks like a smaller diff of 0.001 can be measured by smaller peaks.
// your mqtt-topic
String topic = "homeassistant/sensor/"; // will changed to mac-gas like so: homeassistant/sensor/AC0BFBD9F5A0-gas/ - if you want it fixed comment the lines in setup() function

const char *deviceName = "gas-sensor-1";
// Change END