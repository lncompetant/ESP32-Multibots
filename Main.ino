#include <ESP32Servo.h>
#include <Bluepad32.h>

Servo escLeft;
Servo escRight;
Servo escWeapon;

const int driftoffset = 30;
bool controllerConnected = false;
const float sensitivityPercentage = 1.2; // Adjusted for better control

// Motor control pins 
const int leftPin = 9;   // Using standard GPIO numbers is safer
const int rightPin = 10;
const int weaponPin = 11; // Added weapon pin

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
bool escArmed = false;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Allow for high-frequency PWM if using ESP32Servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  
  escLeft.attach(leftPin, 1000, 2000);
  escRight.attach(rightPin, 1000, 2000);
  escWeapon.attach(weaponPin, 1000, 2000); // Now attached

  BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
  BP32.update();
  digitalWrite(2, controllerConnected ? HIGH : LOW);

  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected()) {
      if (!escArmed) {
        armESC();
        escArmed = true;
      }
      processJoysticks(ctl);
    }
  }
  delay(20);
}

void onConnectedController(ControllerPtr ctl) {
  controllerConnected = true;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      break;
    }
  }
  Serial.println("Xbox Controller connected");
}

void onDisconnectedController(ControllerPtr ctl) {
  stopMotors(); // Renamed to avoid conflicts
  controllerConnected = false;
  escArmed = false; 
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      break;
    }
  }
  Serial.println("Controller disconnected");
}

void processJoysticks(ControllerPtr ctl) {
  int y = ctl->axisY();   // Forward/Backward
  int x = ctl->axisRX();  // Steering

  // Simple Arcade Drive Mixing
  int leftVal = y - x;
  int rightVal = y + x;

  // Constrain values to prevent map overflow
  leftVal = constrain(leftVal, -512, 512);
  rightVal = constrain(rightVal, -512, 512);

  // Apply deadzone
  int mappedLeft = (abs(leftVal) < driftoffset) ? 1500 : map(leftVal, -512, 512, 1000, 2000);
  int mappedRight = (abs(rightVal) < driftoffset) ? 1500 : map(rightVal, -512, 512, 1000, 2000);

  escLeft.writeMicroseconds(mappedLeft);
  escRight.writeMicroseconds(mappedRight);

  // Weapon controls
  if(ctl->a()) escWeapon.writeMicroseconds(1750);      //half throttle
  else if(ctl->y()) escWeapon.writeMicroseconds(2000); //full throttle
  else if(ctl->x()) escWeapon.writeMicroseconds(1000); //full reverse
  else if(ctl->b()) escWeapon.writeMicroseconds(1500); //stop
}

void stopMotors() {
  escLeft.writeMicroseconds(1500);
  escRight.writeMicroseconds(1500);
  escWeapon.writeMicroseconds(1500);
}

void armESC() {
  Serial.println("Arming...");
  stopMotors(); // Most ESCs arm at neutral (1500)
  delay(2000);
  Serial.println("Armed.");
}
