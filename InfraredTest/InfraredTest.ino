#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
double last = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(10, OUTPUT);
  setup_infrared();
}

void loop() {
  double temp = mlx.readObjectTempC();
  if (last == temp){
    digitalWrite(10, LOW);
    String temp = "Ambient = " + String(mlx.readAmbientTempC()) + "C*\tObject = " + String(mlx.readObjectTempC()) + "*C";
    print(temp);
  } else {
    digitalWrite(10, HIGH);
    last = temp;
  }
  delay(100);
}

void setup_infrared() {
  print("Connecting to MLX90614 Infrared Sensor...");

  while (!mlx.begin()) {
    print("Error connecting to MLX sensor. Check wiring.");
    delay(100);
  };

  print("IR Emissivity = "); Serial.println(mlx.readEmissivity());
  print("Successively connected to Infrared Sensor");
}

void print(String s){
  Serial.println(s);
}

double measure_object_temp() {
  return mlx.readObjectTempC();
}

double measure_ir_sensor_temp() {
  return mlx.readAmbientTempC();
}
