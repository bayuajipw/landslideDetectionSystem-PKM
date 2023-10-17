#include <Wire.h> // Library I2C
#include <ESP8266WebServer.h> // Library ESP8266
#include <WiFiClient.h> // Memasukkan Library ESP8266 sebagai client

const uint8_t MPU6050SlaveAddress = 0x68; //untuk menentukan alamat I2C sensor Gyro (MPU6050)
const uint8_t scl = D6; //Pilih pin SCL untuk komunikasi I2C yang sudah termasuk pada sensor Gyro (MPU6050)
const uint8_t sda = D7;//Pilih pin SDA untuk komunikasi I2C yang sudah termasuk pada sensor Gyro (MPU6050)
const int sensor_pin = A0; 
// Pilih pin A0 analog pada Wemos untuk komunikasi antara sensor Soil Moisture dengan Wemos

// Untuk mengatur faktor skala sensitivitas pada sensor Gyro (MPU6050) ke pengaturan skala penuh
const uint16_t AccelScaleFactor = 16384; 
const uint16_t GyroScaleFactor = 131;

// Alamat I2C  sensor Gyro (MPU6050) 
const uint8_t MPU6050_REGISTER_SMPLRT_DIV = 0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL  = 0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1 = 0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2 = 0x6C;
const uint8_t MPU6050_REGISTER_CONFIG = 0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  = 0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG = 0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN  = 0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE = 0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H = 0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET = 0x68;
int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ; 

#include "CTBot.h" // Library CTBot untuk Telegram 
CTBot myBot;
String ssid = "vivo 1807"; 
String pass = "haihaihello";
String token = "5808434331:AAFyP0dk3CufHp9L30XlJxjAeuG8sKygQ4I"; // ID bot Telegram
const int id = 1563795276; // ID grup Telegram 
const int user = -819746893; // ID grup User

#define relay D4 // Mendefinisikan pin D4 Wemos untuk relay
int lampuNyala = LOW; // Mendefinisikan variabel relay nyala 
int lampuMati = HIGH; // Mendefinisikan variabel relay mati 

void setup() {
  Serial.begin(9600); 
  Serial.println("Starting TelegramBot..."); 
  myBot.wifiConnect(ssid, pass); 
  myBot.setTelegramToken(token);
  
  if (myBot.testConnection()) { 
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Buruk");
  }
    
  myBot.sendMessage(id, "BOT NOTIFIKASI MONITORING TANAH LONGSOR AKTIF"); //Notifikasi ke telegram bahwa alat aktif
  myBot.sendMessage(user, "BOT NOTIFIKASI MONITORING TANAH LONGSOR AKTIF");
    Serial.println("============ Pesan Terkirim: Notifikasi BOT Telegram Aktif ============");
    Wire.begin(sda, scl); 
    MPU6050_Init();
    pinMode(relay, OUTPUT);

}

void loop() {
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  
  Read_RawValue(MPU6050SlaveAddress,MPU6050_REGISTER_ACCEL_XOUT_H);
  
  // Membagi atau menentukan skala x,y,z 
  Ax = (double)AccelX/AccelScaleFactor; 
  Ay = (double)AccelY/AccelScaleFactor; 
  Az = (double)AccelZ/AccelScaleFactor;
  T  = (double)Temperature/340+36.53; // Temperature formula 
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor; 
  Gz = (double)GyroZ/GyroScaleFactor;
  Serial.print("Ax: "); Serial.println(Ax);
  Serial.print("Ay: "); Serial.println(Ay);
  Serial.print("T: "); Serial.println(T);
  Serial.print("Gx: "); Serial.println(Gx);
  Serial.print("Gy: "); Serial.println(Gy);
  Serial.print("Gz: "); Serial.println(Gz);
  Serial.println("============ Pembacaan Sensor Gyroscope ============");

  // Pembacaan Sensor Hygrometer
  Serial.print("Soil Moisture: "); Serial.println(analogRead(sensor_pin));
  Serial.println("============ Pembacaan Sensor Hygrometer ============");
  
  if ((Ax < -0.30 ) && !(Ax < -0.40)) {
  Serial.print("Ax: "); Serial.println(Ax); 
  Serial.print("Ay: "); Serial.println(Ay); 
  Serial.print("Az: "); Serial.println(Az); 
  Serial.print("T: "); Serial.println(T); 
  Serial.print("Gx: "); Serial.println(Gx); 
  Serial.print("Gy: "); Serial.println(Gy); 
  Serial.print("Gz: "); Serial.println(Gz);
  myBot.sendMessage(id, "AWAS WASPADA SIAGA 1 !!!! Tanah longsor");
  myBot.sendMessage(user, "AWAS WASPADA SIAGA 1 !!!! Tanah longsor");
  Serial.println("============ Pesan Terkirim: SIAGA 1! ============");
  digitalWrite(relay, lampuNyala);
  }
     
  else if (Ax < -0.45) {
  Serial.print("Ax: "); Serial.println(Ax); 
  Serial.print("Ay: "); Serial.println(Ay); 
  Serial.print("Az: "); Serial.println(Az); 
  Serial.print("T: "); Serial.println(T); 
  Serial.print("Gx: "); Serial.println(Gx); 
  Serial.print("Gy: "); Serial.println(Gy); 
  Serial.print("Gz: "); Serial.println(Gz); 
  myBot.sendMessage(id, "AWAS BAHAYA SIAGA 2 !!!! Tanah longsor"); 
  myBot.sendMessage(user, "AWAS BAHAYA SIAGA 2 !!!! Tanah longsor"); 
  Serial.println("============ Pesan Terkirim: SIAGA 2! ============");
  digitalWrite(relay, lampuNyala);
  }

  else if (Az < 0.85) { 
  Serial.print(" Ax: "); Serial.println(Ax);
  Serial.print(" Ay: "); Serial.println(Ay); 
  Serial.print(" Az: "); Serial.println(Az); 
  Serial.print(" T: "); Serial.println(T); 
  Serial.print(" Gx: "); Serial.println(Gx); 
  Serial.print(" Gy: "); Serial.println(Gy); 
  Serial.print(" Gz: "); Serial.println(Gz);
  myBot.sendMessage(id, "Hati-Hati Longsor"); 
  myBot.sendMessage(user, "Hati-Hati Longsor");
  Serial.println("============ Pesan Terkirim: Hati-Hati ============");
  digitalWrite(relay, lampuNyala);
  }
  
  else {
    digitalWrite(relay, lampuMati);
       }

// Konfigurasi sensor Soil Moisture
  float moisture_percentage;
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) ); // Untuk menentukan sensor Soil Moisture jika kelembapan yang sudah ditentukan maka alat akan mengirim notifikasi ke Telegram
  if (moisture_percentage > 50.00){
  Serial.print("Soil Moisture(in Percentage) = "); 
  Serial.println(moisture_percentage);
  myBot.sendMessage(id, "AWAS WASPADA !!!! Tanah longsor! KELEMBABAN TANAH 50% = Kemungkinan Berbahaya" ); 
  myBot.sendMessage(user, "AWAS WASPADA !!!! Tanah longsor! KELEMBABAN TANAH 50% = Kemungkinan Berbahaya" ); 
  Serial.println("============ Pesan Terkirim: WASPADA! ============");
  delay(1000);
  }
  delay(500);
  }
  
// Konfigurasi I2C pada sensor Gyro untuk membaca alamat pada sensor Gyro
void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress); 
  Wire.write(regAddress); 
  Wire.write(data); 
  Wire.endTransmission();
} 

// Membaca semua register yang sudah ditentukan pada konfigurasi di atas pada sensor Gyro
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){ 
  Wire.beginTransmission(deviceAddress); 
  Wire.write(regAddress);
  Wire.endTransmission(); 
  Wire.requestFrom(deviceAddress, (uint8_t)14); 
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read()); 
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read()); 
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read()); 
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read()); 
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

// Konfigurasi sensor Gyro MPU6050 
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00); 
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG,0x00); // Set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00); // Set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00); 
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE,0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
