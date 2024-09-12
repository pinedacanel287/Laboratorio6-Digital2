//Librerías usadas
#include <Arduino.h>          
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Pines para la I2C
#define I2C_SDA 21
#define I2C_SCL 22

//Pines de ADC, entradas y referencia
#define Pot1 15
#define Pot2 4
#define Referencia 23

//Pines para los botones
#define botonSuma 27
#define botonResta 25

//Variables para controlar el debounce del botón
unsigned long t_Boton = 0;
unsigned long ultimo_t_Boton = 0;


LiquidCrystal_I2C lcd(0x27,16,2);

//Variables globales
float volt1=0, volt2=0, print1=0, print2=0;
int cont=0;

//Función para aumentar el contador
void IRAM_ATTR Suma() {
  t_Boton = millis();
  if (t_Boton - ultimo_t_Boton > 250) {
    ultimo_t_Boton = t_Boton;
    cont++;
  }
}

//Función para reducir el contador
void IRAM_ATTR Resta() {
  t_Boton = millis();
  if (t_Boton - ultimo_t_Boton > 250) {
    ultimo_t_Boton = t_Boton;
    cont--;
  }
}

void setup() {
  //Configuración de la LCD
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  lcd.clear();

  //Configuración para las entradas ADC
  analogSetVRefPin(Referencia);
  pinMode(Pot1, INPUT);
  pinMode(Pot2, INPUT);

  //Configuración de los pines para los botones
  pinMode(botonSuma, INPUT_PULLUP);
  pinMode(botonResta, INPUT_PULLUP);

  //Encabezado en la LCD
  lcd.setCursor(0,0);
  lcd.print("POT1: POT2: CPU:");
  lcd.setCursor(12,1);
  lcd.print("000");

  //Configuración de las interrupciones
  attachInterrupt(botonSuma, Suma, FALLING);
  attachInterrupt(botonResta, Resta, FALLING);
}

void loop() {
  //Lectura de potenciómetros
  volt1=analogReadMilliVolts(Pot1);
  volt2=analogReadMilliVolts(Pot2);

  //Voltaje en voltios
  print1 = volt1/1000;
  print2 = volt2/1000;

  //Limites del contador
  if (cont > 255) {
    cont = 0;
    lcd.setCursor(12,1);
    lcd.print("000");
  } else if (cont < 0) {
    cont = 255;
  }

  //Imprimir los datos de voltaje
  lcd.setCursor(0,1);
  lcd.print(print1);
  lcd.setCursor(6,1);
  lcd.print(print2);
  delay(100);
  lcd.setCursor(12,1);
  lcd.print(cont);
}
