/*
#include <Arduino.h>

#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Parametros Encoder
const int salida_A = 17;    //CLK 
const int salida_B = 5;    //DT
const int swPin  = 4;      //SW

#define led1 9             //Salidas PWM 
#define led2 10
#define led3 11

int contador = 0; 

int A_estado_actual;
int A_ultimo_estado;

int velocidad_scroll = 300;

//::::::::::MENU::::::::::::::::::::::

String menu1[] = {"Led1","Led2","Led3","Todos","Secuencias","Intensidad"};  //Inicializamos nuestro Array con los elementos del menu
int sizemenu1 = sizeof(menu1) / sizeof(menu1[0]);                           //Obtenemos el número de elementos ocupados en la matriz. en este caso 6     

String menu2[] = {"On","Off","Atras"};
int sizemenu2 = sizeof(menu2) / sizeof(menu2[0]);

String menu3[] = {"Izq --> Der","Der <-- Izq","Atras"};
int sizemenu3 = sizeof(menu3) / sizeof(menu3[0]);

String menu4[] = {"Led1","Led2","Led3","Atras"};
int sizemenu4 = sizeof(menu4) / sizeof(menu4[0]);

String linea1,linea2;           //Lineas del LCD
int seleccion = 0;
int pos = 0;
int level_menu = 0;             //Iniciamos la variable en el menu principal 0 --> 6
int opcion = 0;
bool btnpress = false;          //Esta variable de retorno en false aegurando que el boton del Encoder aun no se ha oprimido
int brillo_led1 = 0;
int brillo_led2 = 0;
int brillo_led3 = 0;
int dir_led1=1;                 //Direccion EEPROM para almacenar la variable brillo_led1 
int dir_led2=2;
int dir_led3=3;
byte flecha[] = {B10000,B11000,B11100,B11110,B11100,B11000,B10000,B00000};      //Creamos un array de 8 posiciones para la flecha del menu


void setup() {
  
  Serial.begin(9600);                     //Habilitamos la salida serial por USB
  
  pinMode (salida_A,INPUT);
  pinMode (salida_B,INPUT);
  pinMode (swPin,INPUT_PULLUP);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, flecha);              //Caracter personalizado       
  fn_creditos();
  fn_menu(contador,menu1,sizemenu1);      //Iniciamos presentando el menu principal

  brillo_led1 = EEPROM.read(dir_led1);    // Obtiene el dato de EEPROM y lo envía a la variable brillo_led1
  brillo_led2 = EEPROM.read(dir_led2);
  brillo_led3 = EEPROM.read(dir_led3);

  analogWrite(led1,map(brillo_led1,0,9,0,255));  //Enviamos el dato leido de EEPROM al Led 1
  analogWrite(led2,map(brillo_led2,0,9,0,255));
  analogWrite(led3,map(brillo_led3,0,9,0,255)); 
  
  A_ultimo_estado = digitalRead(salida_A);         //Leemos el estado de la salida del Encoder usando el pin CLK

}

void loop() {

  selectOption();                             //Funcion para detectar cuando se oprime el encoder.  btnpress == true


  //Menu 1 {"Led1","Led2","Led3","Todos","Secuencias","Intensidad"}
  
  if(level_menu == 0){                        //Esta variable corresponde al nivel principal del menu. level_menu = 0.
 
    if(fn_encoder(sizemenu1) ){               //Esta funcion muestra en el LCD el menu en el que estamos
      fn_menu(contador,menu1,sizemenu1);      //Esta funcion muestra la posicion dentro de ese menu segun el valor de la variable contador
    }
 
    if(btnpress){                             //Verificamos si el boton del encoder fue oprimido. btnpress == true
      //Led 1      
      if(contador == 0){
          contador = 0;                       //Seleccionamos la posicion donde quedara la flecha de seleccion. On, Off, Atras
          fn_menu(contador,menu2,sizemenu2);  //Vamos a la funcion que nos muestra las opciones para el Led 1
          level_menu = 1;                     //Esta variable corresponde al nivel secundario del menu. level_menu = 1.                  
      }

      //Led 2
      if(contador == 1){
          contador = 0;
          fn_menu(contador,menu2,sizemenu2);
          level_menu = 2;  
      }

      //Led 3
      if(contador == 2){
          contador = 0;
          fn_menu(contador,menu2,sizemenu2);
          level_menu = 3;  
      }

      //Todos
      if(contador == 3){
          contador = 0;
          fn_menu(contador,menu2,sizemenu2);
          level_menu = 4;  
      }

      //Secuencias
      if(contador == 4){
          contador = 0;
          fn_menu(contador,menu3,sizemenu3);
          level_menu = 5;  
      }

      //Intensidad
      if(contador == 5){
          contador = 0;
          fn_menu(contador,menu4,sizemenu4);   
          level_menu = 6;         
      }
     
      btnpress = false;                               //  Nos aseguramos que esta variable de retorno de la funcion selectOption() vuelva a su estado inicial
    }

  }
  

  //SubMenu Led 1 {"On","Off","Atras"}

  if(level_menu == 1){                                 

    if(fn_encoder(sizemenu2)){                         //Nos desplazamos con el encoder sleccionando las diferentes opciones
      fn_menu(contador,menu2,sizemenu2);
    }
    
    if(btnpress){                                      //Verificamos si el boton del encoder fue oprimido. btnpress == true

      if(contador == 0){
        digitalWrite(led1,HIGH);                       //Encendemos Led 1
        brillo_led1 = 9;                              //Aseguramos el maximo valor de brillo del Led 1 para cuando seleccionemos Intensidad
        EEPROM.write(dir_led1, brillo_led1);           // Se agrega a EEPROM con la dirección primero y valor de la variable después
      }

      if(contador == 1){
        digitalWrite(led1,LOW);                        //Apagamos Led 1
        brillo_led1 = 0;                               //Aseguramos el minimo valor de brillo del Led 1 para cuando seleccionemos Intensidad
        EEPROM.write(dir_led1, brillo_led1);      
      }
      
      if(contador == 2){
          contador = 0;
          fn_menu(contador,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }
  

  //SubMenu Led 2 {"On","Off","Atras"}

  if(level_menu == 2){

    if(fn_encoder(sizemenu2)){
      fn_menu(contador,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(contador == 0){
        digitalWrite(led2,HIGH);
        brillo_led2 = 9;
        EEPROM.write(dir_led2, brillo_led2);      // Se agrega a EEPROM con la dirección primero y valor de la variable después
      }

      if(contador == 1){
        digitalWrite(led2,LOW);
        brillo_led2 = 0;
        EEPROM.write(dir_led2, brillo_led2);      // Se agrega a EEPROM con la dirección primero y valor de la variable después
      }
      
      if(contador == 2){
          contador = 1;
          fn_menu(contador,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }


  //SubMenu Led 3 {"On","Off","Atras"}

  if(level_menu == 3){

    if(fn_encoder(sizemenu2)){
      fn_menu(contador,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(contador == 0){
        digitalWrite(led3,HIGH);
        brillo_led3 = 9;
        EEPROM.write(dir_led3, brillo_led3);      // Se agrega a EEPROM con la dirección primero y valor de la variable después
      }

      if(contador == 1){
        digitalWrite(led3,LOW);
        brillo_led3 = 0;
        EEPROM.write(dir_led3, brillo_led3);   
      }
      
      if(contador == 2){
          contador = 2;
          fn_menu(contador,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }


  //SubMenu Todos  {"On","Off","Atras"};

  if(level_menu == 4){

    if(fn_encoder(sizemenu2)){
      fn_menu(contador,menu2,sizemenu2);              
    }
    
    if(btnpress){

      if(contador == 0){                             
        digitalWrite(led1,HIGH);                          //Encendemos todos los Leds
        digitalWrite(led2,HIGH);
        digitalWrite(led3,HIGH);
        brillo_led1 = 9;
        brillo_led2 = 9;
        brillo_led3 = 9;
        EEPROM.write(dir_led1, brillo_led1);
        EEPROM.write(dir_led2, brillo_led2);
        EEPROM.write(dir_led3, brillo_led3);
      }

      if(contador == 1){
        digitalWrite(led1,LOW);                           //Apagamos todos los Leds
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        brillo_led1 = 0;
        brillo_led2 = 0;
        brillo_led3 = 0;
        EEPROM.write(dir_led1, brillo_led1);
        EEPROM.write(dir_led2, brillo_led2);
        EEPROM.write(dir_led3, brillo_led3);
      }
      
      if(contador == 2){
          contador = 3;
          fn_menu(contador,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }


  //SubMenu Secuencias {"Izq --> Der","Der <-- Izq","Atras"}

  if(level_menu == 5){

    if(fn_encoder(sizemenu3)){
      fn_menu(contador,menu3,sizemenu3);
    }
    
    if(btnpress){

      if(contador == 0){
        digitalWrite(led1,HIGH);
        delay(500);
        digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
        delay(500);
        digitalWrite(led2,LOW);
        digitalWrite(led3,HIGH);
        delay(500);
        digitalWrite(led3,LOW);
       
      }

      if(contador == 1){
        digitalWrite(led3,HIGH);
        delay(500);
        digitalWrite(led3,LOW);
        digitalWrite(led2,HIGH);
        delay(500);
        digitalWrite(led2,LOW);
        digitalWrite(led1,HIGH);
        delay(500);
        digitalWrite(led1,LOW);
      
      }
      

      if(contador == 2){
          contador = 4;
          fn_menu(contador,menu1,sizemenu1);
          level_menu = 0; 
      }
         
      btnpress = false;
      
    }
  }

  //SubMenu Intensidad {"Led1","Led2","Led3","Atras"}

  if(level_menu == 6){

    if(fn_encoder(sizemenu4)){
      fn_menu(contador,menu4,sizemenu4);
    }
   
    if(btnpress){    
                
      if(contador == 0){ 
        
        contador = brillo_led1;     
        lcd.clear();
        
        do{

        fn_contador_brillo();           
        brillo_led1 = contador;
        
        lcd.setCursor(0, 0);
        lcd.print("Brillo Led 1");
        analogWrite(led1,map(brillo_led1,0,9,0,255));   
        lcd.setCursor(0, 1);  
        lcd.print(brillo_led1);            
                                     
        }while(digitalRead(swPin) == HIGH);        //Instrucciones despues del do se repetirán mientras esta condición sea cierta
         delay(500);       
         EEPROM.write(dir_led1, brillo_led1);      // Se agrega a EEPROM con la dirección primero y valor de la variable después 
         contador = 3;     
      }

      if(contador == 1){       
        
        contador = brillo_led2;     
        lcd.clear();
        
        do{

        fn_contador_brillo(); 
        brillo_led2 = contador;          
            
        lcd.setCursor(0, 0);
        lcd.print("Brillo Led 2");
        analogWrite(led2,map(brillo_led2,0,9,0,255));      
        lcd.setCursor(0, 1);
        lcd.print(brillo_led2);            
                                     
        }while(digitalRead(swPin) == HIGH);    //Instrucciones despues del do se repetirán mientras esta condición sea cierta
         delay(500); 
         EEPROM.write(dir_led2, brillo_led2);       
         contador = 3;                
      }

      if(contador == 2){       
        contador = brillo_led3;     
        lcd.clear();
        
        do{

        fn_contador_brillo(); 
        brillo_led3 = contador;          
            
        lcd.setCursor(0, 0);
        lcd.print("Brillo Led 3");
        analogWrite(led3,map(brillo_led3,0,9,0,255));      
        lcd.setCursor(0, 1);
        lcd.print(brillo_led3);            
                                     
        }while(digitalRead(swPin) == HIGH);    //Instrucciones despues del do se repetirán mientras esta condición sea cierta
         delay(500);  
         EEPROM.write(dir_led3, brillo_led3);      
         contador = 3;              
      } 

      if(contador == 3){                    
          contador = 5;                        //Posicion del menu donde se genero el llamado 0 --> 5               
          fn_menu(contador,menu1,sizemenu1);   //Retorna al Menu Principal a la posicion del menu donde se genero el llamado
          level_menu = 0; 
      }
          
      btnpress = false;
       
    }
     
  }

}

void selectOption(){
  if(digitalRead(swPin) == LOW){
    delay(500);
    btnpress = true;
  }
}

void fn_contador_brillo(){
  
         A_estado_actual = digitalRead(salida_A); 
  
         if (A_estado_actual != A_ultimo_estado){     

          if (digitalRead(salida_B) != A_estado_actual){        //DT != CLK  ``````````    
          contador ++;
          delay(250);     
          }
          else {
          contador --;
          delay(250); 
          } 
        }
                     
        if(contador>9){
        contador = 9;
        }
        if(contador<0){
          contador = 0;
        }
        
}

void fn_creditos(){

  lcd.setCursor(0, 0);
  lcd.print("Robotica, Impresion, 3D y algo mas..."); 
  delay(2000);
  
  for (int positionCounter = 0; positionCounter < 21; positionCounter++) {  
        
  lcd.scrollDisplayLeft();             //Desplaza el texto del LCD hacia la Izquierda
  delay(velocidad_scroll);
  
  }
  
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encoder-Arduino");
  lcd.setCursor(0, 1);
  lcd.print("Menu - SubMenu");
  delay(2000);

}


void fn_menu(int pos,String menus[],byte sizemenu){
  lcd.clear();
  linea1="";
  linea2="";
   
  if((pos % 2) == 0){  

     lcd.setCursor(0, 0);
     lcd.write(byte(0));
     linea1 = menus[pos];
    
    if(pos+1 != sizemenu){
      linea2 = menus[pos+1];
    }
    
  }else{
    linea1 = menus[pos-1];
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    linea2 = menus[pos];
  }
  
     lcd.setCursor(1, 0);
     lcd.print(linea1);

     lcd.setCursor(1, 1);
     lcd.print(linea2); 
   
}


bool fn_encoder(byte sizemenu){ 
  bool retorno = false;

  A_estado_actual = digitalRead(salida_A); 
  
  if (A_estado_actual != A_ultimo_estado){     

    if (digitalRead(salida_B) != A_estado_actual){        //DT != CLK     
     contador ++;
     delay(250);     
    }
    else {
     contador --;
     delay(250); 
    }
    
    if(contador <=0){
      contador = 0;
    }
    
    if(contador >= sizemenu-1 ){
      contador = sizemenu-1;
    }
    
    retorno = true;

  } 

  return retorno; 
}











*/