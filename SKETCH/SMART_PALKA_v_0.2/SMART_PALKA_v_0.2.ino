

#define BUTTON_PIN 9

#include <Adafruit_NeoPixel.h>


#define PIN1 12
#define num1 15
Adafruit_NeoPixel pasek = Adafruit_NeoPixel(num1, PIN1, NEO_GRB + NEO_KHZ800);

#define PIN2 13
#define num2 15
Adafruit_NeoPixel pasek2 = Adafruit_NeoPixel(num2, PIN2, NEO_GRB + NEO_KHZ800);



bool Led_S1 = LOW;
bool Led_S2 = LOW;

byte Led_Light1;
byte Led_Light2;

byte multiplier = 2;

int LED_X[20]; 
////...............................................
//// LED_X[0] - the number of times the button is clicked
//// LED_X[10] - withholding the checksum
//// LED_X[10] delay
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
  Serial.println(F(""));
  Serial.println(F("|setup|"));  
  Serial.println(F(""));  


  //// test  ////
  LED_X[4]=2;
  LED_X[8]=2;
  LED_X[16]=50;
  
  /// auto on
  LED_X[0]=1;
  
}




void loop(){
 buton_control();
 Oper_control(); 
  
}



void Oper_control(){
  if (LED_X[0]>0){
   Serial.println("Oper_control START");  
    
    if ( LED_X[0] < 10){
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
    
    
    
    
    ////////
    
    if ( LED_X[0] > 9){ 
     Serial.println("hold"); 
      LED_X[16]=0;
      
      while (digitalRead(BUTTON_PIN) == HIGH ) {
        if (LED_X[0] == 10){          
          if (LED_X[1] != 0){LED_X[1]=LED_X[1]-multiplier; LED_X[5]=LED_X[1];}
          if (LED_X[2] != 0){LED_X[2]=LED_X[2]-multiplier; LED_X[6]=LED_X[2];}
          if (LED_X[3] != 0){LED_X[3]=LED_X[3]-multiplier; LED_X[7]=LED_X[3];}
         
        }
        
        
        if (LED_X[0] == 20){          
          if (LED_X[1] != 255){LED_X[1]=LED_X[1]+multiplier; LED_X[5]=LED_X[1];}
          if (LED_X[2] != 255){LED_X[2]=LED_X[2]+multiplier; LED_X[6]=LED_X[2];}
          if (LED_X[3] != 255){LED_X[3]=LED_X[3]+multiplier; LED_X[7]=LED_X[3];}
         
        }
        
        
        
          Serial.print("LED_X[1]=");Serial.print(LED_X[1]);
          Serial.print("  ||  LED_X[2]=");Serial.print(LED_X[2]);
          Serial.print("  ||  LED_X[3]=");Serial.println(LED_X[3]); 
        
       Farba();   
      }
      
      
      
      
      
      
      
      
      
      
      
      
     LED_X[0]=0;
     LED_X[16]=50;
    }
    
    
    
  Serial.println("Oper_control END");    
  } 
} 







void Farba(){
  
Serial.println("Farba START");  

  
      for (int i=0; i < num1; i++) { 
        
        
      pasek.setPixelColor(i, pasek.Color(LED_X[1], LED_X[2], LED_X[3]));
      pasek.show(); 

        
      pasek2.setPixelColor(i, pasek2.Color(LED_X[5], LED_X[6], LED_X[7]));
      pasek2.show();


      
      delay(LED_X[16]); 
    }

  Serial.println("Farba END");
}
























void buton_control(){
if (digitalRead(BUTTON_PIN) == HIGH) {  
  
  Serial.println(F("buton_control START")); 
  bool click = 1;
  byte Num = 0;
//  bool  HOLD;
  
  // ловимо натиски
  for (int i=0; i < 100; i++){
    if (digitalRead(BUTTON_PIN) == HIGH && click == 1){Num++; click = 0;}
    if (digitalRead(BUTTON_PIN) == LOW){click = 1;}
    delay (5);
  } 
   

  
  

  ///////
  if (digitalRead(BUTTON_PIN) == LOW){  
    LED_X[0] = Num; Serial.print(F("Num = LOW  ||  "));  
  }
  
  
  //hold
  if (digitalRead(BUTTON_PIN) == HIGH){  
    LED_X[0] = (Num*10); Serial.print(F("Num = HIGH  ||  ")); 
  }
 
  
  
  
  Serial.print(F("Num = "));  
  Serial.println(LED_X[0]); 
  
  
 ////
  ////
  ////
  ////
  ////
  ////////
  ////
  ////////////
  ////////////
  ////////v
  ////////////
  ////
  ////
  ////
  ////
  Serial.println(F("buton_control END"));
  Serial.println(F(""));
  } 
  }
  
