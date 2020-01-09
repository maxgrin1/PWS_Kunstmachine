#include <math.h>

#define enBottom 7
#define stepBottom 6
#define dirBottom 5
#define enTop 4
#define dirTop 3
#define stepTop 2

#define endSwitchMid 9
#define endSwitchOut 8
#define endSwitchRot 10
int pulseWidthDelay = 1;
int speedDelay = 10;

int mid = 0;
int out = 0;
int len = 0;
int rot = 0;

float Ar = 0;
float Aa = 0;

float Br = 0;
float Ba = 0;
int n = 50;
float k = 2000;
boolean dirM, dirA;
float DeltaM[100];
float DeltaA[100];

float m1 = 0;
float a1 = 0;
  

float radMultiplier = PI/180;
float degMultiplier = 180/PI;

void setup() {
  Serial.begin(9600);
  pinMode(dirBottom, OUTPUT);
  pinMode(stepBottom, OUTPUT);
  pinMode(enBottom, OUTPUT);
  digitalWrite(enBottom, LOW);

  pinMode(dirTop, OUTPUT);
  pinMode(stepTop, OUTPUT);
  pinMode(enTop, OUTPUT);
  digitalWrite(enTop, LOW);

  pinMode(endSwitchMid, INPUT);
  pinMode(endSwitchOut, INPUT);
  delay(1000);
  goHome();
  delay(1000);

  drawStar();
}
/*
  Bottom 14 x 200 = 2800 steps/rev  0
  Top    200 x 4  = 800  steps/rev  1
*/
//Length arm  =  5391 steps at 800 steps/rev

void loop() {


}


void steps(int numSteps, boolean dir, boolean bottomOrtop) {
  //Example: steps(800, 0, 0); for 800 steps, clockwise and steppermotor 0.
  if (bottomOrtop == 0) {
    if (dir == 0) {
      digitalWrite(dirBottom, LOW);
    } else digitalWrite(dirBottom, HIGH);
    for (int c = 0; c < numSteps; c++) {
      digitalWrite(stepBottom, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepBottom, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelay);
    }
  }
  if (bottomOrtop == 1) {
    if (dir == 0) {
      digitalWrite(dirTop, LOW);
    } else digitalWrite(dirTop, HIGH);
    for (int c = 0; c < numSteps; c++) {
      Serial.println(c);
      digitalWrite(stepTop, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepTop, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelay);
    }
  }
}
void stepsSafeMode(int numSteps, boolean dir, boolean bottomOrtop) {
  //Example: steps(800, 0, 0); for 800 steps, clockwise and steppermotor 0.
  if (bottomOrtop == 0) {
    if (dir == 0) {
      digitalWrite(dirBottom, LOW);
    } else digitalWrite(dirBottom, HIGH);
    for (int c = 0; c < numSteps; c++) {
      checkSwitches();
      if(rot == 1){
        break;
      }
      digitalWrite(stepBottom, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepBottom, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelay);
    }
  }
  if (bottomOrtop == 1) {
    if (dir == 0) {
      digitalWrite(dirTop, LOW);
    } else digitalWrite(dirTop, HIGH);
    for (int c = 0; c < numSteps; c++) {
      checkSwitches();
      if (mid == 0) {
        steps(50, 0, 1);
        break;
        checkSwitches();
      } else if (out == 0) {
        len = c;
        steps(50, 1, 1);
        break;
        checkSwitches();
      }
      digitalWrite(stepTop, HIGH);
      delayMicroseconds(pulseWidthDelay);
      digitalWrite(stepTop, LOW);
      delayMicroseconds(pulseWidthDelay);
      delay(speedDelay);
    }
  }
}
void checkSwitches() {
  mid = digitalRead(endSwitchMid);
  out = digitalRead(endSwitchOut);
  rot = digitalRead(endSwitchRot);
  //  Serial.print("Mid: ");
  //  Serial.println(mid);
  //  Serial.print("Out: ");
  //  Serial.println(out);
}

void goHome() {
  stepsSafeMode(9999, 1, 1);
  stepsSafeMode(9999, 1, 0);
}

void calibration() {
  goHome();
  delay(100);
  stepsSafeMode(9999, 0, 1);
  Serial.print("Arm length = ");
  Serial.println(len);
}

void cutLine(float n, float Ar, float Aa, float Br, float Ba) {
  float x1 = 0;
  float y1 = 0;
  float da = 0;
  float aStepsP = 0;
  float xp, yp, m, a, dm, aSteps;
  Ar = map(Ar, 0, Ar, 0, Ar); //Getallen ter aanpassing voor later maar voor ik het vergeet.
  Br = map(Br, 0, Br, 0, Br);
  Aa = Aa * (PI / 180); //Convert degrees to radians
  Ba = Ba * (PI / 180);
  for (int i = 0; i <= n; i++) {
    xp = Ar * cos(Aa) + ((1 / (n)) * i) * Br * cos(Ba) - ((1 / (n)) * i) * Ar * cos(Aa);
    yp = Ar * sin(Aa) + ((1 / (n)) * i) * Br * sin(Ba) - ((1 / (n)) * i) * Ar * sin(Aa);
    m = sqrt(pow(xp, 2) + pow(yp, 2));
    a = acos(xp / m);
//            if(i == 1|| i == 0){
//      Serial.println(a);
//      Serial.println(yp);
//    }
    if (yp < 0) {
      a = 360 - a;
    }
      a = a * degMultiplier;
//    if(i == 1 || i == 0){
//      Serial.println(a);
//      Serial.println(da);
//    }
    dm = m - m1;
    da = a - a1;
//    if(i==1|| i == 0){
//      Serial.println(da);
//    }
    m1 = m;
    a1 = a;
    aSteps = (da/360)*2800;
    if(aSteps > 5000 || aSteps < -5000){
      aSteps = aStepsP;
    }
    aStepsP = aSteps;
    DeltaM[i] = round(dm);
    DeltaA[i] = round(aSteps);
    //Available as outputs are: the delta's m&a and the lengts
//    Serial.print("m = ");
//    Serial.println(dm);
//    Serial.print("a = ");
//    Serial.print(a);
//    Serial.print(" "); 
//    Serial.print("m = ");
//    Serial.print(m);
//    Serial.print(" ");
//    Serial.print("da = ");
//    Serial.print(da);
//    Serial.print(" "); 
//    Serial.print("dm = ");
//    Serial.print(dm); 
//    Serial.print(" "); 
    Serial.print("aSteps = ");
    Serial.println(aSteps);
   
   
  }
 draw();
}

void draw() {
  for (int i = 0; i < n; i++) {
    Serial.print("KAAAAS");
    if (DeltaM[i] < 0) {
      dirM = 1;
      DeltaM[i] = DeltaM[i]*-1;
    } else dirM = 0;

    if (DeltaA[i] < 0) {
      dirA = 1;
      DeltaA[i] = DeltaA[i]*-1;
    } else dirA = 0;
    Serial.println(DeltaA[i]);
    stepsSafeMode(DeltaM[i], dirM, 1);
    steps(DeltaA[i], dirA, 0);

  }
}

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-DRAWINGS*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void drawHouse(){
  cutLine(n, 0, 0, k, 0);
  delay(500);
  cutLine(n, k, 0, sqrt(2*pow(k,2)), 45);
  delay(500);
  cutLine(n, sqrt(2*pow(k,2)), 45, 2*k, 90);
  delay(500);
  cutLine(n, 2*k, 90, sqrt(2*pow(k,2)), 135);
  delay(500);
  cutLine(n, sqrt(2*pow(k,2)), 135, k, 180);
  delay(500);
  cutLine(n, k, 180, 0, 180);
}
//  stepsSafeMode(9999, 1, 1);
 // stepsSafeMode(9999, 1, 0);
void drawSpiral(){
  int p = 5000;
  cutLine(n, 0, 0, p, 0);
  delay(500);
  steps(100, 1, 0);
  delay(100);
  stepsSafeMode(9999, 1, 0);
  delay(500);
  stepsSafeMode(300, 1, 1);
  delay(500);
  steps(100, 0, 0);
  delay(100);
  stepsSafeMode(9999, 0, 0);
  delay(500);
  stepsSafeMode(300, 1, 1);
  delay(500);
  steps(100, 1, 0);
  delay(100);
  stepsSafeMode(9999, 1, 0);
  delay(500);
  stepsSafeMode(300, 1, 1);
  delay(500);
  steps(100, 0, 0);
  delay(100);
  stepsSafeMode(9999, 0, 0);
  delay(500);
  stepsSafeMode(300, 1, 1);
  delay(500);
  steps(100, 1, 0);
  delay(100);
  stepsSafeMode(9999, 1, 0);
  delay(500);
  stepsSafeMode(300, 1, 1);
  delay(500);
  steps(100, 0, 0);
  delay(100);
  stepsSafeMode(9999, 0, 0);
  goHome();
}
void randomShizzel(){
float Aar = 0;
float Aaa = 0;
float Bbr = 0;
float Bba = 0;

float angles[] = {0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77, 360.00, 343.77, 309.4, 275.02, 240.64, 206.26, 171.89, 137.51, 103.13, 68.75, 34.38, 0.0, 34.38, 68.75, 103.13, 137.51, 171.89, 206.26, 240.64, 275.02, 309.4, 343.77};
float distances[] = {400.0, 399.68, 398.71, 397.12, 394.89, 392.06, 388.63, 384.63, 380.09, 375.03, 369.49, 363.5, 369.49, 375.03, 380.09, 384.63, 388.63, 392.06, 394.89, 397.12, 398.71, 399.68, 400.0, 399.68, 398.71, 397.12, 394.89, 392.06, 388.63, 384.63, 380.09, 375.03, 369.49, 363.5, 369.49, 375.03, 380.09, 384.63, 388.63, 392.06, 394.89, 397.12, 398.71, 399.68, 400.0, 399.68, 398.71, 397.12, 394.89, 392.06, 388.63, 384.63, 380.09, 375.03, 369.49, 363.5, 369.49, 375.03, 380.09, 384.63, 388.63, 392.06, 394.89, 397.12, 398.71, 399.68, 400.0, 399.68, 398.71, 397.12, 394.89, 392.06, 388.63, 384.63, 380.09, 375.03, 233.09, 364.62, 342.74, 200.33, 212.37, 275.41, 263.65, 397.14, 209.88, 200.08, 353.88, 265.02, 400.0, 386.85, 344.49, 252.37, 270.83, 202.87, 321.69, 200.32, 223.07, 254.16, 209.97, 392.72, 280.99, 204.08, 399.58, 201.28, 207.99, 323.1, 301.32, 327.72, 346.04, 280.59, 400.0, 320.38, 396.87, 356.71, 350.03, 200.86, 282.39, 305.5, 373.01, 286.33, 221.9, 206.41, 345.32, 390.24, 219.92, 294.67, 308.79, 264.08, 336.65, 305.14, 310.29, 267.97, 400.0, 288.04, 382.97, 234.61, 219.02, 393.46, 212.29, 239.15, 399.06, 320.13, 201.43, 222.32, 200.03, 251.12, 202.36, 222.27, 343.16, 237.05, 212.7, 396.69, 227.68, 353.18, 400.0, 391.3, 339.63, 246.59, 202.67, 202.25, 365.84, 387.27, 248.58, 294.62, 360.54, 341.2, 296.61, 200.96, 264.05, 394.97, 242.48, 354.54, 397.97, 302.85, 232.53, 330.58, 400.0, 232.71, 302.44, 398.92, 385.21, 200.72, 398.81, 271.18, 302.2, 221.42, 207.45, 200.07, 204.07, 396.02, 200.37, 259.02, 374.94, 276.45, 322.43, 399.07, 228.55, 290.18, 400.0, 399.58, 399.41, 212.69, 201.3, 366.18, 397.67, 263.27, 269.46, 235.2, 370.82, 283.37, 390.96, 328.93, 385.43, 237.02, 237.32, 379.76, 218.85, 363.26, 212.02, 387.45, 400.0, 323.98, 301.87, 250.06, 212.24, 233.6, 256.88, 350.8, 302.2, 290.88, 297.42};

for(int i = 0; i < sizeof(distances); i++){
  distances[i] = map(distances[i], 0, 400, 0, 3500);
}

for(int i = 0; i < sizeof(distances); i++){
  Bbr = distances[i];
  Bba = angles[i];
  
  cutLine(n, Ar, Aa, Br, Ba);
  
  Aar = Bbr;
  Aaa = Bba; 
}
}
void drawSquare(){
  float q = 4000;
  cutLine(n, 0, 0, q ,0);
  cutLine(n, q, 0, sqrt(2*pow(q,2)), 45);
  cutLine(n, sqrt(2*pow(q,2)), 45, q, 90);
  cutLine(n, q, 90, 0, 0);
  cutLine(n, 0,0,sqrt(2*pow(q,2)), 45);
  cutLine(n, sqrt(2*pow(q,2)), 45, q, 90);
  cutLine(n, q, 90, q, 0);
}

void drawMm(){
  float p = 3000;
  cutLine(n, 0, 0, 1, 90);
  cutLine(n, 1, 90, p, 90);
  cutLine(n, p, 90, sqrt(1.25*pow(p, 2)), 67.5);
  cutLine(n, sqrt(1.25*pow(p, 2)), 67.5, 0.5*p, 0);
  cutLine(n, 0.5*p, 0, sqrt(1.25*pow(p, 2)), 67.5);
  cutLine(n, sqrt(1.25*pow(p, 2)), 67.5, sqrt(2*pow(p, 2)), 45);
  cutLine(n, sqrt(2*pow(p, 2)), 45, p, 0); 
}

void drawHex(){
  float q = 4000;
  steps(234, 0, 0);
  cutLine(n, 0, 0, q, 0); //0=30)
  cutLine(n, q, 0, q, 60);
  cutLine(n, q, 60, q, 120);
  cutLine(n, q, 120, q, 180);
  cutLine(n, q, 180, q, 240);
  cutLine(n, q, 240, q, 300);
  cutLine(n, q, 300, q, 360);
  steps(1000, 1, 0);
  stepsSafeMode(9999, 1, 0); 
}

void drawStar(){
  n = 70;
  float q = 3000;
  steps(234, 0, 0);
  cutLine(n, 0, 0, q, 0);
  cutLine(n, q, 0, q, 120);
  cutLine(n, q, 120, q, 240);
  cutLine(n, q, 240, q, 0);
  steps(467, 1, 0);
  cutLine(n, q, 0, q, 240);
  cutLine(n, q, 240, q, 120);
  cutLine(n, q, 120, q, 0);
  cutLine(n, q, 0, q+50, 0);
  stepsSafeMode(9999, 1, 0);
  
}


