#ifndef ampmute_h
#define ampmute_h
#include "Arduino.h"
#include "player.h"
#include "options.h"

// Управление пином MUTE усилителя (PAM8406): размьют с задержкой после старта
// воспроизведения (гасим щелчок/пуп), мгновенный мьют на паузе/стопе.
// Поларитет настраивается в options.h: AMP_MUTE_ACTIVE_HIGH

class AmpMute {
  public:
    void init(uint8_t pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      _apply(false);                       // при загрузке — тишина
      Serial.println("AMP mute: ON (initial)");
    }
    void loop() {
      switch(_state) {
        case MUTED:
          if(player.isRunning()) { _state = WAIT_UNMUTE; _t = millis(); }
          break;
        case WAIT_UNMUTE:                  // пауза/стоп во время задержки — обратно в мьют
          if(!player.isRunning()) _state = MUTED;
          else if(millis() - _t >= AMP_MUTE_UNMUTE_DELAY) { _apply(true); _state = UNMUTED; }
          break;
        case UNMUTED:
          if(!player.isRunning()) { _apply(false); _state = MUTED; }
          break;
      }
    }
    void set(bool unmute) {                // ручное управление (плагин, MQTT, кнопка)
      if(unmute) { _apply(true); _state = UNMUTED; }
      else       { _apply(false); _state = MUTED; }
    }
  private:
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
    uint32_t _t;
};

extern AmpMute ampMute;
#endif
