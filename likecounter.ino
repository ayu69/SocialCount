#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <NeoPixelBus.h>

const uint16_t PixelCount = 4; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor black(0);


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


  strip.Begin();
  strip.Show();
  
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

  int FBLike_Brut = FBLike;
  int FB_unite = FBLike_Brut % 10;
  int FB_dizaine = FBLike_Brut / 10 % 10;
  int FB_centaine = FBLike_Brut / 100 % 10;
  int FB_millier = FBLike_Brut / 1000 % 10;
  int FB_dizaine_millier = FBLike_Brut / 10000 % 10;

  Serial.print("FB Like: ");
  Serial.println(FBLike_Brut);
  Serial.println(FB_unite);
  Serial.println(FB_dizaine);
  Serial.println(FB_centaine);
  Serial.println(FB_millier);
  Serial.println(FB_dizaine_millier);
}

void twitter() {

  
  if (!client.connect(TWhost, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/widgets/followbutton/info.json?screen_names=" + TW_Name;

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

  int TWLike_Brut = followers_count;
  int TW_unite = TWLike_Brut % 10;
  int TW_dizaine = TWLike_Brut / 10 % 10;
  int TW_centaine = TWLike_Brut / 100 % 10;
  int TW_millier = TWLike_Brut / 1000 % 10;
  int TW_dizaine_millier = TWLike_Brut / 10000 % 10;

  Serial.print("Followers Twitter: ");
  Serial.println(TWLike_Brut);
  Serial.println(TW_unite);
  Serial.println(TW_dizaine);
  Serial.println(TW_centaine);
  Serial.println(TW_millier);
  Serial.println(TW_dizaine_millier);
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
  int INfollower = Body_Data["counts"]["followed_by"];

  int INLike_Brut = INfollower;
  int IN_unite = INLike_Brut % 10;
  int IN_dizaine = INLike_Brut / 10 % 10;
  int IN_centaine = INLike_Brut / 100 % 10;
  int IN_millier = INLike_Brut / 1000 % 10;
  int IN_dizaine_millier = INLike_Brut / 10000 % 10;

  Serial.print("Insta Fans:");
  Serial.println(INLike_Brut);
  Serial.println(IN_unite);
  Serial.println(IN_dizaine);
  Serial.println(IN_centaine);
  Serial.println(IN_millier);
  Serial.println(IN_dizaine_millier);



}

void loop() {

  //facebook();
  //twitter();
  //instagram();

}