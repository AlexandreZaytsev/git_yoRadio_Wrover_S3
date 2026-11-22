/* ============================================================
   audioroute.cpp — реализация маршрутизатора аудио

   Точки интеграции (TODO):
   (1) конфиг: два поля audioSource / audioSink (0..N), дефолт 0
   (2) доступность SD: подставить реальную проверку вашего форка
   (3) переинициализация тракта при смене потребителя
       (в single core — пустая операция, точка резервируется)
   (4) останов/запуск потока: единые точки stop()/start()
       по именам функций вашего форка

   Логика переключения: stop -> смена маршрута -> запуск нового.
   ============================================================ */
#include "audioroute.h"

namespace audioroute {

static AudioSource s_source = AudioSource::RADIO;
static AudioSink   s_sink   = AudioSink::LOCAL;

/* ---------- (2) доступность источников/потребителей ---------- */

static bool sdAvailable() {
  // TODO(2): подставить реальную проверку вашего форка,
  // например: return SD_MMC.cardType() != CARD_NONE;
  return false;
}

static bool srcAllowed(AudioSource s) {
  switch (s) {
    case AudioSource::RADIO: return true;
    case AudioSource::SD:    return sdAvailable();
    case AudioSource::BT:    return false;  // dual core: этап 4
  }
  return false;
}

static bool sinkAllowed(AudioSink k) {
  switch (k) {
    case AudioSink::LOCAL:   return true;
    case AudioSink::BT_A2DP: return false;  // dual core: этап 4
  }
  return false;
}

/* ---------- (4) единые точки останова и запуска ---------- */

static void stopCurrent() {
  // TODO(4): единая точка останова — остановить активный поток.
  // Радио и SD-плеер: stop() по именам вашего форка.
  // Важно: останов ДО смены маршрута, чтобы два потока
  // не дрались за декодер.
}

static void startSource(AudioSource s) {
  switch (s) {
    case AudioSource::RADIO:
      // TODO(4): запуск радиопотока — тот же путь,
      // что и при старте радио сейчас (connecttohost и т.п.)
      break;
    case AudioSource::SD:
      // TODO(4): старт SD-плеера вашего форка
      break;
    default:
      break;
  }
}

/* ---------- (3) применение маршрута ---------- */

static void applyRouteNow() {
  // TODO(3): если потребитель сменился — переинициализация тракта.
  // В single core LOCAL остаётся единственным потребителем,
  // поэтому здесь пусто; в dual core — переключение направления
  // I2S (RX-источник / TX-вывод на Wrover).
  startSource(s_source);
}

/* ---------- публичный API ---------- */

bool begin() {
  // TODO(1): восстановить из конфига вашего форка, например:
  //   int s = config.general.audioSource;
  //   if (s >= 0 && s <= (int)AudioSource::BT) s_source = (AudioSource)s;
  //   int k = config.general.audioSink;
  //   if (k >= 0 && k <= (int)AudioSink::BT_A2DP) s_sink = (AudioSink)k;
  // Недоступные значения принудительно откатываются на дефолт:
  if (!srcAllowed(s_source)) s_source = AudioSource::RADIO;
  if (!sinkAllowed(s_sink))  s_sink   = AudioSink::LOCAL;
  applyRouteNow();
  return true;
}

bool setSource(AudioSource s) {
  if (!srcAllowed(s)) return false;
  if (s == s_source)  return true;
  stopCurrent();
  s_source = s;
  applyRouteNow();
  save();
  return true;
}

bool nextSource() {
  // циклический перебор только доступных источников:
  // кнопка «Источник» не застревает на недоступных вариантах
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
  // в single core доступен только LOCAL — фактический no-op,
  // но интерфейс уже готов к dual core
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
  // TODO(1): сохранить в конфиг вашего форка, например:
  //   config.general.audioSource = (uint8_t)s_source;
  //   config.general.audioSink   = (uint8_t)s_sink;
  //   config.commit();
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
