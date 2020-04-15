//Use Arduino nano with Atmegs386AP with OLD BOOTLOADER

#include "Wire.h"
#include "LiquidCrystal.h"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}
void setup()
{
  lcd.begin(16, 2);

  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //אחת ושש דקות בלילה בתאריך חמישה ביולי יום שני נכתב כך -
  //30,6,1,2,05,07,15
  //מקלידים שניות,דקות,שעות בפורמט 24 שעות,מספר יום בשבוע למשל 3 יום שלישי,תאריך בחודש, חודש ושנה בשתי   ספרות. לא מוסיפים אפסים עם הערך קטן מ 10
  // מבטלים את שני הלוכסנים ומורידים את התוכנה לבקר. אחרי שהבקר עדכן את השעוןובשעון ישנה סוללה לגיבוי הזכרון,עוצרים את התוכנה
  //ומוסיפים את 56 הלוכסנים בכדי שבהורדת התוכנה בפעם הבאה לא יעודכן השעון בשעה שגויה. הסוללת מבטיחת שהשעון ימשיך לפעול גם כשאינו מחובר לבקר.

//setDS3231time(30,26,11,6,27,3,20);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  lcd.setCursor(0, 0);
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
                 &year);
  // send it to the serial monitor
  if (hour < 10)
  {
    Serial.print("0");
    lcd.print("0");
  }


  Serial.print(hour, DEC);
  lcd.print(hour);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  lcd.print(":");

  if (minute < 10)
  {
    Serial.print("0");
    lcd.print("0");

  }
  Serial.print(minute, DEC);
  lcd.print(minute);
  Serial.print(":");
  //  lcd.print(":");

  if (second < 10)
  {
    Serial.print("0");
    //    lcd.print("0");

  }
  Serial.print(second, DEC);
  //   lcd.print(second);

  Serial.print(" ");
  lcd.print("   ");

  if (dayOfMonth < 10)
  {
    lcd.print("0");
  }
  Serial.print(dayOfMonth, DEC);
  lcd.print(dayOfMonth);

  Serial.print("/");
  lcd.print("/");

  if (month < 10)
  {
    lcd.print("0");
  }
  Serial.print(month, DEC);
  lcd.print(month);

  Serial.print("/");
  lcd.print("/");

  Serial.print(year, DEC);
  lcd.print(year);

  lcd.setCursor(0, 1);
  Serial.print(" Day of week: ");
  // lcd.print("day of week: ");

  switch (dayOfWeek) {
    case 1:
      Serial.println("Sunday");
      lcd.print("Sunday   ");
      break;
    case 2:
      Serial.println("Monday");
      lcd.print("Monday   ");
      break;
    case 3:
      Serial.println("Tuesday");
      lcd.print("Tuesday   ");
      break;
    case 4:
      Serial.println("Wednesday");
      lcd.print("Wednesday   ");
      break;
    case 5:
      Serial.println("Thursday");
      lcd.print("Thursday   ");
      break;
    case 6:
      Serial.println("Friday");
      lcd.print("Friday   ");
      break;
    case 7:
      Serial.println("Saturday");
      lcd.print("Saturday  ");
      break;
  }
  lcd.setCursor(14, 1);
  if (second < 10)
  {

    lcd.print("0");

  }

  lcd.print(second);
}
void loop()
{
  // lcd.clear();
  //lcd.setCursor(0,0);
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
