#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>    //Libraries required for the Serial communication and DHT
//#include <TinyGPS++.h>



#define BT_Serial Serial1

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SS_PIN 49
#define RST_PIN 48
#define esp32_Serial Serial3
MFRC522 rfid(SS_PIN, RST_PIN);
// Init array that will store new NUID
byte nuidPICC[4];
String RFID_String="";
String character="";
String App_Data="";
String Tracking_Data="";
String ESP_Data="";



void setup() {
 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //gps_Serial.begin(9600);
  BT_Serial.begin(9600);
  esp32_Serial.begin(9600);

}

void loop() {

  RFID_String=readRFID();
  
  if(RFID_String!=""){
    //Write to BT
    esp32_Serial.print(RFID_String);
    //esp32_Serial.println();
    Serial.println(RFID_String);
  }
  RFID_String="";
/*
  while(esp32_Serial.available()>0){
 
    character = (char)esp32_Serial.read();
    delay(1); //wait for the next byte, if after this nothing has arrived it means the text was not part of the same stream entered by the user
    ESP_Data+=character;
  }*/
  
  ESP_Data="";
  App_Data="";
  Tracking_Data="";
  

}

String readRFID(){

  String RFID_string_local="";
  // Look for new card
  if ( ! rfid.PICC_IsNewCardPresent()) 
    return RFID_string_local;
  
    // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
    return RFID_string_local;
  
  /*if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] )*/ {
    //Serial.println(F("A new card has been detected."));
  
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
   
  //Serial.print(F("RFID tag in HEX: "));
  RFID_string_local=printDec(rfid.uid.uidByte, rfid.uid.size);
  //Serial.println();
  }
   
  // Halt PICC
  rfid.PICC_HaltA();
  
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  
  return RFID_string_local;

}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
String printDec(byte *buffer, byte bufferSize){
  String RFID_string_local;
  for (byte i = 0; i < bufferSize; i++){
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], DEC);
    
    RFID_string_local.concat(String(buffer[i] < 0x10 ? " 0" : ""));
    RFID_string_local.concat(String(buffer[i], HEX));
  }
  RFID_string_local.toUpperCase();
  
  if(RFID_string_local.length()==8){
    return RFID_string_local;
  }  
  else
    RFID_string_local="";
}
