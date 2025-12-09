// --- Pin Definitions ---
// Motor A (Left)
#define enA 3
#define in1 4
#define in2 5

// Motor B (Right)
#define enB 9
#define in3 7
#define in4 6

// Ultrasonic Sensor Pins (New)
#define trigPin 11
#define echoPin 10

// --- Settings ---
int motorSpeed = 200;
int turnTimeFor90Deg = 600; // Calibrate this for your specific robot
int obstacleThreshold = 10; // Distance in cm to trigger avoidance

//this function runs ONCE every restart/powerup
void setup() {
  // Motor pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Debugging
  Serial.begin(115200);

  stopMotors();
  delay(2000); // 2 second startup delay to get ready
}

//this function runs REPEATEDLY after setup() completes
void loop() { // it is a while(true) loop
  // 1. Measure distance
  int distance = measureDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  // 2. Logic: Avoid or Move Forward
  // We check if distance is > 0 (valid reading) and < threshold
  if (distance > 0 && distance < obstacleThreshold) {
    // Obstacle detected!
    stopMotors();
    delay(500); // Short pause
    
    // Rotate 90 degrees to avoid
    rotate90Clockwise();
    stopMotors();
    delay(500); // Wait a moment before moving again
    
  } else {
    // Path is clear
    goForward();
  }
  
  delay(100); // Small delay for sensor stability
}

// --- FUNCTIONS ---

// Function to measure distance in cm
int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the duration of the echo
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance (speed of sound is ~343m/s)
  int distanceCm = duration * 0.034 / 2;
  
  return distanceCm;
}

// Motor functions

void goForward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, motorSpeed);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, motorSpeed);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void rotate90Clockwise() {
  // Motor A Forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, motorSpeed);

  // Motor B Backward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, motorSpeed);

  delay(turnTimeFor90Deg);
}

void rotate90CounterClockwise() {
  // Motor A Backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, motorSpeed);

  // Motor B Forward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, motorSpeed);

  delay(turnTimeFor90Deg);
}