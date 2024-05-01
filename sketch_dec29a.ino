#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <iarduino_RTC.h>

#define MAX_TEXT_SIZE 100
#define EEPROM_START_ADDRESS 0

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows
#define BACKLIGHT_PIN 13

char textBuffer[MAX_TEXT_SIZE];
int cursorPosition = 0;
bool initialSetupComplete = false;
String username;
String password;
String fontType;
String textColor;
const int buttonPin = 7;
const int speakerPin = 9;
iarduino_RTC clock(RTC_DS1302, 8, 6, 7);

void executeCommand(String command);
void printText();

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);
  // lcd.begin(16, 2); // commented out
  lcd.home();
  lcd.print("Hello In_the_os");
  lcd.setCursor(0, 1);
  lcd.print("hello user");
  delay(3000);

  Serial.begin(9600);
  clock.begin();
  clock.settime(30, 54, 9, 4, 11, 23);
}

void loop() {
  if (!initialSetupComplete) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Initial Setup");

    lcd.setCursor(0, 1);
    lcd.print("Username: ");
    while (Serial.available() == 0);
    username = Serial.readStringUntil('\n');
    delay(2000);
    lcd.clear();
    lcd.print("Username=" + String(username));
    delay(2000);
    lcd.clear();

    lcd.print("Set password? (y/n): ");
    while (Serial.available() == 0);
    char response = Serial.read();
    delay(2000);
    lcd.clear();    

      if (response == 'y' || response == 'Y') {
      lcd.print("Password:");
      delay(9000);      
      while (Serial.available() == 0);
      password = Serial.readStringUntil('\n');
      lcd.print("Set password");
    } else {
      lcd.print("Is not a password");
      delay(9000);      
      password = ""; // Скидаємо пароль
      delay(9000);
      lcd.clear();           
      lcd.print("Setup Complete");
      initialSetupComplete = true;
      delay(9000);
      lcd.clear(); 
    }
  }

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.toLowerCase();
    executeCommand(command);
  }

  digitalWrite(speakerPin, HIGH);
  delay(3000);
  digitalWrite(speakerPin, LOW);
  delay(1000);

  if (millis() % 1000 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Username: " + String(username));
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(clock.gettime("d-m-Y, H:i:s")));
  }
}

void executeCommand(String command) {
  command.trim();

  if (command.equals("calc")) {
    openCalculator();
  } else if (command.equals("edit")) {
    openTextEditor();
  } else if (command.equals("settings")) {
    personalizeSettings();
  } else if (command.equals("time")) {
    displayTime();
  } else if (command.equals("help")) {
    lcd.print("calc-calculator");
    delay(3000);    
    lcd.print("edit-text editor");
    delay(9000);
    lcd.clear();    
    lcd.print("settings-персонализация");
    lcd.print("time-time");
    lcd.print("help-command list");
    lcd.print("ver.-версия ос");
    lcd.print("exit-exit");
  } else if (command.equals("ver.")) {
    lcd.print("ver.-1.0");
  } else if (command.equals("exit")) {
    lcd.print("Exiting program.");
    while (true) {
    }
  } else {
    lcd.print("Unknown command");
  }
}

void displayTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ім'я: " + String(username));
  lcd.setCursor(0, 1);
  lcd.print("Текущее время: " + String(clock.gettime("d-m-Y, H:i:s")));
  delay(1000);
}

void personalizeSettings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=== Персоналізація ===");

  lcd.setCursor(0, 1);
  lcd.print("Ваше ім'я: ");
  lcd.print(username);

  lcd.setCursor(0, 2);
  lcd.print("Бажаєте змінити ім'я? (y/n): ");
  while (Serial.available() == 0);
  char response = Serial.read();

  if (response == 'y' || response == 'Y') {
    lcd.setCursor(0, 3);
    lcd.print("Введіть нове ім'я: ");
    while (Serial.available() == 0);
    username = Serial.readStringUntil('\n');
    lcd.print("Ім'я змінено на: " + String(username));
  }

  lcd.setCursor(0, 4);
  lcd.print("Обраний шрифт: ");
  lcd.print(fontType);

  lcd.setCursor(0, 5);
  lcd.print("Бажаєте змінити шрифт? (y/n): ");
  while (Serial.available() == 0);
  response = Serial.read();

  if (response == 'y' || response == 'Y') {
    lcd.setCursor(0, 6);
    lcd.print("Введіть новий шрифт: ");
    while (Serial.available() == 0);
    fontType = Serial.readStringUntil('\n');
    lcd.print("Шрифт змінено на: " + String(fontType));
  }

  lcd.setCursor(0, 7);
  lcd.print("Обраний колір тексту: ");
  lcd.print(textColor);

  lcd.setCursor(0, 8);
  lcd.print("Бажаєте змінити колір тексту? (y/n): ");
  while (Serial.available() == 0);
  response = Serial.read();

  if (response == 'y' || response == 'Y') {
    lcd.setCursor(0, 9);
    lcd.print("Введіть новий колір тексту: ");
    while (Serial.available() == 0);
    textColor = Serial.readStringUntil('\n');
    lcd.print("Колір тексту змінено на: " + String(textColor));
  }

  lcd.setCursor(0, 10);
  lcd.print("=== Персоналізація завершена ===");
}

void openCalculator() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calculator - Type your expression");
  lcd.setCursor(0, 1);
  lcd.print("(e.g., 5 + 3) and press Enter:");

  while (true) {
    if (Serial.available() > 0) {
      String expression = Serial.readStringUntil('\n');
      if (expression.equals("exit")) {
        lcd.clear();
        lcd.print("Exiting calculator.");
        delay(2000);
        break;
      }
      calculate(expression);
    }
  }
}

void calculate(String expression) {
  int spaceIndex = expression.indexOf(' ');

  if (spaceIndex == -1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid expression format.");
    lcd.setCursor(0, 1);
    lcd.print("Please use: <num1> <operator> <num2>");
    delay(3000);
    return;
  }

  String num1String = expression.substring(0, spaceIndex);
  String operatorString = expression.substring(spaceIndex + 1, spaceIndex + 2);
  String num2String = expression.substring(spaceIndex + 3);

  double num1 = num1String.toDouble();
  double num2 = num2String.toDouble();

  double result;
  switch (operatorString.charAt(0)) {
    case '+':
      result = num1 + num2;
      break;
    case '-':
      result = num1 - num2;
      break;
    case '*':
      result = num1 * num2;
      break;
    case '/':
      if (num2 != 0) {
        result = num1 / num2;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error: Division by zero");
        delay(2000);
        return;
      }
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid operator");
      delay(2000);
      return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Result: " + String(result));
  delay(2000);
}

void openTextEditor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Text Editor-Enter text and");
  lcd.setCursor(0, 1);
  lcd.print("press Enter to process commands:");

  while (true) {
    if (Serial.available() > 0) {
      char inputChar = Serial.read();
      if (inputChar == '\n') {
        processTextEditorCommand();
      } else {
        insertChar(inputChar);
      }
    }
  }
}

void insertChar(char ch) {
  if (cursorPosition < MAX_TEXT_SIZE - 1) {
    for (int i = MAX_TEXT_SIZE - 1; i > cursorPosition; --i) {
      textBuffer[i] = textBuffer[i - 1];
    }

    textBuffer[cursorPosition] = ch;
    ++cursorPosition;

    printText();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Text buffer is full.");
    delay(2000);
  }
}

void processTextEditorCommand() {
  textBuffer[cursorPosition] = '\0';

  if (strcmp(textBuffer, "exit") == 0) {
    lcd.clear();
    lcd.print("Exiting text editor.");
    delay(2000);
    while (true) {
    }
  } else if (strcmp(textBuffer, "save") == 0) {
    lcd.clear();
    lcd.print("Saving text to EEPROM...");
    saveTextToEEPROM();
    delay(2000);
  } else {
    // Handle other commands if needed
  }
}

void printText() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(textBuffer);
}

void saveTextToEEPROM() {
  for (int i = 0; i < MAX_TEXT_SIZE; ++i) {
    EEPROM.write(EEPROM_START_ADDRESS + i, textBuffer[i]);
  }
  lcd.clear();
  lcd.print("Text saved to EEPROM.");
  delay(2000);
}

void loadTextFromEEPROM() {
  for (int i = 0; i < MAX_TEXT_SIZE; ++i) {
    textBuffer[i] = EEPROM.read(EEPROM_START_ADDRESS + i);
  }
}
