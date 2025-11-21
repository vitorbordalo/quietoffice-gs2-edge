#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <U8g2lib.h>

// --------------------------------------------------
// DISPLAY OLED SSD1306 (I2C)
// Pinos no Wokwi:
//   OLED SDA -> ESP32 GPIO21
//   OLED SCL -> ESP32 GPIO22
//   OLED VCC -> 3V3
//   OLED GND -> GND
// --------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// --------------------------------------------------
// CONFIG Wi-Fi (Wokwi)
// --------------------------------------------------
const char* WIFI_SSID     = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";
const uint8_t WIFI_CHANNEL = 6;

// --------------------------------------------------
// CONFIG FIWARE (Orion na Azure)
// --------------------------------------------------
const char* FIWARE_URL =
  "http://52.156.26.21:1026/v2/entities/WorkArea:001/attrs";

const char* FIWARE_SERVICE      = "quietoffice";
const char* FIWARE_SERVICE_PATH = "/";

// --------------------------------------------------
// PARÂMETROS DO MONITOR DE RUÍDO
// --------------------------------------------------
const float NOISE_ALERT_THRESHOLD   = 55.0;      // dB para alerta
const unsigned long SEND_INTERVAL_MS = 5000;     // envio a cada 5s

unsigned long lastSendTime = 0;

// --------------------------------------------------
// LEITURA DE RUÍDO (SIMULADA)
// --------------------------------------------------
float readNoiseLevel() {
  // Simula ruído entre 40.0 e 80.0 dB
  int raw = random(400, 800);   // 400..799
  return raw / 10.0;
}

// --------------------------------------------------
// AVALIAÇÃO DO NÍVEL DE RUÍDO
// --------------------------------------------------
void evaluateNoise(float noise,
                   String& status,
                   bool& alert,
                   String& alertMessage) {
  if (noise >= NOISE_ALERT_THRESHOLD) {
    status       = "noisy";
    alert        = true;
    alertMessage = "ALERTA: Nivel de ruido alto no ambiente de trabalho!";
  } else {
    status       = "quiet";
    alert        = false;
    alertMessage = "Ambiente dentro do nivel aceitavel de ruido.";
  }
}

// --------------------------------------------------
// ATUALIZA O OLED SSD1306
// --------------------------------------------------
void updateDisplay(float noise,
                   float limit,
                   const String& status,
                   const String& message) {
  display.clearBuffer();

  // Título
  display.setFont(u8g2_font_6x10_tf);
  display.setCursor(0, 10);
  display.print("QuietOffice");

  display.setCursor(0, 22);
  display.print("Monitor de Ruido");

  // Valores
  display.setCursor(0, 36);
  display.print("Ruido: ");
  display.print(noise, 1);
  display.print(" dB");

  display.setCursor(0, 48);
  display.print("Limite: ");
  display.print(limit, 1);
  display.print(" dB");

  // Status/alerta
  display.setCursor(0, 60);
  if (status == "noisy") {
    display.print("ALERTA! ");
  } else {
    display.print("OK   ");
  }

  display.sendBuffer();
}

// --------------------------------------------------
// ENVIO PARA O FIWARE (SEM POLUIR A SERIAL)
// --------------------------------------------------
void sendToFiware(float noise,
                  const String& status,
                  bool alert,
                  const String& alertMessage) {
  if (WiFi.status() != WL_CONNECTED) {
    return; // sem WiFi, só sai
  }

  HTTPClient http;
  http.begin(FIWARE_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Fiware-Service", FIWARE_SERVICE);
  http.addHeader("Fiware-ServicePath", FIWARE_SERVICE_PATH);

  String payload = "{";
  payload += "\"noiseLevel\":{\"type\":\"Number\",\"value\":" + String(noise, 1) + "},";
  payload += "\"status\":{\"type\":\"Text\",\"value\":\"" + status + "\"},";
  payload += "\"alert\":{\"type\":\"Boolean\",\"value\":" + String(alert ? "true" : "false") + "},";
  payload += "\"alertMessage\":{\"type\":\"Text\",\"value\":\"" + alertMessage + "\"}";
  payload += "}";

  (void) http.POST(payload); // ignora retorno
  http.end();
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println(" PROJETO: QuietOffice - Monitor de Ruido");
  Serial.println(" GS Futuro do Trabalho - ESP32 + FIWARE");
  Serial.println("======================================");

  // WiFi só pro Wokwi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

  uint8_t retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    retries++;
  }

  // OLED
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.setCursor(0, 16);
  display.print("Inicializando...");
  display.sendBuffer();

  // semente pro gerador de ruído aleatório
  randomSeed(analogRead(0));
}

// --------------------------------------------------
// LOOP PRINCIPAL
// --------------------------------------------------
void loop() {
  unsigned long now = millis();

  if (now - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = now;

    float noise = readNoiseLevel();
    String status;
    bool alert;
    String alertMessage;

    evaluateNoise(noise, status, alert, alertMessage);

    // Saída enxuta na Serial
    Serial.println("--------------------------------------");
    Serial.println("QuietOffice - Monitor de Ruido");
    Serial.printf("Ruido atual : %.1f dB\n", noise);
    Serial.printf("Ruido limite: %.1f dB\n", NOISE_ALERT_THRESHOLD);
    Serial.printf("Mensagem    : %s\n", alertMessage.c_str());

    // Atualiza OLED
    updateDisplay(noise, NOISE_ALERT_THRESHOLD, status, alertMessage);

    // Envia pro FIWARE
    sendToFiware(noise, status, alert, alertMessage);
  }

  delay(100);
}
