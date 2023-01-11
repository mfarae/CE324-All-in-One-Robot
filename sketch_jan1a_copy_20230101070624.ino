#include <NewPing.h>

char t;

#define R_S 2  //ir sensor Right
#define L_S 4  //ir sensor Left

#define Echo1 A0  // right
#define Trig1 A1
#define Trig2 A5  // left
#define Echo2 A4
#define Trig3 A3  // middle
#define Echo3 A2
#define MAX_DISTANCE 400

NewPing sonar1(Trig1, Echo1, MAX_DISTANCE);
NewPing sonar2(Trig2, Echo2, MAX_DISTANCE);
NewPing sonar3(Trig3, Echo3, MAX_DISTANCE);

void setup() {
  pinMode(11, OUTPUT);  //left motors forward
  pinMode(10, OUTPUT);  //left motors reverse
  pinMode(9, OUTPUT);   //right motors forward
  pinMode(6, OUTPUT);   //right motors reverse
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(R_S, INPUT);  // IR sensors defined
  pinMode(L_S, INPUT);

  pinMode(Echo1, INPUT);  // ultrasonic setup
  pinMode(Trig1, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo3, INPUT);
  pinMode(Trig3, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    t = Serial.read();
    Serial.println(t);
  }

  if (t == 'F') {  //move forward(all motors rotate in forward direction)
    analogWrite(11, 150);
    analogWrite(9, 190);
  }

  else if (t == 'B') {  //move reverse (all motors rotate in reverse direction)
    //digitalWrite(LED_BUILTIN,LOW);
    analogWrite(10, 150);
    analogWrite(6, 190);
  }

  else if (t == 'L') {  //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
    analogWrite(9, 255);
  }

  else if (t == 'R') {  //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
    analogWrite(11, 255);
  }

  else if (t == 'S') {  //STOP (all motors stop)
    analogWrite(11, 0);
    analogWrite(10, 0);
    analogWrite(9, 0);
    analogWrite(6, 0);
  } else if (t == 'X') {  // LINE FOLLOWING MODE
    while (1) {
      if (Serial.available()) {
        t = Serial.read();
        Serial.println(t);
      }
      if (t == 'F') {
        break;
      }
      if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 1)) { forward(); }  
      if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) { right(); }    
      if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) { left(); }    
      if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) { stop(); } 
      delay(100);
    }
  } else if (t == 'Y') {  // OBSTACLE AVOIDING MODE
    while (1) {
      if (Serial.available()) {
        t = Serial.read();
        Serial.println(t);
      }
      if (t == 'F') {
        break;
      }
      int Right_Distance = sonar1.ping_cm();
      int Middle_Distance = sonar3.ping_cm();
      int Left_Distance = sonar2.ping_cm();
      if (Middle_Distance <= 30){
        reverse();
        delay(100);
        stop();
        delay(200);
        if (Right_Distance > Left_Distance){
          if (Right_Distance <= 30 && Left_Distance <= 30){
            reverse();
            delay(500);
          }
          else{
            right_fast();
            delay(300);
          }
        }
        else if (Right_Distance < Left_Distance){
          if (Right_Distance <= 30 && Left_Distance <= 30){
            reverse();
            delay(500);
          }
          else{
            left_fast();
            delay(300);
          }
        }
      }
      else if (Right_Distance <= 25)
      {
        left_fast();
        delay(500);
      }
      else if (Left_Distance <= 25)
      {
        right_fast();
        delay(500);
      }
      else {
        forward();
      }
    }
  }
  delay(100);
}


  /////////////////////////LINE FOLLOWING///////////////////////////////////////
  void forward() {
    analogWrite(11, 50);
    analogWrite(9, 60);
  }

  void right() {
    analogWrite(11, 110);
    analogWrite(9, 0);
  }

  void left() {
    analogWrite(9, 110);
    analogWrite(11, 0);
  }

  void stop() {
    analogWrite(11, 0);
    analogWrite(10, 0);
    analogWrite(9, 0);
    analogWrite(6, 0);
  }

  void reverse() {
    analogWrite(10, 50);
    analogWrite(6, 60);
  }


  void right_fast() {
    analogWrite(11, 150);
    analogWrite(9, 0);
  }

  void left_fast() {
    analogWrite(9, 150);
    analogWrite(11, 0);
  }
