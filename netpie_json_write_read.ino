/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

/**
 * Plugin Name:     Netpie Json Write Read
 * Description:     Netpie Config from Write Json and Read Json File
 * Author:          lamloei
 * Author URI:      http://www.lamloei.com
 * Modified Date:   2016071501
 * Version:         V01
 */
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "FS.h"

StaticJsonBuffer<400> jsonBuffer;
char json[400];

char ssid[20];
char password[20];
char broker[20];
char netpie_appid[20];
char netpie_key[40];
char netpie_secret[40];
char netpie_alias[20];
char netpie_tls[20];

char* token_yes = "yes";
char* token_netpie = "netpie.io";

WiFiClient client;
MicroGear microgear(client);
char* file_config = "/config.json";

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("");
  Serial.println("Starting...");

  SPIFFS.begin();
  SPIFFS.remove(file_config);
  File fa = SPIFFS.open(file_config, "w+");
  String sa = "<WIFI SSID>";
  String sb = "<WIFI PASSWORD>";
  String sc = "netpie.io"; // Broker on change
  String sd = "<APPID>";
  String se = "<KEY>";
  String sf = "<SECRET>";
  String sg = "<ALIAS>";
  String sh = "no"; // TLS on change

  String message = ""
"{\n"
"\t\"ssid\":\""+sa+"\",\n"
"\t\"password\":\""+sb+"\",\n"
"\t\"broker\":\""+sc+"\",\n"
"\t\"netpie_appid\":\""+sd+"\",\n"
"\t\"netpie_key\":\""+se+"\",\n"
"\t\"netpie_secret\":\""+sf+"\",\n"
"\t\"netpie_alias\":\""+sg+"\",\n"
"\t\"netpie_tls\":\""+sh+"\"\n"
"}\n"
;
  fa.println(message);
  fa.close();
  
  File fb = SPIFFS.open(file_config,"r");
  fb.read((uint8_t*)json,fb.size());
  fb.close();

  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  if (root.containsKey("ssid"))           { strcpy(ssid,root["ssid"]); }
  if (root.containsKey("password"))       { strcpy(password,root["password"]); }
  if (root.containsKey("broker"))         { strcpy(broker,root["broker"]); }
  if (root.containsKey("netpie_appid"))   { strcpy(netpie_appid,root["netpie_appid"]); }
  if (root.containsKey("netpie_key"))     { strcpy(netpie_key,root["netpie_key"]); }
  if (root.containsKey("netpie_secret"))  { strcpy(netpie_secret,root["netpie_secret"]); }
  if (root.containsKey("netpie_alias"))   { strcpy(netpie_alias,root["netpie_alias"]); }
  if (root.containsKey("netpie_tls"))     { strcpy(netpie_tls,root["netpie_tls"]); }
  
  Serial.print("ssid: ");                 Serial.println( String(ssid) );
  Serial.print("password: ");             Serial.println( String(password) );
  Serial.print("broker: ");               Serial.println( String(broker) );
  Serial.print("netpie_appid: ");         Serial.println( String(netpie_appid) );
  Serial.print("netpie_key: ");           Serial.println( String(netpie_key) );
  Serial.print("netpie_secret: ");        Serial.println( String(netpie_secret) );
  Serial.print("netpie_alias: ");         Serial.println( String(netpie_alias) );
  Serial.print("netpie_tls: ");           Serial.println( String(netpie_tls) );

  WiFi.mode(WIFI_STA);
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (strcmp(broker,token_netpie)==0) {
    if (strcmp(netpie_tls,token_yes)==0) {
      microgear.useTLS(true);
    }
    microgear.init(netpie_key,netpie_secret,netpie_alias);
    microgear.connect(netpie_appid);
  }
}

void loop() {
  if (strcmp(broker,token_netpie)==0) {
    if (microgear.connected()) {
      microgear.loop();
    }
    else {
      Serial.println("connection lost, reconnect...");
      microgear.connect(netpie_appid);
    }
  }
}
