
const int TRIG_A = 5;
const int ECHO_A = 4;

const int TRIG_B = 7;
const int ECHO_B = 6;

const int LED_PIN = 3;

void setup(){
  Serial.begin(9600);
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

class sonar_sensor{
   const int DELAY_MS = 10;
   const int THRESHOLD_DIST_CM = 10 ;
   bool flag ;
   bool in_flag ;
   bool out_flag ;
   float distance_cm;
   public: 
   int TRIG_PIN;
   int ECHO_PIN;
   sonar_sensor(){
    flag = in_flag = out_flag = 0;
   }
   sonar_sensor(int t,int e){
      TRIG_PIN = t;
      ECHO_PIN = e;
      flag = in_flag = out_flag = 0;
   }


  void run(){
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(DELAY_MS);
    digitalWrite(TRIG_PIN, LOW);
    float duration_us = pulseIn(ECHO_PIN, HIGH);
    float distance_cm = 0.017 * duration_us;
    in_flag = 0;
    if( distance_cm < THRESHOLD_DIST_CM ) {
      flag = 1;
    }
    else {
      if( flag ) in_flag = 1;
      flag = 0;
    }
//    flag = (distance_cm < THRESHOLD_DIST_CM );
    
//    in_flag = (prev_flag && !flag );
//    out_flag = (!prev_flag && flag ) ;
    // return distance_cm;
  }

  bool get_in_flag(){ return in_flag; }
  bool get_out_flag() {return out_flag; }
  bool get_flag() {  return flag; }
  float get_distance() { return distance_cm ; }
};

class Lane{
  int cnt ;
  sonar_sensor in_sensor, out_sensor ;
  
  public:
  Lane(int in_sen_trig, int in_sen_echo, int out_sen_trig, int out_sen_echo ){
    in_sensor.TRIG_PIN = in_sen_trig ;
    in_sensor.ECHO_PIN = in_sen_echo;
    out_sensor.TRIG_PIN = out_sen_trig ;
    out_sensor.ECHO_PIN = out_sen_echo ;
    cnt = 0;
  }

  void run() {
    in_sensor.run();  
    if( in_sensor.get_in_flag() )
      cnt++;
    out_sensor.run() ;
    if( out_sensor.get_in_flag() )
      cnt--;
    
  }

  int get_count(){return cnt ; }
};

Lane lane1(TRIG_A,ECHO_A,TRIG_B,ECHO_B);

void loop(){
  lane1.run();

  if (lane1.get_count()>0) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);
}
