/*
  Nathan Seidle
  February 20th, 2025

  If B goes low then we know we have movement.
  If A is low when B goes low then we know the movement is reverse.
*/

//#define CONTROLLER_ESP32
#define CONTROLLER_ATMEGA

//Pins
#ifdef CONTROLLER_ATMEGA
int encoder_PinA = 3;
int encoder_PinB = 2;
int encoder_Switch = 4;
#endif

#ifdef CONTROLLER_ESP32
int encoder_PinA = 4;
int encoder_PinB = 13;
int encoder_Switch = 26;
#endif

volatile int16_t encoderCount = 0; //+ for forward, - for reverse
volatile bool encoderChange = false; // Allows for print when encoder has changed

#ifdef CONTROLLER_ESP32
void IRAM_ATTR updateEncoder() //ESP32
#endif

#ifdef CONTROLLER_ATMEGA
void updateEncoder() //ATmega328
#endif
{
  if (digitalRead(encoder_PinA) == HIGH)
    encoderCount++;
  else
    encoderCount--;
  encoderChange = true;
}
void setup()
{
  Serial.begin(115200);

#ifdef CONTROLLER_ATMEGA
  pinMode(encoder_PinA, INPUT_PULLUP);
  pinMode(encoder_PinB, INPUT_PULLUP);
  pinMode(encoder_Switch, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoder_PinB), updateEncoder, FALLING);
#endif


#ifdef CONTROLLER_ESP32
  pinMode(encoder_PinA, INPUT);
  pinMode(encoder_PinB, INPUT);
  pinMode(encoder_Switch, INPUT);

  attachInterrupt(encoder_PinB, updateEncoder, FALLING);

  delay(250);
#endif

  Serial.println("Roller Encoder");

}

void loop()
{
  if (digitalRead(encoder_Switch) == LOW)
  {
    Serial.println("Button");
    delay(25); //Debounce
    if (digitalRead(encoder_Switch) == LOW)
    {
      encoderCount = 0;
      Serial.print("Encoder count:");
      Serial.println(encoderCount);
      while (digitalRead(encoder_Switch) == LOW)
        ; //Wait for user to stop pressing
    }
  }

  if (encoderChange)
  {
    encoderChange = false;
    Serial.print("Encoder count:");
    Serial.println(encoderCount);
  }
}
