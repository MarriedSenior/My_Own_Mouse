// 아두이노 코드
const int motorPin = 9; // 진동 모터 핀 번호
int strength = 0;       // 진폭 (0-255)
char pattern;           // 패턴 코드
unsigned long previousMillis = 0; // 이전 시간 추적
int waveIndex = 0;      // 파동 단계

// 패턴별 주기 및 설정
const int waveResolution = 100; // 한 주기의 분할 수 (진동 품질 향상)
const int wavePeriods[8] = {500, 300, 200, 400, 600, 250, 700, 150}; // 각 패턴의 주기(ms)

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(115200); // 시리얼 통신 초기화
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // 파이썬에서 전송된 명령 읽기
    if (input.length() > 1) {
      pattern = input.charAt(0); // 패턴 코드
      strength = input.substring(1).toInt(); // 진폭
    }
  }

  unsigned long currentMillis = millis();
  int period = getPatternPeriod(pattern); // 패턴의 주기 가져오기
  if (currentMillis - previousMillis >= period / waveResolution) {
    previousMillis = currentMillis;

    // 파동 생성 (사인파 기반)
    float angle = (waveIndex / float(waveResolution)) * 2 * PI;
    int waveValue = int(strength * (sin(angle) + 1) / 2); // 사인파 진폭 계산

    analogWrite(motorPin, waveValue); // 진동 출력
    waveIndex = (waveIndex + 1) % waveResolution;
  }
}

// 패턴에 따라 주기 반환
int getPatternPeriod(char pattern) {
  switch (pattern) {
    case 'S': return wavePeriods[0]; // Single
    case 'D': return wavePeriods[1]; // Double
    case 'H': return wavePeriods[2]; // Hold
    case 'B': return wavePeriods[3]; // Burst
    case 'W': return wavePeriods[4]; // Wave
    case 'P': return wavePeriods[5]; // Pulse
    case 'R': return wavePeriods[6]; // Random
    case 'T': return wavePeriods[7]; // Stutter
    default: return 500; // 기본값
  }
}
