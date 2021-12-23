#include <Adafruit_Fingerprint.h>
int getFingerprintIDez();
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//For finding sensor
void setup(){
  Serial.begin(9600);
  Serial.println("Unlock the solenoid");
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor is found");
  } else {
    Serial.println("Fingerprint sensor could not found");
  while (1);
  }
  Serial.println("Waiting for valid finger…");
}
void loop(){                     
  getFingerprintIDez();
  delay(50);            
}
//function that is responsible for scanning finger.
uint8_t getFingerprintID() {
  
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
    Serial.println("Print converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }
  //searchimg fimgerprint in flash memory of sensor
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint is matched!");
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR){
    Serial.println("Communication error");
      return p;
      
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Fingerprint did not matched");
      return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  //printing ID and confidence
   Serial.print("Found ID #"); 
   Serial.print(finger.fingerID);
   Serial.print("with confidence of "); 
   Serial.println(finger.confidence);
}

  int getFingerprintIDez() {
    uint8_t p = finger.getImage();
  
    if (p != FINGERPRINT_OK)  return -1;
      p = finger.image2Tz();

    if (p != FINGERPRINT_OK)  return -1;
      p = finger.fingerFastSearch();
  
    if (p != FINGERPRINT_OK)  return -1;
  
    Serial.print("Found in ID #"); 
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); 
    Serial.println(finger.confidence);
    //sending signal to pin 13 to open solenoid lock
    digitalWrite(13, HIGH);
    delay(3000);
    digitalWrite(13, LOW);
    return finger.fingerID;
  }
