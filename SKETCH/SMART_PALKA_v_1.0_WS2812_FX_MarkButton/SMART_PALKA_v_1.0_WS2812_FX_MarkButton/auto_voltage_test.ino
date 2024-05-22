
//vcc- 202 -pin- 51.1 -gnd
void VOL (){

  // if (Vol_in != Cop_Vol_in){
 if (Vol_in > Cop_Vol_in+3 ||  Vol_in < Cop_Vol_in-3){
   Cop_Vol_in = Vol_in; 
   
   ST =  Cop_Vol_in/Volt_Max;               
                           
                           
                           // дальше выводим сервисную информацию в порт
                           if (anWrite == 0) {      
                         Serial.print(F("| analogWrite="));
                         Serial.print(Cop_Vol_in); 
                         Serial.println(F(" |"));}
                           
                    if (anWrite > 0) {       
                    Serial.print(F("| Volt="));
                    Serial.print(ST); 
                    Serial.println(F(" |"));}
                   
                          
  
  
}

 if (vol_an){
                    Serial.print(F("| Volt="));
                    Serial.print(ST); 
                    Serial.println(F(" |"));}
}
