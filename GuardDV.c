// RFID Variables
const int tagLength = 13;
char offenderID[tagLength] = {"18008386D6CB"};
char scannedTag[tagLength];

// SMS Variables
int warningCount = 0;

// Customizable Warning messages for first to third occurance
char firstMessage[] = {"This is the first warning."};
char secondMessage[] = {"This is the second warning"};
char thirdMessage[] = {"This is the third warning"};

// Phone number must include the country code. Use '+1' for US and PR.
char phoneNumber[] = {"+17871234567"};

// Enum for warning ocurrances
enum WarningType {
  FIRST = 1,
  SECOND,
  THIRD
};

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
      
      if (readByte != 2 && readByte != 13 && readByte != 10 && readByte != 3) {
        scannedTag[i] = readByte;
        i++;
      }
      
      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }
    }
    
    if (strlen(scannedTag)== 0) {
      return;
      
    }  else if (isOffenderTag(scannedTag)) {
      
      warningCount++;
      
      switch (warningCount) {
        case 1:
          sendSMS(WarningType::FIRST);
          break;
          
        case 2:
          sendSMS(WarningType::SECOND);
          break;
          
        case 3:
          sendSMS(WarningType::THIRD);
          break;
          
        default:
          sendSMS(WarningType::THIRD);
          break;
      }
    }
  }
}

/// Checks is the scanned tag matches the offender tag stored in offenderID
boolean isOffenderTag(char scannedTag[]) {
  for (int i = 0; i < tagLength; i++) {
    if (scannedTag[i] != offenderID[i]) {
      return false;
    }
  }
  return true;
}

/// Powers on the GSM module
void powerGSMModule() {
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(9,LOW);
  delay(10000);
}

void sendSMS(WarningType warning) {
  Serial.println("AT+CMGF=1\r");
  delay(500);
  
  Serial.print("AT+CMGS=\"");
  Serial.print(phoneNumber);
  Serial.print("\"\r\n");
  delay(3000);
  
  switch (warning) {
    case FIRST:
      Serial.println(firstMessage);
      break;
      
    case SECOND:
      Serial.println(secondMessage);
      break;
      
    case THIRD:
      Serial.println(thirdMessage);
      break;
  }
  
  Serial.println((char)26);
  delay(100);
  Serial.println();
}
