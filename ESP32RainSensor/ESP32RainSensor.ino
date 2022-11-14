#define BLYNK_TEMPLATE_ID "TMPLXLy_ddnz"
#define BLYNK_DEVICE_NAME "chuva"
#define BLYNK_AUTH_TOKEN "Sg2Zb1ZgYezNT1XmI0YhgOQvi9Swfo6S"

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <ezTime.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "xxxxx";
char pass[] = "xxxxx";

bool estaChovendo = false;
int timeInSeconds = 3600000;
int pinRainSensor = 4;

unsigned long millisRainSensor = millis();

WidgetTerminal terminal(V1);
BLYNK_WRITE(V1)
{
  String terminalInput = param.asStr();
  if(terminalInput.substring(0,8) == "/setdimn "){
    terminalInput.remove(0,8);
    timeInSeconds = terminalInput.toInt();
    terminal.print("O tempo de atualizacao foi definido para: ");
    terminal.print(timeInSeconds);
    //terminal.write(timeInSeconds.getBuffer(), timeInSeconds.getLength());
    terminal.println();
    terminal.flush();
  } else {
    terminal.print("Você disse: ");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
    terminal.flush();
  }
  
}

void setup()
{
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  terminal.clear();

  pinMode(pinRainSensor, INPUT);

  terminal.println(F("Blynk v" BLYNK_VERSION " : Device started"));
  terminal.flush();
}

void loop()
{
  if(!digitalRead(pinRainSensor) && (!estaChovendo || ((millis()-millisRainSensor)>timeInSeconds))){
    estaChovendo = true; 
    Blynk.logEvent("est_chovendo");
    /*Timezone local;
    local.setLocation("America/Fortaleza");
    Blynk.logEvent("est_chovendo", String("Detecção feita as: ") + local.dateTime("H:i:s T"));*/
    //terminal.println(F("notificacao enviada")); " + local.dateTime("l, d-M-y H:i:s.v T") + "
    Serial.print("notificacao enviada\n");
    millisRainSensor = millis();

  } else if(digitalRead(pinRainSensor) && estaChovendo){
    estaChovendo = false;
  }

  Blynk.run();
  delay(70);
}

