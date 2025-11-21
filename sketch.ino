#include <WiFi.h>
#include <HTTPClient.h>

// --------------------------------------------------
// Configuracoes de WiFi (somente Wokwi, sem roteador real)
// --------------------------------------------------
constexpr const char* WIFI_SSID     = "Wokwi-GUEST";
constexpr const char* WIFI_PASSWORD = "";
constexpr uint8_t     WIFI_CHANNEL  = 6;

// --------------------------------------------------
// Configuracoes de FIWARE (Orion na Azure)
// --------------------------------------------------
constexpr const char* FIWARE_URL =
  "http://52.156.26.21:1026/v2/entities/WorkArea:001/attrs";

constexpr const char* FIWARE_SERVICE      = "quietoffice";
constexpr const char* FIWARE_SERVICE_PATH = "/";

// --------------------------------------------------
// Parametros do monitor de ruido
// --------------------------------------------------
constexpr float  NOISE_ALERT_THRESHOLD  = 55.0f;   // dB para gerar alerta
constexpr unsigned long SEND_INTERVAL_MS = 5000;   // periodo de envio ao FIWARE

unsigned long lastSendTime = 0;

// --------------------------------------------------
// Leitura de ruido (simulada)
// --------------------------------------------------
float readNoiseLevel() {
  // Simula ruido entre 40.0 e 80.0 dB
  int raw = random(400, 800);   // 400..799
  return raw / 10.0f;
}

// --------------------------------------------------
// Tradução do valor em dB para status/alerta/mensagem
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
// Envio dos dados ao FIWARE (Orion) - sem logs na serial
// --------------------------------------------------
void sendToFiware(float noise,
                  const String& status,
                  bool alert,
                  const String& alertMessage) {
  if (WiFi.status() != WL_CONNECTED) {
    // Silencioso para nao poluir a apresentacao
    return;
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

  (void) http.POST(payload);  // ignoramos o retorno para manter a tela limpa
  http.end();
}

// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println(" PROJETO: QuietOffice - Monitor de Ruido");
  Serial.println(" GS Futuro do Trabalho - ESP32 + FIWARE");
  Serial.println("======================================");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  uint8_t retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    retries++;
  }

  randomSeed(analogRead(0));
}

// --------------------------------------------------
// Loop principal
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

    Serial.println("--------------------------------------");
    Serial.println("QuietOffice - Monitor de Ruido");
    Serial.printf("Ruido atual : %.1f dB\n", noise);
    Serial.printf("Ruido limite: %.1f dB\n", NOISE_ALERT_THRESHOLD);
    Serial.printf("Mensagem    : %s\n", alertMessage.c_str());

    sendToFiware(noise, status, alert, alertMessage);
  }

  delay(100);
}
