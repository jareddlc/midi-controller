/*
 MIDI Controller
 Created by: Jared De La Cruz 6/25/2012
*/

const int analogInPin = A0;  // Potentiometer Analog Pin.
const int buttonPin = 7;     // Button Digital Pin.

int analogValue = 0;         // Value of potentiometer.
int outputValue = 0;         // Value of potentiometer (0-127).

int note = 0;
int lastNotePlayed = 0;     // note turned on when you press the switch

int buttonState = 0;        // Button state.
int lastButtonState = 0;    // Button last state.
int currentButtonState = 0; // Button current state.



// MIDI Messages.          http://www.blitter.com/~russtopia/MIDI/~jglatt/tech/midispec.htm
// Note On/Off:            message(status on/off, pitch/note, velocity).
// Controller:             message(states, controller #0-127, value).
// Program Change:         message(status, program #0-127).
// Pitch Wheel:            message(status, data 0-6 14-bit value, data 0-6 14-bit value).
const int middleC = 60;    // middle c = 0x3C 
int noteOn = 0x90;         // 0x90 to 0x9F where the low nibble is the MIDI channel.
int noteOff = 0x80;        // 0x80 to 0x8F where the low nibble is the MIDI channel. 
int controller = 0xb0;     // 0xB0 to 0xBF where the low nibble is the MIDI channel. 
int velocityOn = 0x40;     // Velocity set to 64.
int velocityOff = 0x00;    // Velocity set to 0.    
int modWheel = 0x01;       // Modwheel Channel 
void setup() 
{
  // Set MIDI baud rate.
  Serial.begin(31250);
  // Initialize the buttonPin as an input.
  pinMode(buttonPin, INPUT);
  // Setting note as middle c.
  note = middleC;
}

void loop() 
{
  // Potentiameter range from 0-1023.
  analogValue = analogRead(analogInPin);
  // Convert range to 0-127.
  outputValue = analogValue/8;
  sendMod(controller, modWheel, outputValue);
  
  // Setting current button state.
  currentButtonState = digitalRead(buttonPin);
  
  // Checking to see if button is pressed.
  if(currentButtonState == 1)
  {
    // 
    if(lastButtonState == 0)
    {
      // Send note.
      sendNote(0x90, note, 0x40);
      // Saving last note played, to send off signal later.
      lastNotePlayed = note;
    }
  }
  // The switch has not been pressed.
  else
  {
    // If button is lifted.
    if(lastButtonState == 1)
    {
      sendNote(0x90, lastNotePlayed, 0x00);
    }
  }
  // Set last state to current state, should be 0.
  lastButtonState = currentButtonState;
}

void sendNote(int message, int pitch, int velocity) 
{
  Serial.write(message);
  Serial.write(pitch);
  Serial.write(velocity);
}

void sendMod(int message, int controller, int value)
{
  Serial.write(message);
  Serial.write(controller);
  Serial.write(value);
}
