#include <ESP32Servo.h>
#include <Bluepad32.h>

Servo escLeft;    // Left wheel motor
Servo escRight;   // Right wheel motor
Servo escWeapon;  // weapon motor

const int driftoffset = 30;  // amount to offset joystick drift by
bool controllerConnected = false;
const float sensitivityPercentage = 1.5;
bool inverted = false;

// Motor control pins
const int leftPin = D9;
const int rightPin = D10;
const int weaponPin = D8;

// Controller pointer
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// ESC initialization flag
bool escArmed = false;

// Function prototypes
void processJoysticks(ControllerPtr ctl);
void processButtons(ControllerPtr ctl);
void armESC();
void stop();

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);

  escLeft.attach(leftPin, 1000, 2000);    // Attach the ESC for the left wheel
  escRight.attach(rightPin, 1000, 2000);  // Attach the ESC for the right wheel
  escWeapon.attach(weaponPin, 1000, 2000);

  // Initialize Bluepad32
  BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
  // Update controller data
  bool dataUpdated = BP32.update();
  if (controllerConnected == true) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }

  // Process controller input
  if (dataUpdated) {
    for (auto ctl : myControllers) {
      if (ctl && ctl->isConnected()) {
        if (!escArmed) {
          armESC();
          escArmed = true;
        }
        processJoysticks(ctl);
        processButtons(ctl);
      }
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
  Serial.println("Controller connected");
}

void onDisconnectedController(ControllerPtr ctl) {
  stop();
  controllerConnected = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      break;
    }
  }
  Serial.println("Controller disconnected");
}

void processButtons(ControllerPtr ctl) {
  if (ctl->a()) {
    escWeapon.writeMicroseconds(1000);
  } else if (ctl->b()) {
    escWeapon.writeMicroseconds(1500);
  } else if (ctl->y()) {
    escWeapon.writeMicroseconds(1750);
  } else if (ctl->x()) {
    escWeapon.writeMicroseconds(2000);
  }
}

void processJoysticks(ControllerPtr ctl) {
  // Control the wheels using the joystick
  int leftyAxis = ctl->axisY();    // Y-axis for forward/backward
  int rightxAxis = ctl->axisRX();  // X-axis for left/right rotation
  int rightyAxis = ctl->axisRY();  // Right stick Y (unused currently)

  int processedRight;
  int processedLeft;
  int mappedRight;
  int mappedLeft;

  // Calculate motor values based on joystick input
  processedLeft = (leftyAxis - (rightxAxis * sensitivityPercentage));
  processedRight = (leftyAxis + (rightxAxis * sensitivityPercentage));

  // Apply deadzone/drift offset
  if (abs(processedLeft) < driftoffset) {
    mappedLeft = 1500;  // Neutral position
  } else {
    mappedLeft = map(processedLeft, -512, 512, 1000, 2000);
  }

  if (abs(processedRight) < driftoffset) {
    mappedRight = 1500;  // Neutral position
  } else {
    mappedRight = map(processedRight, -512, 512, 1000, 2000);
  }

  Serial.print("Left: ");
  Serial.print(processedLeft);
  Serial.print(" | Right: ");
  Serial.println(processedRight);

  escLeft.writeMicroseconds(mappedLeft);
  escRight.writeMicroseconds(mappedRight);
}

void stop() {
  escLeft.writeMicroseconds(1500);
  escRight.writeMicroseconds(1500);
  escWeapon.writeMicroseconds(1500);
}

// Function to arm the ESC (if necessary)
void armESC() {
  // Send a low signal to the ESC for arming
  Serial.println("Arming ESC...");
  escLeft.writeMicroseconds(1000);
  escRight.writeMicroseconds(1000);
  escWeapon.writeMicroseconds(1000);

  delay(2000);  // Wait 2 seconds
  Serial.println("ESC armed.");
}
