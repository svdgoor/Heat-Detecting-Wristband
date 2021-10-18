#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_SleepyDog.h>
#include <Adafruit_MLX90614.h>
#include <notes.h>

const Adafruit_MLX90614 IR_SENSOR = Adafruit_MLX90614();
#define IR_PIN 0

void ir_setup() {

  Serial.println("Starting IR setup...");
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, HIGH);

  while (!IR_SENSOR.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    
    delay(100);
  };

  Serial.println("IR Emissivity = " + String(IR_SENSOR.readEmissivity()));
  Serial.println("Successively (re)connected to Infrared Sensor");
}

double ir_get_object_temp() {
  return IR_SENSOR.readObjectTempC();
}

double ir_get_sensor_temp() {
  return IR_SENSOR.readAmbientTempC();
}

void ir_enable() {
  digitalWrite(IR_PIN, HIGH);
  setup();
}

void ir_disable() {
  digitalWrite(IR_PIN, LOW);
}

// CircuitPlayground.playTone(tone, ms);

bool switched = false;
bool switch_on = false;
void switch_update() {
  if (CircuitPlayground.slideSwitch()){
    if (!switch_on) {
      switched = true;
    } else {
      switched = false;
    }
    switch_on = true;
  } else {
    if (switch_on) {
      switched = true;
    } else {
      switched = false;
    }
    switch_on = false;
  }
}

float accel_get_x() {
  return CircuitPlayground.motionX();
}
float accel_get_y() {
  return CircuitPlayground.motionY();
}
float accel_get_z() {
  return CircuitPlayground.motionZ();
}

#define LASER_MAX_MS 5000
#define LASER_PIN 1
long laser_start = -1;

void light_set(int number, int r, int g, int b) {
  CircuitPlayground.strip.setPixelColor(number, r, g, b);
}
void light_set_all(int r, int g, int b) {
  for (int i = 0; i <= 10; i++){
    light_set(i, r, g, b);
  }
  light_load();
}
void light_load() {
  CircuitPlayground.strip.show();
}
void light_brightness(int brightness){ 
  CircuitPlayground.strip.setBrightness(brightness);
}

void laser_enable() {
  Serial.println("Enabling laser for " + String(LASER_MAX_MS) + " ms");
  laser_start = millis();
  digitalWrite(LASER_PIN, HIGH);
}
void laser_update() {
  if (laser_start == -1) return;
  if (millis() - laser_start > LASER_MAX_MS) {
    Serial.println("Shutdown laser because timeout");
    digitalWrite(LASER_PIN, LOW);
    laser_start = -1;
  }
}

#define TAP_EXPIRE_MS 2000
#define DTAP_EXPIRE_MS 1500
#define TAP_THRESHOLD 30

unsigned long tap_tapt = 0;
unsigned long tap_dtapt = 0;
void tap_tap() { 
  Serial.println("Tapped! MS " + String(millis()) + " | TAPT " + String(tap_tapt) + " (d: " + String(millis()-tap_tapt) + ") | DTAPT " + String(tap_dtapt));
  if (tap_tapt != 0 && millis() - tap_tapt < TAP_EXPIRE_MS) {
      tap_tapt = 0;
      tap_dtapt = millis();
      Serial.println("DTap_tapt set to: " + String(tap_dtapt));
      return;
  }
  tap_tapt = millis();
  Serial.println("Tap_tapt set to: " + String(tap_tapt));
}
bool tap_tapped() {
  if (tap_tapt != 0 && millis() - TAP_EXPIRE_MS < tap_tapt) {
    return true;
  } else {
    tap_tapt = 0;
    return false;
  }
}
bool tap_dtapped() {
  if (tap_dtapt != 0 && millis() - DTAP_EXPIRE_MS < tap_dtapt) {
    return true;
  } else {
    tap_dtapt = 0;
    return false;
  }
}
void tap_clear() {
  tap_tapt = 0;
  tap_dtapt = 0;
}

void setup() {
  setup_serial();
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  CircuitPlayground.setAccelTap(1, TAP_THRESHOLD);
  attachInterrupt(digitalPinToInterrupt(7), tap_tap, FALLING);
  pinMode(LASER_PIN, OUTPUT);
  Serial.println("Setup doneish");
  ir_setup();
}

void loop() {
  float t = ir_get_object_temp();
  Serial.println(String(t));
  for (int i = 0; i < 10; i++){ 
    if (t - 18 > i) {
      light_set(i, 64, 64, 64);
    }
  }
  light_load();
  Serial.print(".");
  switch_update();
}

void setup_serial() {
  while (!Serial) delay(10);
  Serial.begin(9600);
  print("Setup Serial");
}

void print(String s){
  Serial.println(s);
}
