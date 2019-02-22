#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>



#ifndef STASSID
#define STASSID "WIFI-NEVERS"
#define STAPSK  "$Wif-1!!"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* FBhost = "graph.facebook.com";
const char* TWhost = "cdn.syndication.twimg.com";
const char* INhost = "api.instagram.com";


String FB_PageId = "1714217158802621";
String FB_Token = "EAAQGcoi7jDsBACMnqLzt5ii6Gshtys2EPmJaxANbF4WhdkvYSRbqebEz2H0n2nFoWtAeyBzWsCZBjogp1FSrlfHgGPZCLTafQVyCxijBAyv0hxXFaNivoqfDqjn0sL9p0N3fiMmR2vbxw0wngfySAGWyh68NPh9r2ZCoNnpXQZDZD";


String TW_Name = "POLE__FORMATION";


String IN_Token = "10728025542.e9432ee.84490f410c55489e8e0913372400e7a5";

const int httpsPort = 443;

WiFiClientSecure client;


void setup() {
  client.setInsecure();
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void facebook() {

  if (!client.connect(FBhost, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/" + FB_PageId + "?access_token=" + FB_Token + "&fields=fan_count";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + FBhost + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);
  int FBLike = root[String("fan_count")];

  Serial.print("FB Like: ");
  Serial.println(FBLike);
}

void twitter() {

  
  if (!client.connect(TWhost, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/widgets/followbutton/info.json?screen_names="+TW_Name;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + TWhost + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  // Removing brackets [] around the json as they're making it not readable by the parser
  line.remove(0, 1);
  line.remove(line.length() - 1, 1);
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);
  int followers_count = root[String("followers_count")];

  Serial.print("Followers Twitter: ");
  Serial.println(followers_count);
}

void instagram() {

  if (!client.connect(INhost, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/v1/users/self/?access_token="+IN_Token;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + INhost + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);
  
  JsonObject& Body_Data = root["data"];
  int follower = Body_Data["counts"]["followed_by"];

  Serial.print("Insta Fans: ");
  Serial.println(follower);
}

void loop() {

  facebook();
  twitter();
  instagram();
  delay(10000);

}