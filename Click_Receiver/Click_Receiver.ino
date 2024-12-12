#include <Arduino.h>

// 진동자 핀 설정
const int vibrationPin = 9;

// 전역 변수
int currentStrength = 255;

void setup() {
  pinMode(vibrationPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Arduino ready.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    handleCommand(command);
  }
}

void handleCommand(String command) {
  if (command.length() < 2) return;

  char pattern = command[0];
  int strength = command.substring(1).toInt();

  if (strength >= 0 && strength <= 255) {
    currentStrength = strength;
  }

  switch (pattern) {
    case 'S': singleClick(); break;
    case 'D': doubleClick(); break;
    case 'H': holdClick(); break;
    case 'B': burstClick(); break;
    case 'W': wavePattern(); break;
    case 'P': pulsePattern(); break;
    case 'R': randomPattern(); break;
    case 'B1': heartbeatPattern(); break;
    case 'T': stutterPattern(); break;
    default: stopVibration(); break;
  }
}

void singleClick() {
  analogWrite(vibrationPin, currentStrength);
  delay(50);
  analogWrite(vibrationPin, 0);
}

void doubleClick() {
  for (int i = 0; i < 2; i++) {
    analogWrite(vibrationPin, currentStrength);
    delay(50);
    analogWrite(vibrationPin, 0);
    delay(50);
  }
}

void holdClick() {
  analogWrite(vibrationPin, currentStrength);
  delay(200);
  analogWrite(vibrationPin, 0);
}

void burstClick() {
  for (int i = 0; i < 5; i++) {
    analogWrite(vibrationPin, currentStrength);
    delay(30);
    analogWrite(vibrationPin, 0);
    delay(30);
  }
}

void wavePattern() {
  for (int i = 0; i <= currentStrength; i += 10) {
    analogWrite(vibrationPin, i);
    delay(20);
  }
  for (int i = currentStrength; i >= 0; i -= 10) {
    analogWrite(vibrationPin, i);
    delay(20);
  }
  analogWrite(vibrationPin, 0);
}

void pulsePattern() {
  for (int i = 0; i < 3; i++) {
    analogWrite(vibrationPin, currentStrength);
    delay(50);
    analogWrite(vibrationPin, 0);
    delay(100);
  }
}

void randomPattern() {
  for (int i = 0; i < 5; i++) {
    int randStrength = random(50, currentStrength + 1);
    analogWrite(vibrationPin, randStrength);
    delay(random(30, 100));
    analogWrite(vibrationPin, 0);
    delay(random(30, 100));
  }
}

void heartbeatPattern() {
  for (int i = 0; i < 2; i++) {
    analogWrite(vibrationPin, currentStrength);
    delay(100);
    analogWrite(vibrationPin, 0);
    delay(100);
  }
  delay(300);
}

void stutterPattern() {
  for (int i = 0; i < 10; i++) {
    analogWrite(vibrationPin, currentStrength);
    delay(10);
    analogWrite(vibrationPin, 0);
    delay(10);
  }
}

void stopVibration() {
  analogWrite(vibrationPin, 0);
}
