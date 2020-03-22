/*
#       _\|/_   A ver..., ¿que tenemos por aqui?
#       (O-O)        
# ---oOO-(_)-OOo---------------------------------
 
 
##########################################################
# ****************************************************** #
# *           DOMOTICA PARA PRINCIPIANTES              * #
# *                 BIG NUMERO Demo                    * #
# *          Autor:  Eulogio López Cayuela             * #
# *                                                    * #
# *         Version v1.0   Fecha: 07/09/2015           * #
# ****************************************************** #
##########################################################
*/

#define __VERSION__ "BIG NUMERO Demo\n"


/*
      ===== NOTAS DE LA VERSION ===== 
 - Programa demostracion de la funcion BIG NUMERO.
   Util para relojes RCT, temporizadores con millis() o simples contadores.


      ===== OPCIONES DISPONIBLES =====  

   Esta demostracion cuenta las opciones siguientes opciones 
   todas ellas basadas en temporizacion con millis()
   
   1) Reloj.
   2) Temporizador creciente.
   3) Cuenta atras.
 

   
   Tamaño actual compilado 4356 bytes de programa y 419 bytes de uso de RAM

*/


//------------------------------------------------------
//Importamos las librerias necesarias
//------------------------------------------------------
#include <Wire.h>                 //libreria para comunicaciones I2C
#include <LiquidCrystal_I2C.h>    //Biblioteca para el control del LCD
#include <Temporizador_inopya.h>


//------------------------------------------------------
// Algunas definiciones personales para mi comodidad al escribir codigo
//------------------------------------------------------
#define AND &&
#define OR ||

//------------------------------------------------------
// Otras definiciones para pines y variables
//------------------------------------------------------

#define LCD_ADDR          0x27    // Direccion I2C de nuestro LCD color verde -- 0x3F
#define LED_ONBOARD         13    //


//------------------------------------------------------
//DECLARACION DE CONSTANTES GLOBALES
//------------------------------------------------------




/* definicion de nuevos caracteres que seran bloques que nos ayuden a construir digitos de gran tamaño */
byte bloques[5][8] = {{B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
                      {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000},
                      {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
                      {B11111, B11111, B00000, B00000, B00000, B00000, B11111, B11111},
                      {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111}};
                      
byte numeros[13][6] = {{4,1,4,4,2,4},  //0
                       {1,4,0,2,4,2},  //1
                       {3,3,4,4,2,2},  //2
                       {1,3,4,2,2,4},  //3
                       {4,2,4,0,0,4},  //4
                       {4,3,3,2,2,4},  //5
                       {4,3,3,4,2,4},  //6
                       {1,1,4,0,4,0},  //7
//                     {1,1,4,0,0,4},  //7 otra version del numero 7 (mola menos)
                       {4,3,4,4,2,4},  //8
                       {4,3,4,2,2,4},  //9
                       {0,0,0,0,0,0},  //   caracter en 'blanco' (vacio)   -Sin uso-                     
                       {4,4,4,4,4,4},  //   caracter en 'negro'            -Sin uso-               
                       {0,0,0,2,2,2}}; //_  caracter en 'raya baja'        -Sin uso- 



//------------------------------------------------------
// Creamos las instancia de los objetos:
//------------------------------------------------------

//Creamos el objeto 'lcd' como una instancia del tipo "LiquidCrystal_I2C"
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

Temporizador_inopya relojSegundos;
Temporizador_inopya mediosSegundos;


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                    FUNCION DE CONFIGURACION
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void setup()  
{
  //Serial.begin(9600);  //Se inicia el puerto serie para depuracion
  //Serial.println(F(__VERSION__));

  lcd.begin();                      //Inicializar el display
  
  pinMode(LED_ONBOARD, OUTPUT);
  digitalWrite(LED_ONBOARD, LOW);
  
  /* Mensaje inicial para demostrar que el LCD esta bien conectado y comunicado */
  lcd.clear();                      //Reset del display 
  lcd.setBacklight(true);           //Activamos la retroiluminacion
  lcd.print("BIG NUMEROS demo");  
  delay(3000);
  lcd.clear();                      //Reset del display

  /* cargamos en la memoria del lcd los caracteres personalizados que hemos diseñado */ 
  lcd.createChar(0, bloques[0]);  
  lcd.createChar(1, bloques[1]);
  lcd.createChar(2, bloques[2]);
  lcd.createChar(3, bloques[3]);
  lcd.createChar(4, bloques[4]);
  
  relojSegundos.begin(1000);
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                  BUCLE PRINCIPAL DEL PROGRAMA
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void loop()
{
  //reloj_demo();
  //temporizador_demo();
  cuenta_atras();
}

/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ###################################################################################################### 
        BLOQUE DE FUNCIONES: LECTURAS DE SENSORES, COMUNICACION SERIE, INTERRUPCIONES...
   ###################################################################################################### 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    TEMPORIZADORES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
// DEMO RELOJ CON millis()
//========================================================

void reloj_demo()
{
  /* Mensaje inicial */
  lcd.clear();
  lcd.print(F("DEMO ")); 
  lcd.setCursor(0, 1);
  lcd.print(F("RELOJ millis()")); 
  delay(2000);
  lcd.clear();
  
  byte horas = 19;
  byte minutos = 10;
  byte segundos = 00;
  bool FLAG_mediosSegundos = false;
  
  while(true){
    unsigned long tiempoActual = millis();
    if(relojSegundos.estado()==false){
      relojSegundos.begin(1000);
      segundos++;
      if(segundos>59){
        segundos = 0;
        minutos++;
        if(minutos>59){
          minutos=0;
          horas++;
          if(horas>23){
            horas=0;
          }
        }
      }
    }
    
    lcd.setCursor(15, 0); 
    lcd.print(segundos/10);    //mostrar 2 cifra de los segundos (mayor peso)
    lcd.setCursor(15, 1);
    lcd.print(segundos%10);    //mostrar 1 cifra de los segundos
  
    bigNumero(minutos%10, 12); //mostrar 1 cifra de los minutos 
    bigNumero(minutos/10, 8);  //mostrar 2 cifra de los minutos (mayor peso)
  
    bigNumero(horas%10, 4);    //mostrar 1 cifra de las horas 
    bigNumero(horas/10, 0);    //mostrar 2 cifra de las horas (mayor peso) 
    
    if(mediosSegundos.estado()==false){
      mediosSegundos.begin(500);
      FLAG_mediosSegundos = !FLAG_mediosSegundos;
      digitalWrite(LED_ONBOARD, FLAG_mediosSegundos);  //opcional, parpadeo de un led como segundero
      /* parte 1 del parpadeo de los (:) de separacion entre horas y minutos */ 
      if(FLAG_mediosSegundos){
        lcd.setCursor(7, 0);
        lcd.write(165);
        lcd.setCursor(7, 1);  
        lcd.write(165); 
      }
      else{
        /* parte 2 del parpadeo de los (:) de separacion entre horas y minutos */
        lcd.setCursor(7, 0);
        lcd.write(32);
        lcd.setCursor(7, 1);  
        lcd.write(32);
      }
    }
  } 
}


//========================================================
// TEMPORIZADOR CRECIENTE
//========================================================

#define INTERVALO_TEMPORIZADOR 1000

void temporizador_demo()
{
  /* Mensaje inicial */
  lcd.clear();
  lcd.print(F("DEMO")); 
  lcd.setCursor(0, 1);
  lcd.print(F("TEMPORIZADOR >>")); 
  delay(2000);
  lcd.clear();
  
  byte horas = 0;
  byte minutos = 0;
  byte segundos = 0;
  bool FLAG_mediosSegundos = false;

  while(true){
    unsigned long tiempoActual = millis();
    if(relojSegundos.estado()==false){
      relojSegundos.begin(1000);
      segundos++;
      if(segundos>59){
        segundos = 0;
        minutos++;
        if(minutos>59){
          minutos=0;
          horas++;
          if(horas>23){
            horas=0;
          }
        }
      }
    }
    
    lcd.setCursor(0, 0); 
    lcd.print(horas/10);    //mostrar 2 cifra de los segundos (mayor peso)
    lcd.setCursor(0, 1);
    lcd.print(horas%10);    //mostrar 1 cifra de los segundos
  
    bigNumero(segundos%10, 13); //mostrar 1 cifra de los minutos 
    bigNumero(segundos/10, 9);  //mostrar 2 cifra de los minutos (mayor peso)
  
    bigNumero(minutos%10, 5);    //mostrar 1 cifra de las horas 
    bigNumero(minutos/10, 1);    //mostrar 2 cifra de las horas (mayor peso) 
  
    if(mediosSegundos.estado()==false){
      mediosSegundos.begin(500);
      FLAG_mediosSegundos = !FLAG_mediosSegundos;
      digitalWrite(LED_ONBOARD, FLAG_mediosSegundos);  //opcional, parpadeo de un led como segundero
      /* parte 1 del parpadeo de los (:) de separacion entre horas y minutos */ 
      if(FLAG_mediosSegundos){
        lcd.setCursor(8, 0);
        lcd.write(165);
        lcd.setCursor(8, 1);  
        lcd.write(165); 
      }
      else{
        /* parte 2 del parpadeo de los (:) de separacion entre horas y minutos */
        lcd.setCursor(8, 0);
        lcd.write(32);
        lcd.setCursor(8, 1);  
        lcd.write(32);
      }
    }
  }  
}



void cuenta_atras()
{
  /* Mensaje inicial */
  lcd.clear();
  lcd.print(F("DEMO")); 
  lcd.setCursor(0, 1);
  lcd.print(F("CUENTA ATRAS <<")); 
  delay(2000);
  lcd.clear();
  
  int horas = 0;      //0 a 99
  int minutos = 5;    //0 a 59
  int segundos = 1;   //0 a 59
  bool FLAG_mediosSegundos = false;

  while(true){
    unsigned long tiempoActual = millis();
    if(relojSegundos.estado()==false){
      relojSegundos.begin(1000);
      segundos--;
      if(segundos<0){
        segundos = 59;
        minutos--;
        if(minutos<0){
          minutos=59;
          horas--;
          if(horas<0){
            horas=0;
            minutos=0;
            segundos=0;
            break;
          }
        }
      }
    }
    
    lcd.setCursor(0, 0); 
    lcd.print(horas/10);    //mostrar 2 cifra de los segundos (mayor peso)
    lcd.setCursor(0, 1);
    lcd.print(horas%10);    //mostrar 1 cifra de los segundos
  
    bigNumero(segundos%10, 13); //mostrar 1 cifra de los minutos 
    bigNumero(segundos/10, 9);  //mostrar 2 cifra de los minutos (mayor peso)
  
    bigNumero(minutos%10, 5);    //mostrar 1 cifra de las horas 
    bigNumero(minutos/10, 1);    //mostrar 2 cifra de las horas (mayor peso) 
  
    if(mediosSegundos.estado()==false){
      mediosSegundos.begin(500);
      FLAG_mediosSegundos = !FLAG_mediosSegundos;
      digitalWrite(LED_ONBOARD, FLAG_mediosSegundos);  //opcional, parpadeo de un led como segundero
      /* parte 1 del parpadeo de los (:) de separacion entre horas y minutos */ 
      if(FLAG_mediosSegundos){
        lcd.setCursor(8, 0);
        lcd.write(165);
        lcd.setCursor(8, 1);  
        lcd.write(165); 
      }
      else{
        /* parte 2 del parpadeo de los (:) de separacion entre horas y minutos */
        lcd.setCursor(8, 0);
        lcd.write(32);
        lcd.setCursor(8, 1);  
        lcd.write(32);
      }
    }
  }
  delay(2000); //pausa mostrado el tiempo agotado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("  CUENTA ATRAS")); 
  lcd.setCursor(0, 1);
  lcd.print(F("   TERMINADA")); 
  while(true);  //pausa infinit mostrando mensaje 
}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    DISPLAY
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//   Funcion para crear un DIGITO GRANDE
//========================================================

void bigNumero(byte i, byte posicion)
{
  //el parametro i es el numero que queremos representar
  //que conincide con el indice de la lista en la que se guardan 
  //los caracteres personalizados que forman cada numero
  lcd.setCursor(posicion, 0);
  lcd.write(numeros[i][0]); lcd.write(numeros[i][1]); lcd.write(numeros[i][2]);
  lcd.setCursor(posicion, 1);
  lcd.write(numeros[i][3]); lcd.write(numeros[i][4]); lcd.write(numeros[i][5]);   
  return;
}



//*******************************************************
//                    FIN DE PROGRAMA
//*******************************************************
