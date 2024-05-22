

#define BUTTON_PIN 9

#include <Adafruit_NeoPixel.h>


#define PIN1 12
#define num1 16
Adafruit_NeoPixel pasek = Adafruit_NeoPixel(num1, PIN1, NEO_GRB + NEO_KHZ800);

#define PIN2 13
#define num2 16
Adafruit_NeoPixel pasek2 = Adafruit_NeoPixel(num2, PIN2, NEO_GRB + NEO_KHZ800);



bool Led_S1 = LOW;
bool Led_S2 = LOW;

byte Led_Light1;
byte Led_Light2;


int LED_X[20]; 
////...............................................
//// LED_X[0] - the number of times the button is clicked
//// LED_X[10] - withholding the checksum
////...............................................
////...............................................
////     Led 1                  ////    Led 2     .
////...............................................
//// 1-3 blok ,                 //// 5-7  blok ,
//// 4 Delay blok ,             //// 8 Delay blok , 












void setup(){
  pinMode(BUTTON_PIN, INPUT_PULLUP);  
  
  pasek.begin();
  pasek2.begin();
  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);  
  
 
  Serial.begin (115200); // Задаем скорость обмена com-порта 115200    
  Serial.println(F("|setup|"));  
  Serial.println(F(""));  


  ////
  LED_X[4]=2;
  LED_X[8]=2;
  LED_X[16]=50;
  ///
  LED_X[0]=1;
  
}




void loop(){
 buton_control();
 Oper_control(); 
  
}



void Oper_control(){
  if (LED_X[0]>0){
    byte Col = 6;
    
   
    if (LED_X[0] == 1) {LED_X[10]++; if (LED_X[10] > Col) LED_X[10]=0;}
    if (LED_X[0] == 2) {LED_X[10]--; if (LED_X[10] < 0) LED_X[10]=Col;} 
    
    
    
  
    Serial.print(F("buttons - ")); Serial.println(LED_X[10]);  //}
    
    switch (LED_X[10]) {
      case 2: {LED_X[1] = 255; LED_X[2] = 0  ; LED_X[3] = 0  ;   LED_X[5] = 255; LED_X[6] = 0  ; LED_X[7] = 0  ; break;} //green
      case 0: {LED_X[1] = 255; LED_X[2] = 255; LED_X[3] = 0  ;   LED_X[5] = 255; LED_X[6] = 255; LED_X[7] = 0  ; break;} //yellow
      case 1: {LED_X[1] = 0  ; LED_X[2] = 255; LED_X[3] = 0  ;   LED_X[5] = 0  ; LED_X[6] = 255; LED_X[7] = 0  ; break;} //red
      case 6: {LED_X[1] = 0  ; LED_X[2] = 255; LED_X[3] = 255;   LED_X[5] = 0  ; LED_X[6] = 255; LED_X[7] = 255; break;} //purple
      case 4: {LED_X[1] = 0  ; LED_X[2] = 0  ; LED_X[3] = 255;   LED_X[5] = 0  ; LED_X[6] = 0  ; LED_X[7] = 255; break;} //blue
      case 5: {LED_X[1] = 255; LED_X[2] = 0  ; LED_X[3] = 255;   LED_X[5] = 255; LED_X[6] = 0  ; LED_X[7] = 255; break;} //turquoise
      case 3: {LED_X[1] = 255; LED_X[2] = 255; LED_X[3] = 255;   LED_X[5] = 255; LED_X[6] = 255; LED_X[7] = 255; break;} //white
     // case 7: {LED_X[1] = 100; LED_X[2] = 100; LED_X[3] = 100;   LED_X[5] = 100; LED_X[6] = 100; LED_X[7] = 100; break;} //white
      
    } 
   
  Serial.print(F("LED_X[1] - ")); Serial.print(LED_X[1]);Serial.print(F(" LED_X[5] - ")); Serial.println(LED_X[5]);
  Serial.print(F("LED_X[2] - ")); Serial.print(LED_X[2]);Serial.print(F(" LED_X[6] - ")); Serial.println(LED_X[6]);
  Serial.print(F("LED_X[3] - ")); Serial.print(LED_X[3]);Serial.print(F(" LED_X[7] - ")); Serial.println(LED_X[7]);  
    
    
  LED_X[0]=0;
  Farba();
  } 
} 







void Farba(){
  
Serial.println("Farba START");
  
//for (int i0=0; i0 < (num1+num2)/2+1; i0++) { 
  
      for (int i=0; i < num1; i++) {  
      pasek.setPixelColor(i, pasek.Color(LED_X[1], LED_X[2], LED_X[3]));
      pasek.show(); 
    //   delay(LED_X[4]);   
    //  pasek2.setPixelColor(i, pasek2.Color(LED_X[5], LED_X[6], LED_X[7]));
   //   pasek2.show();  
       
        
  
        //  }
  
  
  
     //for (int i2=0; i2 < num2; i2++){  
      pasek2.setPixelColor(i, pasek2.Color(LED_X[5], LED_X[6], LED_X[7]));
      pasek2.show();


      
      delay(LED_X[16]); 
    }
//}
  Serial.println("Farba END");
}
























void buton_control(){
if (digitalRead(BUTTON_PIN) == HIGH) {   
  bool click = 1;
  byte Num = 0;
  bool  HOLD;
  
  // ловимо натиски
  for (int i=0; i < 100; i++){
    if (digitalRead(BUTTON_PIN) == HIGH && click == 1){Num++; click = 0;}
    if (digitalRead(BUTTON_PIN) == LOW){click = 1;}
    delay (5);
  } 
   
  Serial.print(F("Num = "));  
  Serial.println(Num); 
  
  

  ///////
  if (digitalRead(BUTTON_PIN) == LOW){  
    LED_X[0] = Num;  
  }
  
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*
  click = 1;
  HOLD = 0;///////

 
  while (digitalRead(BUTTON_PIN) == HIGH ) {
    
    if (click == 1){click = 0; HOLD = 1;
                   Serial.print(F("Num (HOLD) = "));  
            Serial.println(Num); 
                   }
 //////////////////////////                   /////
   if (Num == 1){
      if (Led_S1 == 1){Led_Light1--;} 
      if (Led_S1 == 0){Led_Light1++;} 
     
      
  //  analogWrite(LED1, Led_Light1);
    Serial.print("LED1 = "); 
    Serial.println(Led_Light1);  
    }
    
    
  
    if (Num == 2 ){  
      if (Led_S2 == 0){Led_Light2--;} 
      if (Led_S2 == 1){Led_Light2++;} 
      
    
 //   analogWrite(LED2, Led_Light2);
    Serial.print("LED2 = "); 
    Serial.println(Led_Light2);                  
    }
   
    
  }
*/
  
  } 
  }
  
