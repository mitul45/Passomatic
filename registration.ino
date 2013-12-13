
#include <SoftwareSerial.h>
#include <EEPROM.h>

int rfidRxPin = 8;
int rfidTxPin = 9;
SoftwareSerial rfid(rfidRxPin,rfidTxPin);  // to talk with RFID reader.

int tagIndex = 0;
int passwordIndex = 12;
int count = 0;                    // A variable to count the length of the Tag DATA
int IDLength = 12;
char tagID[12];

void setup()
{
  for (int i = 0; i < 1024; i++) {
    EEPROM.write(i,0);              //Clear EEPROM old data
  }
  Serial.begin(9600);             // Initialize Serial Communication - Both with the RFID reader & the Serial Monitor
  rfid.begin(9600);
}

void loop()
{
  tagIndex = 0;
  passwordIndex = 12;
  count = 0;
  char c;
  if(rfid.available())                     // Check if there is Incoming Data in the Serial Buffer. This is data coming from the RFID reader
  {
    while(rfid.available() && count < 12)  // Keep reading Byte by Byte from the Buffer till the Buffer is empty
    {
      tagID[count] = rfid.read();            // Read 1 Byte of data and store it in a character variable
      EEPROM.write(tagIndex++,tagID[count]);
      count++;                             // Increment the Byte count after every Byte Read
      delay(5);                            // A small delay - Removing this might make the Program run faster and not respond properly as data from the reader could be slow
    }
    Serial.write('R');                     // inform PC back that you have read the value.
  }

  if(Serial.available()) {
    while(Serial.available()){   
      c = Serial.read();
      EEPROM.write(passwordIndex++, c);          // store the password in ROM.
      delay(5);
    }
  }
}
