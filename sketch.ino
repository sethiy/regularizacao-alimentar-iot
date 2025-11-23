#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Tópicos MQTT
const char* topico_alerta = "regularizacao_alimentar/alerta";
const char* topico_confirmacao = "regularizacao_alimentar/confirmacao";
const char* topico_sensor = "regularizacao_alimentar/sensor";

// Pinos
#define BUZZER_PIN 25
#define LED_PIN 26
#define SWITCH_PIN 4

// Display
#define LARGURA_TELA 128
#define ALTURA_TELA 64
Adafruit_SSD1306 display(LARGURA_TELA, ALTURA_TELA, &Wire, -1);

// Variáveis
WiFiClient espClient;
PubSubClient client(espClient);
bool alarme_ativo = false;
int ultimo_estado_switch = -1;
unsigned long ultimo_display = 0;

// Medições
unsigned long inicio_sensor = 0;
unsigned long inicio_atuador = 0;
unsigned long tempos_sensor[4] = {0, 0, 0, 0};
unsigned long tempos_atuador[4] = {0, 0, 0, 0};
int contador_medicoes = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro no display");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  conectarWiFi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(chegada_mqtt);
  
  mostrar_display("Sistema Pronto", "Aguardando MQTT", "");
}

void conectarWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado");
}

void chegada_mqtt(char* topico, byte* mensagem_bytes, unsigned int comprimento) {
  String mensagem = "";
  for (int i = 0; i < comprimento; i++) {
    mensagem += (char)mensagem_bytes[i];
  }
  
  Serial.print("MQTT Recebido: ");
  Serial.println(mensagem);

  if (mensagem == "INICIAR") {
    inicio_atuador = millis();
    ativar_alarme();
    tempos_atuador[contador_medicoes] = millis() - inicio_atuador;
  }
}

void reconectar_mqtt() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    
    String client_id = "ESP32-";
    client_id += String(random(0xffff), HEX);
    
    if (client.connect(client_id.c_str())) {
      Serial.println("Conectado");
      client.subscribe(topico_alerta);
    } else {
      delay(5000);
    }
  }
}

void ativar_alarme() {
  Serial.println("ALARME: Hora da refeição!");
  alarme_ativo = true;
  
  tone(BUZZER_PIN, 1500);
  digitalWrite(LED_PIN, HIGH);
  
  mostrar_display("HORA DE COMER!", "Use o switch", "para confirmar");
}

void confirmar_refeicao() {
  Serial.println("Refeição confirmada!");
  alarme_ativo = false;
  
  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);
  
  client.publish(topico_confirmacao, "CONFIRMADO");
  
  imprimir_medicoes();
  
  mostrar_display("Refeição", "Confirmada", "Obrigado!");
  delay(2000);
}

void verificar_switch() {
  int estado_atual = digitalRead(SWITCH_PIN);
  
  if (estado_atual != ultimo_estado_switch) {
    
    if (ultimo_estado_switch != -1) {
      inicio_sensor = millis();
      
      client.publish(topico_sensor, "SWITCH_ACIONADO");
      
      if (alarme_ativo) {
        tempos_sensor[contador_medicoes] = millis() - inicio_sensor;
        confirmar_refeicao();
        contador_medicoes++;
        
        if (contador_medicoes >= 4) {
          contador_medicoes = 0;
        }
      }
    }
    
    ultimo_estado_switch = estado_atual;
    delay(50);
  }
}

void atualizar_display() {
  if (millis() - ultimo_display > 1000) {
    display.clearDisplay();
    display.setCursor(0,0);
    
    display.println("REG. ALIMENTAR");
    display.println("------------");
    
    if (alarme_ativo) {
      display.println("ALARME ATIVO!");
      display.println("Use o switch");
    } else {
      display.println("Aguardando...");
    }
    
    display.display();
    ultimo_display = millis();
  }
}

void mostrar_display(String linha1, String linha2, String linha3) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(linha1);
  display.println(linha2);
  display.println(linha3);
  display.display();
}

void imprimir_medicoes() {
  Serial.println("=== MEDICOES ===");
  Serial.print("Tempo sensor: ");
  Serial.print(tempos_sensor[contador_medicoes]);
  Serial.println(" ms");
  
  Serial.print("Tempo atuador: ");
  Serial.print(tempos_atuador[contador_medicoes]);
  Serial.println(" ms");
  Serial.println("================");
}

void loop() {
  if (!client.connected()) {
    reconectar_mqtt();
  }
  client.loop();
  
  verificar_switch();
  atualizar_display();
  
  delay(100);
}