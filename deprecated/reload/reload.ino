#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_SleepyDog.h>
#include <Adafruit_MLX90614.h>
#include <notes.h>

class IR {
  private:
    const Adafruit_MLX90614 sensor = Adafruit_MLX90614();
    uint8_t _pin = NULL;
  public:
    void setup() {

      while (!sensor.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        
        delay(100);
      };

      Serial.println("Successively (re)connected to Infrared Sensor");
    }

    IR(uint8_t pin){
      _pin = pin;
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, HIGH);
      setup();
      Serial.println("IR Emissivity = " + String(sensor.readEmissivity()));
    }

    double get_object_temp() {
      return sensor.readObjectTempC();
    }

    double get_sensor_temp() {
      return sensor.readAmbientTempC();
    }

    void enable() {
      digitalWrite(_pin, HIGH);
      setup();
    }

    void disable() {
      digitalWrite(_pin, LOW);
    }
};
class Lights {
  public:
    void set(int number, int r, int g, int b) {
      CircuitPlayground.strip.setPixelColor(number, r, g, b);
    }
    void set_all(int r, int g, int b) {
      for (int i = 0; i <= 10; i++){
        set(i, r, g, b);
      }
      load();
    }
    void load() {
      CircuitPlayground.strip.show();
    }
    void brightness(int brightness){ 
      CircuitPlayground.strip.setBrightness(brightness);
    }
};
class Sounds {
  public:
    Sounds(Adafruit_CircuitPlayground &cp) {
      _cp = cp;
    }
    void play(int tone, int ms) {
      _cp.playTone(tone, ms);
    }
  private:
    Adafruit_CircuitPlayground &_cp;
};
class Switch {
  public:
    bool switched = false;
    bool switch_on = false;
    Switch(Adafruit_CircuitPlayground &cp) {
      _cp = cp;
    };
    void update() {
      if (on()){
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
  private:
    Adafruit_CircuitPlayground &_cp;
    bool on() {
      return _cp.slideSwitch();
    }
};
class Accelerometer {
  public:
    Accelerometer(Adafruit_CircuitPlayground &cp) {
      _cp = cp;
    }
    float get_x() {
      return CircuitPlayground.motionX();
    }
    float get_y() {
      return CircuitPlayground.motionY();
    }
    float get_z() {
      return CircuitPlayground.motionZ();
    }
  private:
    Adafruit_CircuitPlayground &_cp;
};
class Laser {
  public:
    Laser(uint8_t pin, int max_ms) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      delay(3000);
      _pin = pin;
      _max_ms = max_ms;
      pinMode(pin, OUTPUT);
      off();
    }
    void enable() {
      Serial.println("Enabling laser for " + String(_max_ms) + " ms");
      _start = millis();
      on();
    }
    void update() {
      if (_start == -1) return;
      if (millis() - _start > _max_ms) {
        Serial.println("Shutdown laser because timeout");
        off();
        _start = -1;
      }
    }
  private:
    uint8_t _pin;
    int _max_ms;
    unsigned long _start = -1;
    void on() {
      digitalWrite(_pin, HIGH);
    }
    void off() {
      digitalWrite(_pin, LOW);
    }
};
class Tapping {
  public:
    Tapping(int doubletap_ms, int expire_ms, int click_threshold, Adafruit_CircuitPlayground &cp) {
      cp.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
      cp.setAccelTap(1, click_threshold);
      _doubletap_ms = doubletap_ms;
      _expire_ms = expire_ms;
    }
    void tap() { 
      if (_tapt != -1 && millis() - _tapt < _doubletap_ms) {
          _tapt = -1;
          _dtapt = millis();
          return;
      }
      _tapt = millis();
    }
    bool tapped() {
      if (_tapt != -1 && millis() - _expire_ms < _tapt) {
        return true;
      } else {
        _tapt = -1;
        return false;
      }
    }
    bool dtapped() {
      if (_dtapt != -1 && millis() - _expire_ms < _dtapt) {
        return true;
      } else {
        _dtapt = -1;
        return false;
      }
    }
    void clear() {
      _tapt = -1;
      _dtapt = -1;
    }
  private:
    int _doubletap_ms;
    int _expire_ms;
    int _tapt = -1;
    int _dtapt = -1;
};

void setup() {
  setup_serial();
  CircuitPlayground.begin();
}

void loop() {
  Serial.println(String(ir.get_object_temp()));
  Serial.print(".");
}

void setup_serial() {
  while (!Serial) delay(10);
  Serial.begin(9600);
  print("Setup Serial");
}

void print(String s){
  Serial.println(s);
}
