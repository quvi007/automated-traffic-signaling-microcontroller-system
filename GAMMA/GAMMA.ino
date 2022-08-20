const int TRIG_A = 5;
const int ECHO_A = 4;

const int TRIG_B = 7;
const int ECHO_B = 6;
 
const int TRIG_C = 10;
const int ECHO_C = 9;

const int TRIG_D = 12;
const int ECHO_D = 11;

const int TRIG_E = 23;
const int ECHO_E = 22;

const int TRIG_F = 25;
const int ECHO_F = 24;
 
const int TRIG_G = 27;
const int ECHO_G = 26;

const int TRIG_H = 29;
const int ECHO_H = 28;

const int TRIG_I = 31;
const int ECHO_I = 30;

const int TRIG_J = 33;
const int ECHO_J = 32;
 
const int TRIG_K = 35;
const int ECHO_K = 34;

const int TRIG_L = 37;
const int ECHO_L = 36;

const int LED_PIN1 = 3;
const int LED_PIN2 = 8;
const int LED_PIN3 = 40;
const int LED_PIN4 = 41;
const int LED_PIN5 = 42;
const int LED_PIN6 = 43;

const int average_car_length = 5;
void setup(){
  Serial.begin(9600);
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(TRIG_C, OUTPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(TRIG_D, OUTPUT);
  pinMode(ECHO_D, INPUT);
  pinMode(TRIG_E, OUTPUT);
  pinMode(ECHO_E, INPUT);
  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);
  pinMode(TRIG_G, OUTPUT);
  pinMode(ECHO_G, INPUT);
  pinMode(TRIG_H, OUTPUT);
  pinMode(ECHO_H, INPUT);
  pinMode(TRIG_I, OUTPUT);
  pinMode(ECHO_I, INPUT);
  pinMode(TRIG_J, OUTPUT);
  pinMode(ECHO_J, INPUT);
  pinMode(TRIG_K, OUTPUT);
  pinMode(ECHO_K, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(LED_PIN2,OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN3,OUTPUT);
  pinMode(LED_PIN4, OUTPUT);
  pinMode(LED_PIN5,OUTPUT);
  pinMode(LED_PIN6, OUTPUT);
}

struct sonar_sensor{
   const int DELAY_MS = 100;
   const int THRESHOLD_DIST_CM = 5;
   bool flag ;
   bool in_flag ;
   bool out_flag ;
   float distance_cm;
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
    bool prev_flag = flag; 
    flag = (distance_cm < THRESHOLD_DIST_CM );    
    in_flag = (prev_flag && !flag );
//    out_flag = (!prev_flag && flag ) ;
    // return distance_cm;
  }

  bool get_in_flag(){ return in_flag; }
  bool get_out_flag() {return out_flag; }
  bool get_flag() {  return flag; }
  float get_distance() { return distance_cm ; }
};

struct Lane{
  int cnt ;
  int len;
  long timer;
  sonar_sensor in_sensor, out_sensor ;
  Lane(int in_sen_trig, int in_sen_echo, int out_sen_trig, int out_sen_echo ){
    in_sensor.TRIG_PIN = in_sen_trig ;
    in_sensor.ECHO_PIN = in_sen_echo;
    out_sensor.TRIG_PIN = out_sen_trig ;
    out_sensor.ECHO_PIN = out_sen_echo ;
    cnt = 0;timer=0;
  }

  Lane(int in_sen_trig, int in_sen_echo, int out_sen_trig, int out_sen_echo,int len1){
    in_sensor.TRIG_PIN = in_sen_trig ;
    in_sensor.ECHO_PIN = in_sen_echo;
    out_sensor.TRIG_PIN = out_sen_trig ;
    out_sensor.ECHO_PIN = out_sen_echo ;
    timer=0;
    len = len1 ;
    cnt = 0;
  }

  void run() {
    in_sensor.run();  
    if( in_sensor.get_in_flag() )
      cnt++;
    out_sensor.run() ;
    if( out_sensor.get_in_flag() )
      cnt--;
    if( cnt < 0 ) cnt = 0;

//    long now=
  }
  int get_count(){return cnt ; }
  int get_capacity() { return len-average_car_length*cnt; }
  int get_traffic()
  {
    return average_car_length*cnt;
  }
};

Lane lane1(TRIG_A,ECHO_A,TRIG_B,ECHO_B,14);
Lane lane2(TRIG_C,ECHO_C,TRIG_D,ECHO_D,14);
Lane lane3(TRIG_G,ECHO_G,TRIG_H,ECHO_H,14);
Lane lane4(TRIG_E,ECHO_E,TRIG_F,ECHO_F,14);
Lane lane5(TRIG_I,ECHO_I,TRIG_J,ECHO_J,26);
Lane lane6(TRIG_K,ECHO_K,TRIG_L,ECHO_L,26);

const long mn_time=15000,mx_time=40000,mx_wait=90000;
int p = 0;

struct Controller{

    int active_road,prev;
    Controller(int x)
    {
       active_road=x;prev=0;
    }
    
    void run(){
      lane1.run();
      lane2.run();
      lane3.run();
      lane4.run();
//      lane5.run();
      lane6.run(); 

      long now = millis() ;
      Serial.print(p);
      Serial.print(" ") ;
      Serial.print("now: ");
      Serial.print(now);

      Serial.print(" active_road: ") ;      
      Serial.print(active_road);
      Serial.print(" lane timer: ");Serial.print(lane1.timer);Serial.print(" "); Serial.print(lane3.timer); Serial.print(" "); Serial.print(lane5.timer); 
//      if(active_road==1)Serial.print(lane1.timer);
//      else if(active_road==3)Serial.print(lane3.timer);
//      else Serial.print(lane5.timer);
      
      Serial.print(" lane traffic: ") ;
      Serial.print(lane1.get_traffic());Serial.print(" ");Serial.print(lane3.get_traffic());Serial.print(" ");Serial.print(lane5.get_traffic());Serial.print(" ");
     
      if(active_road==1)
      {
        if(now-lane1.timer<=mn_time)
        {
        }
        else if(now-lane1.timer<=mx_time)
        {
          if(now-lane3.timer>mx_wait && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
            active_road=3;
            lane1.timer=now;lane3.timer=now;
          }
          else if(now-lane5.timer>mx_wait && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity() )
          {
            active_road=5;
            lane1.timer=now;lane5.timer=now;
          }         
          else if(lane3.get_traffic()>=lane5.get_traffic() && lane3.get_traffic()>=lane1.get_traffic() && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
             active_road=3;
             lane1.timer=now;lane3.timer=now;
          }
          else if(lane5.get_traffic()>=lane3.get_traffic() && lane5.get_traffic()>=lane1.get_traffic() && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())
          {
             active_road=5;
             lane1.timer=now;lane5.timer=now;
          }
        }
        else
        {
          if(now-lane3.timer>mx_wait && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
            active_road=3;
            lane1.timer=now;lane3.timer=now;
          }
          else if(now-lane5.timer>mx_wait && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())
          {
            active_road=5;
            lane1.timer=now;lane5.timer=now;
          }         
          else if(lane3.get_traffic()>=lane5.get_traffic() && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
             active_road=3;
             lane1.timer=now;lane3.timer=now;
          }
          else if(lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity()) // lane5.get_traffic()>lane3.get_traffic() &&  
          {
             active_road=5;
             lane1.timer=now;lane5.timer=now;
          }
        }
      }
      else if(active_road==3)
      {
        if(now-lane3.timer<=mn_time)
        {
        }
        else if(now-lane3.timer<=mx_time)
        {
          if(now-lane1.timer>mx_wait && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
            p = 1;
            active_road=1;
            lane1.timer=now;lane3.timer=now;
          }
          else if(now-lane5.timer>mx_wait && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())
          {p=2;
            active_road=5;
            lane5.timer=now;lane3.timer=now;
          }         
          else if(lane1.get_traffic()>=lane5.get_traffic() && lane1.get_traffic()>=lane3.get_traffic() && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {p=3;
             Serial.println("111111111111111111111111111");
             active_road=1;
             lane1.timer=now;lane3.timer=now;
          }
          else if(lane5.get_traffic()>=lane1.get_traffic() && lane5.get_traffic()>=lane3.get_traffic() && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())
          {
            p=4;
             //Serial.println("222222222222222222222222222");
             active_road=5;
             lane5.timer=now;lane3.timer=now;
          }
        }
        else
        {
          if(now-lane1.timer>mx_wait && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
            p=5;
            active_road=1;
            lane1.timer=now;lane3.timer=now;
          }
          else if(now-lane5.timer>mx_wait && lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())
          {
            p=6;
            active_road=5;
            lane3.timer=now;lane5.timer=now;
          }         
          else if(lane1.get_traffic()>=lane5.get_traffic() && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
            p=7;
             active_road=1;
             lane1.timer=now;lane3.timer=now;
          }
          else if(lane5.get_traffic()<=lane2.get_capacity()+lane4.get_capacity())  // lane5.get_traffic()>=lane1.get_traffic() && 
          {p=8;
             active_road=5;
             lane3.timer=now;lane5.timer=now;
          }
        }  
      }
      else
      {
        if(now-lane5.timer<=mn_time)
        {
        }
        else if(now-lane5.timer<=mx_time)
        {
          if(now-lane1.timer>mx_wait && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
            active_road=1;
            lane1.timer=now;lane5.timer=now;
          }
          else if(now-lane3.timer>mx_wait && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
            active_road=3;
            lane5.timer=now;lane3.timer=now;
          }         
          else if(lane1.get_traffic()>=lane3.get_traffic() && lane1.get_traffic()>=lane5.get_traffic() && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
             active_road=1;
             lane1.timer=now;lane5.timer=now;
          }
          else if(lane3.get_traffic()>=lane1.get_traffic() && lane3.get_traffic()>=lane5.get_traffic() && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
             active_road=3;
             lane5.timer=now;lane3.timer=now;
          }
        }
        else
        {
          if(now-lane1.timer>mx_wait && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
            active_road=1;
            lane1.timer=now;lane5.timer=now;
          }
          else if(now-lane3.timer>mx_wait && lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())
          {
            active_road=3;
            lane3.timer=now;lane5.timer=now;
          }         
          else if(lane1.get_traffic()>=lane3.get_traffic() && lane1.get_traffic()<=lane4.get_capacity()+lane6.get_capacity())
          {
             active_road=1;
             lane1.timer=now;lane5.timer=now;
          }
          else if(lane3.get_traffic()<=lane2.get_capacity()+lane6.get_capacity())  // lane5.get_traffic()>=lane1.get_traffic() && 
          {
             active_road=3;
             lane3.timer=now;lane5.timer=now;
          }
        }
      }

      //Serial.println(active_road);
      if(active_road!=prev){
      if( active_road == 1) {
        digitalWrite(LED_PIN1,HIGH);
        digitalWrite(LED_PIN4,HIGH);
        digitalWrite(LED_PIN6,HIGH);
        
        digitalWrite(LED_PIN2,LOW);
        digitalWrite(LED_PIN3,LOW);
        digitalWrite(LED_PIN5,LOW);
      }
      else if( active_road==3) {
        digitalWrite(LED_PIN3,HIGH);
        digitalWrite(LED_PIN2,HIGH);
        digitalWrite(LED_PIN6,HIGH);

        digitalWrite(LED_PIN4,LOW);
        digitalWrite(LED_PIN1,LOW);
        digitalWrite(LED_PIN5,LOW);
      }
      else {
        digitalWrite(LED_PIN5,HIGH);
        digitalWrite(LED_PIN2,HIGH);
        digitalWrite(LED_PIN4,HIGH);
      
        digitalWrite(LED_PIN6,LOW);
        digitalWrite(LED_PIN1,LOW);
        digitalWrite(LED_PIN3,LOW);
      }
      }
      prev=active_road;
    }
};

Controller controller(1);
int x=0;
void loop(){
  
  controller.run();
  Serial.print(x);x++;
  Serial.print(" lane1: ");
  Serial.print(lane1.cnt);
  Serial.print(" lane2: ");
  Serial.print(lane2.cnt);
  Serial.print(" lane3: ");
  Serial.print(lane3.cnt);
  Serial.print(" lane4: ");
  Serial.print(lane4.cnt);
  Serial.print(" lane5: ");
  Serial.print(lane5.cnt); 
  Serial.print(" lane6: "); 
  Serial.println(lane6.cnt); 

////  Serial.println(lane1.get_count());
//  if (lane1.get_count()) digitalWrite(LED_PIN1, HIGH);
//  else digitalWrite(LED_PIN1, LOW);
}
