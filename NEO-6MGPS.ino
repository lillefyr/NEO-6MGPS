
#include <SoftwareSerial.h>
#include <TinyGPS.h>

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

SoftwareSerial mySerial(12, 34);
TinyGPS gps;

void gpsdump(TinyGPS &gps);

void getGPSData() {
  unsigned long start = millis();
  Serial.println(F("getGPSData"));
  do
  {
    while (mySerial.available()) { gps.encode(mySerial.read()); }
  } while (millis() - start < 1000);
}

void getData(){
  uint8_t mydata[20];
  unsigned long int age, hdop, cnt;
  int year;
  float alt, lat, lon;
  byte month, day, hour, minute, second, hundredths;
  
  getGPSData();
  gps.f_get_position(&lat, &lon, &age);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  hdop = gps.hdop();
  alt = gps.f_altitude();
  Serial.print(F("Lat:")); Serial.println(lat);
  Serial.print(F("Lon:")); Serial.println(lon);
  Serial.print(F("Alt:")); Serial.println(alt);
  Serial.println(year);
  Serial.println(month);
  Serial.println(day);
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(second);
  delay(2000);
}

//#define DUMPDATA 1
#ifdef DUMPDATA
void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
     Serial.print(F('-'));
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print(F("."));

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}

void gpsdump( float flat, float flon, float falt, float fcourse, float fkmph, unsigned long age, unsigned long datetime, unsigned long hdop )
{
  unsigned long chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

//  gps.get_position(&lat, &lon, &age);
//  Serial.print(F("Lat/Long(10^-5 deg): ")); Serial.print(lat); Serial.print(F(", ")); Serial.print(lon);
//  Serial.print(F(" Fix age: ")); Serial.print(age); Serial.println(F("ms."));

  // On Arduino, GPS characters may be lost during lengthy Serial.print()
  // On Teensy, Serial prints to USB, which has large output buffering and
  //   runs very fast, so it's not necessary to worry about missing 4800
  //   baud GPS characters.

  Serial.print(F("Lat/Long(float): ")); printFloat(flat, 5);
  Serial.print(F(", ")); printFloat(flon, 5);
//  Serial.print(F(" Fix age: ")); Serial.print(age); Serial.print(F("ms. "));
  Serial.print(F(" (hdop): ")); printFloat(hdop); Serial.println();
  //Serial.print(F("Date(ddmmyy): ")); Serial.print(date); 
  //Serial.print(F(" Time(hhmmsscc): ")); 
  Serial.println(datetime);

/*
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);

  Serial.print(F("Date: ")); Serial.print(static_cast<int>(month)); Serial.print(F("/")); Serial.print(static_cast<int>(day)); Serial.print(F("/")); Serial.print(year);
  Serial.print(F("  Time: ")); Serial.print(static_cast<int>(hour+8));  Serial.print(F(":")); 
    Serial.print(static_cast<int>(minute)); Serial.print(F(":")); Serial.print(static_cast<int>(second));
    Serial.print(F(".")); Serial.print(static_cast<int>(hundredths)); Serial.print(F(" UTC +01:00 Switzerland"));
    */
    /*
  Serial.print(F("  Fix age: "));  Serial.print(age); Serial.println(F("ms."));
  */

 /*
  Serial.print(F("Alt(cm): ")); Serial.print(gps.altitude());
  Serial.print(F(" Course(10^-2 deg): ")); Serial.print(gps.course());
  Serial.print(F(" Speed(10^-2 knots): ")); Serial.println(gps.speed());
  */
  Serial.print(F("Alt(float): ")); printFloat(falt); 
/*  
  Serial.print(F(" Course(float): ")); printFloat(gps.f_course()); Serial.println();
  Serial.print(F("Speed(knots): ")); printFloat(gps.f_speed_knots()); 
  Serial.print(F(" (mph): ")); printFloat(gps.f_speed_mph());
  Serial.print(F(" (mps): ")); printFloat(gps.f_speed_mps()); 
  Serial.print(F(" (kmph): ")); printFloat(gps.f_speed_kmph()); Serial.println();

  gps.stats(&chars, &sentences, &failed);
  Serial.print(F("Stats: characters: ")); Serial.print(chars); 
  Serial.print(F(" sentences: ")); Serial.print(sentences); 
  Serial.print(F(" failed checksum: ")); Serial.println(failed);
  */
}
#endif

void setup() {
  Serial.begin(115200);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println(F("simplenode1"));
  Serial.println(F("uBlox Neo 6M"));
  Serial.print(F("Sizeof(gpsobject) = "));
  Serial.println(sizeof(TinyGPS));
  
  // initialize digital pin LED_BUILTIN as an output and turn off led.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  getData();
  delay(2000);
}
