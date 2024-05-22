
#include <Adafruit_NeoPixel.h>

///....................................................................///
///.........настройки..................................................///

double Volt_Max = 4.0,              // максимальное значение в вольтах (при подключении по юсби 5.2v) с условием смещение на сегменты
       Volt_Min = 2.6;              // минимальное значение в вольтах (минимальное значение для li-on 2.8v) с условием смещение на сегменты
int     anWrite = 747;              // analogWrite значение при максимальных вольтах

#define Vol_in analogRead(5)        // Пин к которому подключен делитель напряжения
#define PIN 13                      // входной контакт Neopixel
#define power_supply digitalRead(7) // подача тока
#define charge_end digitalRead(8)   // окончания заряда
#define NUMPIXELS 30                // количество неопикселей 
byte CHARGING_MODE = 1;             // режим зарядки (0 ВЫКЛ, 1 ВКЛ)

byte segm = 4,                      // количество сегментов (max - 6, min - 1)
     prop = 1,                      // количество пробелов между сегментами (min - 1)
     addi = 2;                      // количество составления ленты (min - 1, max - 2)

///.........конец настроек.............................................///
///....................................................................///

  

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


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


 

void setup(){
  
  pixels.begin();                                   // Инициализирует библиотеку NeoPixel.
  pinMode(Vol_in, INPUT);
  pinMode(power_supply, INPUT );
  pinMode(charge_end, INPUT );
   //Input Pull-up, Pull-Down INPUT_PULLUP  INPUT_PULLDOWN
  
  
  // встроенное опорное напряжение 1.1 В на микроконтроллерах ATmega168 и ATmega328, и 2.56 В на ATmega8
  analogReference(INTERNAL); 
  
  Serial.begin (9600);                              // Задаем скорось обмена com-порта 
  Serial.println();                                 // отступление
  Serial.println(F("| setup |"));                   // выводим начальный надпись 
  
   
   
  Led_Average = NUMPIXELS/addi - (segm - 1) * prop; // LED в одном сегменте
  Led_Average = Led_Average/segm;
 }





void VOL (){
byte poh;
if (power_supply == 0)poh=30;else poh=10;
 //проверяем не изменилась ли напряжение 
 if (Vol_in > Cop_Vol_in+poh ||  Vol_in < Cop_Vol_in-poh){Cop_Vol_in = Vol_in; 
                                                      
                                         
                                                      
 int V_Average;                                     // временная
 V_Average = (Volt_Max-Volt_Min)*100;              
 V_Average = V_Average / segm;                      // стоимость одного сегмента                                                     
                           
  ST =   Cop_Vol_in/(anWrite/Volt_Max);                       // подсчет вольт на аккумуляторе
  V_Current = (ST-Volt_Min)*100/ V_Average;         // конвертация в сегменты
                                                      
  if ( V_Current > segm) V_Current = segm;          // корректор ошибки                                                
                                
                         
                   // дальше выводим сервисную информацию в порт
                         if (anWrite == 0) {      
                         Serial.print(F("| analogWrite="));
                         Serial.print(Cop_Vol_in); 
                         Serial.println(F(" |"));}
                           
                    if (anWrite > 0 && ST > 1.3) {
                      
                    Serial.print(F("| Volt="));
                    Serial.print(ST); 
                    Serial.println(F(" |"));
                    
                    } 
                                                      
//обробка заливки сегментов                                                  
if (anWrite > 0 && ST > 1.3) {
    Serial.print(F("| one segment = "));
    Serial.print(Led_Average); 
    Serial.println(F(" |")); 
  
    Serial.print(F("| conversion to segments = "));
    Serial.print( V_Current); 
    Serial.println(F(" |"));
  
      
   ///прямоход  
  if ( V_Current > 0){G0=1;E0=0;} else {G0=0;E0=1;}
  
  F1=0;
  F2=0;
  F3=0;
  F4=0;
  F5=0;
  F6=0;
  
   if (V_Current >= 1){F1=1; Fm1=Led_Average + prop;  }//Led_Average*2+prop  
   if (V_Current >= 2){F2=1; Fm2=Fm1+Led_Average+prop;}
   if (V_Current >= 3){F3=1; Fm3=Fm2+Led_Average+prop;}
   if (V_Current >= 4){F4=1; Fm4=Fm3+Led_Average+prop;}
   if (V_Current >= 5){F5=1; Fm5=Fm4+Led_Average+prop;}
   if (V_Current >= 6){F6=1; Fm6=Fm5+Led_Average+prop;}
   
  
  
  
  
  
  
                        
   Serial.println(F(""));
  if (power_supply == 0){Serial.println(F("| charging |"));}
 
  if (G0 && power_supply) Serial.println(F("| normal display |")); 
  if (E0 && power_supply) Serial.println(F("| discharge |"));
  
    
   Serial.println(F(""));
   if (V_Current >= 1){Serial.print(F("| F1="));Serial.print(F1); Serial.print(F(" |")); Serial.print(F("| Fm1="));Serial.print(Fm1); Serial.println(F(" |"));}
   if (V_Current >= 2){Serial.print(F("| F2="));Serial.print(F2); Serial.print(F(" |")); Serial.print(F("| Fm2="));Serial.print(Fm2); Serial.println(F(" |"));}
   if (V_Current >= 3){Serial.print(F("| F3="));Serial.print(F3); Serial.print(F(" |")); Serial.print(F("| Fm3="));Serial.print(Fm3); Serial.println(F(" |"));}
   if (V_Current >= 4){Serial.print(F("| F4="));Serial.print(F4); Serial.print(F(" |")); Serial.print(F("| Fm4="));Serial.print(Fm4); Serial.println(F(" |"));}
   if (V_Current >= 5){Serial.print(F("| F5="));Serial.print(F5); Serial.print(F(" |")); Serial.print(F("| Fm5="));Serial.print(Fm5); Serial.println(F(" |"));}
   if (V_Current >= 6){Serial.print(F("| F6="));Serial.print(F6); Serial.print(F(" |")); Serial.print(F("| Fm6="));Serial.print(Fm6); Serial.println(F(" |"));}
   

///інверсія  
  if (addi == 2){
  R1=0;
  R2=0;
  R3=0;
  R4=0;
  R5=0;
  R6=0;
  
                                //Rm1=(float)round(NUMPIXELS - Led_Average/100-prop*2);
   if (V_Current >= 1){R1=1; Rm1=NUMPIXELS - Led_Average-prop-1;}//se1i = (float)NUMPIXELS - segment1*2-prop;  
   if (V_Current >= 2){R2=1; Rm2=Rm1-Led_Average-prop;}
   if (V_Current >= 3){R3=1; Rm3=Rm2-Led_Average-prop;}
   if (V_Current >= 4){R4=1; Rm4=Rm3-Led_Average-prop;}
   if (V_Current >= 5){R5=1; Rm5=Rm4-Led_Average-prop;}
   if (V_Current >= 6){R6=1; Rm6=Rm5-Led_Average-prop;}
 
  
  
  
  
  

   Serial.println(F(""));
   if (V_Current >= 1){Serial.print(F("| R1="));Serial.print(R1); Serial.print(F(" |")); Serial.print(F("| Rm1="));Serial.print(Rm1); Serial.println(F(" |"));}
   if (V_Current >= 2){Serial.print(F("| R2="));Serial.print(R2); Serial.print(F(" |")); Serial.print(F("| Rm2="));Serial.print(Rm2); Serial.println(F(" |"));}
   if (V_Current >= 3){Serial.print(F("| R3="));Serial.print(R3); Serial.print(F(" |")); Serial.print(F("| Rm3="));Serial.print(Rm3); Serial.println(F(" |"));}
   if (V_Current >= 4){Serial.print(F("| R4="));Serial.print(R4); Serial.print(F(" |")); Serial.print(F("| Rm4="));Serial.print(Rm4); Serial.println(F(" |"));}
   if (V_Current >= 5){Serial.print(F("| R5="));Serial.print(R5); Serial.print(F(" |")); Serial.print(F("| Rm5="));Serial.print(Rm5); Serial.println(F(" |"));}
   if (V_Current >= 6){Serial.print(F("| R6="));Serial.print(R6); Serial.print(F(" |")); Serial.print(F("| Rm6="));Serial.print(Rm6); Serial.println(F(" |"));}
  
  
  }
} 

                      
                        
}}


void setColor(){  
  
  if (millis() - deboB >= 2000 && power_supply == 0 && charge_end == 1 && CHARGING_MODE) {
    deboB = millis();
    CAE=1;
     R1=0;
     R2=0;
     R3=0;
     R4=0;
     R5=0;
     R6=0;
    
     F1=0;
     F2=0;
     F3=0;
     F4=0;
     F5=0;
     F6=0;
    
    if (++A_charge > V_Current) A_charge=0; else {
      if (A_charge >= 1){R1=1; } 
      if (A_charge >= 2){R2=1; }
      if (A_charge >= 3){R3=1; }
      if (A_charge >= 4){R4=1; }
      if (A_charge >= 5){R5=1; }
      if (A_charge >= 6){R6=1; }
    
      if (A_charge >= 1){F1=1; }
      if (A_charge >= 2){F2=1; }
      if (A_charge >= 3){F3=1; }
      if (A_charge >= 4){F4=1; }
      if (A_charge >= 5){F5=1; }
      if (A_charge >= 6){F6=1; }}
    
    if (G0 == 0)G0 = 1;
  }
 
  if (charge_end == 0 && CAE){
      Serial.println(F("charge_end"));
      CAE=0;
      A_charge=0;
      if (G0 == 0)G0 = 1;
      if (V_Current >= 1){R1=1; } 
      if (V_Current >= 2){R2=1; }
      if (V_Current >= 3){R3=1; }
      if (V_Current >= 4){R4=1; }
      if (V_Current >= 5){R5=1; }
      if (V_Current >= 6){R6=1; }
    
      if (V_Current >= 1){F1=1; }
      if (V_Current >= 2){F2=1; }
      if (V_Current >= 3){F3=1; }
      if (V_Current >= 4){F4=1; }
      if (V_Current >= 5){F5=1; }
      if (V_Current >= 6){F6=1; }} 
  
  
 if (millis() - deboA >= 1000 && E0){
    deboA = millis(); 
    for (int i = 0 ; i < NUMPIXELS; i++ ) {
    pixels.setPixelColor(i, pixels.Color(CP[0][0],CP[0][1],CP[0][2]));}
    pixels.show();} 
  
 if (G0){
 byte Lx = 0;
 
   //clear
  for (int i = 0 ; i < NUMPIXELS; i++ ) {pixels.setPixelColor(i, pixels.Color(0, 0, 0));} 
  
   


   if (F1){                    
   for (int i = 0 ; i < Fm1 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm1-prop){pixels.setPixelColor(i, pixels.Color(CP[1][0],CP[1][1],CP[1][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
  
   if (F2){                   
   for (int i = Lx ; i < Fm2 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm2-prop){pixels.setPixelColor(i, pixels.Color(CP[2][0],CP[2][1],CP[2][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
   
   
   if (F3){                    
   for (int i = Lx ; i < Fm3 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm3-prop){pixels.setPixelColor(i, pixels.Color(CP[3][0],CP[3][1],CP[3][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
  
   if (F4){                    
   for (int i = Lx ; i < Fm4 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm4-prop){pixels.setPixelColor(i, pixels.Color(CP[4][0],CP[4][1],CP[4][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
   
   
   if (F5){                    
   for (int i = Lx ; i < Fm5 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm5-prop){pixels.setPixelColor(i, pixels.Color(CP[5][0],CP[5][1],CP[5][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
   
   
   if (F6){                    
   for (int i = Lx ; i < Fm6 && i < NUMPIXELS/addi; i++ ) {
     if (i < Fm6-prop){pixels.setPixelColor(i, pixels.Color(CP[6][0],CP[6][1],CP[6][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i+1;}
   }}
   
   
   
  
   ///інверсія
  

   if (R1){  
           
   for (int i = NUMPIXELS ; i > Rm1 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm1+prop){pixels.setPixelColor(i, pixels.Color(CP[1][0],CP[1][1],CP[1][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}
   
   
   
   if (R2){                    
   for (int i = Lx ; i > Rm2 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm2+prop){pixels.setPixelColor(i, pixels.Color(CP[2][0],CP[2][1],CP[2][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}   
  
   
   
   
   if (R3){                    
   for (int i = Lx ; i > Rm3 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm3+prop){pixels.setPixelColor(i, pixels.Color(CP[3][0],CP[3][1],CP[3][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}
   
   
   
   if (R4){                    
   for (int i = Lx ; i > Rm4 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm4+prop){pixels.setPixelColor(i, pixels.Color(CP[4][0],CP[4][1],CP[4][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}
   
   
   
   if (R5){                    
   for (int i = Lx ; i > Rm5 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm5+prop){pixels.setPixelColor(i, pixels.Color(CP[5][0],CP[5][1],CP[5][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}
   
   
   if (R6){                    
   for (int i = Lx ; i > Rm6 && i > NUMPIXELS/addi-1; i-- ) {
     if (i > Rm6+prop){pixels.setPixelColor(i, pixels.Color(CP[6][0],CP[6][1],CP[6][2]));}
     else {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); Lx=i-1;}
   }}
   
  
   pixels.show();
   
   if (power_supply == 0) G0 = 0; 
     
 }}



void loop(){
  VOL(); 
  setColor();
} 
