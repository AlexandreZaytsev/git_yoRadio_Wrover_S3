#ifndef MYOPTIONS_S3_ALT_H
#define MYOPTIONS_S3_ALT_H

/*******************************************************
 ESP32-S3-WROOM-1 N16R8 (16MB Flash, 8MB OPI PSRAM)

 Раскладка GPIO:
   GPIO 22-25 — заняты OPI flash
   GPIO 26-32 — заняты OPI PSRAM
   GPIO 33-37 — заняты OPI PSRAM
   GPIO 19-20 — заняты USB
   Свободны: 0,1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,17,18,21,38-48
   Примечания:
     - 12 отдан TFT_CLK;
     - 46 — страповый пин (участвует в выборе режима загрузки), но
       как выход после старта использовать можно — см. блок AMP MUTE.
********************************************************/

#define L10N_LANGUAGE       RU
#define DSP_MODEL           DSP_ILI9341

/* ---------- TFT DISPLAY (SPI) ---------- */
#define TFT_MISO            13
#define TFT_MOSI            11
#define TFT_CLK             12
#define TFT_ROTATE          2
#define TFT_CS              10
#define TFT_RST             -1   // припаяно к EN — сброс вместе с платой
#define TFT_DC              9
#define BRIGHTNESS_PIN      14

/* ---------- I2S DAC ---------- */
#define I2S_DOUT            47
#define I2S_BCLK            38
#define I2S_LRC             39

/* ---------- VS1053 — отключён (255) ---------- */
#define VS1053_CS           255

/* ---------- I2C — не используется ---------- */

/* ---------- Энкодер 1: громкость ---------- */
#define ENC_BTNL            2    // s1
#define ENC_BTNB            1    // кнопка
#define ENC_BTNR            42   // s2
#define ENC_HALFQUARD       true

/* ---------- Энкодер 2: список станций ---------- */
#define ENC2_BTNL           40   // s1
#define ENC2_BTNB           41   // кнопка
#define ENC2_BTNR           21   // s2
#define ENC2_HALFQUARD      false

/* ---------- ИК-приёмник ---------- */
#define IR_PIN              8
#define IR_TIMEOUT          80

/* ---------- TOUCHSCREEN — не используется ---------- */
#define TS_CS               255
#define TS_SDA              255
#define TS_SCL              255
#define TS_INT              255
#define TS_RST              255

/* ---------- SDCARD (SDIO, slot 0) ---------- */
#define SDIO_SCK    7
#define SDIO_CMD    6
#define SDIO_D0     15
#define SDIO_D1     16
#define SDIO_D2     4
#define SDIO_D3     5
// SDC_CS: реального CS в SDIO нет, но значение !=255 нужно, чтобы
// включилась компиляция USE_SD. Ставим заведомо несуществующий GPIO 49 —
// пин никуда не подключается и ни с чем не конфликтует.
#define SDC_CS      49

/* ---------- AMP MUTE — управление усилителем PAM8406 ----------
   Логика (реализация в src/core/ampmute.h):
     при загрузке пин в состоянии «тишина», при старте воспроизведения
     через AMP_MUTE_UNMUTE_DELAY мс — «играть», на паузе/стопе — снова тишина.
   GPIO46 — страповый на S3, но на выбор режима загрузки не влияет:
   pinMode ставится только в ampMute.init() из setup(), до этого пин —
   вход с подтяжкой вниз. */
#ifndef AMP_MUTE_GPIO
  #define AMP_MUTE_GPIO       46   // пин MUTE усилителя (обёртка #ifndef — чтобы задать другой пин из платформенно-зависимого options)
#endif
#define AMP_MUTE_ACTIVE_HIGH   true  // HIGH = играть, LOW = тишина. Сверь с разводкой MUTE своего PAM8406 — если звук и тишина перепутаны, поменяй на false
#define AMP_MUTE_UNMUTE_DELAY  100   // мс от старта звука до снятия мьюта (гасим щелчок/пуп; если пробивает при включении — ставь 200-300)

/* ---------- Громкость: программный предел и подсветка перегруза ----------
   MAX_VOL — потолок громкости на ЦАПе (используется в Player::volToI2S, player.cpp):
   ход ручки 0..254 размазывается на 0..MAX_VOL, выше звук уже не прибавляется.
   VOL_OVER_COLOR — цвет числа громкости (_voltxt) и цифр в диалоге (_nums)
   за пределом MAX_VOL; подмешивается плавно, через blendColor()
   в Display::_volume() (display.cpp): чем дальше за предел — тем насыщеннее. */
#define MAX_VOL 190               // потолок громкости на ЦАПе, подбери на слух (170–200)
#ifndef VOL_OVER_COLOR
  #define VOL_OVER_COLOR 0xF800   // RGB565, красный
#endif

#endif
