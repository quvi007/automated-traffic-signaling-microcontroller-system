const int TRIG_A = 5;
const int ECHO_A = 4;

const int TRIG_B = 7;
const int ECHO_B = 6;
 
const int TRIG_C = 10;
const int ECHO_C = 9;

const int TRIG_D = 12;
const int ECHO_D = 11; 

const int LED_PIN1 = 3;
const int LED_PIN2 = 8;
const int DISTANCE_THRESHOLD = 10;

float duration_us_A, distance_cm_A, duration_us_B, distance_cm_B,duration_us_C, distance_cm_C, duration_us_D, distance_cm_D;

bool flagA = false, flagB = false,flagC = false, flagD = false;
int cnt1 = 0,cnt2=0;

void setup(){
  Serial.begin(9600);
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(LED_PIN1, OUTPUT);
  
  pinMode(TRIG_C, OUTPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(TRIG_D, OUTPUT);
  pinMode(ECHO_D, INPUT);
  pinMode(LED_PIN2, OUTPUT);
}

float SonarSensor(int TRIG_PIN, int ECHO_PIN) {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = 0.017 * duration_us;
  return distance_cm;
}

void loop(){
  distance_cm_A = SonarSensor(TRIG_A, ECHO_A);
  distance_cm_B = SonarSensor(TRIG_B, ECHO_B);
  distance_cm_C = SonarSensor(TRIG_C, ECHO_C);
  distance_cm_D = SonarSensor(TRIG_D, ECHO_D);
  
  if (distance_cm_A < DISTANCE_THRESHOLD) {
    flagA = true;
  } else {
    if (flagA) cnt1++;
    flagA = false;
  }
  
  if (distance_cm_B < DISTANCE_THRESHOLD) {
    flagB = true;
  } else {
    if (flagB) cnt1--;
    flagB = false;
  }
  
   if (distance_cm_C < DISTANCE_THRESHOLD) {
    flagC = true;
  } else {
    if (flagC) cnt2++;
    flagC = false;
  }
  
  if (distance_cm_D < DISTANCE_THRESHOLD) {
    flagD = true;
  } else {
    if (flagD) cnt2--;
    flagD = false;
  }
  
  if (cnt1) digitalWrite(LED_PIN1, HIGH);
  else digitalWrite(LED_PIN1, LOW);

  if (cnt2) digitalWrite(LED_PIN2, HIGH);
  else digitalWrite(LED_PIN2, LOW);

  
}
