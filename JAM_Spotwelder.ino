#define pedal 12
#define pulse_out 11
#define DEFAULT_T1 100
#define DEFAULT_T2 50
#define DEFAULT_T3 150
//######################### GLOBAL VARIABLES #########################################
int wtime[3] = {DEFAULT_T1, DEFAULT_T2, DEFAULT_T3};
int state = 0;
//int state[] = {0, 0};
int state_x = 0;
int state_y = 0;
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <Keypad.h>
LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display


const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'M', 'R', '#', '*'},
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'L'},
  {'7', '8', '9', 'K'},
  {'B', '0', 'J', 'E'}
};
byte rowPins[ROWS] = {10, 9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int c = 1;
//############################### SETUP #########################################
void setup() {
  int error;
  digitalWrite(pulse_out, HIGH);
  pinMode(pulse_out, OUTPUT);
  digitalWrite(pulse_out, HIGH);
  pinMode(pedal, INPUT);
  Serial.begin(9600);
  // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2); // initialize the lcd
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
    logo();
  } else {
    Serial.println(": LCD not found.");
  }
}
//################################ MODULES ###############################
void torlo(void) {
  lcd.home();
  lcd.clear();
}

void cont_weld(void) {
  if (digitalRead(pedal) == 1 ) {
    digitalWrite(pulse_out, HIGH);
  }
  if (digitalRead(pedal) == 0 ) {
    digitalWrite(pulse_out, LOW);
  }
}

void weld(void){

  torlo();
  lcd.print("    PRE-HEAT");
  digitalWrite(pulse_out, 0);
  delay(wtime[0]);
  torlo();
  lcd.print("     BREAK");
  digitalWrite(pulse_out, 1);
  delay(wtime[1]);
  torlo();
  lcd.print("    WELDING");
  digitalWrite(pulse_out, 0);
  delay(wtime[2]);
  digitalWrite(pulse_out, 1);
  torlo();
  //meghivni az elozo menut
}
//############################## END MODULS ##############################
//############################## MENU ####################################
void logo(void) { //0_0 elem
  torlo();
  lcd.setCursor(0, 0);
  lcd.print(" JAM SpotWelder ");
  lcd.setCursor(0, 1);
  lcd.print("       TM       ");
  state_x = 0;
  state_y = 0;
}
void menu_1_1(void) {
  torlo();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL WELD<");
  lcd.setCursor(0, 1);
  lcd.print("AUTOMATA WELD");

}

void menu_1_1_1(void) {
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL WELDING<");
  lcd.setCursor(0, 1);
  lcd.print("BACK");
}
void menu_1_1_2(void) {
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL WELDING");
  lcd.setCursor(0, 1);
  lcd.print("BACK<");
  state_y--;
}

void menu_1_2(void) {
  lcd.home();
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL WELD");
  lcd.setCursor(0, 1);
  lcd.noCursor();
  lcd.print("AUTOMATA WELD<");

}
void escape(void) {
  logo();
  state_x = 0;
  state_y = 0;

}
//############################## END MENU ################################

void loop() {
  char customKey = customKeypad.getKey();

  switch (customKey) {
    case 'K':
      escape();
      break;
    case 'L':
      //do something when var equals 2
      state_x++;
      break;
    case 'F':
      //do something when var equals 2
      menu_1_1();
      state_x--;
      break;
    case 'E':
      //do something when var equals 2
      state_y ++;
      break;
  } 
      if (state_x <= 0) {
        state_x = 0;
      }
      if (state_y <= 0) {
        state_y = 0;
      }
      Serial.print(state_x);
      Serial.print(" : ");
      Serial.println(state_y);
      
      if (state_x == 0 && state_y == 0) {
        logo();
      }
      if (state_x == 1 && state_y == 0) {
        menu_1_1();
      }
      if (state_x == 2 && state_y == 0) {
        menu_1_2();
      }
      if (state_x == 1 && state_y == 1) {
        menu_1_1_1();
      }
      delay(3);//ne villogjon
  }
