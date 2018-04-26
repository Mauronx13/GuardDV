const int tagLength = 13;
char offenderID[tagLength] = {"18008386D6CB"};
char scanTag[tagLength];

void setup() {
  powerGSMModule();
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  int i = 0;
  int readByte;
  boolean tag = false;
  
  if (Serial1.available() == 16) {
    tag = true;
  }
  
  if (tag == true) {
    while (Serial1.available()) {
      
      //Take each byte out of the serial buffer, one at a time
      readByte = Serial1.read();
      
      /* This will skip the first byte (2, STX, start of text) and the last three,
       ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of
       text, leaving only the unique part of the tag string. It puts the byte into
       the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte != 13 && readByte != 10 && readByte != 3) {
        scanTag[i] = readByte;
        i++;
      }
      
      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }
    }
    
    if (strlen(scanTag)== 0) {
      return;
    }
    
    else if (isOffenderTag(scanTag, offenderID) == true) {
      sendSMS();
    } else {
      //Serial.println("false");
    }
  }
}

boolean isOffenderTag(char nTag[], char oTag[]) {
  for (int i = 0; i < tagLength; i++) {
    if (nTag[i] != oTag[i]) {
      return false;
    }
  }
  return true;
}

void powerGSMModule() {
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(9,LOW);
  delay(10000);
}

void sendSMS() {
  Serial.println("AT+CMGF=1\r");
  delay(2000);
  Serial.println("AT+CMGS=\"+17876885771\"\r");
  delay(10000);
  Serial.println("This is the second message!");
  delay(100);
  Serial.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100);
  Serial.println();
}
