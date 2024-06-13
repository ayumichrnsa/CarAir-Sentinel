#include <MQUnifiedsensor.h>

// Definisi board dan sensor
#define Board "ESP-32"
#define Voltage_Resolution 3.3  // Tegangan kerja ESP32 adalah 3.3V
#define ADC_Bit_Resolution 12  // Resolusi ADC ESP32 adalah 12-bit
#define Pin 34  // Pin analog ESP32 yang terhubung ke MQ-135

#define Type "MQ-135"
#define RL 1  // Beban resistor (kilo-ohm)
#define R0 10  // Nilai awal R0, sesuaikan dengan hasil kalibrasi

MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

void setup() {
  Serial.begin(115200);
  MQ135.setRegressionMethod(1);  // Metode regresi 1 adalah metode regresi linear
  MQ135.init();

  // Kalibrasi sensor
  Serial.print("Kalibrasi, silakan tunggu.");
  float calcR0 = 0;
  for(int i = 1; i <= 10; i++) {
    MQ135.update();  // Update pembacaan sensor
    calcR0 += MQ135.calibrate(RL);
    Serial.print(".");
    delay(1000);
  }
  MQ135.setR0(calcR0 / 10);  // Set nilai kalibrasi R0
  Serial.println(" selesai!");

  if (isinf(calcR0)) {
    Serial.println("Peringatan: Masalah koneksi atau kalibrasi buruk terdeteksi.");
    while(1);
  }
}

void loop() {
  MQ135.update();  // Update pembacaan sensor

  // Mengatur nilai regresi untuk CO2 dan membaca konsentrasi CO2
  MQ135.setA(110.47);  // Nilai konstanta A untuk CO2 (ganti dengan nilai yang benar)
  MQ135.setB(-2.862);  // Nilai konstanta B untuk CO2 (ganti dengan nilai yang benar)
  float CO2_ppm = MQ135.readSensor();  // Membaca konsentrasi CO2

  // Mengatur nilai regresi untuk CO dan membaca konsentrasi CO
  MQ135.setA(605.18);  // Nilai konstanta A untuk CO (ganti dengan nilai yang benar)
  MQ135.setB(-3.937);  // Nilai konstanta B untuk CO (ganti dengan nilai yang benar)
  float CO_ppm = MQ135.readSensor();  // Membaca konsentrasi CO

  // Menampilkan hasil pembacaan ke Serial Monitor
  Serial.print("CO2 (ppm): ");
  Serial.print(CO2_ppm);
  Serial.print(" | CO (ppm): ");
  Serial.println(CO_ppm);

  delay(1000);  // Tunggu 2 detik sebelum pembacaan berikutnya
}
