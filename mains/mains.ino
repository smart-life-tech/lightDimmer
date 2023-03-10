
#include "HX711.h"
#include "RBDdimmer.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial ESP_Serial(10, 11);

#define CLK 8
#define DOUT 9
#define zeroCrossPin  2
#define pwmPin 3
#define solenoidPin 7
#define footSwPin 5
#define handSwPin 6
#define testLedPin 13

HX711 scale;
//Dimmer dimmer(3, DIMMER_COUNT);
dimmerLamp dimmer(pwmPin); // initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero

int MIN_POWER  = 0;
int MAX_POWER  = 100;

float valueOz;
uint8_t bagSize = 1;
float weightOption = 0;
float weightProgress;
unsigned long timeReadWeight = 0;
float calibration_factor = 11669.42;
byte stateFeeder = 0;
unsigned long timeOperation;
int hoursOperation;
int cycleBag[4];
int screen = 0;
unsigned long timeSendData = 0;
int p1;
int powerValue;
unsigned long startAgain;
byte startA;
float weightCut1 , weightCut2 , weightCut3 , weightCut4;
float weightTar1 , weightTar2 , weightTar3 , weightTar4;
float weightUnd1 , weightUnd2 , weightUnd3 , weightUnd4;
float weightOve1 , weightOve2 , weightOve3 , weightOve4;
int speedDribble1 , speedDribble2 , speedDribble3 , speedDribble4;
int speedBulk1, speedBulk2 , speedBulk3 , speedBulk4;
float pause1 , pause2 , pause3 , pause4;
float bucket1 , bucket2 , bucket3 , bucket4;
unsigned long timeOpenBag = 1000;
float weightCut , weightTar , weightUnd , weightOve;
String colorV = "W";
int speedDribble = 10, speedBulk = 40;
int accept = 0;
int pauseTime = 1000;

void setup() {
  initSerial();
  initHardware();
  initScale();
  initFeeder();
  readMemory();
}

void loop() {
  readInterface();
  controlInput();
  readWeight();
  controlOutput();
  sendData();
}


void sendData() {
  if (millis() - timeSendData > 2000) {
    String data = String(weightProgress) + "," + String(hoursOperation) + "," + String(cycleBag[0]) + "," + String(cycleBag[1]) + "," + String(cycleBag[2]) + "," + String(cycleBag[3]) + "," + colorV;
    //Serial.println(String(weightProgress));
    ESP_Serial.println(data);
    timeSendData = millis();
  }
}


void readInterface() {
  if (ESP_Serial.available()) {
    String data = ESP_Serial.readStringUntil('\n');
    Serial.println(data);
    if (data.indexOf("data") > -1) {
      String temp = data;
      temp.remove(0 , temp.indexOf(",") + 1);
      Serial.println(temp);
      weightCut1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightTar1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightUnd1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightOve1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      weightCut2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightTar2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightUnd2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightOve2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      weightCut3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightTar3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightUnd3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightOve3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      weightCut4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightTar4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightUnd4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      weightOve4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      
      speedDribble1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      speedBulk1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      pause1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      bucket1 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      speedDribble2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      speedBulk2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      pause2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      bucket2 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      speedDribble3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      speedBulk3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      pause3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      bucket3 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);

      speedDribble4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      speedBulk4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      pause4 = temp.substring(0 , temp.indexOf(",")).toInt();
      temp.remove(0 , temp.indexOf(",") + 1);
      bucket4 = temp.substring(0 , temp.indexOf(",")).toInt();
    }
    else if (data.indexOf("12oz") > -1) {
      bagSize = 1;
      weightOption = 12;
      weightCut = weightCut1 / 100;
      weightTar = weightTar1 / 100;
      weightUnd = weightUnd1 / 100;
      weightOve = weightOve1 / 100;
      speedDribble = speedDribble1;
      speedBulk = speedBulk1;
      pauseTime = pause1;
      timeOpenBag = bucket1;
      Serial.println("soeedDribble1: " + String(speedDribble) + " SpeedBulk: " + String(speedBulk) + " pauseTime: " + String(pauseTime) + " timeOpenBag: " + String(timeOpenBag));
    }
    else if (data.indexOf("16oz") > -1) {
      bagSize = 2;
      weightOption = 16;
      weightCut = weightCut2 / 100;
      weightTar = weightTar2 / 100;
      weightUnd = weightUnd2 / 100;
      weightOve = weightOve2 / 100;
      speedDribble = speedDribble2;
      speedBulk = speedBulk2;
      pauseTime = pause2;
      timeOpenBag = bucket2;
      Serial.println("Cut off: " + String(weightCut) + " Target: " + String(weightTar));
    }
    else if (data.indexOf("32oz") > -1) {
      bagSize = 3;
      weightOption = 32;
      weightCut = weightCut3 / 100;
      weightTar = weightTar3 / 100;
      weightUnd = weightUnd3 / 100;
      weightOve = weightOve3 / 100;
      speedDribble = speedDribble3;
      speedBulk = speedBulk3;
      pauseTime = pause3;
      timeOpenBag = bucket3;
      Serial.println("Cut off: " + String(weightCut) + " Target: " + String(weightTar));
    }
    else if (data.indexOf("96oz") > -1) {
      bagSize = 4;
      weightOption = 96;
      weightCut = weightCut4 / 100;
      weightTar = weightTar4 / 100;
      weightUnd = weightUnd4 / 100;
      weightOve = weightOve4 / 100;
      speedDribble = speedDribble4;
      speedBulk = speedBulk4;
      pauseTime = pause4;
      timeOpenBag = bucket4;
      Serial.println("Cut off: " + String(weightCut) + " Target: " + String(weightTar));
    }
    else if (data.indexOf("Start") > -1 && startA == 0) {
      stateFeeder = 1;
      powerValue = 0;
      for (int i = 0; i < 10; i++) {
        dimmer.setPower(powerValue);
        powerValue += 10;
        delay(20);
      }
      timeOperation = millis();
      startA = 1;
    }
    else if (data.indexOf("Stop") > -1 && startA == 1) {
      stateFeeder = 0;
      powerValue = 0;
      startA = 0;
    }
    else if (data.indexOf("Tare") > -1) {
      calibrationScale();
    }
    else if (data.indexOf("Clear") > -1) {
      hoursOperation = 0;
      cycleBag[0] = 0;
      cycleBag[1] = 0;
      cycleBag[2] = 0;
      cycleBag[3] = 0;
      EEPROM.put(0, hoursOperation);
      EEPROM.put(20, cycleBag[0]);
      EEPROM.put(40, cycleBag[1]);
      EEPROM.put(60, cycleBag[2]);
      EEPROM.put(80, cycleBag[3]);
    }
    else if (data.indexOf("Empty") > -1 ) {
      Serial.println(timeOpenBag);
      digitalWrite(solenoidPin, HIGH);
      delay(timeOpenBag);
      digitalWrite(solenoidPin, LOW);
      delay(timeOpenBag);
    }
    else if (data.indexOf("Accept") > -1 && accept == 1) {
      accept = 0;
    }
  }
}

void controlOutput() {
  if (stateFeeder == 1) {
    readWeight();
    Serial.println(String(weightProgress) + " " + String(weightCut) + " " + String(weightTar));
    if (weightProgress < weightCut) {
      powerValue = speedBulk;
      colorV = "W";
    }
    else if (weightProgress >= weightCut && weightProgress < weightTar) {
      powerValue = speedDribble;
      colorV = "W";
    }
    else if (weightProgress > weightTar) {
      powerValue = 0;
      stateFeeder = 2;
      if (weightProgress < weightUnd || weightProgress > weightOve) {
        colorV = "R";
        accept = 1;
      }
      else {
        colorV = "G";
        accept = 0;
      }
    }
  }
  if (millis() - timeOperation > 3600000 && stateFeeder > 0) {
    hoursOperation++;
    timeOperation = millis();
  }
  controlFeeder(powerValue);
}

void readWeight() {
  if (millis() - timeReadWeight > 500 && screen == 0) {
    valueOz = scale.get_units(), 1;
    weightProgress = valueOz;
    timeReadWeight = millis();
    colorV = "W";
  }
}

void controlFeeder(int power) {
  dimmer.setPower(power);
}

void controlInput() {
  if ((digitalRead(footSwPin) == LOW || digitalRead(handSwPin) == LOW) && p1 == 0 && stateFeeder == 2 && accept == 0) {
    p1 = 1;
    digitalWrite(solenoidPin, HIGH);
    delay(timeOpenBag);
    digitalWrite(solenoidPin, LOW);
    delay(timeOpenBag);
    cycleBag[bagSize - 1] += 1;
    dimmer.setPower(speedBulk);
    stateFeeder = 1;
    delay(pauseTime);
  }
  else if (digitalRead(footSwPin) == HIGH && digitalRead(handSwPin) == HIGH && p1 == 1) {
    p1 = 0;
  }
}

void calibrationScale() {
  if (scale.is_ready()) {
    scale.tare();
  }
}

void initHardware() {
  pinMode(footSwPin, INPUT_PULLUP);
  pinMode(handSwPin, INPUT_PULLUP);
  pinMode(solenoidPin, OUTPUT);
  pinMode(testLedPin, OUTPUT);
}

void initScale() {
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void initFeeder() {
  dimmer.begin(NORMAL_MODE, ON);
}

void readMemory() {
  EEPROM.get(0, hoursOperation);
  EEPROM.get(20, cycleBag[0]);
  EEPROM.get(40, cycleBag[1]);
  EEPROM.get(60, cycleBag[2]);
  EEPROM.get(80, cycleBag[3]);
}

void initSerial() {
  Serial.begin(9600);
  ESP_Serial.begin(9600);
}
