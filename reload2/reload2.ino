#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_SleepyDog.h>
#include <Adafruit_MLX90614.h>
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

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
    } else {
      light_set(i, 0, 0, 0);
    }
  }
  light_load();
  Serial.print(".");
  switch_update();
//  laser_enable();
//  laser_update();
}

void setup_serial() {
  while (!Serial) delay(10);
  Serial.begin(9600);
  print("Setup Serial");
}

void print(String s){
  Serial.println(s);
}
