//RFID Scanner - Checks card ID

#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define Speaker 7
#define Switch 3
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String Input = "200";
int Flag = 0;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(Speaker, OUTPUT);
  pinMode(Switch, INPUT);

  Serial.println("Please scan your card\n");

}
void loop() 
{

  Flag = digitalRead(Switch);
  
  if (Serial.available() > 0) {
    
    Input = Serial.readString();
    Serial.println(Input);
    int Frequency = atoi(Input.c_str());
    tone(Speaker, Frequency, 500);
    
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "C3 A8 AA 40" || content.substring(1) == "A9 53 6A C2") && Flag == 0) //change here the UID of the card/cards that you want to give access
  {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(Speaker, 500, 200);
    delay(200);
    tone(Speaker, 200, 400);
    delay(800);
    digitalWrite(LED_R, LOW);
  }
 
  else {
    Serial.println("Authorized access\n");
    digitalWrite(LED_G, HIGH);
    tone(Speaker, 500, 200);
    delay(200);
    tone(Speaker, 800, 200);
    delay(800);
    digitalWrite(LED_G, LOW);
  }
  
} 

//Red Card - C3 A8 AA 40
//Green Card - D4 5A DA 2A
//Blue Card - 12 A8 1F 34
//Yellow Card - F4 77 85 2A
//Double Green Card - E4 89 B8 2A
//Tag - A9 53 6A C2
