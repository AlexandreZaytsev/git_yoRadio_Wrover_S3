/* ============================================================
   audioroute.h — маршрутизатор аудио: источник x потребитель

   Две независимые оси маршрута (активна ровно одна пара):
     источник:  RADIO -> SD -> BT
     потребитель: LOCAL -> BT_A2DP

   Заглушки BT честно отклоняются в single core (srcAllowed /
   sinkAllowed) и заработают в dual core без правок меню.

   Замечание для dual core: BT-источник (Wrover = A2DP sink,
   поток приходит по I2S) и BT-потребитель (Wrover = A2DP
   source, выдача в колонку) — два РАЗНЫХ режима работы Wrover,
   одновременно они недопустимы. Валидацию комбинаций добавить
   в setSource()/setSink() на этапе dual core.
   ============================================================ */
#ifndef AUDIOROUTE_H
#define AUDIOROUTE_H

#include <Arduino.h>

namespace audioroute {

enum class AudioSource : uint8_t { RADIO = 0, SD, BT };
enum class AudioSink   : uint8_t { LOCAL  = 0, BT_A2DP };

/* Инициализация: восстановить маршрут из конфига (TODO(1)),
   применить его к плееру. Вызывать после загрузки конфига. */
bool begin();

/* --- ось «источник» --- */
bool setSource(AudioSource s);  // false = источник недоступен
bool nextSource();              // циклически по доступным
AudioSource source();
const char* sourceName();       // "Radio" / "SD card" / "Bluetooth"

/* --- ось «потребитель» --- */
bool setSink(AudioSink k);
bool nextSink();
AudioSink sink();
const char* sinkName();         // "Speakers" / "Bluetooth"

/* Единая точка последовательности: stop -> смена маршрута ->
   запуск нового. Вызывать из пунктов меню ПОСЛЕ setSource /
   setSink (или вместо пары «останов+запуск» в своём коде). */
void applyRoute();

/* Сохранить текущий маршрут в конфиг (TODO(1)). */
void save();

/* SD исчезла на ходу: откат источника на RADIO + сохранение.
   Вызывать ДО config.changeMode(PM_WEB) — имена функций не вызываем. */
void onSourceLost();

} // namespace audioroute

#endif // AUDIOROUTE_H
