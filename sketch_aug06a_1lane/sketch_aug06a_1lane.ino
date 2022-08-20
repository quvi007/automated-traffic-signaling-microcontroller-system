const int TRIG_A = 5;
const int ECHO_A = 4;

const int TRIG_B = 7;
const int ECHO_B = 6;

const int LED_PIN = 3;
const int DISTANCE_THRESHOLD = 10;

float duration_us_A, distance_cm_A, duration_us_B, distance_cm_B;

bool flagA = false, flagB = false;
int cnt = 0;

void setup(){
  Serial.begin(9600);
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

float SonarSensor(int TRIG_PIN, int ECHO_PIN) {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = 0.017 * duration_us;
  return distance_cm;
}
int x=0;
void loop(){
  distance_cm_A = SonarSensor(TRIG_A, ECHO_A);
  distance_cm_B = SonarSensor(TRIG_B, ECHO_B);
  if (distance_cm_A < DISTANCE_THRESHOLD) {
    flagA = true;
  } else {
    if (flagA) cnt++;
    flagA = false;
  }
  if (distance_cm_B < DISTANCE_THRESHOLD) {
    flagB = true;
  } else {
    if (flagB) cnt--;
    flagB = false;
  }
  if(cnt < 0 ) cnt = 0;
  if (cnt) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);
  Serial.println(x);x++;
}
