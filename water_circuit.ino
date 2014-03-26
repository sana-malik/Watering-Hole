/* This includes code from the play_melody example <2005 D. Cuartielles for K3> and 
 Alpesh Vitha's Makey Makey example <http://blog.arduino.cc/2013/03/23/how-to-make-makey-makey/#.UytvlFFdWYk> 
 */
#include <MovingAvarageFilter.h>

// SETUP ============================================
MovingAvarageFilter movingAvarageFilter(20);
boolean complete = false;
int DEBUG = 1;
int speakerOut = 9;
int flowerOut = 11;

// TONES  ==========================================
// Start by defining the relationship between 
//       note, period, &  frequency. 
int temp = 0;
#define  c     955    // 1047 Hz
#define  d     851    // 1175 Hz 
#define  e     758    // 1319 Hz 
#define  f     715    // 1397 Hz 
#define  g     638    // 1568 Hz 
#define  a     568    // 1760 Hz 
#define  b     506    // 1976 Hz 
#define  C     477    // 2093 Hz 
// Define a special note, 'R', to represent a rest
#define  R     0

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
int melody[] = { 
  c, C, c, C, c, C, g, R, b };
int beats[]  = { 
  2, 2, 2, 2, 2, 2, 2, 2, 2 }; 
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.
// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 0;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}

// PLAY ENTIRE SONG  ==============================================
void playSong() {
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<MAX_COUNT; i++) {
    tone_ = melody[i];
    beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone(); 
    // A pause between notes...
    delayMicroseconds(pause);
  }
}

void setup() {
  pinMode(speakerOut, OUTPUT);
  pinMode(flowerOut, OUTPUT);
  if (DEBUG) {
    Serial.begin(115200);     
  }
}


void loop() {        
  // declare input and output variables
  float input =  analogRead(0); // without a real input, looking at the step respons (input at unity, 1)
  float output = 0;
  output = movingAvarageFilter.process(input);

  if (DEBUG) {
    Serial.print("I = ");
    Serial.print(input);
    Serial.print(", O = ");
    Serial.println(output);
  }

  if (output < 600 ) {         
    if (!complete){            
      complete = true;   
    }
    digitalWrite(flowerOut, HIGH);
    playSong();         
    digitalWrite(flowerOut, LOW);
    delay(2000);

  }

  if (output > 600) {  
    digitalWrite(flowerOut, LOW);   
    if (complete){               
      complete = false;   
    }     
  }
}


