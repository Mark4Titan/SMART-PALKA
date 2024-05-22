/*
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver
  Смена выбранных режимов кнопкой. Кнопка подключена на D2 и GND
*/

#include "FastLED.h"          // библиотека для работы с лентой

#define LED_COUNT 30          // число светодиодов в кольце/ленте
#define LED_DT 13             // пин, куда подключен DIN ленты

int max_bright = 255;          // максимальная яркость (0 - 255)
boolean adapt_light = 0;       // адаптивная подсветка (1 - включить, 0 - выключить)

byte fav_modes[] = {22, 7, 24, 29, 52, 1, 10, 2, 26, 13 };  // список "любимых" режимов 38 уменьш скорасть
byte num_modes = sizeof(fav_modes);                         // получить количество "любимых" режимов (они все по 1 байту..)
unsigned long change_time, last_change, last_bright;
int new_bright;

volatile byte ledMode = 45;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
  254 - демо-режим А
  255 - демо-режим Б
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
#define BUTTON_PIN 2          // BUTTON
byte ledModex, BUTTON_PINx;



// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------






///....................................................................///
///.........настройки...voltage test animation v5......................///

double Volt_Max = 3.92,              // максимальное значение в вольтах (при подключении по юсби 5.2v) с условием смещение на сегменты
       Volt_Min = 2.6;              // минимальное значение в вольтах (минимальное значение для li-on 2.8v) с условием смещение на сегменты
int     anWrite = 732;              // analogWrite значение при максимальных вольтах

#define Vol_in analogRead(5)        // Пин к которому подключен делитель напряжения
#define power_supply digitalRead(7) // подача тока
#define charge_end digitalRead(8)   // окончания заряда
byte CHARGING_MODE = 1;             // режим зарядки (0 ВЫКЛ, 1 ВКЛ)
byte temporary;
bool temporary_Flag = 0;

byte segm = 4,                      // количество сегментов (max - 6, min - 1)
     prop = 1,                      // количество пробелов между сегментами (min - 1)
     addi = 2;                      // количество составления ленты (min - 1, max - 2)

///.........конец настроек.....voltage test animation..................///
///....................................................................///





//https://youtu.be/VOdszMYU87A
 
int Cop_Vol_in;                    // переменная в которой будем хранить копию значения Vol_in (analogRead)
int V_Current;                     // Текущий вольтаж на акб
int Led_Average;                   // Середня LED в одном сегменте


double ST;                         // Корректор значений в авто
volatile uint32_t deboA;           // переменная для хранения пройденного времени
volatile uint32_t deboB;

byte Fm1, Fm2, Fm3, Fm4, Fm5, Fm6; // перемінні для зберігання значень заливки
byte Rm1, Rm2, Rm3, Rm4, Rm5, Rm6; // перемінні для зберігання значень заливки 

byte A_charge;                     //анимация при зарядке

bool G0,                           // флаг отображение сегментов если не вышли за нижний порог (вольт)
     E0,                           // отображение сегментов если вышли вышли за нижний порог (вольт)
     CAE,
     F1, F2, F3, F4, F5, F6,       // флаг заливки forward
     R1, R2, R3, R4, R5, R6;       // флаг заливки reverse
   

     
int CP[][3] = {                    //color package
// R     G   B 
  {20,  5,   5},                   // = заливка при низком заряде (Красный)  
  {165, 0,   0},                   // = segment №1 (Красный) 
  {130, 10, 0},                   // = segment №2 (Оранжевый) 
  {80, 50, 0},                   // = segment №3 (Желтый)  
  {0,   165, 0},                   // = segment №4 (Зеленый)    
  {0, 165, 10},                   // = segment №5 
  {0, 10, 165},                   // = segment №6
};

///....................................................................///
///....................................................................///























//.................Mark_Button................................................///
int Lx = 255;
bool Lx_f = 0;
byte press = 0;  // счетчик нажатий максимум 255, занимает 1 байт
bool hold_flag = 0; // контроль удержание нажатий
volatile uint32_t debounce;
#define waiting_time  80 // настройка антидребезга (по умолчанию 80 мс) 
#define time_again 300   //  настройка таймаута на удержание (по умолчанию 300 мс)
bool hold_UP_DO = 0;
bool vol_an = 0;
//----Mark_Button END----///
////sis_END///

///.........конец настроек......Mark_Button...................................///







void setup()
{
  pinMode(5, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  
  Serial.begin(9600);              // открыть порт для связи
  //.. этот кусок не обязательно, но с ним красивее))..//
  Serial.println(F(""));
  Serial.println(F("|setup|"));  
  Serial.println(F("")); 
  //...................................................//

  

  ///////////auto_voltage_test///
  pinMode(Vol_in, INPUT);
  pinMode(power_supply, INPUT );
  pinMode(charge_end, INPUT );
  analogReference(INTERNAL);  // встроенное опорное напряжение 1.1 В на микроконтроллерах ATmega168 и ATmega328, и 2.56 В на ATmega8
  Led_Average = LED_COUNT/addi - (segm - 1) * prop; // LED в одном сегменте
  Led_Average = Led_Average/segm;  
  //////////auto_voltage_test////
  
 
  
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  LEDS.show();                     // отослать команду

  randomSeed(analogRead(0));
 
 // прерывание на D2 (UNO/NANO)
 attachInterrupt(0, btnisr, CHANGE);



}

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void loop() {
 
  Log();  
 
  
  if (adapt_light) {                        // если включена адаптивная яркость
    if (millis() - last_bright > 500) {     // каждые полсекунды
      last_bright = millis();               // сброить таймер
      new_bright = map(analogRead(6), 1, 1000, 3, max_bright);   // считать показания с фоторезистора, перевести диапазон
      LEDS.setBrightness(new_bright);        // установить новую яркость      
    }
  }

  if (Lx_f == 1) {                        // если включена адаптивная яркость
    if (millis() - last_bright > 100) {     // каждые полсекунды
      last_bright = millis();               // сброить таймер      
      new_bright = Lx;
      LEDS.setBrightness(new_bright);        // установить новую яркость        
    }
  }

  if (digitalRead(10) == 0){//читаємо
    digitalWrite(5, 1);
    
    // pinMode(5, OUTPUT);
    //pinMode(10, INPUT_PULLUP);
    }

  
    if (Serial.available() > 2) {     // если что то прислали
        ledMode = Serial.parseInt();    // парсим в тип данных int            
    //   ledMode
      change_mode(ledMode);           // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
    }

     if (ledMode != ledModex ) {
    ledModex = ledMode;
    Serial.print(F("ledMode = "));  
    Serial.println(ledMode);}

    
  
  switch (ledMode) { 
    case 256: break;                           // пазуа
    case  1: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  2: rainbow_loop(); break;            // крутящаяся радуга
    case  3: random_burst(); break;            // случайная смена цветов
    case  4: color_bounce(); break;            // бегающий светодиод
    case  5: color_bounceFADE(); break;        // бегающий паровозик светодиодов
    case  6: ems_lightsONE(); break;           // вращаются красный и синий
    case  7: ems_lightsALL(); break;           // вращается половина красных и половина синих
    case  8: flicker(); break;                 // случайный стробоскоп
    case  9: pulse_one_color_all(); break;     // пульсация одним цветом
    case 10: pulse_one_color_all_rev(); break; // пульсация со сменой цветов
    case 11: fade_vertical(); break;           // плавная смена яркости по вертикали (для кольца)
    case 12: rule30(); break;                  // безумие красных светодиодов
    case 13: random_march(); break;            // безумие случайных цветов
    case 14: rwb_march(); break;               // белый синий красный бегут по кругу (ПАТРИОТИЗМ!)
    case 15: radiation(); break;               // пульсирует значок радиации
    case 16: color_loop_vardelay(); break;     // красный светодиод бегает по кругу
    case 17: white_temps(); break;             // бело синий градиент (?)
    case 18: sin_bright_wave(); break;         // тоже хрень какая то
    case 19: pop_horizontal(); break;          // красные вспышки спускаются вниз
    case 20: quad_bright_curve(); break;       // полумесяц
    case 21: flame(); break;                   // эффект пламени
    case 22: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)
    case 23: pacman(); break;                  // пакман
    case 24: random_color_pop(); break;        // безумие случайных вспышек
    case 25: ems_lightsSTROBE(); break;        // полицейская мигалка
    case 26: rgb_propeller(); break;           // RGB пропеллер    
    case 27: kitt(); break;                    // случайные вспышки красного в вертикаьной плоскости
    case 28: matrix(); break;                  // зелёненькие бегают по кругу случайно
    case 29: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга
    case 30: strip_march_ccw(); break;         // чёт сломалось
    case 31: strip_march_cw(); break;          // чёт сломалось
    case 32: colorWipe(0x00, 0xff, 0x00, thisdelay);
      colorWipe(0x00, 0x00, 0x00, thisdelay); break;                                // плавное заполнение цветом
    case 33: CylonBounce(0xff, 0, 0, 4, 10, thisdelay); break;                      // бегающие светодиоды
    case 34: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 35: NewKITT(0xff, 0, 0, 8, 10, thisdelay); break;                          // беготня секторов круга (не работает)
    case 36: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга    
    case 37: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни
    case 38: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета
    case 39: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    case 40: theaterChase(0xff, 0, 0, thisdelay); break;                            // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 41: theaterChaseRainbow(thisdelay); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 42: Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000); break;                  // стробоскоп

    case 43: BouncingBalls(0xff, 0, 0, 3); break;                                   // прыгающие мячики
    case 44: BouncingColoredBalls(3, ballColors); break;                            // прыгающие мячики цветные

    case 45: one_color_all(120, 120, 120); LEDS.show(); break; //---ALL White
    case 46: one_color_all(255, 0, 0); LEDS.show(); break; //---ALL RED
    case 47: one_color_all(0, 255, 0); LEDS.show(); break; //---ALL GREEN
    case 48: one_color_all(0, 0, 255); LEDS.show(); break; //---ALL BLUE
    case 49: one_color_all(255, 255, 0); LEDS.show(); break; //---ALL COLOR X
    case 50: one_color_all(0, 255, 255); LEDS.show(); break; //---ALL COLOR Y
    case 51: one_color_all(255, 0, 255); LEDS.show(); break; //---ALL COLOR Z

    case 52: SnowSparkle(0, 0, 0, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    
    case 110: VOL();setColor(); break;
    
    case 254: demo_modeA(); break;             // длинное демо
    case 255: demo_modeB(); break;             // короткое демо
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
    case 0: one_color_all(0, 0, 0); LEDS.show(); break; //---ALL OFF
    case 1: one_color_all(255, 255, 255); LEDS.show(); break; //---ALL ON
    case 2: thisdelay = 60; break;                      //---STRIP RAINBOW FADE
    case 3: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 4: thisdelay = 20; break;                      //---RANDOM BURST
    case 5: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    case 6: thisdelay = 80; thishue = 0; break;         //---CYLON v2
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 8: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SOLID
    case 9: thishue = 160; thissat = 50; break;         //---STRIP FLICKER
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 11: thisdelay = 30; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 12: thisdelay = 60; thishue = 180; break;      //---VERTICAL SOMETHING
    case 13: thisdelay = 100; break;                    //---CELL AUTO - RULE 30 (RED)
    case 14: thisdelay = 80; break;                     //---MARCH RANDOM COLORS
    case 15: thisdelay = 80; break;                     //---MARCH RWB COLORS
    case 16: thisdelay = 60; thishue = 95; break;       //---RADIATION SYMBOL
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    case 19: thisdelay = 35; thishue = 180; break;      //---SIN WAVE BRIGHTNESS
    case 20: thisdelay = 100; thishue = 0; break;       //---POP LEFT/RIGHT
    case 21: thisdelay = 100; thishue = 180; break;     //---QUADRATIC BRIGHTNESS CURVE
    //---PLACEHOLDER FOR FLAME VARS
    case 23: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 24: thisdelay = 50; break;                     //---PACMAN
    case 25: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 26: thisdelay = 25; thishue = 0; break;        //---EMERGECNY STROBE
    case 27: thisdelay = 100; thishue = 0; break;        //---RGB PROPELLER
    case 28: thisdelay = 100; thishue = 0; break;       //---KITT
    case 29: thisdelay = 100; thishue = 95; break;       //---MATRIX RAIN
    case 30: thisdelay = 15; break;                      //---NEW RAINBOW LOOP
    case 31: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW
    case 32: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW
    case 33: thisdelay = 50; break;                     // colorWipe
    case 34: thisdelay = 50; break;                     // CylonBounce
    case 35: thisdelay = 15; break;                     // Fire
    case 36: thisdelay = 50; break;                     // NewKITT
    case 37: thisdelay = 20; break;                     // rainbowCycle
    case 38: thisdelay = 10; break;                     // rainbowTwinkle
    case 39: thisdelay = 50; break;                     // RunningLights
    case 40: thisdelay = 0; break;                      // Sparkle
    case 41: thisdelay = 30; break;                     // SnowSparkle
    case 42: thisdelay = 50; break;                     // theaterChase
    case 43: thisdelay = 50; break;                     // theaterChaseRainbow
    case 44: thisdelay = 100; break;                    // Strobe

    case 101: one_color_all(255, 0, 0); LEDS.show(); break; //---ALL RED
    case 102: one_color_all(0, 255, 0); LEDS.show(); break; //---ALL GREEN
    case 103: one_color_all(0, 0, 255); LEDS.show(); break; //---ALL BLUE
    case 104: one_color_all(255, 255, 0); LEDS.show(); break; //---ALL COLOR X
    case 105: one_color_all(0, 255, 255); LEDS.show(); break; //---ALL COLOR Y
    case 106: one_color_all(255, 0, 255); LEDS.show(); break; //---ALL COLOR Z

    
  }
  bouncedirection = 0;
  one_color_all(0, 0, 0);
  ledMode = newmode;
}
