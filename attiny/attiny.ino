#include "pitches.h"
void song(int n){
  switch(n){
    case 1:
            int melody[] = {//周杰伦晴天 for 晴天
                            NOTE_B5,NOTE_A5,NOTE_C6,NOTE_B5,NOTE_G5,NOTE_D6,NOTE_FS6,NOTE_G6,NOTE_FS6,NOTE_B5,NOTE_G5,
                            NOTE_G5,NOTE_E6,NOTE_E6,0,NOTE_E6,NOTE_D6,NOTE_D6,NOTE_D6,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_B5
                          };

            int noteDurations[] = {

              2,2,2,1,2,2,2,2,2,2,1,
              2,2,2,2,2,2,1,2,2,2,2,2,2,1
            };
            for (int thisNote = 0; thisNote < 25; thisNote++) {

            int noteDuration = 500 / noteDurations[thisNote];

            tone(pin_buzzer, melody[thisNote], noteDuration);

            int pauseBetweenNotes = noteDuration * 1.30;

            delay(pauseBetweenNotes);

            noTone(pin_buzzer);

            }
            break;
    case 2:
            int melody[] = {//别找我麻烦蔡健雅

              NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,
              0,NOTE_B6,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_F5

            };

            int noteDurations[] = {

            2,2,2,2,2,4,4,2,4,2,2,2,2,4,2,4,2,2,1
            };
            for (int thisNote = 0; thisNote <19; thisNote++) {

            int noteDuration = 500 / noteDurations[thisNote];

            tone(pin_buzzer, melody[thisNote], noteDuration);
              
            int pauseBetweenNotes = noteDuration * 1.30;

            delay(pauseBetweenNotes);

            noTone(pin_buzzer);

          }
          break;
    case 3:
           int melody[] = {//冰雨刘德华

              NOTE_G5,NOTE_G5,NOTE_C6,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_E5,NOTE_G5,
              NOTE_G5,NOTE_G5,NOTE_C6,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_D5,NOTE_E5

            };

           double noteDurations[] = {

          4,4,4,4,4,4,2,4,4,2,4,(4/3),
            4,4,4,4,4,4,2,4,4,2,4,1
          }; 
          for (int thisNote = 0; thisNote <24; thisNote++) {

          double noteDuration = 800/ noteDurations[thisNote];

          tone(pin_buzzer, melody[thisNote], noteDuration);
            
          int pauseBetweenNotes = noteDuration * 1.30;

          delay(pauseBetweenNotes);

          noTone(pin_buzzer);

          }
      break;

      
    default:break;
  }
  
}
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
    //read pwm high time
    float val = pulseIn(input, HIGH);
    //convert to 10-bit and remove last 2 digits
    switch (int(val * 10.23 / 2000))
    {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    default:
      break;
    }
    

}
