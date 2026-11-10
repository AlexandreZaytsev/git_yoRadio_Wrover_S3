//защита от повторного включения (уникальное имя макроса)
#ifndef MYOPTIONS_WROVER_H
#define MYOPTIONS_WROVER_H

/*******************************************************
 ESP32-WROVER (классический ESP32, 4MB flash + PSRAM)

 Особенности раскладки GPIO:
   GPIO 34, 35, 36, 39 — входы БЕЗ внутренних подтяжек:
     для кнопок/энкодеров на них нужны внешние резисторы ~10 кОм на 3.3V
   GPIO 6-11 — заняты внутренней flash
   GPIO 12 (MTDI) — страповый: ВЫСОКИЙ уровень при подаче питания
     выбирает напряжение flash 1.8V и плата не стартует.
     См. блок AMP MUTE — на этом пине у нас усилитель!
********************************************************/

#define L10N_LANGUAGE       RU //EN                /*  Язык интерфейса (EN, RU) */

/*  DSP_MODEL — модель дисплея. Обязательный параметр.
    Список доступных значений: https://github.com/e2002/yoradio/wiki/Available-display-models  */
#define DSP_MODEL			      DSP_ILI9341    // 320x240 IPS/SPI

/*
 * !!! Важно для классического ESP32 (WROOM/WROVER) с цветным TFT:
 * памяти не хватает — правим библиотеку Arduino/libraries/AsyncTCP/src/AsyncTCP.cpp,
 * строка 221: 8192 * 2  →  8192 / 2
 * (на ESP32-S3 с его 512KB RAM это не требуется)
*/

/******************************************/
/* VSPI — по умолчанию основной SPI для дисплея и SD.
   Штатные пины VSPI: SCK=18, MOSI=23, MISO=19 */

#define TFT_MISO            19                /* данные от дисплея */
#define TFT_MOSI            23                /* данные к дисплею */
#define TFT_CLK             18                /* тактирование SPI */
#define TFT_ROTATE          2                 // ориентация экрана

#define TFT_CS              5                 /*  SPI CS pin  */
#define TFT_RST             -1                /* сброс дисплея: -1 = припаян к EN платы, ресетится вместе с ней */
#define TFT_DC              4                 /*  SPI DC/RS pin  */
#define BRIGHTNESS_PIN      22                /*  ШИМ-управление подсветкой, 0..3.3V */

//#define DSP_HSPI          false             /*  Use HSPI for display  */
/******************************************/
/*  NEXTION  */                           /* дисплей NEXTION — не используется */
//#define NEXTION_RX        255
//#define NEXTION_TX        255

/*  I2C  */                               /* не используется */
//#define I2C_SDA           -1 //21
//#define I2C_SCL           -1 //22
//#define I2C_RST           -1

/*  I2S DAC  */
#define I2S_DOUT            27                /* DIN — данные на ЦАП */
#define I2S_BCLK            26                /* BCLK — битовый такт */
#define I2S_LRC             25                /* WSEL — левый/правый канал */
/******************************************/
/*  VS1053 — отключён (255).
   Если когда-нибудь включать: штатные пины на VSPI совпадают с дисплеем
   (SCK=18, MISO=19, MOSI=23), либо переехать на HSPI: 14/12/13  */
#define VS1053_CS           255
//#define VS1053_DCS        25
//#define VS1053_DREQ       26
//#define VS1053_RST        -1
//#define VS_HSPI           false
/******************************************/

/*  ENCODER 1 — громкость  */
#define ENC_BTNL            33           /*  вращение влево  */
#define ENC_BTNB            32           /*  кнопка энкодера */
#define ENC_BTNR            35           /*  вращение вправо */
//#define ENC_INTERNALPULLUP  true        /*  внутренние подтяжки (для 35 нужна внешняя!) */
#define ENC_HALFQUARD       true         /*  точность отсчёта щелчков — экспериментируй */
/******************************************/

/*  SDCARD — не используется в этой сборке (SDC_CS закомментирован).
   При включении SD на VSPI: SCK=18, MISO=19(=D0), MOSI=23(=D1), плюс CS.
   На HSPI: SCK=14, MISO=12, MOSI=13  */
//#define SDC_CS                 255
//#define SD_HSPI       				 false
/*        RTC  — не используется */
//#define RTC_MODULE            RTC_MODULE_UNDEFINED
//#define RTC_SDA               255
//#define RTC_SCL               255

/*  ENCODER 2 — список станций  */
/*  ВНИМАНИЕ: 34, 36, 39 — входы БЕЗ внутренних подтяжек.
      ENC2_INTERNALPULLUP закомментирован, поэтому на все три контакта
      нужны внешние резисторы ~10 кОм на 3.3V, иначе будет «фантомное»
      срабатывание от наводок. */
#define ENC2_BTNL              34           /*  вращение влево  */
#define ENC2_BTNB              39           /*  кнопка энкодера */
#define ENC2_BTNR              36           /*  вращение вправо */
//#define ENC2_INTERNALPULLUP    true
#define ENC2_HALFQUARD         false        /*  (true, false, 255) — экспериментируй */
/******************************************/

/*  BUTTONS, LCD 1602, TOUCHSCREEN — не используются, оставлены
    закомментированными как справка из шаблона  */
//#define BTN_LEFT              255
//#define BTN_CENTER            255
//#define BTN_RIGHT             255
//#define BTN_UP                255
//#define BTN_DOWN              255
//#define BTN_INTERNALPULLUP    true
//#define BTN_LONGPRESS_LOOP_DELAY    200
//#define BTN_CLICK_TICKS    300
//#define BTN_PRESS_TICKS    500
//#define BTN_MODE              255

//#define LCD_RS                255
//#define LCD_E                 255
//#define LCD_D4                255
//#define LCD_D5                255
//#define LCD_D6                255
//#define LCD_D7                255

//#define TS_MODEL              TS_MODEL_UNDEFINED
//#define TS_CS                 255
//#define TS_HSPI               false
//#define TS_SDA                33
//#define TS_SCL                32
//#define TS_INT                21
//#define TS_RST                25

/*  Прочие настройки — шаблонные, не трогаем. Справка: DTYPE — подмодель ST7735;
    WAKE_PIN — пин для выхода из сна (только 0,2,4,12,13,14,15,25,26,27,32,33,34,35,36,39) */
//#define DTYPE             INITR_BLACKTAB
//#define MUTE_PIN          255
//#define MUTE_VAL          HIGH
//#define PLAYER_FORCE_MONO false
//#define I2S_INTERNAL      false
//#define ROTATE_90         false
//#define WAKE_PIN          255
//#define LIGHT_SENSOR      255
//#define AUTOBACKLIGHT(x)  *function*
//#define DSP_INVERT_TITLE  true

/*  IR control  */
#define IR_PIN                15
#define IR_TIMEOUT            80              /*  задержка детектирования пачки ИК-битов */

/******************************************/

/* ---------- AMP MUTE — управление усилителем PAM8406 ----------
   Логика (реализация в src/core/ampmute.h):
     при загрузке пин в состоянии «тишина», при старте воспроизведения
     через AMP_MUTE_UNMUTE_DELAY мс — «играть», на паузе/стопе — снова тишина.

   !!! ВНИМАНИЕ с GPIO12: на классическом ESP32 это страповый пин MTDI.
   Если в момент подачи питания на нём HIGH — плата выбирает flash 1.8V
   и не стартует. Схема мьюта не должна подтягивать этот пин вверх при
   включении. Надёжнее перенести мьют на любой свободный GPIO (например,
   13 или 14) и перепаять провод. */
#ifndef AMP_MUTE_GPIO
    #define AMP_MUTE_GPIO       12 // пин MUTE усилителя PAM8406 — см. предупреждение выше!
#endif
#define AMP_MUTE_ACTIVE_HIGH   true   // HIGH = играть, LOW = тишина. Если полярность перепутана (звук при «тишине») — поставить false
#define AMP_MUTE_UNMUTE_DELAY  100    // мс после старта звука до снятия мьюта (гасим щелчок/пуп; если пробивает при включении — ставь 200–300)

/* ---------- Громкость: программный предел и подсветка перегруза ----------
   MAX_VOL — потолок громкости на ЦАПе (используется в Player::volToI2S, player.cpp):
   ход ручки 0..254 размазывается на 0..MAX_VOL, выше звук уже не прибавляется.
   VOL_OVER_COLOR — цвет числа громкости (_voltxt) и цифр в диалоге (_nums)
   за пределом MAX_VOL; подмешивается плавно через blendColor()
   в Display::_volume() (display.cpp): чем дальше за предел — тем насыщеннее. */
#define MAX_VOL 190               // потолок громкости на ЦАПе, подбери на слух (170–200)
#ifndef VOL_OVER_COLOR
  #define VOL_OVER_COLOR 0xF800   // RGB565, красный
#endif

#endif        //конец защиты от повторного включения
