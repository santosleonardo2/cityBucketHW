#include <Ultrasonic.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
/*
 * Some functions in the LiquidCrystal library
 *  begin()
 *  clear()
 *  setCursor()
 *  print()
 *  cursor()  ||  noCursor()
 *  blink()   ||  noBlink()
 *  display() ||  noDisplay()
 *  scrollDisplayLeft() ||  scrollDisplayRight()
 *  autoscroll()  ||  noAutoscroll()
 *  leftToRight() || rightToLeft()
 */

#define TRIGGER_PIN  2
#define ECHO_PIN     3
#define MAX_DISTANCE 10.0

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);  //initialize an instance of class NewKeypad
int points = 0;
int count = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); //Coluns=16 X rows=2
  lcd.clear();
}

void loop()
{
  float distanceCM;
  char customKey;
  unsigned long startTime;

  lcd.clear();
  lcd.display();
  lcd.setCursor(0, 0);
  distanceCM = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
  Serial.print("INICIO: ");
  Serial.println(distanceCM);
  if(distanceCM <= MAX_DISTANCE)
  {
    lcd.print("Lixeira cheia!");
    delay(5000);
  }
  else
  {
    //Clears screen, set display on, and cursor to (0,0) 
    lcd.print("Digite seu CPF: ");
    lcd.setCursor(0, 1);
    customKey = 'S';
  
    //Wait user insert ID
    while(customKey!='A')
    {
      customKey = customKeypad.getKey();
      if (customKey)
      {
         lcd.print(customKey);
         //futuramente criar um vetor para armazenar o cpf
      }
    }

    customKey = 'S';
    points=0;
    startTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Insira garrafas");
    lcd.setCursor(0, 1);
    lcd.print("e aperte B ao final.");
    delay(1000);
  
    //Count bottles
    while(customKey!='B')
    {
      distanceCM = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
      Serial.print("PEGA GARRAFA: ");
      Serial.println(distanceCM);
      if(distanceCM <= MAX_DISTANCE)
      {
        points = points + 1;
        startTime = millis();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(points);
        delay(3000);
        distanceCM = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
        if (distanceCM <= MAX_DISTANCE) //Full bag
        {
          break;
        }
      }
      else if ((millis()- startTime) >= 10000) //If nothing happen in 10 sec, it will end task
      {
        break;
      }
      customKey = customKeypad.getKey();
    }
  
    //Send points to server
    if(false)
    {
      //Envia informacao pro servidor
    }

    customKey = 'S';
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voce ganhou:");
    lcd.setCursor(0, 1);
    lcd.print(points);
    lcd.print (" pts.");
    delay (5000);
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finalizando");
    lcd.setCursor(0, 1);
    lcd.print("Tarefa.");
    delay(4000);
  }
}
