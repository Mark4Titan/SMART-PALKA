#include <avr/sleep.h>

#define BUTTON_PIN 9

#include <Adafruit_NeoPixel.h>
#define Vol_Bat analogRead(1)

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

byte multiplier = 1;

bool L_IN[8];

byte LED_RGB[32]; 

byte LED_X[20]; 
////...............................................
//// LED_X[0] - the number of times the button is clicked
//// LED_X[10] - withholding the checksum
//// LED_X[16] delay
//// LED_X[19] index menu
//// LED_X[18] load
//// LED_X[11] - LED_X[14] actually fill
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
  pinMode(Vol_Bat, INPUT); // bat control
  analogReference(DEFAULT); //міняємо опорноє напряженіє
 
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
  LED_X[18] = num1;
  
}




void loop(){
 buton_control();
 Oper_control(); 
  
}



void Oper_control(){
  if (LED_X[0]>0){
 //  Serial.print("||  Oper_control START"); 
    
   Serial.println(F(""));
   Serial.print(F("||  Click - ")); Serial.print(LED_X[10]);  //}
   Serial.println(F("  ||")); 
    
    
    
    //////////////////////////
    if ( LED_X[19] == 0 ){//// 
     if ( LED_X[0] < 10){
     byte Col = 6;    
   
     if (LED_X[0] == 1) {LED_X[10]++; if (LED_X[10] > Col) LED_X[10]=0;}
     if (LED_X[0] == 2) {LED_X[10]--; if (LED_X[10] < 0) LED_X[10]=Col;} 
    
        
    
      
     switch (LED_X[10]) {
      case 2: {LED_X[1] = 255; LED_X[2] = 0  ; LED_X[3] = 0  ;   LED_X[5] = 255; LED_X[6] = 0  ; LED_X[7] = 0  ; break;} //green
      case 0: {LED_X[1] = 255; LED_X[2] = 255; LED_X[3] = 0  ;   LED_X[5] = 255; LED_X[6] = 255; LED_X[7] = 0  ; break;} //yellow
      case 1: {LED_X[1] = 0  ; LED_X[2] = 255; LED_X[3] = 0  ;   LED_X[5] = 0  ; LED_X[6] = 255; LED_X[7] = 0  ; break;} //red
      case 6: {LED_X[1] = 0  ; LED_X[2] = 255; LED_X[3] = 255;   LED_X[5] = 0  ; LED_X[6] = 255; LED_X[7] = 255; break;} //purple
      case 4: {LED_X[1] = 0  ; LED_X[2] = 0  ; LED_X[3] = 255;   LED_X[5] = 0  ; LED_X[6] = 0  ; LED_X[7] = 255; break;} //blue
      case 5: {LED_X[1] = 255; LED_X[2] = 0  ; LED_X[3] = 255;   LED_X[5] = 255; LED_X[6] = 0  ; LED_X[7] = 255; break;} //turquoise
    //  case 3: {LED_X[1] = 255; LED_X[2] = 255; LED_X[3] = 255;   LED_X[5] = 255; LED_X[6] = 255; LED_X[7] = 255; break;} //white
      case 3: {LED_X[1] = 100; LED_X[2] = 100; LED_X[3] = 100;   LED_X[5] = 100; LED_X[6] = 100; LED_X[7] = 100; break;} //white
      
    } 
   
       if (LED_X[1] == 0)L_IN[1]=0; else  L_IN[1]=1; if (LED_X[5] == 0)L_IN[5]=0; else  L_IN[5]=1;
       if (LED_X[2] == 0)L_IN[2]=0; else  L_IN[2]=1; if (LED_X[6] == 0)L_IN[6]=0; else  L_IN[6]=1;
       if (LED_X[3] == 0)L_IN[3]=0; else  L_IN[3]=1; if (LED_X[7] == 0)L_IN[7]=0; else  L_IN[7]=1;
      
       LED_X[11]=1;LED_X[12]=1;LED_X[13]=0;LED_X[14]=0;//свічі засветки
   
    
  LED_X[0]=0;
  Farba();
    } 
    }
      }  /////
      /////////
   //////// якщо натиснули ітримаємо
  
    //перемикач
  //  if ( LED_X[0] == 30){ /// три рази натиснули і затримали
 //     Serial.println(F("HOLD = 3"));
  //  }
  
  
  
  
  
  
    if ( LED_X[0] == 30){ /// чотири рази натиснули і затримали
      Serial.println(F("||  HOLD = 3 || Vol_Bat  ||")); ///test bat.
    
      
      
      byte  pix1 = 1,//min
            pix2 = 2,//max
            pix3 = 0;//load
           ///pix100 = 0;//LED_X[18] - 100%
      
      byte L1, L2, L3, L5, L6, L7;
      LED_X[16] = 100;
      LED_X[11]=0;LED_X[12]=0;LED_X[13]=1;LED_X[14]=1;//свічі засветки
      
      while (digitalRead(BUTTON_PIN) == HIGH ) {
        
      float  bat;
      bat = map(Vol_Bat, 0, 1023, 0, 500);
      bat = bat/100; 
        
      Serial.print(F("||  Vol_Bat = "));        
      Serial.print(bat);
      Serial.println(F("  ||"));
        
        ///виводимо на ленту
      //bat =  (num1+num2)/2        
     
       
        
        
        //bat = bat*100;
            
           bat = map(bat*100, 0, 500, -10, num1+pix2);/// нижній поріг 2,8V Максимум 4,2V
           
            
             Serial.print(F("||  map bat = "));Serial.print(bat);Serial.println(F("  ||"));
           // delay(3000);
            LED_X[18] = bat; pix3 = LED_X[18];
            Farba_mod();
        
        
        
                        
          
         
         
        
         Serial.print(F("||  pix1 = ")); Serial.print(pix1); Serial.println(F("  ||"));
         Serial.print(F("||  pix2 = ")); Serial.print(pix2); Serial.println(F("  ||"));
         Serial.print(F("||  pix3 = ")); Serial.print(pix3); Serial.println(F("  ||")); 
        
        

        
 
        
        }
      LED_X[16]=50;
      LED_X[18] = num1;
      LED_X[11]=1;LED_X[12]=1;LED_X[13]=0;LED_X[14]=0;//свічі засветки
      Farba();
       

     
      } 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    if ( LED_X[0] > 9){ 
    // Serial.println("  ||  hold  ||");       
  
      
      
      ////////////////////////////
      if ( LED_X[19] == 0 ){////// hold start ( +/- яскравість)
        
      LED_X[16]=0; // обнулюємо затримку виводу, щоб швидше було
      
      while (digitalRead(BUTTON_PIN) == HIGH ) {
        if (LED_X[0] == 10){          
          if (L_IN[1] != 0){LED_X[1]=LED_X[1]-multiplier; 
                            if (LED_X[1] <= 0){LED_X[1]=multiplier;}LED_X[5]=LED_X[1];}
          if (L_IN[2] != 0){LED_X[2]=LED_X[2]-multiplier; 
                            if (LED_X[2] <= 0){LED_X[2]=multiplier;}LED_X[6]=LED_X[2];}
          if (L_IN[3] != 0){LED_X[3]=LED_X[3]-multiplier; 
                            if (LED_X[3] <= 0){LED_X[3]=multiplier;}LED_X[7]=LED_X[3];}
          
         
        }
        
        
        if (LED_X[0] == 20){ 
          if (L_IN[1] != 0){LED_X[1]=LED_X[1]+multiplier; 
                            if (LED_X[1] > 255){LED_X[1]=255;}LED_X[5]=LED_X[1];}
          if (L_IN[2] != 0){LED_X[2]=LED_X[2]+multiplier; 
                            if (LED_X[2] > 255){LED_X[2]=255;}LED_X[6]=LED_X[2];}
          if (L_IN[3] != 0){LED_X[3]=LED_X[3]+multiplier; 
                            if (LED_X[3] > 255){LED_X[3]=255;}LED_X[7]=LED_X[3];}
          
          
         
        }
        
        
        
          Serial.print("LED_X[1]=");Serial.print(LED_X[1]);
          Serial.print("  ||  LED_X[2]=");Serial.print(LED_X[2]);
          Serial.print("  ||  LED_X[3]=");Serial.println(LED_X[3]); 
        
       Farba();   
      }
      
     
      
      
      
      
      
      
      
      
      
     LED_X[0]=0;
     LED_X[16]=50;
    }
   
    
    
 // Serial.println("  => END  ||");    
  } /// hold end ( +/- яскравість)
   /////
   
} 








void Farba(){
  
Serial.print("||  Farba START");  

  
      for (int i=0; i < LED_X[18]; i++) { 
        
        
      pasek.setPixelColor(i, pasek.Color(LED_X[1], LED_X[2], LED_X[3]));
      if (LED_X[11]>0) pasek.show(); 

        
      pasek2.setPixelColor(i, pasek2.Color(LED_X[5], LED_X[6], LED_X[7]));
      if (LED_X[12]>0) pasek2.show();


      
      delay(LED_X[16]); 
    }
  
  if (LED_X[13]>0) pasek.show();
  if (LED_X[14]>0)pasek2.show();
  Serial.println("  => END   ||");
}





void Farba_mod(){
  
///Serial.print("||  Farba_mod START");  
  
 // LED_X[18] (14)

 //byte L1 = LED_X[1], L2 = LED_X[2], L3 =LED_X[3];
Serial.print("||  LED_X[18] = "); Serial.println(LED_X[18]);
  
  for (int i=0; i < num1; i++) { 
        
        pasek.setPixelColor(i, pasek.Color(0, 0, 0));
        pasek2.setPixelColor(i, pasek2.Color(0, 0, 0));     
      delay (50);
        pasek.show();
        pasek2.show();
    }
  
  
  
  
  
  
      for (int i=0; i < LED_X[18]+1; i++) {
         
        for (int j=0; j < 55; j++) {
        if (i==1){pasek.setPixelColor(i, pasek.Color(j, 0, 0));
                  pasek2.setPixelColor(i, pasek2.Color(j, 0, 0));}        
        if (i==2){pasek.setPixelColor(i, pasek.Color(j, j, 0));
                  pasek2.setPixelColor(i, pasek2.Color(j, j, 0));}
        if (i==3){pasek.setPixelColor(i, pasek.Color(j, j, 0));
                  pasek2.setPixelColor(i, pasek2.Color(j, j, 0));}
        if (i==4){pasek.setPixelColor(i, pasek.Color(j, j, 0));
                  pasek2.setPixelColor(i, pasek2.Color(j, j, 0));}
        if (i>4){pasek.setPixelColor(i, pasek.Color(0, j, 0));
                  pasek2.setPixelColor(i, pasek2.Color(0, j, 0));}
           pasek.show();
           pasek2.show();     
        }
      
    }
  
  
//  Serial.println("  => Farba_mod END   ||");
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
  
