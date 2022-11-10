
/// Change here

// Difference the Magnet of the gas counter generates - you may need to adapt this to your needs
// My tests showed a mag-field of usual 12000-14000 - which goes up to 22000-24000 and then lowers to 300-500 to switch back to 12000.
const int TL_TRESHOLD = 17000;
const int LL_TRESHOLD  = 3000;

// Your Wifi SID
const char *wifi_ssid = "yourwifi";
// Your Wifi Password
const char *wifi_password = "yourpassword";

// your MQTT Server
//IPAddress mqttserver(192, 168, 176, 6);
const char *mqttserver = "dns-of-my-mqtt";
const int port = 1883;

float step = 0.01; // @todo: It looks like a smaller diff of 0.001 can be measured by smaller peaks.
// your mqtt-topic
String topic = "homeassistant/sensor/"; // will changed to mac-gas like so: homeassistant/sensor/AC0BFBD9F5A0-gas/ - if you want it fixed comment the lines in setup() function

const char *deviceName = "gas-sensor-1";

const char *username = "";
const char *password = "";