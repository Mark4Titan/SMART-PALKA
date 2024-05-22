// чтение
float EEPROM_float_read(int addr) {    
  byte raw[4];
  for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr+i);
  float &num = (float&)raw;
  return num;
}

// запись
void EEPROM_float_write(int addr, float num) {
  byte raw[4];
  (float&)raw = num;
  for(byte i = 0; i < 4; i++) EEPROM.write(addr+i, raw[i]);
}


String inString;


bool 
FS, // 's' SAVE
FX, // 'x' Flag Volt_Max
FN, // 'n' Flag Volt_Min
FE, // 'e' Flag anWrite
FL; // 'l' Flag LedMod

volatile uint32_t deboR;

void reads(){   
  
  
  if (Serial.available() > 0) {
    char inChar = Serial.read();
     deboR = millis();
    if (inChar == 's') {FS = 1;}
    if (inChar == 'x') {FX = 1; FN = 0; FE = 0; FL = 0;  Serial.println(F("| setup Volt_Max |"));}  
    if (inChar == 'n') {FX = 0; FN = 1; FE = 0; FL = 0;  Serial.println(F("| setup Volt_Min |"));} 
    if (inChar == 'e') {FX = 0; FN = 0; FE = 1; FL = 0;  Serial.println(F("| setup anWrite  |"));} 
    if (inChar == 'l') {FX = 0; FN = 0; FE = 0; FL = 1;  Serial.println(F("| setup LedMod   |"));}
   
      
    
    inString += inChar;  
 
  }
  
  if (millis() - deboR >= 50 && inString != "" && Serial.available() == 0) {
   deboR = millis();


 

   
   
    if (FX && FS != 1){     
      inString.replace("x",""); //Чистые от "r"
      
     //if (inString.toInt() 
    Volt_Max = inString.toInt() ;
    Volt_Max/=100;
    Serial.print("Volt_Max = ");   
    Serial.print(Volt_Max);
      
    }
    
    if (FN && FS != 1){     
      inString.replace("n",""); //Чистые от "b"
      
    Volt_Min = inString.toInt() ;
    Volt_Min/=100;
    Serial.print("Volt_Min = ");   
    Serial.print(Volt_Min);
    
    }
    
    if (FE && FS != 1){     
      inString.replace("e",""); //Чистые от "r"

    anWrite = inString.toInt() ;   
    Serial.print("anWrite = ");   
    Serial.print(anWrite);
      
    }

   if (FL && FS != 1){     
      inString.replace("l",""); //Чистые от "r"

      ledMode = inString.toInt() ;      
    }




    
    if (FS){ 

    if (FL != 1){
    Serial.println(F("| SAVE |"));
        
    if (FX){EEPROM_float_write(0, Volt_Max); Serial.println(F("| ==== Volt_Max ==== |"));}    
    if (FN){EEPROM_float_write(4, Volt_Min); Serial.println(F("| ==== Volt_Min ==== |"));}  
    if (FE){EEPROM_float_write(8, anWrite);  Serial.println(F("| ==== anWrite ==== |"));}  
    
    
   Serial.println(F("| ==== OK! ==== |"));   
   FS = 0;   
    }}
    
    inString = ""; 
   
  } 
  
///
}
