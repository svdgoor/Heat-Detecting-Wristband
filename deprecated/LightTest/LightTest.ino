#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.strip.setBrightness(128);
  for (int i=0; i<10; ++i) {
    CircuitPlayground.strip.setPixelColor(i, 128, 0, 64);
  }
  CircuitPlayground.strip.show();
}

void loop() {}
