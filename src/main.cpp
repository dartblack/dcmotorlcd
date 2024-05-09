#include <Arduino.h>
#include <LiquidCrystal.h>
#include "GyverMotor2.h"

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
GMotor2<DRIVER2WIRE> motor(2, 3);

int lcd_key = 0;
int adc_key_in = 0;

bool motor_reverse = false;
int motor_speed = 0;
unsigned long keyTime = 0;

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1500)
    return btnNONE;
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 195)
    return btnUP;
  if (adc_key_in < 380)
    return btnDOWN;
  if (adc_key_in < 500)
    return btnLEFT;
  if (adc_key_in < 730)
    return btnSELECT;
  return btnNONE;
}

void printConfig()
{
  lcd.home();
  lcd.print("DC SPD: ");
  lcd.setCursor(7, 0);
  lcd.print("    ");
  lcd.setCursor(7, 0);
  lcd.print(motor_speed);
  lcd.setCursor(3, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("DR: ");
  if (motor_reverse)
  {
    lcd.print("DOWN");
  }
  else
  {
    lcd.print("UP");
  }
}

void setConfig(int btn)
{
  switch (btn)
  {
  case btnNONE:
    return;
  case btnRIGHT:
    if (motor_speed < 255)
    {
      motor_speed++;
      motor.setSpeed(motor_speed);
    }
    break;

  case btnLEFT:
    if (motor_speed > 0)
    {
      motor_speed--;
      motor.setSpeed(motor_speed);
    }
    break;

  case btnUP:
    motor_reverse = false;
    motor.brake();
    motor.reverse(false);
    motor.setSpeed(motor_speed);
    break;

  case btnDOWN:
    motor_reverse = true;
    motor.brake();
    motor.reverse(true);
    motor.setSpeed(motor_speed);
    break;

  case btnSELECT:
    break;
  }

  printConfig();
}

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  motor.setDeadtime(100);
  motor.setSpeed(0);
  keyTime = millis();

  printConfig();
}

void loop()
{
  if (keyTime + 100 < millis())
  {
    lcd_key = read_LCD_buttons();
    setConfig(lcd_key);
    keyTime = millis();
  }
}
