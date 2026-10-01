#include "Arduino.h"
#include "src/core/options.h"
#include "src/core/config.h"
#include "src/pluginsManager/pluginsManager.h"
#include "src/core/telnet.h"
#include "src/core/player.h"
#include "src/core/display.h"
#include "src/core/network.h"
#include "src/core/netserver.h"
#include "src/core/controls.h"
#include "src/core/mqtt.h"
#include "src/core/optionschecker.h"
#include "src/core/timekeeper.h"
#ifdef USE_NEXTION
#include "src/displays/nextion.h"
#endif

#if USE_OTA
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
#include <NetworkUdp.h>
#else
#include <WiFiUdp.h>
#endif
#include <ArduinoOTA.h>
#endif

#if DSP_HSPI || TS_HSPI || VS_HSPI
SPIClass  SPI2(HSPI);
#endif

extern __attribute__((weak)) void yoradio_on_setup();

#if USE_OTA
void setupOTA(){
  if(strlen(config.store.mdnsname)>0)
    ArduinoOTA.setHostname(config.store.mdnsname);
#ifdef OTA_PASS
  ArduinoOTA.setPassword(OTA_PASS);
#endif
  ArduinoOTA
    .onStart([]() {
      player.sendCommand({PR_STOP, 0});
      display.putRequest(NEWMODE, UPDATING);
      telnet.printf("Start OTA updating %s\n", ArduinoOTA.getCommand() == U_FLASH?"firmware":"filesystem");
    })
    .onEnd([]() {
      telnet.printf("\nEnd OTA update, Rebooting...\n");
      ESP.restart();
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      telnet.printf("Progress OTA: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      telnet.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        telnet.printf("Auth Failed\n");
      } else if (error == OTA_BEGIN_ERROR) {
        telnet.printf("Begin Failed\n");
      } else if (error == OTA_CONNECT_ERROR) {
        telnet.printf("Connect Failed\n");
      } else if (error == OTA_RECEIVE_ERROR) {
        telnet.printf("Receive Failed\n");
      } else if (error == OTA_END_ERROR) {
        telnet.printf("End Failed\n");
      }
    });
  ArduinoOTA.begin();
}
#endif

//---------------------------------------------------------
//const int AMP_MUTE_GPIO = 12;//2;//21;//2;      // Выход на PAM8406
// Состояние машины состояний для задержки
enum MuteState {
  STATE_IDLE,
  STATE_WAIT_STARTUP,      // ждём 500 мс после старта
  STATE_WAIT_UNMUTE_CLICK, // ждём 100 мс перед снятием Mute после клика
  STATE_NORMAL
};
MuteState currentState = STATE_WAIT_STARTUP;
unsigned long stateStartTime = 0;
//---------------------------------------------------------


void setup() {
  Serial.begin(115200);

 // Выводим все критические пины
  Serial.println("DEBUG PINS:");
  Serial.print("BRIGHTNESS_PIN: "); Serial.println(BRIGHTNESS_PIN);
  Serial.print("I2C_SCL: "); Serial.println(I2C_SCL);
  Serial.print("VS1053_DCS: "); Serial.println(VS1053_DCS);
  Serial.print("VS1053_DREQ: "); Serial.println(VS1053_DREQ);
  Serial.print("I2S_DOUT: "); Serial.println(I2S_DOUT);
  Serial.print("I2S_BCLK: "); Serial.println(I2S_BCLK);
  Serial.print("I2S_LRC: "); Serial.println(I2S_LRC);
  Serial.print("TS_SDA: "); Serial.println(TS_SDA);
  Serial.print("TS_SCL: "); Serial.println(TS_SCL);
  Serial.print("TS_RST: "); Serial.println(TS_RST);
  Serial.print("TFT_MOSI: "); Serial.println(TFT_MOSI);






  if(REAL_LEDBUILTIN!=255) pinMode(REAL_LEDBUILTIN, OUTPUT);
  if (yoradio_on_setup) yoradio_on_setup();
  pm.on_setup();
  config.init();
  display.init();
  player.init();
  network.begin();
  if (network.status != CONNECTED && network.status!=SDREADY) {
    netserver.begin();
    initControls();
    display.putRequest(DSP_START);
    while(!display.ready()) delay(10);
    return;
  }
  if(SDC_CS!=255) {
    display.putRequest(WAITFORSD, 0);
    Serial.print("##[BOOT]#\tSD search\t");
  }
  config.initPlaylistMode();
  netserver.begin();
  telnet.begin();
  initControls();
  display.putRequest(DSP_START);
  while(!display.ready()) delay(10);
  #ifdef MQTT_ROOT_TOPIC
    mqttInit();
  #endif
  #if USE_OTA
    setupOTA();
  #endif
  if (config.getMode()==PM_SDCARD) player.initHeaders(config.station.url);
  player.lockOutput=false;
  if (config.store.smartstart == 1) {
    player.sendCommand({PR_PLAY, config.lastStation()});
  }
  pm.on_end_setup();

//---------------------------------------------------------
  pinMode(AMP_MUTE_GPIO, OUTPUT);
  // Начальное состояние — тишина
  digitalWrite(AMP_MUTE_GPIO, LOW);


  Serial.println("Mute ON (initial)");
  currentState = STATE_WAIT_STARTUP;
  stateStartTime = millis();            //время старта
//---------------------------------------------------------
}


void loop() {
  unsigned long now = millis();         //текущее время

  timekeeper.loop1();
  telnet.loop();
  if (network.status == CONNECTED || network.status==SDREADY) {
    player.loop();

    digitalWrite(AMP_MUTE_GPIO, HIGH);
   // digitalWrite(AMP_MUTE_GPIO, LOW);

#if USE_OTA
    ArduinoOTA.handle();
#endif
  }
  
  loopControls();

  #ifdef NETSERVER_LOOP1
    netserver.loop();
  #endif
}

#include "src/core/audiohandlers.h"
