/*
 author : hymen81
 
 Note : This code was depend on HX711 library
*/

//SPI lib
#include <SPI.h>
//Weight sensor
#include <HX711.h>

//Weight sensor lib pindefine
HX711 hx(22, 24);

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;
double weight = 0;

//Button cpoy from Google
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


void setup() {
  Serial.begin(9600);
  //Button pin
  pinMode(7, INPUT);
  // disable SD SPI
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(2000);
}

//Get weight from sensor
void GetWeight()
{
  weight = hx.read();
  weight /= 100;
  Serial.println(weight);
}

//Get Button is down or not
void GetButtonStates()
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
      //Using string to notify tool when button down
      Serial.println("send");
    } else {

    }
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
}

void loop()
{
  GetWeight();
  GetButtonStates();
}


