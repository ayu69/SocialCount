#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN            6
#define PIXELS_PER_SEGMENT  3
#define DIGITS 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * DIGITS, PIN, NEO_GRB + NEO_KHZ800);

byte segments[10] = {
  0b1110111,
  0b0010001,
  0b0111110,
  0b0111011,
  0b1011011,
  0b1101110,
  0b1101111,
  0b0110001,
  0b1111111,
  0b1111011
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
    uint32_t color = seg & 0x01 != 0 ? strip.Color(125, 255, 255) : strip.Color(0, 0, 0);
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
  writeDigit(5, FB_unite);
  writeDigit(4, FB_dizaine);
  writeDigit(3, FB_centaine);
  writeDigit(2, FB_millier);
  writeDigit(1, FB_dizaine_millier);
  writeDigit(0, FB_millions);
  strip.show();
  delay(7000);
  rainbow(4);
  
  clearDisplay();
  writeDigit(5, TW_unite);
  writeDigit(4, TW_dizaine);
  writeDigit(3, TW_centaine);
  writeDigit(2, TW_millier);
  writeDigit(1, TW_dizaine_millier);
  writeDigit(0, TW_millions);
  strip.show();
  delay(7000);
  rainbow(4);

  clearDisplay();
  writeDigit(5, IN_unite);
  writeDigit(4, IN_dizaine);
  writeDigit(3, IN_centaine);
  writeDigit(2, IN_millier);
  writeDigit(1, IN_dizaine_millier);
  writeDigit(0, IN_millions);
  strip.show();
  delay(7000);
  rainbow(4);
  
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


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
