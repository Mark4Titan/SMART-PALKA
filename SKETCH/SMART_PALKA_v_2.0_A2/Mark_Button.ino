/*
разработаны на основе урока AlexGyver :
https://alexgyver.ru/lessons/interrupts/#%D0%BB%D0%BE%D0%B2%D0%B8%D0%BC-%D1%81%D0%BE%D0%B1%D1%8B%D1%82%D0%B8%D0%B5
*/

/*
мой проект:


*/









void btnisr() {  
  // CHANGE не предоставляет состояние пина, 
  // придётся узнать его при помощи digitalRead  
  
  if (millis() - debounce >= waiting_time && digitalRead(2)) { 
    debounce = millis();    
    press++;
  }
}

void Log() {
  
  if (millis() - debounce >= time_again && press>0) {
    debounce = millis();
    
    
    if (digitalRead(2) == 0 && hold_flag == 0){
    //  Serial.print(F("| press = "));  Serial.print(press); Serial.println(F(" |"));
      
      if (press == 1){
      if (++modeCounter >= num_modes) modeCounter = 0;
      if (FdeHOLD){FdeHOLD = 0; if (--modeCounter >= num_modes){modeCounter = num_modes-1;}
      }
      
    ledMode = fav_modes[modeCounter];    // получаем новый номер следующего режима
    change_mode(ledMode);               // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)    
    changeFlag = true;}
    
     if (press == 2){ 
      if (--modeCounter >= num_modes){modeCounter = num_modes-1;}
      if (FdeHOLD){FdeHOLD = 0; if (++modeCounter >= num_modes) modeCounter = 0;}   
  
       
      
    ledMode = fav_modes[modeCounter];    // получаем новый номер следующего режима
    change_mode(ledMode);               // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)    
    changeFlag = true;}

    if (press > 2){
      // цей шмат перекидає на кількість натисків((( не підійшов
      // for (byte i = 0; i <= press-1; i++ ) {if (++modeCounter >= num_modes) modeCounter = 0;}        
      // ledMode = fav_modes[modeCounter];

      // скидаємо на початок
      modeCounter = 0;
      ledMode = fav_modes[modeCounter];
     // }  
    }
      press = 0;}
    
    ////.............hold...............///////
    if (digitalRead(2) == 1){
     // Serial.print(F("| hold = "));  Serial.print(press); Serial.println(F(" |"));     
     if (press == 1 && hold_UP_DO == 1){Lx+=10; if (Lx > 255)Lx = 255; Lx_f = 1;  Serial.print(F("| Brightness = "));  Serial.print(Lx); Serial.println(F(" |"));}
     if (press == 1 && hold_UP_DO == 0){Lx-=10; if (Lx < 2)Lx = 5; Lx_f = 1;      Serial.print(F("| Brightness = "));  Serial.print(Lx); Serial.println(F(" |"));}

     //if (press == 2){vol_an = 1;}



   if (press == 2){if (temporary_Flag == 0){temporary_Flag = 1; temporary = ledMode; VOL ();}
   //Serial.print(F("| temporary =  "));Serial.println(temporary); 
   ledMode = 110;
   if (millis() - deHOLD >= 4000) {
    deHOLD = millis();
    if (FdeHOLD){           
      temporary = 110; Serial.println(F("| Charge Level HOLD |"));}else FdeHOLD = 1;}
   }
     
    


      
      hold_flag = 1;}

      
    
    if (digitalRead(2) == 0 && hold_flag == 1){
      // если не нужно выводить окончания удержание, просто закомментируйте следующую строку
      // Serial.println(F("| hold =  END |")); 
       if (press == 2)ledMode = temporary;
       temporary_Flag = 0;
       Lx_f = 0; 
       vol_an = 0; 
       
 //      hold_UP_DO = !hold_UP_DO; 
       if (Lx > 240)hold_UP_DO = 1;
       if (Lx < 25)hold_UP_DO = 0;
       if (Lx > 20 || Lx < 250) hold_UP_DO = !hold_UP_DO;
       
       
      // FdeHOLD = 0;  
      press = 0; hold_flag = 0;}
    
   }
  
    // здесь можно добавить срабатывания своего таймер  
  
}
