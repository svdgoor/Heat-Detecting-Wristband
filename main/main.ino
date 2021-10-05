#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_MLX90614.h>// Tones
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

// Main
void setup() {
  setup_serial();
  print("Starting up...");
  CircuitPlayground.begin();
  set_brightness(128);
  set_all_lights(0, 0, 0);
  startup_stage_done();
  
//  setup_infrared();
  startup_stage_done();
  
  setup_tapping();
  startup_stage_done();

  startup_stage_done();
  startup_stage_done();
  speak(NOTE_CS3, 200);
  speak(NOTE_DS3, 200);
  speak(NOTE_FS3, 500);
}

int startup_points = 0.0;
void startup_stage_done(){
  set_light(startup_points++, 0, 32, 0);
  load_lights();
  delay(200);
  set_light(startup_points++, 0, 32, 0);
  load_lights();
  delay(200);
}

void loop() {
  update_tap();
  update_switch();
}

void setup_serial() {
  Serial.begin(9600);
  while (!Serial) delay(10);
}

#define BEATS 5
#define DEMO_MILLIS 5000
void run_startup_demo() {
  print("Running startup LED show");
  unsigned long start = millis();
  int cycletime = DEMO_MILLIS/BEATS;
  set_all_lights(0, 8, 16);
  
  while (start - millis() + DEMO_MILLIS > 0){
    set_all_lights(round(max(0, sin(((millis() - start)/1500.0f) * PI)) * 64), 8, 16);
    delay(25);
  }
  set_all_lights(0,0,0);
}

// UTIL
void print(String s){
  Serial.println(s);
}

// Infrared Sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
void setup_infrared() {
  print("Connecting to MLX90614 Infrared Sensor...");

  while (!mlx.begin()) {
    print("Error connecting to MLX sensor. Check wiring.");
    
    delay(100);
  };

  print("IR Emissivity = "); print(String(mlx.readEmissivity()));
  print("Successively connected to Infrared Sensor");
}

double measure_object_temp() {
  return mlx.readObjectTempC();
}

double measure_ir_sensor_temp() {
  return mlx.readAmbientTempC();
}

// Slide switch
boolean switch_on = false; 
boolean switched = false;
boolean get_switch() {
  return CircuitPlayground.slideSwitch();
}
void update_switch() {
  if (get_switch()) {
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

// LEDs
void set_light(int number, int r, int g, int b) {
  CircuitPlayground.strip.setPixelColor(number, r, g, b);
}
void set_all_lights(int r, int g, int b) {
  for (int i = 0; i <= 10; i++){
    set_light(i, r, g, b);
  }
  load_lights();
}
void load_lights() {
  CircuitPlayground.strip.show();
}
void set_brightness(int brightness){ 
  CircuitPlayground.strip.setBrightness(brightness);
}

// Accelerometer
void get_acceleration_x() {
  return CircuitPlayground.motionX();
}
void get_acceleration_y() {
  return CircuitPlayground.motionY();
}
void get_acceleration_z() {
  return CircuitPlayground.motionZ();
}


// Tapping
#define CLICK_THRESHOLD 120
#define DOUBLE_TAP_MILLIS 500
#define LASER_ON_MILLIS 5000
#define BATTERY_SHOW_MILLIS 5000

void setup_tapping() {
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  CircuitPlayground.setAccelTap(1, CLICK_THRESHOLD);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), on_tap, FALLING);
}

boolean laser = false;
unsigned long laser_start = -1;
boolean battery_show = false;
unsigned long battery_show_start = -1;

boolean tap = false;
unsigned long last_tap_time = -1;

void on_tap() {
  if (!tap) {
    tap = true;
    last_tap_time = millis();
    return;  
  }
  tap = false;

  // Double tap
  if (!battery_show){
    battery_show = true;
    battery_show_start = millis();
  } else {
    battery_show = false;
  }
}
void update_tap() { // Should be ran often. Say, 10x per second
  if (tap && millis() - last_tap_time - DOUBLE_TAP_MILLIS >= 0){
    tap = false;

    // Single tap
    if (!laser){
      laser = true;
      laser_start = millis();
    } else {
      laser = false;
    }
  }

  if (laser && millis() - laser_start - LASER_ON_MILLIS >= 0) {
    laser = false;
  }

  if (battery_show && millis() - battery_show - BATTERY_SHOW_MILLIS >= 0) {
    battery_show = false;
  }
}

// Speaker
void speak(int hertz, int ms){
  CircuitPlayground.playTone(hertz, ms);
}
