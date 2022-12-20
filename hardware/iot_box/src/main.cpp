#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

#include "common.hh"
#include <WiFi.h>
#include "mqtt_client.h"

#define LED 2

#define MOTOR1_PIN1 0
#define MOTOR1_PIN2 1
#define MOTOR1_PIN3 2
#define MOTOR1_PIN4 3

#define MOTOR2_PIN1 4
#define MOTOR2_PIN2 5
#define MOTOR2_PIN3 6
#define MOTOR2_PIN4 7

#define MOTOR3_PIN1 12
#define MOTOR3_PIN2 13
#define MOTOR3_PIN3 14
#define MOTOR3_PIN4 15

#define MOTOR4_PIN1 8
#define MOTOR4_PIN2 9
#define MOTOR4_PIN3 10
#define MOTOR4_PIN4 11

#define MOTOR5_PIN1 16
#define MOTOR5_PIN2 17
#define MOTOR5_PIN3 18
#define MOTOR5_PIN4 19

using namespace std;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

Adafruit_MCP23X17 mcp;

int motorSpeed = 100;
int motor5Speed = 1000;

int i = 0;

void setupPins()
{
  pinMode(LED, OUTPUT);

  mcp.pinMode(MOTOR1_PIN1, OUTPUT);
  mcp.pinMode(MOTOR1_PIN2, OUTPUT);
  mcp.pinMode(MOTOR1_PIN3, OUTPUT);
  mcp.pinMode(MOTOR1_PIN4, OUTPUT);

  mcp.pinMode(MOTOR2_PIN1, OUTPUT);
  mcp.pinMode(MOTOR2_PIN2, OUTPUT);
  mcp.pinMode(MOTOR2_PIN3, OUTPUT);
  mcp.pinMode(MOTOR2_PIN4, OUTPUT);

  mcp.pinMode(MOTOR3_PIN1, OUTPUT);
  mcp.pinMode(MOTOR3_PIN2, OUTPUT);
  mcp.pinMode(MOTOR3_PIN3, OUTPUT);
  mcp.pinMode(MOTOR3_PIN4, OUTPUT);

  mcp.pinMode(MOTOR4_PIN1, OUTPUT);
  mcp.pinMode(MOTOR4_PIN2, OUTPUT);
  mcp.pinMode(MOTOR4_PIN3, OUTPUT);
  mcp.pinMode(MOTOR4_PIN4, OUTPUT);

  pinMode(MOTOR5_PIN1, OUTPUT);
  pinMode(MOTOR5_PIN2, OUTPUT);
  pinMode(MOTOR5_PIN3, OUTPUT);
  pinMode(MOTOR5_PIN4, OUTPUT);
}

void setup()
{
  Serial.begin(115200);
  initWiFi();
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  configTime(3600, 0, ntpServer);
  mqtt_setup();
  if (!mcp.begin_I2C())
  {
    Serial.println("Error.");
    while (1)
    {
    }
  }

  setupPins();
}

void openColumn1()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR1_PIN1, HIGH);
    mcp.digitalWrite(MOTOR1_PIN2, LOW);
    mcp.digitalWrite(MOTOR1_PIN3, LOW);
    mcp.digitalWrite(MOTOR1_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR1_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR1_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR1_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR1_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR1_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR1_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR1_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
  }
  mcp.digitalWrite(MOTOR1_PIN1, LOW);
  mcp.digitalWrite(MOTOR1_PIN2, LOW);
  mcp.digitalWrite(MOTOR1_PIN3, LOW);
  mcp.digitalWrite(MOTOR1_PIN4, LOW);
}

void closeColumn1()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR1_PIN4, HIGH);
    mcp.digitalWrite(MOTOR1_PIN3, LOW);
    mcp.digitalWrite(MOTOR1_PIN2, LOW);
    mcp.digitalWrite(MOTOR1_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR1_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR1_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR1_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR1_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR1_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR1_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR1_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
  }
}

void openColumn2()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR2_PIN1, HIGH);
    mcp.digitalWrite(MOTOR2_PIN2, LOW);
    mcp.digitalWrite(MOTOR2_PIN3, LOW);
    mcp.digitalWrite(MOTOR2_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR2_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR2_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR2_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR2_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR2_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR2_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR2_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
  }

  mcp.digitalWrite(MOTOR2_PIN1, LOW);
  mcp.digitalWrite(MOTOR2_PIN2, LOW);
  mcp.digitalWrite(MOTOR2_PIN3, LOW);
  mcp.digitalWrite(MOTOR2_PIN4, LOW);
}

void closeColumn2()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR2_PIN4, HIGH);
    mcp.digitalWrite(MOTOR2_PIN3, LOW);
    mcp.digitalWrite(MOTOR2_PIN2, LOW);
    mcp.digitalWrite(MOTOR2_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR2_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR2_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR2_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR2_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR2_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR2_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR2_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
  }
}

void openColumn3()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR3_PIN1, HIGH);
    mcp.digitalWrite(MOTOR3_PIN2, LOW);
    mcp.digitalWrite(MOTOR3_PIN3, LOW);
    mcp.digitalWrite(MOTOR3_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR3_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR3_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR3_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR3_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR3_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR3_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR3_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
  }

  mcp.digitalWrite(MOTOR3_PIN1, LOW);
  mcp.digitalWrite(MOTOR3_PIN2, LOW);
  mcp.digitalWrite(MOTOR3_PIN3, LOW);
  mcp.digitalWrite(MOTOR3_PIN4, LOW);
}

void closeColumn3()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR3_PIN4, HIGH);
    mcp.digitalWrite(MOTOR3_PIN3, LOW);
    mcp.digitalWrite(MOTOR3_PIN2, LOW);
    mcp.digitalWrite(MOTOR3_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR3_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR3_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR3_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR3_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR3_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR3_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR3_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
  }
}

void openColumn4()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR4_PIN1, HIGH);
    mcp.digitalWrite(MOTOR4_PIN2, LOW);
    mcp.digitalWrite(MOTOR4_PIN3, LOW);
    mcp.digitalWrite(MOTOR4_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR4_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR4_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR4_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR4_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR4_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR4_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR4_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
  }
  mcp.digitalWrite(MOTOR4_PIN1, LOW);
  mcp.digitalWrite(MOTOR4_PIN2, LOW);
  mcp.digitalWrite(MOTOR4_PIN3, LOW);
  mcp.digitalWrite(MOTOR4_PIN4, LOW);
}

void closeColumn4()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    mcp.digitalWrite(MOTOR4_PIN4, HIGH);
    mcp.digitalWrite(MOTOR4_PIN3, LOW);
    mcp.digitalWrite(MOTOR4_PIN2, LOW);
    mcp.digitalWrite(MOTOR4_PIN1, LOW);
    delayMicroseconds(motorSpeed);
    // 2
    mcp.digitalWrite(MOTOR4_PIN3, HIGH);
    delayMicroseconds(motorSpeed);
    // 3
    mcp.digitalWrite(MOTOR4_PIN4, LOW);
    delayMicroseconds(motorSpeed);
    // 4
    mcp.digitalWrite(MOTOR4_PIN2, HIGH);
    delayMicroseconds(motorSpeed);
    // 5
    mcp.digitalWrite(MOTOR4_PIN3, LOW);
    delayMicroseconds(motorSpeed);
    // 6
    mcp.digitalWrite(MOTOR4_PIN1, HIGH);
    delayMicroseconds(motorSpeed);
    // 7
    mcp.digitalWrite(MOTOR4_PIN2, LOW);
    delayMicroseconds(motorSpeed);
    // 8
    mcp.digitalWrite(MOTOR4_PIN4, HIGH);
    delayMicroseconds(motorSpeed);
  }
}

void openColumn5()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    digitalWrite(MOTOR5_PIN1, HIGH);
    digitalWrite(MOTOR5_PIN2, LOW);
    digitalWrite(MOTOR5_PIN3, LOW);
    digitalWrite(MOTOR5_PIN4, LOW);
    delayMicroseconds(motor5Speed);
    // 2
    digitalWrite(MOTOR5_PIN2, HIGH);
    delayMicroseconds(motor5Speed);
    // 3
    digitalWrite(MOTOR5_PIN1, LOW);
    delayMicroseconds(motor5Speed);
    // 4
    digitalWrite(MOTOR5_PIN3, HIGH);
    delayMicroseconds(motor5Speed);
    // 5
    digitalWrite(MOTOR5_PIN2, LOW);
    delayMicroseconds(motor5Speed);
    // 6
    digitalWrite(MOTOR5_PIN4, HIGH);
    delayMicroseconds(motor5Speed);
    // 7
    digitalWrite(MOTOR5_PIN3, LOW);
    delayMicroseconds(motor5Speed);
    // 8
    digitalWrite(MOTOR5_PIN1, HIGH);
    delayMicroseconds(motor5Speed);
  }
  digitalWrite(MOTOR5_PIN1, LOW);
  digitalWrite(MOTOR5_PIN2, LOW);
  digitalWrite(MOTOR5_PIN3, LOW);
  digitalWrite(MOTOR5_PIN4, LOW);
}

void closeColumn5()
{
  for (int i = 0; i < 305; i++)
  {
    // 1
    digitalWrite(MOTOR5_PIN4, HIGH);
    digitalWrite(MOTOR5_PIN3, LOW);
    digitalWrite(MOTOR5_PIN2, LOW);
    digitalWrite(MOTOR5_PIN1, LOW);
    delayMicroseconds(motor5Speed);
    // 2
    digitalWrite(MOTOR5_PIN3, HIGH);
    delayMicroseconds(motor5Speed);
    // 3
    digitalWrite(MOTOR5_PIN4, LOW);
    delayMicroseconds(motor5Speed);
    // 4
    digitalWrite(MOTOR5_PIN2, HIGH);
    delayMicroseconds(motor5Speed);
    // 5
    digitalWrite(MOTOR5_PIN3, LOW);
    delayMicroseconds(motor5Speed);
    // 6
    digitalWrite(MOTOR5_PIN1, HIGH);
    delayMicroseconds(motor5Speed);
    // 7
    digitalWrite(MOTOR5_PIN2, LOW);
    delayMicroseconds(motor5Speed);
    // 8
    digitalWrite(MOTOR5_PIN4, HIGH);
    delayMicroseconds(motor5Speed);
  }
}

void service_mode()
{
  String command = "";

  if (Serial.available())
  {
    command = Serial.readStringUntil('\n');
  }

  Serial.println(command);

  if (strcmp(command.c_str(), "1o") == 13)
  {
    openColumn1();
  }
  else if (strcmp(command.c_str(), "1c") == 13)
  {
    closeColumn1();
  }
  else if (strcmp(command.c_str(), "2o") == 13)
  {
    openColumn2();
  }
  else if (strcmp(command.c_str(), "2c") == 13)
  {
    closeColumn2();
  }
  else if (strcmp(command.c_str(), "3o") == 13)
  {
    openColumn3();
  }
  else if (strcmp(command.c_str(), "3c") == 13)
  {
    closeColumn3();
  }
  else if (strcmp(command.c_str(), "4o") == 13)
  {
    openColumn4();
  }
  else if (strcmp(command.c_str(), "4c") == 13)
  {
    closeColumn4();
  }
  else if (strcmp(command.c_str(), "5o") == 13)
  {
    openColumn5();
  }
  else if (strcmp(command.c_str(), "5c") == 13)
  {
    closeColumn5();
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
  {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
  printTime();
  try_dispense_pill();

  service_mode();

  delay(TICK_TIME);
}
