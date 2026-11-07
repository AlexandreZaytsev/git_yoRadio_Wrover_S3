


# yoRadio.vs

авторы:
* Серега Соколов - все четко и продуманно - вся схемотехнка, выбор компонентов, софта, 3d печать, пайка, логика идеи, правильные слова и огромная поддержка!!!
* Александр Зайцев - чисто порисовать, поболтать, помоделировать, поп-п-программировать, попаять - не вдаваясь особенно в детали 
* Алиса Ай - реинженеринг/хакинг - наш чел! полностью перелопатила исходный проект Arduino ёРадио, помогла перенести его на VSCode Platformio, нашла все глюки, испонилла все хотелки, подключила SD в режиме SDIO (я и слов то таких не знаю) - короче без нее - наш проект появился бы на пару лет позже!!! Супер - пока бесплатна!

первая версия         
![v1](/hardware/release_v1.png)    

https://github.com/user-attachments/assets/2cd889ca-598d-4703-a9b4-8989fbc78619

вторая версия    
![3d](/hardware/kompas_3d/asm.png)    
![v1](/hardware/release_v2.png)    

https://github.com/user-attachments/assets/9a0339bc-2b3c-4589-a19d-a416dd21c862        

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
- Корпус - саундбар на шнурке (усилок NS4248 без диф входа, громкость на переменнике 50к) дерево (реально mdf 5mm) Sunbuck LONVIN 80x80x215 динамики 2х10w 4ohm (реально 6w) - выкидываем все кроме динамиков (это фронтт), печатаем морду (/hardware/kompas_3d/box_compas.zip) на 3D принтере со всеми интерфейсами (это бэк)    
*дальше просто меняем морды и начинку в рамках одного корпуса (в первой версии UPS был штатный на TZT Type-C 15Вт 3А во второй уже модерн!!! плюс внешняя аненна - т.к. в тестовой эксплуатации на югах не удавалось отойти долеко от передатчика wifi)*    
- Платы разработки в проекте с чипами esp32 wrover i r8n8 и [esp32 s3 alt1 n16r8 из набора](https://github.com/profharris/GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board) 
- Монитор ILI9341 без тач или с [тач из набора](https://github.com/profharris/GOOUUU-Tech-ESP32-S3-CAM-Expansion-Board) - но на тач пинов не хватит, кстати отличаются по посадке - с тач  pcb короче
- Энкодеры - Питание - вместо 5v - подаем 3.3v иначе входы esp32 сгорят
- Модуль-адаптер JETTING Micro SD SPI или SDIO 3В
- Модуль приемника ИК-датчиков TZT KY-022 или набор с пультом TENSTAR ROBOT HX1838

аудио тракт
- ЦАП pcm5102A - не забыть запаять перемычки и закоротить SCK (этот вывод не используем) Питание 5v
- с ЦАП идем не внешний разъем маму мини джек и обратно возвращаем сигнал на усилитель
- усилитель с диф входом HT8698 имеет один управляющий вход CTRL - через 1 ком притягиваем его на землю - mute, обратно включаем программно через pin esp32    
*вся аналоговая земля = выход ЦАП + джек + вход усилка (левая и правая аудио земля соединены чтобы через трехпиновый разъем пройти) - НЕ ДОЛЖНА быть закорочена на цифровую землю - пойдет помеха (она уже закорочена разработчиком в модуле ЦАПА по схеме звезды - дополнителльно паять НЕ НУЖНО!!!), выход усилка со своей аналоговой левой и правой землей чисто на динамики*

упс 2S
- два аккумулятора 18650
- Модуль зарядного устройства для литиевых батарей 18650 2S
- Модуль Литиевая плата защиты BMS 2S 20A 7,4В 8,4В 18650
- Кнопка On/Off Модуль Регулятор напряжения TZT XL-10AH      
*Кнопка дублируеттся внешней кнопкой (замыкание на землю) с подсветкой без фиксации (Momentary Self-reset) + светодиоод на 3V-6V(5V)*
- Модуль преобразователя напряжения occkic 3.6-32В в 5/9/12В 5А      
*В файле ups_PCB.zip доки на схему и сборку*
- Модуль индикации заряда батареи diymore 1S-4S или Индикатор емкости батареи diymore Battery Capacity Indicator 2S (на две банки) 

вход
- Ставим шнурок Type-C 6pin для быстрой зарядки и передачи данных usb 3.1 (DongLiLian USB-C4P6P водонепроницаемый разъем TYPE C2P) - это 6 проводов (vcc gnd d+ d- cc1 cc2) 6pinFemaly с одной стороны на корпус и с другой стороны разъем 6pinMale     
*из шнурка два вывода d+ d- убираем от выхода 6pinMale и припаиваем их к переходнику 4pinMale*
*разъем 6pinMale внутри корпуса втыкаем в зарядник (vcc gnd cc1 cc2), а 4pinMale в плату ESP32 (d+ d-)*
*таким образом через один внешний разъем Type-C мы можем подключить или быструю зарядку или кабель для прошивки* 

схему собираем по распиновке для выбранной платы         
* для wrover /config/myoptions_wrover.h     
* для s3 alt /config/myoptions_s3_alt.h     

софт   
---

* для wrover используем окружение        
default_envs = esp32-wrover-kit из файла /config/env/config_DevBoard_ESP32_Wrover-kit.ini    
* для s3 alt используем окружение    
default_envs = esp32-s3-devkitc-1-alt из файла /config/env/config_DevBoard_ESP32-S3-DevKitC-1-ALT.ini    

правки оригинала v0.9.693      
---
версия v1.0.0 (релиз)
- добавлен выбор конфигурации в зависимости от используемой платы     
   - [x] /src/core/options.h    
- /config/myoptions_wrover.h и /config/myoptions_s3_alt.h добавлена переменная #define AMP_MUTE_GPIO (пин управлени mute)    
   - [x] в /src/main.cpp отключаем mute (ставим 1) усилка после подключения к wifi    
- добавлено ограничение громкости (чтобы динамики не 'пожечь' по вч) при сохранении диапазона энкодера 255
   - [x] /config/myoptions_wrover.h и /config/myoptions_s3_alt.h добавлена переменная MAX_VOL 190    
   - [x] /config/myoptions_wrover.h и /config/myoptions_s3_alt.h добавлена переменная VOL_WARN_ZONE 20 зона плавного изменения цвета    
   - [x] /src/core/player.cpp переписана функция - uint8_t Player::volToI2S(uint8_t volume)  
- добавлено отображение превышения громкости - основная цифра и полоса сырые данные 0-255, число внизу данные на ЦАП, цвет превышение лимита MAX_VOL в зоне VOL_WARN_ZONE   
   - [x] /src/displays/widgets/widgets.h в классе Widgets добавлена функция setFgColor(uint16_t color)
   - [x] /src/core/display.cpp добавлена структура для плавной генерации цвета static uint16_t blendColor(uint16_t c1, uint16_t c2, uint16_t t)
   - [x] /src/core/display.cpp переписана функция - Display::_volume()
   - [x] /config/myoptions_wrover.h и /config/myoptions_s3_alt.h добавлена переменная цвета превышенния ограничения VOL_OVER_COLOR    
- расширено количество wifi сетей с 5ти до 10    
   - [x] /src/core/config.h neworkItem ssids[10];    
   - [x] /src/core/config.cpp функция - Config::initNetwork    
   - [x] /data/www/options.html.gz добавлено в разметку +5 полей для wifi    
- в вебморду добавлен блок будильника (типа спящий режим)- но пока не работает, не знаю как..., просто нарисовал
   - [x] /data/www/options.html.gz*
    
---    
текущая сборка

- в конфигурацию /config/myoptions_s3_alt.h включена поддержка SD карты в режиме SDIO        
   - [x] /src/core/sdmanager.h заменен целиком
   - [x] /src/core/sdmanager.cpp изменены инклюды и функции bool SDManager::start() void SDManager::stop() bool SDManager::cardPresent();
   - [x] /src/core/config.cpp отключен блок #if defined(SD_SPIPINS) || SD_HSPI
   - [x] /src/core/options.h


