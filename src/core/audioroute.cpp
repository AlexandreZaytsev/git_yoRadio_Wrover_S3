/* ============================================================
   audioroute.cpp — реализация маршрутизатора аудио

   Сшивка (вместо TODO-заглушек):
   (1) конфиг: config.store.audioSource / audioSink (CONFIG_VERSION 6)
   (2) доступность SD: sdman.ready && sdman.cardPresent()
   (3) тракт потребителя: хук, в single core пусто (dual core — этап 4)
   (4) останов/запуск: идиома resetPlayer() — resetQueue +
       PR_STOP + разовый player.loop()
   ============================================================ */
#include "audioroute.h"
#include "options.h"
#include "config.h"
#include "player.h"
#include "sdmanager.h"

#include "network.h"

namespace audioroute {

static AudioSource s_source = AudioSource::RADIO;
static AudioSink   s_sink   = AudioSink::LOCAL;

/* ---------- (2) доступность источников/потребителей ---------- */

static bool sdAvailable() {
#if SDC_CS != 255
  return sdman.ready && sdman.cardPresent();
#else
  return false;
#endif
}

static bool srcAllowed(AudioSource s) {
  switch (s) {
    case AudioSource::RADIO: return true;  // сеть решает сама: свои ошибки коннекта отработает плеер
    case AudioSource::SD:    return sdAvailable();
    case AudioSource::BT:    return false; // dual core: этап 4
  }
  return false;
}

static bool sinkAllowed(AudioSink k) {
  switch (k) {
    case AudioSink::LOCAL:   return true;
    case AudioSink::BT_A2DP: return false; // dual core: этап 4
  }
  return false;
}

/* ---------- (4) единые точки останова и запуска ---------- */

static void stopCurrent() {
  if (!player.isRunning()) return;   // уже тихо — нечего останавливать
  player.resetQueue();               // несработавшие чужие команды — вон
  player.sendCommand({PR_STOP, 0});
  player.loop();                     // та же идиома, что в resetPlayer(): разово прокрутить очередь
}

static void startSource(AudioSource s) {
  int want = (s == AudioSource::SD) ? PM_SDCARD : PM_WEB;
  if (config.getMode() != want) {
    config.changeMode(want);
    config.initPlaylistMode();       // плейлист должен смотреть в файл нужного режима
  }
  player.sendCommand({PR_PLAY, config.lastStation()});
}

/* ---------- (3) применение маршрута ---------- */

static void applyRouteNow() {
  // Хук потребителя: в single core sink всегда LOCAL, I2S-тракт
  // поднят один раз в Player::init() — делать нечего.
  // Dual core: здесь будет переинициализация I2S/TX на Wrover.
  startSource(s_source);
}

/* ---------- публичный API ---------- */

bool begin() {
  /* --- ТЕСТ: принудительный источник SD (убрать после отладки) --- */
  //s_source = AudioSource::SD;


  int s = config.store.audioSource;
  int k = config.store.audioSink;
  if (s >= 0 && s <= (int)AudioSource::BT)    s_source = (AudioSource)s;
  if (k >= 0 && k <= (int)AudioSink::BT_A2DP) s_sink   = (AudioSink)k;
  if (!srcAllowed(s_source)) s_source = AudioSource::RADIO;
  if (!sinkAllowed(s_sink))  s_sink   = AudioSink::LOCAL;

 // Режим плеера выравниваем с источником, но НЕ запускаем
  // воспроизведение — его сделает smartstart или первое нажатие.
  int want = (s_source == AudioSource::SD) ? PM_SDCARD : PM_WEB;
  if (config.getMode() != want) {
    if (network.status == CONNECTED) {
      config.changeMode(want);
      config.initPlaylistMode();
    } else {
      // сеть ещё не поднята (SDREADY на буте) — changeMode сейчас
      // означает ESP.restart(). Выравниваем источник под фактический режим.
      s_source = (config.getMode() == PM_SDCARD) ? AudioSource::SD : AudioSource::RADIO;
      save();
    }
  }
  
  // конфиг мог быть исправлен (недоступный источник) — синхронизируем
  if ((uint8_t)s_source != config.store.audioSource ||
      (uint8_t)s_sink   != config.store.audioSink) save();
  return true;
}

bool setSource(AudioSource s) {
  if (!srcAllowed(s)) return false;
  if (s == s_source)  return true;
  stopCurrent();          // останов ДО смены маршрута — декодер не дёргается
  s_source = s;
  applyRouteNow();
  save();
  return true;
}

bool nextSource() {
  for (uint8_t i = 1; i <= (uint8_t)AudioSource::BT; i++) {
    AudioSource s = (AudioSource)(((uint8_t)s_source + i) % ((uint8_t)AudioSource::BT + 1));
    if (srcAllowed(s)) return setSource(s);
  }
  return false;
}

bool setSink(AudioSink k) {
  if (!sinkAllowed(k)) return false;
  if (k == s_sink)     return true;
  stopCurrent();
  s_sink = k;
  applyRouteNow();
  save();
  return true;
}

bool nextSink() {
  for (uint8_t i = 1; i <= (uint8_t)AudioSink::BT_A2DP; i++) {
    AudioSink k = (AudioSink)(((uint8_t)s_sink + i) % ((uint8_t)AudioSink::BT_A2DP + 1));
    if (sinkAllowed(k)) return setSink(k);
  }
  return false;
}

void applyRoute() {
  stopCurrent();
  applyRouteNow();
}

void save() {
  config.saveValue(&config.store.audioSource, (uint8_t)s_source);
  config.saveValue(&config.store.audioSink,   (uint8_t)s_sink);
}

/* SD исчезла на ходу — источник недоступен, маршрут откатывается.
   Вызывается из PR_CHECKSD ДО changeMode: без рестарта плеера,
   без рентрантности player.loop(). */
void onSourceLost() {
  if (s_source == AudioSource::SD) {
    s_source = AudioSource::RADIO;
    save();
  }
}

AudioSource source() { return s_source; }
AudioSink   sink()   { return s_sink; }

const char* sourceName() {
  switch (s_source) {
    case AudioSource::RADIO: return "Radio";
    case AudioSource::SD:    return "SD card";
    case AudioSource::BT:    return "Bluetooth";
  }
  return "?";
}

const char* sinkName() {
  switch (s_sink) {
    case AudioSink::LOCAL:   return "Speakers";
    case AudioSink::BT_A2DP: return "Bluetooth";
  }
  return "?";
}

} // namespace audioroute
