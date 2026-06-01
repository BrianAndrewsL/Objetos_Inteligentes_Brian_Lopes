#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define MIC_PIN A0

#define LED_VERDE D5
#define LED_AMARELO D2
#define LED_VERMELHO D1

// ======================
// WIFI
// ======================

const char* ssid = "Brian Wifi 2.4G";
const char* password = "mypassword";

// ======================
// HIVEMQ CLOUD
// ======================

const char* mqtt_server =
"fb5c8a1ca92c4c3bb8b8a230d9d70ee5.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

const char* mqtt_user = "brian_lopes32";
const char* mqtt_password = "Br123456";

// ======================

WiFiClientSecure espClient;
PubSubClient client(espClient);

// ======================

int leitura;
int sinalMax;
int sinalMin;
int nivelSom;

unsigned long ultimoEnvio = 0;

// ======================
// WIFI
// ======================

void setup_wifi() {

  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ======================
// CALLBACK MQTT
// ======================

void callback(char* topic, byte* payload, unsigned int length) {

  String mensagem = "";

  for (unsigned int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }

  Serial.print("Topico recebido: ");
  Serial.println(topic);

  Serial.print("Mensagem: ");
  Serial.println(mensagem);

  if (String(topic) == "led/vermelho") {

    if (mensagem == "ON") {
      digitalWrite(LED_VERMELHO, HIGH);
    }

    if (mensagem == "OFF") {
      digitalWrite(LED_VERMELHO, LOW);
    }
  }
}

// ======================
// RECONECTA MQTT
// ======================

void reconnect() {

  while (!client.connected()) {

    Serial.print("Conectando MQTT...");

    String clientId = "NodeMCU-";
    clientId += String(ESP.getChipId());

    if (client.connect(
          clientId.c_str(),
          mqtt_user,
          mqtt_password)) {

      Serial.println(" conectado!");

      client.subscribe("led/vermelho");

    } else {

      Serial.print(" falhou. Estado = ");
      Serial.println(client.state());

      delay(3000);
    }
  }
}

// ======================
// SETUP
// ======================

void setup() {

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  Serial.begin(9600);

  setup_wifi();

  // Ignora validação de certificado
  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(callback);
}

// ======================
// LOOP
// ======================

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // ======================
  // LEITURA MICROFONE
  // ======================

  sinalMax = 0;
  sinalMin = 1024;

  unsigned long inicio = millis();

  while (millis() - inicio < 100) {

    leitura = analogRead(MIC_PIN);

    if (leitura > sinalMax) {
      sinalMax = leitura;
    }

    if (leitura < sinalMin) {
      sinalMin = leitura;
    }
  }

  nivelSom = sinalMax - sinalMin;

  if (nivelSom < 20) {
    nivelSom = 0;
  }

  Serial.print("Som: ");
  Serial.println(nivelSom);

  // ======================
  // ENVIA MQTT
  // ======================

  if (millis() - ultimoEnvio > 1000) {

    char mensagem[10];

    sprintf(mensagem, "%d", nivelSom);

    bool enviado =
      client.publish("sensor/som", mensagem);

    Serial.print("Enviado: ");
    Serial.print(mensagem);
    Serial.print(" | Status: ");
    Serial.println(enviado);

    ultimoEnvio = millis();
  }

  // ======================
  // LEDs
  // ======================

  if (nivelSom < 120) {

    analogWrite(LED_VERDE, 150);

    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);

  } else if (nivelSom < 300) {

    analogWrite(LED_VERDE, 1023);

    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);

  } else if (nivelSom < 600) {

    digitalWrite(LED_VERDE, LOW);

    analogWrite(LED_AMARELO, 300);

    digitalWrite(LED_VERMELHO, LOW);

  } else if (nivelSom < 900) {

    digitalWrite(LED_VERDE, LOW);

    analogWrite(LED_AMARELO, 1023);

    digitalWrite(LED_VERMELHO, LOW);

  } else {

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);

    analogWrite(LED_VERMELHO, 1023);
  }

  delay(10);
}