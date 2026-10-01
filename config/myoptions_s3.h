#ifndef MYOPTIONS_S3_H
#define MYOPTIONS_S3_H

/*******************************************************
ESP32-S3-WROOM-1 N16R8 (16MB Flash, 8MB OPI PSRAM)
GPIO 22-25 НЕ СУЩЕСТВУЮТ на S3
GPIO 26-37 заняты Flash/PSRAM
GPIO 19-20 заняты USB
Доступны: 1,2,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,21,38,39,40,41,42,47,48
********************************************************/

#define L10N_LANGUAGE       RU
#define DSP_MODEL           DSP_ILI9341

/*  TFT DISPLAY (SPI)  */
#define TFT_MISO            13
#define TFT_MOSI            11    /* GPIO 23 не существует на S3 */
#define TFT_CLK             18
#define TFT_ROTATE          2

#define TFT_CS              5
#define TFT_RST             -1
#define TFT_DC              4
#define BRIGHTNESS_PIN      14    /* GPIO 22 не существует на S3 */

/*  I2S DAC  */
#define I2S_DOUT            7
#define I2S_BCLK            6
#define I2S_LRC             8     /* GPIO 25 не существует на S3 */

/*  VS1053 — отключаем все пины  */
#define VS1053_CS           255
#define VS1053_DCS          255   /* дефолт 25 не существует на S3 */
#define VS1053_DREQ         255   /* дефолт 26 занят Flash */

/*  I2C — отключаем (дефолт I2C_SCL=22 не существует)  */
#define I2C_SDA             -1
#define I2C_SCL             -1

/*  ENCODER  */
#define ENC_BTNL            1
#define ENC_BTNB            2
#define ENC_BTNR            15
#define ENC_HALFQUARD       true

/*  ENCODER2  */
#define ENC2_BTNL           38
#define ENC2_BTNB           39
#define ENC2_BTNR           40
#define ENC2_HALFQUARD      false

/*  IR control  */
#define IR_PIN              21
#define IR_TIMEOUT          80

/*  TOUCHSCREEN — отключаем  */
/* дефолты TS_SDA=33, TS_SCL=32 заняты PSRAM, TS_RST=25 не существует  */
#define TS_SDA              255
#define TS_SCL              255
#define TS_RST              255

/*  AMP MUTE  */
#define AMP_MUTE_GPIO       12



#endif

/*
pio run -e esp32-s3-kit -t clean
pio run -e esp32-s3-kit

pio run -e esp32-s3-kit -t clean
pio run -e esp32-s3-kit -t upload
pio device monitor -e esp32-s3-kit


*/