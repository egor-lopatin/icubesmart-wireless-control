/*
  This project is used to control the iCubeSmart 8x8x8 LED cube, which has only physical buttons,
  with Android app RemoteXY.

  Here we use ESP8266 board to send HIGH/LOW signals to the digital pins of the LED cube which
  mimics the button presses (God bless iCubeSmart board's developers who made this possible).

  The source code of graphical user interface has been generated
  automatically by RemoteXY editor: http://remotexy.com/en/library/
  To connect using RemoteXY mobile app by link: http://remotexy.com/en/download/
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

#include "config.h"

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_WIFI
#include <ESP8266WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID WIFI_SSID
#define REMOTEXY_WIFI_PASSWORD WIFI_PASSWORD
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 97 bytes
  { 255, 5, 0, 0, 0, 90, 0, 17, 0, 0, 0, 32, 1, 106, 200, 1, 1, 5, 0, 1,
    9, 127, 90, 18, 11, 12, 31, 83, 116, 97, 114, 116, 47, 80, 97, 117, 115, 101, 0, 1,
    9, 151, 30, 24, 11, 78, 31, 67, 121, 99, 108, 101, 0, 1, 69, 151, 30, 24, 11, 190,
    31, 78, 101, 120, 116, 0, 1, 42, 151, 24, 24, 8, 1, 31, 82, 101, 115, 101, 116, 0,
    10, 28, 42, 51, 51, 56, 64, 26, 31, 79, 78, 0, 31, 79, 70, 70, 0 };

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  uint8_t bt_start;  // =1 if button pressed, else =0
  uint8_t bt_cycle;  // =1 if button pressed, else =0
  uint8_t bt_next;   // =1 if button pressed, else =0
  uint8_t bt_reset;  // =1 if button pressed, else =0
  uint8_t bt_onoff;  // =1 if state is ON, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_BT_START 5
#define PIN_BT_CYCLE 4
#define PIN_BT_NEXT 0
#define PIN_BT_RESET 2

bool isDisabled = false;

void setup() {
  Serial.begin(115200);

  RemoteXY_Init();

  pinMode(PIN_BT_START, OUTPUT);
  pinMode(PIN_BT_CYCLE, OUTPUT);
  pinMode(PIN_BT_NEXT, OUTPUT);
  pinMode(PIN_BT_RESET, OUTPUT);

  initBoard();
}

void initBoard() {
  RemoteXY.bt_onoff = 1;

  digitalWrite(PIN_BT_RESET, LOW);
  digitalWrite(PIN_BT_START, LOW);
  digitalWrite(PIN_BT_CYCLE, LOW);
  digitalWrite(PIN_BT_NEXT, LOW);

  // It's required to start the board with ESP8266 connected to digital pins
  // Otherwise it freezes on the first scene
  next();
  start();
  cycle();
}

void start() {
  digitalWrite(PIN_BT_START, HIGH);
  RemoteXY_delay(100);
  digitalWrite(PIN_BT_START, LOW);
  RemoteXY_delay(100);
}

void cycle() {
  digitalWrite(PIN_BT_CYCLE, HIGH);
  RemoteXY_delay(100);
  digitalWrite(PIN_BT_CYCLE, LOW);
  RemoteXY_delay(100);
}

void next() {
  digitalWrite(PIN_BT_NEXT, HIGH);
  RemoteXY_delay(100);
  digitalWrite(PIN_BT_NEXT, LOW);
  RemoteXY_delay(100);
}

void reset() {
  digitalWrite(PIN_BT_RESET, HIGH);
  RemoteXY_delay(100);
  digitalWrite(PIN_BT_RESET, LOW);
  RemoteXY_delay(100);
}

void loop() {
  RemoteXY_Handler();

  if (RemoteXY.bt_onoff == 0) {
    if (!isDisabled) {
      Serial.println("Disabling board");
      digitalWrite(PIN_BT_RESET, HIGH);
      isDisabled = true;
    }
  }

  if (RemoteXY.bt_onoff == 1) {
    if (isDisabled) {
      Serial.println("Enabling board");
      initBoard();
      isDisabled = false;
    }
  }

  if (!isDisabled) {
    if (RemoteXY.bt_start == 1) {
      Serial.println("START was pressed");
      start();
    }

    if (RemoteXY.bt_cycle == 1) {
      Serial.println("CYCLE was pressed");
      cycle();
    }

    if (RemoteXY.bt_next == 1) {
      Serial.println("NEXT was pressed");
      next();
    }

    if (RemoteXY.bt_reset == 1) {
      Serial.println("RESET was pressed");
      reset();
      initBoard();
    }
  }
}
