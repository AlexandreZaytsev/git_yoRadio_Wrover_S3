#ifndef MYOPTIONS_S3_H
#define MYOPTIONS_S3_H

/*******************************************************
ESP32-S3-WROOM-1 N16R8 (16MB Flash, 8MB OPI PSRAM)
GPIO 22-25: заняты OPI flash
GPIO 26-32: заняты OPI PSRAM  
GPIO 33-37: заняты OPI PSRAM
GPIO 19-20: заняты USB
Доступны: 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,21,38,39,40,41,42,43,44,45,46,47,48
********************************************************/

#define L10N_LANGUAGE       RU
#define DSP_MODEL           DSP_ILI9341

/* TFT DISPLAY (SPI) */
#define TFT_MISO            13
#define TFT_MOSI            11
#define TFT_CLK             18
#define TFT_ROTATE          2
#define TFT_CS              5
#define TFT_RST             -1
#define TFT_DC              4
#define BRIGHTNESS_PIN      14

/* I2S DAC */
#define I2S_DOUT            7
#define I2S_BCLK            6
#define I2S_LRC             8

/* VS1053 — отключён */
#define VS1053_CS           255
#define VS1053_DCS          255   /* дефолт 25 занят OPI flash */
#define VS1053_DREQ         42    /* дефолт 26 занят OPI PSRAM */

/* I2C — отключаем */
#define I2C_SDA             -1    /* дефолт 21 валиден, но не используется */
#define I2C_SCL             -1    /* дефолт 22 занят OPI flash */

/* ENCODER */
#define ENC_BTNL            1
#define ENC_BTNB            2
#define ENC_BTNR            15
#define ENC_HALFQUARD       true

/* ENCODER2 */
#define ENC2_BTNL           38
#define ENC2_BTNB           39
#define ENC2_BTNR            40
#define ENC2_HALFQUARD      false

/* IR control */
#define IR_PIN              21
#define IR_TIMEOUT          80

/* TOUCHSCREEN — отключаем */
#define TS_CS               255   /* не используется */
#define TS_SDA              255   /* дефолт 33 занят OPI PSRAM */
#define TS_SCL              255   /* перекрыто, не используется */
#define TS_INT              255   /* не используется */
#define TS_RST              255   /* дефолт 25 занят OPI flash */

/* AMP MUTE */
#define AMP_MUTE_GPIO       12

#endif
