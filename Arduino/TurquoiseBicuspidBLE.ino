#include <SoftwareSerial.h>

SoftwareSerial btSerial(9, 10); // TX, RX

// Global
int BUTT = 2;
int BUTT_STATE = 0;
int TIME_START = 0;
int REDPIN = 6;
int GREPIN = 3;
int BLUPIN = 5;

// Defaults
/*
Type:
    Blink: 0
    Pulse: 1
    On: 2
    Off: 3
Number:
    1: 0
    2: 1
    3: 2
    5: 3
Time:
    50: 0
    100: 1
    250: 2
    500: 3
Repeat:
    0: 0
    5000: 1
    15000: 2
    30000: 3
*/
//00-00-00-00-00000000-00000000-00000000
int TYPE = 0;
int NUMB = 0;
int TIME = 0;
int REPT = 0;
int arrayNUMB[] = {1, 2, 3, 5};
int arrayTIME[] = {50, 100, 250, 500};
int arrayREPT[] = {0, 5000, 15000, 30000};

unsigned char API;
unsigned char R;
unsigned char G;
unsigned char B;

void setup()
{
  pinMode(BUTT, INPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREPIN, OUTPUT);
  pinMode(BLUPIN, OUTPUT);

  Serial.begin(9600);
  delay(500);
  Serial.println("Type AT commands!");

  // AC-BT v4 defaults to 9600.
  btSerial.begin(9600);
  //btSerial.write("AT+RENEW"); // Reset all settings.
  delay(250);
  btSerial.write("AT+ROLE0"); // Slave mode (0: Peripheral, 1: Central, default: 0)
  delay(250);
  btSerial.write("AT+MODE2"); // (0: Tranmission, 1: PIO+0, 2: Remote Control+0, default: 0)
  delay(250);
  btSerial.write("AT+IMME1"); // (0: work immediately, 1: wait for AT+ commands, wait for AT+START to resume work, default: 0) Don't enter transmission mode until told. ("AT+IMME0" is wait until "AT+START" to work. "AT+WORK1" is connect right away.)
  delay(250);
  btSerial.write("AT+BAUD0");
  delay(250);
  btSerial.write("AT+NAMETurquoiseB");
  delay(250);
  btSerial.write("AT+PASS123456");
  delay(250);
  btSerial.write("AT+START"); // Work immediately when AT+IMME1 is set.
  delay(250);
  if(btSerial.available()) {
    while(btSerial.available()) {
       char buff = (char)btSerial.read();
       Serial.print(buff);
     }
   }
  TIME_START = millis();
}

void loop()
{
  // clear button
  BUTT_STATE = digitalRead(BUTT);
  if(BUTT_STATE == HIGH) {
    Serial.println("Clear");
  }
  // read from bluetooth
  int index = 0;
  if(btSerial.available()) {
    while(btSerial.available()) {
      API = btSerial.read();
      delay(1);
      R = btSerial.read();
      delay(1);
      G = btSerial.read();
      delay(1);
      B = btSerial.read();
      delay(1);
    }
    
    Serial.print("API:");
    Serial.print(API, DEC);
    Serial.print(", Binary:");
    Serial.println(API, BIN);
    
    // parse API
    REPT = 0;
    TIME = 0;
    NUMB = 0;
    TYPE = 0;
    
    REPT = API & 3;
    API >>= 2;
    TIME = API & 3;
    API >>= 2;
    NUMB = API & 3;
    API >>= 2;
    TYPE = API &3;

    Serial.print("TYPE:");
    Serial.print(TYPE, DEC);
    Serial.print(", Binary:");
    Serial.println(TYPE, BIN);
    
    Serial.print("NUMB:");
    Serial.print(NUMB, DEC);
    Serial.print(", Binary:");
    Serial.println(NUMB, BIN);
    
    Serial.print("TIME:");
    Serial.print(TIME, DEC);
    Serial.print(", Binary:");
    Serial.println(TIME, BIN);
    
    Serial.print("REPT:");
    Serial.print(REPT, DEC);
    Serial.print(", Binary:");
    Serial.println(REPT, BIN);
    
    Serial.print("R:");
    Serial.print(R, DEC);
    Serial.print(", Binary:");
    Serial.println(R, BIN);
    
    Serial.print("G:");
    Serial.print(G, DEC);
    Serial.print(", Binary:");
    Serial.println(G, BIN);
    
    Serial.print("B:");
    Serial.print(B, DEC);
    Serial.print(", Binary:");
    Serial.println(B, BIN);
    
    // blink
    if(TYPE == 0) {
      blinkRGB(arrayNUMB[NUMB], arrayTIME[TIME]);
    }
    // pulse
    else if(TYPE == 1) {
      pulseRGB(arrayNUMB[NUMB], arrayTIME[TIME]);
    }
    // turn on
    else if(TYPE == 2) {
      onRGB();
    }
    // turn off
    else if(TYPE == 3) {
      offRGB();
    }
  }
  
  // repeat
  if(REPT > 0) {
    int time_end = millis();
    int time_diff = time_end - TIME_START;
    
    if(time_diff > arrayREPT[REPT]) {
      TIME_START = millis();
      Serial.println("repeat");
      if(TYPE == 0) {
      blinkRGB(arrayNUMB[NUMB], arrayTIME[TIME]);
      }
      // pulse
      else if(TYPE == 1) {
        pulseRGB(arrayNUMB[NUMB], arrayTIME[TIME]);
      }
    }
  }
}

// blinkRGB - blinks an RGB LED
// params:
//  numb: int - number of blinks
//  time: int - delay between each blink (millis)
void blinkRGB(int numb, int time) {
  for(int i=0; i<numb; i++) {
    analogWrite(REDPIN, R);
    analogWrite(GREPIN, G);
    analogWrite(BLUPIN, B);
    delay(time);
    analogWrite(REDPIN, 0);
    analogWrite(GREPIN, 0);
    analogWrite(BLUPIN, 0);
    delay(time);
  }
}

// pulse - pulses an RGB LED
// params:
//   looper: int - number of pulses
//   time: int - delay between each pulse (millis)
void pulseRGB(int numb, int time) {
  int DELAY = 1;
  if(time == 500) {
    DELAY = 8;
  }
  else if(time == 250) {
    DELAY = 5;
  }
  else if(time == 100) {
    DELAY = 3;
  }
  else if(time == 50) {
    DELAY = 1;
  }
  for(int i=0; i<numb; i++) {
    for(int j=0; j<255; j++) {
      if(j <= R) {
        analogWrite(REDPIN, j);
      }
      if(j <= G) {
        analogWrite(GREPIN, j);
      }
      if(j <= B) {
        analogWrite(BLUPIN, j);
      }
      if(j <= 55) {
        delay(DELAY);
      }
      delay(DELAY);
    }
    for(int k=255; k>=0; k--) {
      if(k <= R) {
        analogWrite(REDPIN, k);
      }
      if(k <= G) {
        analogWrite(GREPIN, k);
      }
      if(k <= B) {
        analogWrite(BLUPIN, k);
      }
      delay(DELAY);
    }
    delay(100);
  }
}

// onRGB - turns on an RGB LED
void onRGB() {
  analogWrite(REDPIN, R);
  analogWrite(GREPIN, G);
  analogWrite(BLUPIN, B);
}

// offRGB - turns off an RGB LED
void offRGB() {
  analogWrite(REDPIN, 0);
  analogWrite(GREPIN, 0);
  analogWrite(BLUPIN, 0);
}

