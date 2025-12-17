#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// PMS5003: Arduino RX on 2, TX on 3
SoftwareSerial pmsSerial(2, 3);

// LCD: RS, E, D4, D5, D6, D7 on pins 8–13 (make sure this matches your wiring!)
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

// --------- NEW: AQI from PM2.5 (EPA breakpoints) ----------
int computeAQI_PM25(float C) {
  if (C < 0) C = 0;

  float aqi;

  if (C <= 12.0) {
    // 0–12.0 µg/m3 → AQI 0–50
    aqi = (50.0 / 12.0) * C;
  } else if (C <= 35.4) {
    // 12.1–35.4 → 51–100
    aqi = (100.0 - 51.0) / (35.4 - 12.1) * (C - 12.1) + 51.0;
  } else if (C <= 55.4) {
    // 35.5–55.4 → 101–150
    aqi = (150.0 - 101.0) / (55.4 - 35.5) * (C - 35.5) + 101.0;
  } else if (C <= 150.4) {
    // 55.5–150.4 → 151–200
    aqi = (200.0 - 151.0) / (150.4 - 55.5) * (C - 55.5) + 151.0;
  } else if (C <= 250.4) {
    // 150.5–250.4 → 201–300
    aqi = (300.0 - 201.0) / (250.4 - 150.5) * (C - 150.5) + 201.0;
  } else if (C <= 350.4) {
    // 250.5–350.4 → 301–400
    aqi = (400.0 - 301.0) / (350.4 - 250.5) * (C - 250.5) + 301.0;
  } else if (C <= 500.4) {
    // 350.5–500.4 → 401–500
    aqi = (500.0 - 401.0) / (500.4 - 350.5) * (C - 350.5) + 401.0;
  } else {
    return 500; // cap at max AQI
  }

  // round to nearest int
  if (aqi < 0) aqi = 0;
  if (aqi > 500) aqi = 500;
  return (int)(aqi + 0.5f);
}
// ---------------------------------------------------------

void setup() {
  // Serial monitor
  Serial.begin(115200);

  // PMS sensor UART
  pmsSerial.begin(9600);

  // LCD init
  lcd.begin(16, 2);
  lcd.print("PMS5003 Ready");
}

void loop() {
  if (readPMSdata(&pmsSerial)) {

    // -------- compute AQI from PM2.5 --------
    int aqi = computeAQI_PM25((float)data.pm25_standard);

    // -------- Serial debug output --------
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
    Serial.print("AQI (PM2.5): ");
    Serial.println(aqi);
    Serial.println("---------------------------------------");

    // -------- LCD output --------
    lcd.clear();

    // First row: PM2.5
    lcd.setCursor(0, 0);
    lcd.print("PM2.5:");
    lcd.print(data.pm25_standard);

    // Second row: AQI
    lcd.setCursor(0, 1);
    lcd.print("AQI:");
    lcd.print(aqi);

    delay(1000); // small pause so it doesn't flicker
  }
}

boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }

  // Wait for start byte 0x42
  if (s->peek() != 0x42) {
    s->read(); // throw away byte
    return false;
  }

  // Need full 32-byte frame
  if (s->available() < 32) {
    return false;
  }

  uint8_t buffer[32];
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // Checksum is sum of first 30 bytes
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }

  // Convert to 16-bit values (big-endian)
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }

  return true;
}