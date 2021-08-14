#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

const int red=0, green=1, blue=2, speaker = 4, input = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(input, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
}

void rgb(int r,int g,int b){
  analogWrite(red, 255 - r);
  analogWrite(green, 255 - g);
  analogWrite(blue, 255 - b);
}

void weatherswitch(int weather){
  switch(weather){
    case 1://晴
          rgb(102, 255, 255);
          delay(4500);
          break;
    case 2://阴
          rgb(70, 69, 68);
          delay(4500);
          break;
    case 3://阵雨
          rgb(25, 51, 77);
          delay(4500);
          break;
    case 4://雷阵雨
          rgb(25, 51, 77);
          delay(2000);
          rgb(255, 255, 0);
          delay(50);
          rgb(25, 51, 77);
          delay(100);
          rgb(255, 255, 0);
          delay(10);
          rgb(25, 51, 77);
          delay(2500);
          break;
    case 5://下雪
          rgb(204, 255, 255);
          delay(1000);
          rgb(255, 255, 255);
          delay(1000);
          rgb(204, 255, 255);
          delay(1000);
          rgb(255, 255, 255);
          delay(1000);
          break;
    case 6://沙尘暴
          rgb(255, 204, 0);
          delay(4500);
          break;
    default: 
          rgb(0,0,0);
    break;
  }
}

void song(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speaker, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speaker);
  }
 }

void loop(){
    int action = digitalRead(input);
    switch (action)
    {
    case 1:
        song();
        break;
    
    default:
        for (int i = 0; i < 4; i++)
        {
            //set all pins to low
            digitalWrite(i, LOW);
        }
        
        break;
    }

}
