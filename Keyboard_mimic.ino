
#include <EEPROM.h>
#include <SoftwareSerial.h>
int rfidRxPin = 8;
int rfidTxPin = 9;
SoftwareSerial rfid(rfidRxPin,rfidTxPin);  // to talk with RFID reader.

int count = 0;                    // A variable to count the length of the Tag DATA
int IDLength = 12;
char tagID[12];
boolean flag = true;
char userTagID[12];
int isWindows = 0;

uint8_t buf[8] = {0};            // current keystroke to be sent
uint8_t ascii[255][2] = {{}}; 	 // Keyboard report buffer
 
String password = "";   // password string
int passLength = 0;

void setup()
{
  setupMap();
  readROM();
  Serial.begin(9600);             // Initialize Serial Communication - Both with the RFID reader & the Serial Monitor
  rfid.begin(9600);
}

void loop()
{
  
  flag = true;                            // reset variables.
  count = 0;
  
  if(rfid.available())                     // Check if there is Incoming Data in the Serial Buffer. This is data coming from the RFID reader
  {
    while(rfid.available() && count < 12)  // Keep reading Byte by Byte from the Buffer till the Buffer is empty
    {
      tagID[count] = rfid.read();            // Read 1 Byte of data and store it in a character variable
      count++;                             // Increment the Byte count after every Byte Read
      delay(5);                            // A small delay - Removing this might make the Program run faster and not respond properly as data from the reader could be slow
    }
    
    count = 0;
    while(count < 12) {
      if(tagID[count] != userTagID[count]) {
        flag = false;                      // If card swiped is not same as user's card.
        break;
      }
      count++;
    }
    
    if(flag) {
      //Serial.print("Authentication Successful.");
      enterPassword();                    // authentication successful. Enter password.
    }
    
    for(int i = 0; i < 12; i++) {
      tagID[i] = 'F';
    }
  }
}

void readROM() {
  int ROMIndex = 0;
  int cnt = 0;
  char c = 0;
  for (int i = 0; i < 12; i++) {
    userTagID[i] = EEPROM.read(ROMIndex++);
  }
  char OSCode = EEPROM.read(ROMIndex++);
  if(OSCode == '2')
    isWindows = 1;
  while(c != ' ') {
    c = EEPROM.read(ROMIndex++);
    password += c;
    cnt++;
  }
  passLength = cnt;
}

void enterPassword() {
  
  if(isWindows == 1) {
    buf[0] = 5;          // CTRL + ALT
    buf[2] = 76;         // DLT
    Serial.write(buf,8);
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8);        //release key
  }

  for(int i = 0; i < passLength; i++) {
    enterChar(password[i]);
  }
  delay(10);
  
  enterChar(13);          //Enter.
}

void enterChar(char c) {
  buf[0] = ascii[c][0];        // set flags
  buf[2] = ascii[c][1];        // set char
  Serial.write(buf, 8);        // press key
  delay(10);
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);        //release key
  delay(10);
}

void setupMap(){
  int code = 4;
  for (int i=97; i<124; i++) {
    ascii[i][0] = 0;
    ascii[i][1] = code;
    
    ascii[i-32][0] = 2;
    ascii[i-32][1] = code++;
  }
  
  ascii[96][0] = 0; ascii[96][1] = 50;
  ascii[126][0] = 2; ascii[126][1] = 50;
  
  code = 30;
  for (int i=49; i<58; i++ ) {
    ascii[i][0] = 0;
    ascii[i][1] = code++;
  }
  
  ascii[48][0] = 0;
  ascii[48][1] = 39;
  
  ascii[33][0] = 2; ascii[33][1] = 30 ;
  ascii[64][0] = 2; ascii[64][1] = 31;
  ascii[35][0] = 2; ascii[35][1] = 32;
  ascii[36][0] = 2; ascii[36][1] = 33;
  ascii[37][0] = 2; ascii[37][1] = 34;
  ascii[94][0] = 2; ascii[94][1] = 35;
  ascii[38][0] = 2; ascii[38][1] = 36;
  ascii[42][0] = 2; ascii[42][1] = 37;
  ascii[40][0] = 2; ascii[41][1] = 38;
  ascii[42][0] = 2; ascii[42][1] = 39;
  ascii[45][0] = 0; ascii[45][1] = 45;
  ascii[95][0] = 2; ascii[95][1] = 45;
  ascii[61][0] = 0; ascii[61][1] = 46;
  ascii[43][0] = 0; ascii[43][1] = 46;
  ascii[91][0] = 0; ascii[91][1] = 47;
  ascii[123][0] = 2; ascii[123][1] = 47;
  ascii[93][0] = 0; ascii[93][1] = 48;
  ascii[125][0] = 2; ascii[125][1] = 48;
  ascii[92][0] = 0; ascii[92][1] = 49;
  ascii[124][0] = 2; ascii[124][1] = 49;
  ascii[59][0] = 0; ascii[59][1] = 51;
  ascii[58][0] = 2; ascii[58][1] = 51;
  ascii[39][0] = 0; ascii[39][1] = 52;
  ascii[34][0] = 2; ascii[34][1] = 52;
  ascii[44][0] = 0; ascii[44][1] = 54;
  ascii[60][0] = 2; ascii[60][1] = 54;
  ascii[46][0] = 0; ascii[46][1] = 55;
  ascii[62][0] = 2; ascii[62][1] = 55;
  ascii[47][0] = 0; ascii[47][1] = 56;
  ascii[63][0] = 2; ascii[63][1] = 56;
  ascii[127][0] = 0; ascii[127][1] = 42;
  ascii[13][0] = 0; ascii[13][1] = 40; 
}