#ifndef ampmute_h
#define ampmute_h
#include "Arduino.h"
#include "player.h"
#include "options.h"

/***************************************************************************
  AmpMute — управление пином MUTE усилителя (PAM8406)

  Что делает:
    - при загрузке держит усилитель в мьюте (гасим щелчок при включении питания);
    - при старте воспроизведения снимает мьют с задержкой AMP_MUTE_UNMUTE_DELAY
      (гасим "пуп" после клика);
    - на паузе/стопе мьютит мгновенно;
    - если во время задержки плеер остановился — возвращается в мьют.

  Подключение (main.cpp):
    #include "core/ampmute.h"
    setup():  ampMute.init(AMP_MUTE_GPIO);   // до раннего return в AP-режиме
    loop():   ampMute.loop();                // безусловно, вне зависимости от сети

  Настройка в options.h:
    AMP_MUTE_GPIO          — пин ESP32, куда заведён вход MUTE усилителя
    AMP_MUTE_ACTIVE_HIGH   — true:  HIGH = играть, LOW = тишина
                             false: LOW = играть, HIGH = тишина
    AMP_MUTE_UNMUTE_DELAY  — задержка снятия мьюта после старта, мс

  Ручное управление (плагин, MQTT, кнопка):
    ampMute.set(true)  — открыть усилитель,
    ampMute.set(false) — замьютить. После ручного размьюта loop() снова
                         замьютит при остановке плеера, как обычно.
***************************************************************************/

class AmpMute {
  public:
    // Инициализация пина и стартовое состояние — тишина.
    // Вызывается один раз в setup(), до ветвления на AP/STA
    // (в AP-режиме плеер не играет и класс сам останется в мьюте).
    void init(uint8_t pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      _apply(false);                       // при загрузке — тишина
      Serial.println("AMP mute: ON (initial)");
    }

    // Обслуживание автомата состояний. Вызывается из main loop() безусловно.
    // Пин переключается только по фронту состояния play/stop —
    // никаких digitalWrite на каждый проход цикла.
    void loop() {
      switch(_state) {
        case MUTED:                        // тишина: ждём старта воспроизведения
          if(player.isRunning()) { _state = WAIT_UNMUTE; _t = millis(); }
          break;
        case WAIT_UNMUTE:                  // пауза/стоп во время задержки — обратно в мьют
          if(!player.isRunning()) _state = MUTED;
          else if(millis() - _t >= AMP_MUTE_UNMUTE_DELAY) { _apply(true); _state = UNMUTED; }
          break;
        case UNMUTED:                      // играем: следим, не остановился ли плеер
          if(!player.isRunning()) { _apply(false); _state = MUTED; }
          break;
      }
    }

    // Ручное управление (плагин, MQTT, кнопка).
    void set(bool unmute) {
      if(unmute) { _apply(true); _state = UNMUTED; }
      else       { _apply(false); _state = MUTED; }
    }

  private:
    // Запись состояния на пин с учётом полярности AMP_MUTE_ACTIVE_HIGH
    void _apply(bool unmute) {
#ifdef AMP_MUTE_ACTIVE_HIGH
      digitalWrite(_pin, unmute == AMP_MUTE_ACTIVE_HIGH ? HIGH : LOW);
#else
      digitalWrite(_pin, unmute ? HIGH : LOW);
#endif
    }

    enum State : uint8_t { MUTED, WAIT_UNMUTE, UNMUTED };
    State _state = MUTED;
    uint8_t _pin;
    uint32_t _t;                           // время старта воспроизведения (для задержки)
};

extern AmpMute ampMute;
#endif
