#define enA 10  
#define in1 9   
#define in2 8   
#define in3 7   
#define in4 6   
#define enB 5   

#define ir_R A0 
#define ir_F A1 
#define ir_L A2 

#define servo A4 
#define pump A5  

int Speed = 120; 
int s1, s2, s3;  

// 🛠️ DISTANCE CALIBRATION VARIANCE
// Lower this number if the pump still starts too far away (e.g., try 50, 60, or 80)
// Raise this number if the car crashes into the fire without spraying.
// =================================================================
int fireCloseThreshold = 45;

void setup() {
  Serial.begin(9600); 
  
  pinMode(ir_R, INPUT); 
  pinMode(ir_F, INPUT); 
  pinMode(ir_L, INPUT); 
  
  pinMode(enA, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 
  pinMode(enB, OUTPUT); 
  
  pinMode(servo, OUTPUT); 
  pinMode(pump, OUTPUT);  
  
  digitalWrite(pump, 1); // Keep pump OFF at startup
  
  // Initial Servo Scanning Startup Routine
  for (int angle = 90; angle <= 140; angle += 5) { servoPulse(servo, angle); }
  for (int angle = 140; angle >= 40; angle -= 5) { servoPulse(servo, angle); }
  for (int angle = 40; angle <= 95; angle += 5) { servoPulse(servo, angle); }
  
  analogWrite(enA, Speed); 
  analogWrite(enB, Speed); 
  delay(500); 
}

void loop() {
  s1 = analogRead(ir_R); 
  s2 = analogRead(ir_F); 
  s3 = analogRead(ir_L); 
  
  Serial.print(s1); 
  Serial.print("\t"); 
  Serial.print(s2); 
  Serial.print("\t"); 
  Serial.println(s3); 
  delay(30); 
  
  // =================================================================
  // 1. CRITICAL CLOSE-RANGE STOP EXTINGUISH ZONE
  // The pump will ONLY engage if the fire drops below your tight threshold
  // =================================================================
  if (s2 < fireCloseThreshold) { 
    Stop(); 
    digitalWrite(pump, 0); // Turn ON pump
    
    // Extinguish spray loop
    for (int angle = 90; angle <= 140; angle += 3) { servoPulse(servo, angle); }
    for (int angle = 140; angle >= 40; angle -= 3) { servoPulse(servo, angle); }
    for (int angle = 40; angle <= 90; angle += 3) { servoPulse(servo, angle); }
  }
  
  else if (s1 < (fireCloseThreshold - 10)) { 
    Stop(); 
    digitalWrite(pump, 0); 
    for (int angle = 90; angle >= 40; angle -= 3) { servoPulse(servo, angle); }
    for (int angle = 40; angle <= 90; angle += 3) { servoPulse(servo, angle); }
  }
  
  else if (s3 < (fireCloseThreshold - 10)) { 
    Stop(); 
    digitalWrite(pump, 0); 
    for (int angle = 90; angle <= 140; angle += 3) { servoPulse(servo, angle); }
    for (int angle = 140; angle >= 90; angle -= 3) { servoPulse(servo, angle); }
  }
  
  // =================================================================
  // 2. LONG-RANGE CHASE ZONE
  // If the reading is anywhere above the close threshold up to 960, it MUST drive forward.
  // =================================================================
  else if (s2 >= fireCloseThreshold && s2 <= 960) { 
    digitalWrite(pump, 1); // Absolutely force pump to stay OFF while driving
    forword();             // Drive straight toward fire
  }
  
  else if (s1 >= (fireCloseThreshold + 1) && s1 <= 960) { 
    digitalWrite(pump, 1); 
    backword(); 
    delay(100); 
    turnRight(); 
    delay(150); 
  }
  
  else if (s3 >= (fireCloseThreshold + 1) && s3 <= 960) { 
    digitalWrite(pump, 1); 
    backword(); 
    delay(100); 
    turnLeft(); 
    delay(150); 
  }
  
  // =================================================================
  // 3. STANDBY ZONE (No fire present)
  // =================================================================
  else {
    digitalWrite(pump, 1); 
    Stop(); 
  }
}

void servoPulse(int pin, int angle) { 
  int pwm = (angle * 11) + 500; 
  digitalWrite(pin, HIGH);      
  delayMicroseconds(pwm);       
  digitalWrite(pin, LOW);       
  delay(40);                    
}

// --- DIRECTION SUB-ROUTINES ---

void forword() { 
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);  
}

void backword() { 
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH); 
}

void turnRight() { 
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH); 
}

void turnLeft() { 
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);  
}

void Stop() { 
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);  
}
