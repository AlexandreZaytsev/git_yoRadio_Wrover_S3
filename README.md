# yoRadio.vs

Platformio клоны Arduino сборки [ёРадио](https://github.com/e2002/yoradio/?ysclid=mvw0c4k7jv370463529) Releases v0.9.693    
esp32 wrover e n8r8    
[pinout ESP32-DevKitC V4](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html#overview)
<img width="1359" height="391" alt="image" src="https://github.com/user-attachments/assets/16f74f1d-b205-40cc-8eb1-5b08f8e9d965" />

esp32 s3 n16r8    
[chip and lcd from GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board](https://github.com/profharris/GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board/blob/main/README.md)    
[pinout ESP32-S3-DevKitC-1 v1.1](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.0.html)
<img width="1491" height="479" alt="image" src="https://github.com/user-attachments/assets/954696eb-6092-46a1-a627-f03162bf92f7" />

Железо в каталоге \hardware    
Комментарии   
- Корпус - выкидываем все кроме динамиков (это фронтт), печатаем морду на 3D принтере со всеми интерфейсами (это бэк)    
- Платы разработки в проекте с чипами esp32 wrover i r8n8 и [esp32 s3 alt1 n16r8 из набора](https://github.com/profharris/GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board) 
- Монитор ILI9341 без тач или с [тач из набора](https://github.com/profharris/GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board) - но на тач пинов не хватит, кстати отличаются по посадке - с тач  pcb короче
- Энкодеры - Питание - вместо 5v - подаем 3.3v иначе входы esp32 сгорят
- Модуль-адаптер JETTING Micro SD SPI или SDIO 3В
- ИК - Модуль-адаптер JETTING Micro SD SPI или SDIO 3В или набор с пультом TENSTAR ROBOT HX1838

аудио тракт
- ЦАП pcm5102A - не забыть запаять перемычки и закоротить SCK (этот выавод не используем) Питание 5v, на выходе аналоговую землю не коротим с цифровой пойтет помеха
- с ЦАП идем не внешний джек и обратно возвращаем сигнал на усилитель
- усилитель HT8698 имеет один управляющий вход CTRL - через 1 ком притягиваем его на землю - mute, обратно включаем программно через pin esp32
*вся аналоговая земля = выход ЦАП + джек + вход усилка - НЕ ДОЛЖНА быть закорочена на цифровую землю - пойдет помеха, выход усилка со своей землей чисто на динамики*

упс 2S
- два аккумулятора 18650
- Модуль зарядного устройства для литиевых батарей 18650 2S
- Модуль Литиевая плата защиты BMS 2S 20A 7,4В 8,4В 18650
- Кнопка On/Off Модуль Регулятор напряжения TZT XL-10AH
*Кнопка дублируеттся внешней кнопкой с подсветкой без фиксации (Momentary Self-reset) + светодиоод на 3V-6V(5V)*
- Модуль преобразователя напряжения occkic 3.6-32В в 5/9/12В 5А

вход
- Ставим шнурок Type-C 6pin для быстрой зарядки и передачи данных (DongLiLian USB-C4P6P водонепроницаемый разъем TYPE C2P) 
* с одной стороны Femaly на корпус
* с другой стороны Male разъем 
*из шнурка от Male разъема отключаем D+ D- и припаиваем к ним переходник 4pin VCC GND D+ D-*
*зазъем Male внутри корпуса втыкаем в зарядник, а отведенные два провода через переходник в плату ESP32*
*таким образом через один внешний раъем Type-C мы можем подключить или быструю зарядку или кабель для прошивки* 

схему собираем по распиновке для выбранной платы 
для wrover /config/myoptions_wrover.h
для s3 alt /config/myoptions_s3_alt.h

софт
для wrover используем окружение    
default_envs = esp32-wrover-kit из файла /config/env/config_DevBoard_ESP32_Wrover-kit.ini
для s3 alt используем окружение   
default_envs = esp32-s3-devkitc-1-alt из файла /config/env/config_DevBoard_ESP32-S3-DevKitC-1-ALT.ini 

правки оригинала
- добавлен пин управлени mute - переменная #define AMP_MUTE_GPIO
*в /src/main.cpp отключаем mute (ставим 1) усилка после подключения к wifi*
- добавлено ограничение громкости при сохранении диапазона энкодера 255 - переменная - #define MAX_VOL 190
*переписана функция /src/core/pllayer.cpp функция - volToI2S*
- расширено количество wifi сетей с 5ти до 10
*/src/core/config.h neworkItem ssids[10];*
*/src/core/config.cpp функция - Config::initNetwork*
*/data/www/options.html.gz добавлено в разметку +5 полей для wifi*
- в вебморду добавлен блок будильника (типа спящий режим)- но пока не работает, не знаю как..., просто нарисовал
*/data/www/options.html.gz*

