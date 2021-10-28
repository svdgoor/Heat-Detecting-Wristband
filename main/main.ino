#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_MLX90614.h>

#define SERIAL_ENABLED true

// Symbol boundaries
#define SYM_WATER 37 // Temperature boundary for water
#define SYM_TOUCH 48 // Temperature boundary for surface-touching
#define SYM_DRINK 53 // Temperature boundary for drinking
#define SYM_HOT 65   // Temperature boundary for hot
#define SYM_DANGR 80 // Temperature boundary for danger

// Active-warning
#define BLINK_TEMP 50  // Light-blinking temperature boundary
#define BUZZER_TEMP 50 // Buzzer (sound) temperature boundary

// Starting mode of the system (when you start it / upload it / press reset / recharge the battery after it died)
#define DEFAULT_MODE 0
// Mode 0 = default symbol-based warning system
// Mode 1 = advanced temperature measurement system
// Mode 2 = off, audio only

















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

  print("Starting IR setup...");
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, HIGH);

  while (!IR_SENSOR.begin()) {
    print("Error connecting to MLX sensor. Check wiring.");

    delay(100);
  };

  print("IR Emissivity = " + String(IR_SENSOR.readEmissivity()));
  print("Successively (re)connected to Infrared Sensor");
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
  if (CircuitPlayground.slideSwitch()) {
    if (!switch_on) {
      switched = true;
      ir_enable();
      print("Switched switch on!");
    } else {
      switched = false;
    }
    switch_on = true;
  } else {
    if (switch_on) {
      switched = true;
      ir_disable();
      laser_disable();
      print("Switched switch off!");
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
  for (int i = 0; i <= 10; i++) {
    light_set(i, r, g, b);
  }
  light_load();
}
void light_load() {
  CircuitPlayground.strip.show();
}
void light_brightness(int brightness) {
  CircuitPlayground.strip.setBrightness(brightness);
}

bool laser_enabled = false;
void laser_enable() {
  laser_enabled = true;
  print("Enabling laser");
  digitalWrite(LASER_PIN, HIGH);
}
void laser_disable() {
  laser_enabled = false;
  print("Disabled laser");
  digitalWrite(LASER_PIN, LOW);
}
//void laser_update(bool enable) {
//  if (enable) {
//    laser_enable();
//    return;
//  }
//  if (laser_start == -1) return;
//  if (millis() - laser_start > LASER_MAX_MS) {
//    print("Shutdown laser because timeout");
//    digitalWrite(LASER_PIN, LOW);
//    laser_start = -1;
//  }
//}

//#define TAP_EXPIRE_MS 2000
//#define DTAP_EXPIRE_MS 1500
//#define TAP_THRESHOLD 30
//
//unsigned long tap_tapt = 0;
//unsigned long tap_dtapt = 0;
//void tap_tap() {
//  print("Tapped! MS " + String(millis()) + " | TAPT " + String(tap_tapt) + " (d: " + String(millis()-tap_tapt) + ") | DTAPT " + String(tap_dtapt));
//  if (tap_tapt != 0 && millis() - tap_tapt < TAP_EXPIRE_MS) {
//      tap_tapt = 0;
//      tap_dtapt = millis();
//      print("DTap_tapt set to: " + String(tap_dtapt));
//      return;
//  }
//  tap_tapt = millis();
//  print("Tap_tapt set to: " + String(tap_tapt));
//}
//bool tap_tapped() {
//  if (tap_tapt != 0 && millis() - TAP_EXPIRE_MS < tap_tapt) {
//    return true;
//  } else {
//    tap_tapt = 0;
//    return false;
//  }
//}
//bool tap_dtapped() {
//  if (tap_dtapt != 0 && millis() - DTAP_EXPIRE_MS < tap_dtapt) {
//    return true;
//  } else {
//    tap_dtapt = 0;
//    return false;
//  }
//}
//void tap_clear() {
//  tap_tapt = 0;
//  tap_dtapt = 0;
//}

#define BUTTON_L_PIN 4
#define BUTTON_R_PIN 19
bool button_l_on = false;
bool button_r_on = false;

int main_mode = DEFAULT_MODE;
bool to_on = false;
void next_mode() {
  main_mode = (main_mode + 1) % 3;
  light_set_all(0, 0, 0);
  to_on = true;
}
#define LIG_WATER 0
#define LIG_TOUCH 1
#define LIG_DRINK 2
#define LIG_HOT 3
#define LIG_DANGR 4

void do_mode(float t) {
  switch (main_mode) {
    case 0: {
        if (t > SYM_WATER) {
          light_set(LIG_WATER, 255, 0, 0);
        } else {
          light_set(LIG_WATER, 0, 255, 0);
        }
        if (t > SYM_TOUCH) {
          light_set(LIG_TOUCH, 255, 0, 0);
        } else {
          light_set(LIG_TOUCH, 0, 255, 0);
        }
        if (t > SYM_DRINK) {
          light_set(LIG_DRINK, 255, 0, 0);
        } else {
          light_set(LIG_DRINK, 0, 255, 0);
        }
        if (t > SYM_HOT) {
          light_set(LIG_HOT, 255, 0, 0);
        } else {
          light_set(LIG_HOT, 0, 255, 0);
        }
        if (t > SYM_DANGR) {
          light_set(LIG_DANGR, 255, 0, 0);
        } else {
          light_set(LIG_DANGR, 0, 255, 0);
        }
        if (t > BLINK_TEMP && to_on) {
          to_on = false;
          for (int i = 5; i < 10; i++) {
            light_set(i, 255, 0, 0);
          }
        } else {
          to_on = true;
          for (int i = 5; i < 10; i++) {
            light_set(i, 0, 0, 0);
          }
        }
        light_load();
        break;
      }
    case 1: {
        if (t <= BLINK_TEMP || to_on) {
          for (int i = 0; i < 10; i++) {
            if (t > -30 + i * 2) {
              light_set(i, 0, 0, 255);
            }
          }
          for (int i = 0; i < 10; i++) {
            if (t > -10 + i * 2) {
              light_set(i, 0, 0, 255);
            }
          }
          for (int i = 0; i < 10; i++) {
            if (t > 10 + i * 2) {
              light_set(i, 0, 255, 0);
            }
          }
          for (int i = 0; i < 10; i++) {
            if (t > 30 + i * 2) {
              light_set(i, 255, 255, 0);
            }
          }
          for (int i = 0; i < 10; i++) {
            if (t > 50 + i * 2) {
              light_set(i, 255, 0, 0);
            }
          }
          for (int i = 0; i < 10; i++) {
            if (t > 70 + i * 2) {
              light_set(i, 255, 255, 255);
            }
          }
          light_load();
          to_on = false;
        } else {
          to_on = true;
          light_set_all(0, 0, 0);
        }
        break;
      }
  }
}

void buttons_update() {
  if (digitalRead(BUTTON_L_PIN) == HIGH) {
    if (!button_l_on) {
      if (laser_enabled) {
        laser_disable();
      } else {
        laser_enable();
      }
      button_l_on = true;
      print("Left button pressed!");
    }
  } else {
    if (button_l_on) {
      print("Left button released!");
      button_l_on = false;
    }
  }
  if (digitalRead(BUTTON_R_PIN) == HIGH) {
    if (!button_r_on) {
      next_mode();
      button_r_on = true;
      print("Right button pressed! New mode: " + String(main_mode));
    }
  } else {
    if (button_r_on) {
      print("Right button released!");
      button_r_on = false;
    }
  }
}

void setup() {
  if (SERIAL_ENABLED) {
    setup_serial();
  }
  CircuitPlayground.begin();
  pinMode(LASER_PIN, OUTPUT);
  print("Setup doneish");
  print("Setup doneish");
  ir_setup();
}

void loop() {
  delay(50);
  switch_update();
  if (!switch_on) {
    light_set_all(0, 0, 0);
    laser_disable();
    delay(950);
  } else {
    float t = ir_get_object_temp();
    float t2 = ir_get_sensor_temp();
    if (t > BUZZER_TEMP) {
      CircuitPlayground.playTone(NOTE_D5, 100);
    }
    print(String(t) + " / " + String(t2));
    buttons_update();
    do_mode(max(t, t2));
  }
}

void setup_serial() {
  while (!Serial) delay(10);
  Serial.begin(9600);
  print("Setup Serial");
}

void print(String s) {
  if (SERIAL_ENABLED) {
    Serial.println(s);
  }
}
