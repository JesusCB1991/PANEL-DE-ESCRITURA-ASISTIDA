#include <Keypad.h>
#include <Wire.h> 

#define DISPLAY_WIDTH 20
#define DISPLAY_HEIGHT 4
#define DISPLAY_LEN (DISPLAY_WIDTH * DISPLAY_HEIGHT)

#include "LiquidCrystal_I2C.h"
#define BACKLIGHT_PIN (3)

LiquidCrystal_I2C lcd1(0x3F, 2, 1, 0, 4, 5, 6, 7, BACKLIGHT_PIN, POSITIVE);

//para crear la letra Ñ
byte customChar[] = {
  B01110,
  B00000,
  B10001,
  B11001,
  B10101,
  B10011,
  B10001,
  B00000
};

#define ROWS 4 //cuatro filas
#define COLS 4 //cuatro columnas

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//ID que se devuelve cuando se pulsa una tecla
char keysID[ROWS][COLS] = {
  {1,2,3,13},
  {7,8,9,14},
  {4,5,6,15},
  {10,11,12,16}
};


byte rowPins[ROWS] = {9, 8, 7, 6}; //Conecta las filas a los pines del teclado
byte colPins[COLS] = {5, 4, 3, 2}; //Conecta las columnas a los pines del teclado

Keypad keypad = Keypad( makeKeymap(keysID), rowPins, colPins, ROWS, COLS );

//mapeo de las letras presionando dos teclas
char letterMapping[6][6] = {
  {'\1','N','M','Q','P','O'},
  {'I','H','G','L','K','J'},
  {'C','B','A','F','E','D'},
  {'7','6','5','0','9','8'},
  {'1','Z','Y','4','3','2'},
  {'T','S','R','X','V','U'}
};

//Carga un buffer de texto completo en la pantalla
void setText(LiquidCrystal_I2C *lcd, char *text, uint8_t cursorPos)
{
  char tmpBuf[DISPLAY_WIDTH + 1];
  tmpBuf[DISPLAY_WIDTH] = 0;
//define los renglones
  lcd->setCursor(0, 0);
  memcpy(tmpBuf, text + DISPLAY_WIDTH * 0, DISPLAY_WIDTH);
  lcd->print(tmpBuf);

  lcd->setCursor(0, 1);
  memcpy(tmpBuf, text + DISPLAY_WIDTH * 1, DISPLAY_WIDTH);
  lcd->print(tmpBuf);

  lcd->setCursor(0, 2);
  memcpy(tmpBuf, text + DISPLAY_WIDTH * 2, DISPLAY_WIDTH);
  lcd->print(tmpBuf);

  lcd->setCursor(0, 3);
  memcpy(tmpBuf, text + DISPLAY_WIDTH * 3, DISPLAY_WIDTH);
  lcd->print(tmpBuf);

  uint8_t col = cursorPos % DISPLAY_WIDTH;
  uint8_t row = cursorPos / DISPLAY_WIDTH;
  lcd->setCursor(col, row);
}

//
void setup()
{
  Serial.begin(9600);

  lcd1.begin(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lcd1.createChar(1, customChar);

  //hace una prueba en la que se llena toda la pantalla de letras para ver que todo esta bien
  char buf[DISPLAY_LEN + 1];
  for (int i = 0; i < DISPLAY_LEN; i++)
    buf[i] = 'A' + i % 26;

  setText(&lcd1, buf, 0);
  delay(1000);

  lcd1.clear();

  //monstrar mensaje de preparado
  lcd1.home();
  lcd1.cursor();
  lcd1.print("PANTALLA PREPARADA Y LISTA PARA USAR");  

  delay(4000);

  lcd1.clear();
  delay(4000);

  uint8_t pos = 0;
  memset(buf, ' ', DISPLAY_LEN);
  buf[DISPLAY_LEN] = 0;

  char key1, key2;
  while (1)
  {
    char letter = 0;
    while ((key1 = keypad.getKey()) == NO_KEY); //wait for keypress

    //borrar la última letra
    if (key1 == 7)
    {
      if (pos > 0)
      {
        pos--;
        buf[pos] = ' ';
      }
    }
    //añadir un espacio
    else if (key1 == 9)
    {
      letter = ' ';
    }
    //reiniciar pantalla
    else if (key1 == 8)
    {
      memset(buf, ' ', DISPLAY_LEN);
      pos = 0;
    }
//introducimos las macros con el ID de la tecla que queramos
    else if (key1 == 13)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("TENGO SED");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 14)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("TENGO FRIO");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 15)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("TENGO CALOR");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 16)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("CAMBIAME");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 12)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("TENGO HAMBRE");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 11)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("QUIERO IR AL RETRETE");  
      delay(4000);
      lcd1.clear();
    }

    else if (key1 == 10)
    {
      lcd1.home();
      lcd1.cursor();
      lcd1.print("TE QUIERO");  
      delay(4000);
      lcd1.clear();
    }

    
    //añadir la letra elegida   
    else
    {
      while ((key2 = keypad.getKey()) == NO_KEY); //espera la pulsación de una tecla
      delay(300);
      
      if (key1 >= 1 && key1 <= 6 && key2 >= 1 && key2 <= 6)
        letter = letterMapping[key1 - 1][key2 - 1];
    }

    //cuando la letra se ha añadido
    if (letter)
    {
      buf[pos] = letter;
      if (pos < DISPLAY_LEN - 1)
        pos++;
    }

    setText(&lcd1, buf, pos);

  }
}

//
void loop() {}
