#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <DHT.h>

#define DHT_PIN       2//               <-- MODIFICAR DE ACORDO COM AS CONFIGURAÇÕES DOS SENSORES
#define DHT_TYPE      DHT11//          <--      ~(˘▾˘~)   ♥‿♥   (~˘▾˘)~
#define LDR_PIN       A0
#define SIGNAL_LED    D5

  //Protótipos de funções
    void initAsyncWebServer();
    void ifStatus();
    void requestServer();
    float convertToLux ( int value );
  //

//Configurações de Sensores
DHT dht(DHT_PIN, DHT_TYPE);

//Configurações da Interface de Rede Wifi
const char *ssid = "";//Identificador de rede   <-- MODIFICAR DE ACORDO COM A REDE
const char *passwd = "";//Senha                 <-- ʕ•ᴥ•ʔ

String channel_id = "892363";
String write_api_key = "I9J4ZEW27Z943VHU";


// Informações básicas do servidor...
String host =  "api.thingspeak.com"; //URL servidor
String route = "/channels/" + channel_id + "/bulk_update.json";
String query = ""; //Chave da aplicação |parâmetro chave|

//Parâmetros de Configuração |SETUP| disponíveis via página de configuração do servidor
extern float interval;
extern const char PROGMEM index_html[]; //String HTML
extern AsyncWebServer server;
extern DNSServer dns;

//Inicia o contador que receberá 
//Seu overflow deve ocorrer a cada 50 dias, aproximadamente
unsigned long counter = 0;
//Todos essa valores desconsideram um possível atraso na execução do programa

void setup()
{
    Serial.begin(115200);

    //WiFi.begin(ssid, passwd);
    //WiFi.config(ip, gateway, subnet);

    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.autoConnect("AP_ESP"); // Fuça pelas últimas credenciais salvas na memória
    // wifiManager.startConfigPortal(); // Inicia a página de configuração, sem consultar a memória
    initAsyncWebServer();
    ifStatus();
    
    //dht.begin();
}

void loop()
{
    unsigned long timerControl = millis();
  
    if ((unsigned long)timerControl - counter >= interval*1000) {
      requestServer();
      counter = timerControl;
    }
}
