// Motor pins
#define AIN1 2
#define AIN2 3
#define PWMA 5

#define STBY 4

#define PWMB 6
#define BIN1 7
#define BIN2 8

// Sensor pins: S1..S5 = left..right
int sensors[5] = {A0, A1, A2, A3, A4};
int weights[5] = {-2.8, -1, 0, 1, 2.8};

// Tuning
int whiteLevel[5] = {950, 950, 950, 950, 920}; 
int threshold = 230;

float Kp = 130.0;
float Ki = 0.0;
float Kd = 50.0;

int baseSpeed = 140;
int maxSpeed = 280; 

float lastError = 0;
float integral = 0;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  Serial.begin(9600);
}

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // Left side
  if (leftSpeed >= 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    leftSpeed = -leftSpeed;
  }

  // Right side
  if (rightSpeed >= 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  } else {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    rightSpeed = -rightSpeed;
  }

  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
}

void loop() {

  int s1 = analogRead(A0);
  int s2 = analogRead(A1);
  int s3 = analogRead(A2);
  int s4 = analogRead(A3);
  int s5 = analogRead(A4);

  Serial.print(s1);
  Serial.print("  ");

  Serial.print(s2);
  Serial.print("  ");

  Serial.print(s3);
  Serial.print("  ");

  Serial.print(s4);
  Serial.print("  ");

  Serial.println(s5);

  delay(100);

  long weightedSum = 0;
  long totalBlack = 0;

  for (int i = 0; i < 5; i++) {
    int value = analogRead(sensors[i]);

    // schwarz = kleiner Wert, weiss = ca. 900
    int black = whiteLevel[i] - value;
    black = constrain(black, 0, 900);

    // kleine Störungen ignorieren
    if (black < threshold) black = 0;

    weightedSum += (long)black * weights[i];
    totalBlack += black;
  }

  // Linie verloren
  if (totalBlack == 0) {
    setMotors(0, 0);
    return;
  }

  float position = (float)weightedSum / totalBlack;

  // Ziel: Linie in der Mitte -> position = 0
  float error = position;

  integral += error;
  integral = constrain(integral, -50, 50);

  float derivative = error - lastError;
  lastError = error;

  float correction = Kp * error + Ki * integral + Kd * derivative;

  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  setMotors(leftSpeed, rightSpeed);

  Serial.print("pos: ");
  Serial.print(position);
  Serial.print(" error: ");
  Serial.print(error);
  Serial.print(" corr: ");
  Serial.println(correction);

  delay(10);
}


