#include <AFMotor.h>
AF_DCMotor m1(2);
AF_DCMotor m2(3);
String path;
int mode = 0;
void setup() {
  Serial.begin(9600);
  for (int i = 14; i < 19; i++) {
    pinMode(i, INPUT);
  }
  pinMode(2, INPUT);
  m1.setSpeed(200);
  m2.setSpeed(200);
}

void loop() {

  if(digitalRead(2) == 0){
    for (int i = 14; i < 19; i++) {
    Serial.print("pin");
    Serial.print("i");
    Serial.print(" ** ");
    Serial.println(digitalRead(i));
  }
  //Forward condition
  if (!digitalRead(14) && !digitalRead(15) && digitalRead(16) && !digitalRead(17) && !digitalRead(18)) {
    path += 'S';
    front();
  }
  //Reverse condition
  if (!digitalRead(15) && !digitalRead(16) && !digitalRead(17)) {
    m1.run(FORWARD);
    m2.run(FORWARD);
    delay(1000);
    if (digitalRead(15) && !digitalRead(16) && digitalRead(17)) {
      path += 'B';
      back();
    } else {
      m1.run(BACKWARD);
      m2.run(BACKWARD);
      delay(1000);
    }
  }
  // LEFT/LEFT-T CONDITION
  if (digitalRead(14) && digitalRead(15) && digitalRead(16)) {
    if (path[0] == 'R') { right(); }
    if (path[0] == 'L') { left(); }
    if (path[0] == 'S') { front(); }
    Serial.println("Check LEFT I &/ Move LEFT");
    Serial.println("Move LEFT");
    path += 'L';
    left();
  }
  //RIGHT/RIGHT-T
  if (digitalRead(16) && digitalRead(17) && digitalRead(18)) {
    Serial.println("Check RIGHT T &/ Move RIGHT");
    path += 'R';
    right();
  }
  //Cross Condition
  if (digitalRead(14) && digitalRead(15) && digitalRead(16) && digitalRead(17) && digitalRead(18)) {
    Serial.println("");
    left();
  }
  //Move slight left condition
  if (digitalRead(14) || digitalRead(15)) {
    Serial.println("move slight left");
    left();
  }
  //Move slight Right Condition
  if (digitalRead(17) || digitalRead(18)) {
    Serial.println("move slight right");
    right();
  }

  if(digitalRead(2) == 1)
  {
    ShortPath();
    go();
  }
}
}

String ShortPath() {
  String path = "LBLLLBSBLLBSLL";

  path.replace("LBL", "S"); 
  path.replace("LBS", "R");
  path.replace("RBL", "B");
  path.replace("SBS", "B");
  path.replace("SBL", "R");
  path.replace("LBR", "B");
  return path;
}

void go()
{
  unsigned int i=0;
  // Follow the outpath
  if (!digitalRead(14) && !digitalRead(15) && digitalRead(16) && !digitalRead(17) && !digitalRead(18)) {
    if (path.charAt(i)== 'L') {
      left();
      i++;
    }

    if (path.charAt(i)== 'R') {
      right();
      i++;
    }

    if (path.charAt(i)== 'S') {
      front();
      i++;
    }
  }

  if (!digitalRead(14) && !digitalRead(15) && digitalRead(16) && !digitalRead(17) && !digitalRead(18)) {
    front();
  } 

  if (!digitalRead(14) && !digitalRead(15) && !digitalRead(16) && !digitalRead(17) && !digitalRead(18)) {
    halt();
  }
}

void front() {
  m1.run(FORWARD);
  m2.run(FORWARD);
  delay(100);
  m1.run(RELEASE);
  m2.run(RELEASE);
  delay(1);
}
void back() {
  m1.run(BACKWARD);
  m2.run(BACKWARD);
  delay(100);
  m1.run(RELEASE);
  m2.run(RELEASE);
  delay(1);
}
void right() {
  m1.run(FORWARD);
  m2.run(BACKWARD);
  delay(100);
  m1.run(RELEASE);
  m2.run(RELEASE);
  delay(1);
}
void left() {
  m1.run(BACKWARD);
  m2.run(FORWARD);
  delay(100);
  m1.run(RELEASE);
  m2.run(RELEASE);
  delay(1);
}
void halt() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  delay(10);
}