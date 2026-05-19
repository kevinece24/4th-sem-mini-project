#include <AFMotor.h>

AF_DCMotor m1(2);
AF_DCMotor m2(3);

String path = "";

int index = 0;
int mode = 0;

// =====================================================
// SENSOR ARRANGEMENT
//
//                16
//
// 14        15        17        18

void setup() {

  Serial.begin(9600);

  for (int i = 14; i < 19; i++) {
    pinMode(i, INPUT);
  }

  pinMode(2, INPUT_PULLUP);

  m1.setSpeed(180);
  m2.setSpeed(180);
}

void loop() {

  // BUTTON PRESSED -> SHORTEST PATH MODE
  if (digitalRead(2) == LOW) {

    optimizePath();

    Serial.println("Optimized Path:");
    Serial.println(path);

    mode = 1;

    delay(1000);
  }

  if (mode == 0) {
    explore();
  }

  if (mode == 1) {
    replay();
  }
}

void explore() {

  int s1 = digitalRead(14);
  int s2 = digitalRead(15);
  int s3 = digitalRead(16);
  int s4 = digitalRead(17);
  int s5 = digitalRead(18);

  // SENSOR VALUES

  Serial.print(s1);
  Serial.print(" ");

  Serial.print(s2);
  Serial.print(" ");

  Serial.print(s3);
  Serial.print(" ");

  Serial.print(s4);
  Serial.print(" ");

  Serial.println(s5);

  // CROSS JUNCTION

  if (s3 && ((s1 && s2) && (s4 && s5))) {

    Serial.println("CROSS");

    path += 'L';

    left();
  }

  // LEFT TURN / LEFT T

  else if (s3 && (s1 || s2)) {

    Serial.println("LEFT TURN");

    path += 'L';

    left();
  }

  // RIGHT TURN / RIGHT T

  else if (s3 && (s4 || s5)) {

    Serial.println("RIGHT TURN");

    path += 'R';

    right();
  }

  // FORWARD

  else if (!s1 && !s2 && s3 && !s4 && !s5) {

    front();
  }
  // SLIGHT LEFT

  else if (s2) {

    slightLeft();
  }

  // SLIGHT RIGHT

  else if (s4) {

    slightRight();
  }

  // DEAD END

  else if (!s1 && !s2 && !s3 && !s4 && !s5) {

    delay(30);

    s1 = digitalRead(14);
    s2 = digitalRead(15);
    s3 = digitalRead(16);
    s4 = digitalRead(17);
    s5 = digitalRead(18);

    if (!s1 && !s2 && !s3 && !s4 && !s5) {

      Serial.println("DEAD END");

      path += 'B';

      uTurn();
    }
  }
}

// OPTIMIZE PATH

void optimizePath() {

  bool change = true;

  while (change) {

    change = false;

    if (path.indexOf("LBL") >= 0) {
      path.replace("LBL", "S");
      change = true;
    }

    if (path.indexOf("LBS") >= 0) {
      path.replace("LBS", "R");
      change = true;
    }

    if (path.indexOf("RBL") >= 0) {
      path.replace("RBL", "B");
      change = true;
    }

    if (path.indexOf("SBS") >= 0) {
      path.replace("SBS", "B");
      change = true;
    }

    if (path.indexOf("SBL") >= 0) {
      path.replace("SBL", "R");
      change = true;
    }

    if (path.indexOf("LBR") >= 0) {
      path.replace("LBR", "B");
      change = true;
    }
  }
}

// REPLAY SHORTEST PATH

void replay() {

  int s1 = digitalRead(14);
  int s2 = digitalRead(15);
  int s3 = digitalRead(16);
  int s4 = digitalRead(17);
  int s5 = digitalRead(18);

  // NORMAL LINE FOLLOWING

  if (!s1 && !s2 && s3 && !s4 && !s5) {

    front();
  }

  else if (s2) {

    slightLeft();
  }

  else if (s4) {

    slightRight();
  }

  // INTERSECTION

  else if (s3 && ((s1 || s2) || (s4 || s5))) {

    if (index < path.length()) {

      char move = path.charAt(index);

      Serial.print("MOVE: ");
      Serial.println(move);

      if (move == 'L') {
        left();
      }

      else if (move == 'R') {
        right();
      }

      else if (move == 'S') {
        front();
      }

      else if (move == 'B') {
        uTurn();
      }

      index++;
    }
  }

  // END OF MAZE

  else if (!s1 && !s2 && !s3 && !s4 && !s5 && index >= path.length()) {

    halt();
  }
}

 // MOTOR CONTROLS

void front() {

  m1.run(FORWARD);
  m2.run(FORWARD);
}

void back() {

  m1.run(BACKWARD);
  m2.run(BACKWARD);
}

void right() {

  m1.run(FORWARD);
  m2.run(BACKWARD);

  delay(280);
}

void left() {

  m1.run(BACKWARD);
  m2.run(FORWARD);

  delay(280);
}

void uTurn() {

  m1.run(FORWARD);
  m2.run(BACKWARD);

  delay(550);
}

void slightLeft() {

  m1.setSpeed(120);
  m2.setSpeed(180);

  m1.run(FORWARD);
  m2.run(FORWARD);
}

void slightRight() {

  m1.setSpeed(180);
  m2.setSpeed(120);

  m1.run(FORWARD);
  m2.run(FORWARD);
}

void halt() {

  m1.run(RELEASE);
  m2.run(RELEASE);
}