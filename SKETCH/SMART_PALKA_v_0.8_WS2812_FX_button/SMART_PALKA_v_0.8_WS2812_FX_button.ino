/*
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver
  Смена выбранных режимов кнопкой. Кнопка подключена на D2 и GND
*/

#include "FastLED.h"          // библиотека для работы с лентой

#define BUTTON_PIN 2          // BUTTON

#define LED_COUNT 29// число светодиодов в кольце/ленте
#define LED_DT 13             // пин, куда подключен DIN ленты

int max_bright = 255;          // максимальная яркость (0 - 255)
boolean adapt_light = 0;       // адаптивная подсветка (1 - включить, 0 - выключить)

byte fav_modes[] = {2, 11, 14, 25, 27, 30, 41, 23};  // список "любимых" режимов
byte num_modes = sizeof(fav_modes);         // получить количество "любимых" режимов (они все по 1 байту..)
unsigned long change_time, last_change, last_bright;
int new_bright;

volatile byte ledMode = 3;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
  888 - демо-режим
*/

// цвета мячиков для режима
byte ballColors[3][3] = {
  {0xff, 0, 0},
  {0xff, 0xff, 0xff},
  {0   , 0   , 0xff}
};

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
int ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR


volatile uint32_t btnTimer;
volatile byte modeCounter;
volatile boolean changeFlag;
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
byte ledModex, BUTTON_PINx;

void setup()
{
  Serial.begin(9600);              // открыть порт для связи
  Serial.println(F(""));
  Serial.println(F("|setup|"));  
  Serial.println(F(""));  
  
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость

  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  LEDS.show();                     // отослать команду

  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(0, btnISR, FALLING);
}

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void loop() {

 /* 
  if (Serial.available() > 2) {                        // если включена адаптивная яркость     
      new_bright = Serial.parseInt();
      LEDS.setBrightness(new_bright);        // установить новую яркость

      Serial.print(F("new_bright = "));  
      Serial.println(new_bright);
   // }
  }

   */
   
 // /*
    if (Serial.available() > 2) {     // если что то прислали
      ledMode = Serial.parseInt();    // парсим в тип данных int
      change_mode(ledMode);           // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
    }
//  */
  if (ledMode != ledModex ) {
    ledModex = ledMode;
    Serial.print(F("ledMode = "));  
    Serial.println(ledMode);}

    

    if (digitalRead(BUTTON_PIN) == HIGH ) {
      BUTTON_PINx++;    
      ledMode++;
    Serial.print(F("BUTTON_PINx = "));  
    Serial.println(BUTTON_PINx);}
  
  
  switch (ledMode) {
    case 999: break;                           // пазуа
    case  2: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  3: rainbow_loop(); break;            // крутящаяся радуга
    case  4: random_burst(); break;            // случайная смена цветов
    case  5: color_bounce(); break;            // бегающий светодиод
    case  6: color_bounceFADE(); break;        // бегающий паровозик светодиодов
    case  7: ems_lightsONE(); break;           // вращаются красный и синий
    case  8: ems_lightsALL(); break;           // вращается половина красных и половина синих
    case  9: flicker(); break;                 // случайный стробоскоп
    case 10: pulse_one_color_all(); break;     // пульсация одним цветом
    case 11: pulse_one_color_all_rev(); break; // пульсация со сменой цветов
    case 12: fade_vertical(); break;           // плавная смена яркости по вертикали (для кольца)
    case 13: rule30(); break;                  // безумие красных светодиодов
    case 14: random_march(); break;            // безумие случайных цветов
    case 15: rwb_march(); break;               // белый синий красный бегут по кругу (ПАТРИОТИЗМ!)
    case 16: radiation(); break;               // пульсирует значок радиации
    case 17: color_loop_vardelay(); break;     // красный светодиод бегает по кругу
    case 18: white_temps(); break;             // бело синий градиент (?)
    case 19: sin_bright_wave(); break;         // тоже хрень какая то
    case 20: pop_horizontal(); break;          // красные вспышки спускаются вниз
    case 21: quad_bright_curve(); break;       // полумесяц
    case 22: flame(); break;                   // эффект пламени
    case 23: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)  // зменьшити швидкість
    case 24: pacman(); break;                  // пакман
    case 25: random_color_pop(); break;        // безумие случайных вспышек
    case 26: ems_lightsSTROBE(); break;        // полицейская мигалка
    case 27: rgb_propeller(); break;           // RGB пропеллер
    case 28: kitt(); break;                    // случайные вспышки красного в вертикаьной плоскости
    case 29: matrix(); break;                  // зелёненькие бегают по кругу случайно
    case 30: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга
    case 31: strip_march_ccw(); break;         // чёт сломалось
    case 32: strip_march_cw(); break;          // чёт сломалось
    case 33: colorWipe(0x00, 0xff, 0x00, thisdelay);
      colorWipe(0x00, 0x00, 0x00, thisdelay); break;                                // плавное заполнение цветом
    case 34: CylonBounce(0xff, 0, 0, 4, 10, thisdelay); break;                      // бегающие светодиоды
    case 35: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 36: NewKITT(0xff, 0, 0, 8, 10, thisdelay); break;                          // беготня секторов круга (не работает)
    case 37: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга
    case 38: TwinkleRandom(20, thisdelay, 1); break;                                // случайные разноцветные включения (1 - танцуют все, 0 - случайный 1 диод)
    case 39: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни
    case 40: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета
    case 41: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    case 42: theaterChase(0xff, 0, 0, thisdelay); break;                            // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 43: theaterChaseRainbow(thisdelay); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 44: Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000); break;                  // стробоскоп

    case 45: BouncingBalls(0xff, 0, 0, 3); break;                                   // прыгающие мячики
    case 46: BouncingColoredBalls(3, ballColors); break;                            // прыгающие мячики цветные

   

    case 888: demo_modeA(); break;             // длинное демо
    case 889: demo_modeB(); break;             // короткое демо
  }
}

void btnISR() {
  if (millis() - btnTimer > 150) {
    btnTimer = millis();  // защита от дребезга
    if (++modeCounter >= num_modes) modeCounter = 0;
    ledMode = fav_modes[modeCounter];    // получаем новый номер следующего режима
    change_mode(ledMode);               // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)    
    changeFlag = true;
  }
}

void change_mode(int newmode) {
  thissat = 255;
  switch (newmode) {    
    case 0: one_color_all(255, 255, 255); LEDS.show(); break; //---ALL ON   
    case 1: thisdelay = 60; break;                      //---STRIP RAINBOW FADE
    case 2: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 3: thisdelay = 20; break;                      //---RANDOM BURST
    case 4: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    case 5: thisdelay = 80; thishue = 0; break;         //---CYLON v2
    case 6: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SOLID
    case 8: thishue = 160; thissat = 50; break;         //---STRIP FLICKER
    case 9: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 10: thisdelay = 30; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 11: thisdelay = 60; thishue = 180; break;      //---VERTICAL SOMETHING
    case 12: thisdelay = 100; break;                    //---CELL AUTO - RULE 30 (RED)
    case 13: thisdelay = 80; break;                     //---MARCH RANDOM COLORS
    case 14: thisdelay = 80; break;                     //---MARCH RWB COLORS
    case 15: thisdelay = 60; thishue = 95; break;       //---RADIATION SYMBOL
    
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    case 16: thisdelay = 35; thishue = 180; break;      //---SIN WAVE BRIGHTNESS
    case 17: thisdelay = 100; thishue = 0; break;       //---POP LEFT/RIGHT
    case 18: thisdelay = 100; thishue = 180; break;     //---QUADRATIC BRIGHTNESS CURVE
    
    //---PLACEHOLDER FOR FLAME VARS
    case 19: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 20: thisdelay = 50; break;                     //---PACMAN
    case 21: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 22: thisdelay = 25; thishue = 0; break;        //---EMERGECNY STROBE
    case 23: thisdelay = 100; thishue = 0; break;        //---RGB PROPELLER
    case 24: thisdelay = 100; thishue = 0; break;       //---KITT
    case 25: thisdelay = 100; thishue = 95; break;       //---MATRIX RAIN
    case 36: thisdelay = 15; break;                      //---NEW RAINBOW LOOP
    case 37: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW   ?
    case 38: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW   ?
    case 39: thisdelay = 50; break;                     // colorWipe
    case 40: thisdelay = 50; break;                     // CylonBounce
    case 41: thisdelay = 15; break;                     // Fire
    case 42: thisdelay = 50; break;                     // NewKITT
    case 43: thisdelay = 20; break;                     // rainbowCycle
    case 44: thisdelay = 10; break;                     // rainbowTwinkle
    case 45: thisdelay = 50; break;                     // RunningLights
    case 46: thisdelay = 0; break;                      // Sparkle
    case 47: thisdelay = 30; break;                     // SnowSparkle      !!!
    case 48: thisdelay = 50; break;                     // theaterChase
    case 49: thisdelay = 50; break;                     // theaterChaseRainbow
    case 50: thisdelay = 100; break;                    // Strobe

    case 51: one_color_all(255, 0, 0); LEDS.show(); break; //---ALL RED
    case 52: one_color_all(0, 255, 0); LEDS.show(); break; //---ALL GREEN
    case 53: one_color_all(0, 0, 255); LEDS.show(); break; //---ALL BLUE
    case 54: one_color_all(255, 255, 0); LEDS.show(); break; //---ALL COLOR X
    case 55: one_color_all(0, 255, 255); LEDS.show(); break; //---ALL COLOR Y
    case 56: one_color_all(255, 0, 255); LEDS.show(); break; //---ALL COLOR Z



  }
  bouncedirection = 0;
  one_color_all(0, 0, 0);
  ledMode = newmode;
}
