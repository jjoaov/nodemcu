#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <FS.h>
#include <LittleFS.h>

#define DHT_PIN       2//               <-- MODIFICAR DE ACORDO COM AS CONFIGURAÇÕES DOS SENSORES
#define DHT_TYPE      DHT11//          <--      ~(˘▾˘~)   ♥‿♥   (~˘▾˘)~
#define LDR_PIN       A0
#define POWER_LED    D5

  //Protótipos de funções
    void initWebServer();
    void startWiFi();
    void initWiFiManager();
    void displayNetworkConfiguration();
    void saveAPIP();
    void requestServer();
    float convertToLux ( int value );
  //

//Configurações de Sensores
DHT dht(DHT_PIN, DHT_TYPE);

//Configurações da Interface de Rede Wifi
const char *ssid = "";//Identificador de rede   <-- MODIFICAR DE ACORDO COM A REDE
const char *passwd = "";//Senha                 <-- ʕ•ᴥ•ʔ

// Informações básicas do servidor...
const char *host =  "megatecnologia.com.br"; //URL servidor
const char *route = "/controle/silas.json";
const char *query = "?chave=523DA-0D1DD-A84D9-EF34B-F1B31-99AC9-28"; //Chave da aplicação |parâmetro chave|

//Parâmetros de Configuração |SETUP| disponíveis via página de configuração do servidor
extern const char PROGMEM index_html[]; //String HTML
extern ESP8266WebServer server;
extern DNSServer dns;

//Inicia o contador que receberá o valor de millis a cada envio
//Seu overflow deve ocorrer a cada 50 dias, aproximadamente
unsigned long counter = 0;
unsigned long timerControl;
extern unsigned long interval;
//Todos essa valores desconsideram um possível atraso na execução do programa

void setup()
{
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH); //Liga led sinalizador, indicando que o programa foi iniciado
  Serial.begin(115200);
  LittleFS.begin();
  setLedsPinMode(); //Inicializa pinMode dos leds de sinalização como output

  initWiFiManager();
  initWebServer();
  displayNetworkConfiguration(); //Exibe SSID, IP e RSSI da rede na comunicacao Serial
  saveAPIP();
  dht.begin();
  interval = getInterval();
}

void loop()
{
  server.handleClient();

  if ( (timerControl = millis()) - counter >= interval ) {
    requestServer();
    Serial.println("Enviando Requisição ao servidor");
    Serial.print("Intervalo definido para: "); Serial.println(interval);
    counter = timerControl;
  }
}
