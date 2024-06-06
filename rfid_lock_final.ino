
 /*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *  
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte accessUID_H[4] = {0x47, 0x81, 0xFD, 0x2C}; //enter your UID here
byte accessUID_K[7] = {0x02, 0x30, 0x54, 0x94, 0x06, 0x80, 0x00}; //enter your UID here

Servo s1;

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Initialize MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);  //Set LCD cursor to upper left corner, column 0, row 0
  lcd.print("scan your card ");
  delay(1000);

//  s1.attach(7);   
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	} 

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
  
 
  
  s1.write(0);
  
	// Dump debug info about the card; PICC_HaltA() is automatically called
	if(mfrc522.uid.uidByte[0] == accessUID_H[0] && mfrc522.uid.uidByte[1] == accessUID_H[1] && mfrc522.uid.uidByte[2] == accessUID_H[2] && mfrc522.uid.uidByte[3] == accessUID_H[3])
	{
  lcd.setCursor(0,1);
  lcd.print("USER 1");
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println("access given");
   
  s1.write(180);
  delay(5000);
  s1.write(0);
	}   
 
  else if(mfrc522.uid.uidByte[0] == accessUID_K[0] && mfrc522.uid.uidByte[1] == accessUID_K[1] && mfrc522.uid.uidByte[2] == accessUID_K[2] && mfrc522.uid.uidByte[3] == accessUID_K[3])
  {
  lcd.setCursor(0,1);
  lcd.print("GRP. NO. 02");
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println("access given");
   delay(5000);
  }
 
  else if(mfrc522.uid.uidByte[0] == accessUID_S[0] && mfrc522.uid.uidByte[1] == accessUID_S[1] && mfrc522.uid.uidByte[2] == accessUID_S[2] && mfrc522.uid.uidByte[3] == accessUID_S[3])
  {
  lcd.setCursor(0,1);
  lcd.print("USER 2");
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println("access given");

  s1.write(180);
  delay(5000);
  s1.write(0);
  }
  
  else
  {
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println("access denied");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("you dont belong");
  lcd.setCursor(5,1);
  lcd.print("here");
  delay(3000);
	}
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WELCOME");
  
  mfrc522.PICC_HaltA();
  
  lcd.clear();
  lcd.setCursor(1,0);  
  lcd.print("scan your card ");

  s1.write(0);
}
