#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN            6
#define PIXELS_PER_SEGMENT  1
#define DIGITS 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * DIGITS, PIN, NEO_GRB + NEO_KHZ800);

byte segments[10] = {
  0b1111110,
  0b0011000,
  0b0110111,
  0b0111101,
  0b1011001,
  0b1101101,
  0b1101111,
  0b0111000,
  0b1111111,
  0b1111001
};

int FB_unite;
int FB_dizaine;
int FB_centaine;
int FB_millier;
int FB_dizaine_millier;
int FB_millions;

int TW_unite;
int TW_dizaine;
int TW_centaine;
int TW_millier;
int TW_dizaine_millier;
int TW_millions;

int IN_unite;
int IN_dizaine;
int IN_centaine;
int IN_millier;
int IN_dizaine_millier;
int IN_millions;

void writeDigit(int index, int value) {
  byte seg = segments[value];
  for (int i = 6; i >= 0; i--) {
    int offset = index * (PIXELS_PER_SEGMENT * 7) + i * PIXELS_PER_SEGMENT;
    uint32_t color = seg & 0x01 != 0 ? strip.Color(25, 50, 50) : strip.Color(0, 0, 0);
    for (int x = offset; x < offset + PIXELS_PER_SEGMENT; x++) {
      strip.setPixelColor(x, color);
    }
    seg = seg >> 1;
  }
}

void setup() {
  strip.begin();
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Serial.begin(9600);           /* start serial for debug */
}


void loop() {
  
  clearDisplay();
  animateLoop(true, true);
  animateLoop(false, false);
  writeDigit(5, FB_unite);
  writeDigit(4, FB_dizaine);
  writeDigit(3, FB_centaine);
  writeDigit(2, FB_millier);
  writeDigit(1, FB_dizaine_millier);
  writeDigit(0, FB_millions);
  strip.show();
  delay(5000);

  clearDisplay();
  animateLoop(true, true);
  animateLoop(false, false);
  writeDigit(5, TW_unite);
  writeDigit(4, TW_dizaine);
  writeDigit(3, TW_centaine);
  writeDigit(2, TW_millier);
  writeDigit(1, TW_dizaine_millier);
  writeDigit(0, TW_millions);
  strip.show();
  delay(5000);

  clearDisplay();
  animateLoop(true, true);
  animateLoop(false, false);
  writeDigit(5, IN_unite);
  writeDigit(4, IN_dizaine);
  writeDigit(3, IN_centaine);
  writeDigit(2, IN_millier);
  writeDigit(1, IN_dizaine_millier);
  writeDigit(0, IN_millions);
  strip.show();
  delay(5000);
  
}

void receiveEvent(int howMany) {
 while (0 <Wire.available()) {

////////////////////////////////////////////////////////////////////////////
    int FBLike_Brut = Wire.read() << 8;
    FBLike_Brut |= Wire.read();


    FB_unite = FBLike_Brut % 10;
    FB_dizaine = FBLike_Brut / 10 % 10;
    FB_centaine = FBLike_Brut / 100 % 10;
    FB_millier = FBLike_Brut / 1000 % 10;
    FB_dizaine_millier = FBLike_Brut / 10000 % 10;
  
    Serial.print("FB Like: ");
    Serial.println(FBLike_Brut);
    Serial.println(FB_unite);
    Serial.println(FB_dizaine);
    Serial.println(FB_centaine);
    Serial.println(FB_millier);
    Serial.println(FB_dizaine_millier);
////////////////////////////////////////////////////////////////////////////
    int TWLike_Brut = Wire.read() << 8;
    TWLike_Brut |= Wire.read();
    
    TW_unite = TWLike_Brut % 10;
    TW_dizaine = TWLike_Brut / 10 % 10;
    TW_centaine = TWLike_Brut / 100 % 10;
    TW_millier = TWLike_Brut / 1000 % 10;
    TW_dizaine_millier = TWLike_Brut / 10000 % 10;
    
    Serial.print("Followers Twitter: ");
    Serial.println(TWLike_Brut);
    Serial.println(TW_unite);
    Serial.println(TW_dizaine);
    Serial.println(TW_centaine);
    Serial.println(TW_millier);
    Serial.println(TW_dizaine_millier);
//////////////////////////////////////////////////////////////////////////
    int INLike_Brut = Wire.read() << 8;
    INLike_Brut |= Wire.read();
    
    IN_unite = INLike_Brut % 10;
    IN_dizaine = INLike_Brut / 10 % 10;
    IN_centaine = INLike_Brut / 100 % 10;
    IN_millier = INLike_Brut / 1000 % 10;
    IN_dizaine_millier = INLike_Brut / 10000 % 10;
  
    Serial.print("Insta Fans:");
    Serial.println(INLike_Brut);
    Serial.println(IN_unite);
    Serial.println(IN_dizaine);
    Serial.println(IN_centaine);
    Serial.println(IN_millier);
    Serial.println(IN_dizaine_millier);

  }
 Serial.println();             /* to newline */
}


void clearDisplay() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

void animateLoop(bool fromInner, bool clearing) {

  for (int b = fromInner ? 0 : 7; fromInner ? b < 7 : b >= 0; b += (fromInner ? 1 : -1)) {
    if (clearing) {
      clearDisplay();
    }
    switch (b) {
      case 0:
        strip.setPixelColor(19, strip.Color(100, 0, 100));
        break;
      case 1:
        strip.setPixelColor(20, strip.Color(100, 0, 100));
        strip.setPixelColor(18, strip.Color(100, 0, 100));
        break;
      case 2:
        strip.setPixelColor(0, strip.Color(100, 0, 100));
        strip.setPixelColor(8, strip.Color(100, 0, 100));
        strip.setPixelColor(9, strip.Color(100, 0, 100));
        strip.setPixelColor(17, strip.Color(100, 0, 100));
        break;
      case 3:
        strip.setPixelColor(1, strip.Color(100, 0, 100));
        strip.setPixelColor(7, strip.Color(100, 0, 100));
        strip.setPixelColor(10, strip.Color(100, 0, 100));
        strip.setPixelColor(16, strip.Color(100, 0, 100));
        break;
      case 4:
        strip.setPixelColor(2, strip.Color(100, 0, 100));
        strip.setPixelColor(6, strip.Color(100, 0, 100));
        strip.setPixelColor(11, strip.Color(100, 0, 100));
        strip.setPixelColor(15, strip.Color(100, 0, 100));
        break;
      case 5:
        strip.setPixelColor(3, strip.Color(100, 0, 100));
        strip.setPixelColor(5, strip.Color(100, 0, 100));
        strip.setPixelColor(12, strip.Color(100, 0, 100));
        strip.setPixelColor(14, strip.Color(100, 0, 100));
        break;
      case 6:
        strip.setPixelColor(4, strip.Color(100, 0, 100));
        strip.setPixelColor(13, strip.Color(100, 0, 100));
        break;
      default: break;
    }
    strip.show();
    delay(100);
  }
}
