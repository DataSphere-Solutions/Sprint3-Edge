#include <WiFi.h>
#include <PubSubClient.h>

const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "20.197.230.60"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/motor003/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/motor003/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/motor003/attrs/s"; // Tópico para direção
const char* default_TOPICO_PUBLISH_3 = "/TEF/motor003/attrs/v"; // Tópico para velocidade
const char* default_ID_MQTT = "fiware_003"; // ID MQTT

WiFiClient espClient;
PubSubClient MQTT(espClient);
int motorSpeed = 0;
char motorDirection = 's'; // 'f' para frente, 'b' para trás, 's' para parado
const int motorPin1 = 18;
const int motorPin2 = 19;

void setup() {
  Serial.begin(115200);
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  initWiFi();
  MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
  
  // Inicializa o motor parado
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

void loop() {
  if (!MQTT.connected()) {
    reconnectMQTT();
  }
  
  MQTT.loop();

  // Verifica se há entrada de comandos via Serial
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Lê o comando digitado
    processCommand(input);
  }

  // Periodicamente, envia o estado do motor (direção e velocidade)
  sendMotorStatus();
  
  delay(1000);
}

void initWiFi() {
  WiFi.begin(default_SSID, default_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Conectado.");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Mensagem recebida: ");
  Serial.println(message);

  // Dividir a mensagem usando o delimitador '|'
  int separatorIndex = message.indexOf('|');
  if (separatorIndex > 0) {
    String command = message.substring(0, separatorIndex); // Comando: f, b, ou s
    String value = message.substring(separatorIndex + 1); // Valor da velocidade

    // Identificar o comando
    if (command[command.length() - 1] == 'f') {
      motorDirection = 'f';
      motorSpeed = value.toInt(); // Converter para inteiro
      setMotor(motorDirection, motorSpeed);
    } else if (command[command.length() - 1] == 'b') {
      motorDirection = 'b';
      motorSpeed = value.toInt(); // Converter para inteiro
      setMotor(motorDirection, motorSpeed);
    } else if (command[command.length() - 1] == 's') {
      motorDirection = 's';
      motorSpeed = 0; // Parar o motor
      setMotor(motorDirection, motorSpeed);
    }
  }
}


void processCommand(String input) {
  input.trim(); // Remove espaços extras
  char direction = input[0]; // O primeiro caractere é a direção
  int speed = input.substring(2).toInt(); // O número após o espaço é a velocidade
  
  if ((direction == 'f' || direction == 'b' || direction == 's') && speed >= 0 && speed <= 255) {
    motorDirection = direction;
    motorSpeed = speed;
    setMotor(motorDirection, motorSpeed);
    Serial.println("Comando Serial recebido: Direção = " + String(motorDirection) + ", Velocidade = " + String(motorSpeed));
  } else {
    Serial.println("Comando inválido. Use 'f 10' ou 'b 50', por exemplo.");
  }
}

void setMotor(char direction, int speed) {
  if (direction == 'f') {
    ledcWrite(0, speed); // Frente
    ledcWrite(1, 0);
    Serial.println("Motor girando para frente com velocidade: " + String(speed));
  } else if (direction == 'b') {
    ledcWrite(0, 0);
    ledcWrite(1, speed); // Trás
    Serial.println("Motor girando para trás com velocidade: " + String(speed));
  } else {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    Serial.println("Motor parado.");
  }
}

void sendMotorStatus() {
  // Publica a direção
  MQTT.publish(default_TOPICO_PUBLISH_2, String(motorDirection).c_str());
  Serial.println("Direção enviada: " + String(motorDirection));
  
  // Publica a velocidade
  MQTT.publish(default_TOPICO_PUBLISH_3, String(motorSpeed).c_str());
  Serial.println("Velocidade enviada: " + String(motorSpeed));
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT...");
    if (MQTT.connect(default_ID_MQTT)) {
      Serial.println("Conectado!");
      MQTT.subscribe(default_TOPICO_SUBSCRIBE);
    } else {
      Serial.print("Falha ao conectar, rc=");
      Serial.print(MQTT.state());
      Serial.println(" Tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}
