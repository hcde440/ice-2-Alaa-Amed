// Sketch with two feeds: a feed for button press and a feed for photoresistors 

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin 5
#define BUTTON_PIN 5
// analog pin 0
#define PHOTOCELL_PIN A0

// button state
bool current1 = false;
bool last1 = false;

// Photoresistor state
int current = 0;
int last = -1;

// set up the 'analog' feed
AdafruitIO_Feed *analog = io.feed("analog");
// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("digital");

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // start the serial connection
  Serial.begin(115200);
  Serial.print("This board is running: ");
  Serial.println(F(__FILE__));
  Serial.print("Compiled: ");
  Serial.println(F(__DATE__ " " __TIME__));
   
  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  // grab the current state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  if(digitalRead(BUTTON_PIN) == LOW)
    current1 = true;
   else
    current1 = false;
    
  // grab the current state of the photocell
  current = analogRead(PHOTOCELL_PIN);
 
  // return if the values of the two inputs haven't changed
  if(current == last && current1 == last1) {
    return;
  }

  // save the current state to the analog feed
  Serial.print("sending -> ");
  Serial.println(current);
  analog->save(current);
  
  // save the current state to the 'digital' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(current1);
  digital->save(current1);
 
  // store last photocell state
  last = current;
  // store last button state
  last1 = current1;
 
  // wait one second (1000 milliseconds == 1 second)
  delay(1000);
    

}
