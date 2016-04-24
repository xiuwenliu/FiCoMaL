/*
   Web client sketch for IDE v1.0.1 and w5100/w5200
   Uses POST method.
   Posted November 2012 by SurferTim
*/

#include <SPI.h>
#include <Ethernet.h>

#include <SPI.h>                          //library for SPI interface
#include <MFRC522.h>                  //library for RFID MRFC522 module
#include <HX711.h>
#include <Servo.h> 

Servo myservo;  // create servo object to control a servo
HX711 hx(22, 24);

#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Number of known default keys (hard-coded)
// NOTE: Synchronize the NR_KNOWN_KEYS define with the defaultKeys[] array
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};


byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Change to your server domain
char serverName[] = "iopet543.appspot.com";

// change to your server's port
int serverPort = 80;

// change to the page on that server
char pageName[] = "/";

EthernetClient client;
int totalCount = 0; 
// insure params is big enough to hold your variables
char params[256];
char res[4];

// set this to the number of milliseconds delay
// this is 30 seconds
#define delayMillis 1000UL

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

  double weight = 100;

//String ww="";


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


void setup() {
  Serial.begin(9600);

  pinMode(7, INPUT);

  // disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
 // for(int i =0;i<4;i++)
   // res[i] = 'g';

//  Serial.print(F("Starting ethernet..."));
//  if(!Ethernet.begin(mac)) Serial.println(F("failed"));
//  else Serial.println(Ethernet.localIP());

  delay(2000);
  Serial.println(F("Ready"));


  

  //mfrc522.PCD_Init();         // Init MFRC522 card
    //Serial.println(F("Try the most used default keys to print block 0 of a MIFARE PICC."));
     //myservo.attach(8);
     //feed(0);
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

boolean try_key(MFRC522::MIFARE_Key *key)
{
    boolean result = false;
    byte buffer[18];
    byte block = 0;
    byte status;
    
    // Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        // Serial.print(F("PCD_Authenticate() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        // Serial.print(F("MIFARE_Read() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("Success with key:"));
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
        digitalWrite(13, HIGH);
    }
    Serial.println();

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}

void feed(int flag)
{
   //static int flag=0;
     if(!flag)
  {   
        myservo.write(170); 
        flag= 1;
        Serial.print(F("180"));
        delay(200);  
  }
       else
{
       myservo.write(0); 
       flag= 0;
       Serial.print(F("0"));
       delay(200);  
       }
}

void rfid()
{
   // double sum = 0;
 // for (int i = 0; i < 10; i++)
  //  sum += hx.read();

 //Serial.println("123");
  
  weight = hx.read();
  weight/=100;
  Serial.println(weight);
  
    digitalWrite(13, LOW);
  // put your main code here, to run repeatedly:
   // Look for new cards
    
    
    /*if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
        
   
        
        
    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
    // Try the known default keys
    MFRC522::MIFARE_Key key;
    for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
        // Try the key
        if (try_key(&key)) {
            // Found and reported on the key and block,
            // no need to try other keys for this PICC
            break;
        }
    }

    */
  }


  void button()
  {
    
     // read the pushbutton input pin:
  buttonState = digitalRead(7);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      //Serial.println("on");
      //Serial.print("number of button pushes:  ");
      //Serial.println(buttonPushCounter);
      Serial.println("send");
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      //Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
  if (buttonPushCounter % 4 == 0) {
    //digitalWrite(ledPin, HIGH);
  } else {
    //digitalWrite(ledPin, LOW);
  }
    
    
    
    }

void loop()
{
  // If using a static IP, comment out the next line
 // Ethernet.maintain();

  //feed(0);

  /*thisMillis = millis();

  if(thisMillis - lastMillis > delayMillis)
  {
    lastMillis = thisMillis;

    // params must be url encoded.
    sprintf(params,"device=feeter&name=11&weight=%dg&petID=1234",(int)weight);     
    if(!postPage(serverName,serverPort,pageName,params)) Serial.print(F("Fail "));
    else Serial.print(F("Pass \n"));
  //  totalCount++;
   // Serial.println(totalCount,DEC);
  }    */
  rfid();
    button();
}


byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData)
{
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      res[3] = inChar;
      
      if(res[0]=='t' && res[1]=='r' && res[2]=='u' && res[3]=='e')
      {
          Serial.println(res[0]);
          feed(1);
          client.stop();
          break;
          
      }
       else
     {
       res[0] = res[1];
       res[1] = res[2];
       res[2] = res[3];
     }
       
      
      //Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 1000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}
