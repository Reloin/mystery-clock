#include "pitches.h"

const int red=0, green=1, blue=2, pin_buzzer = 4, input = 3;

//1晴
void sunny(){
  int melody[] = {//周杰伦晴天 for 晴天
    NOTE_B5,NOTE_A5,NOTE_C6,NOTE_B5,NOTE_G5,NOTE_D6,NOTE_FS6,NOTE_G6,NOTE_FS6,NOTE_B5,NOTE_G5,
    NOTE_G5,NOTE_E6,NOTE_E6,0,NOTE_E6,NOTE_D6,NOTE_D6,NOTE_D6,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_B5
  };

  int noteDurations[] = {
    2,2,2,1,2,2,2,2,2,2,1,
    2,2,2,2,2,2,1,2,2,2,2,2,2,1
  };
  for (int thisNote = 0; thisNote < 25; thisNote++) {

    double noteDuration = 500 / noteDurations[thisNote];

    tone(pin_buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(pin_buzzer);

  }
  delay(1000);
}

//2阴
void cloudy(){
  int melody[] = {//别找我麻烦蔡健雅

    NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,
    0,NOTE_B6,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_G5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_F5

  };

  int noteDurations[] = {

  2,2,2,2,2,4,4,2,4,2,2,2,2,4,2,4,2,2,1
  };
  for (int thisNote = 0; thisNote <19; thisNote++) {

    double noteDuration = 500 / noteDurations[thisNote];

    tone(pin_buzzer, melody[thisNote], noteDuration);
      
    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(pin_buzzer);

  }
  delay(1000);
}

//3雨
void rain(){
  int melody[] = {//冰雨刘德华
    NOTE_G5,NOTE_G5,NOTE_C6,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_E5,NOTE_G5,
    NOTE_G5,NOTE_G5,NOTE_C6,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_D5,NOTE_E5
  };

  double noteDurations[] = {
  4,4,4,4,4,4,2,4,4,2,4,4/3,
    4,4,4,4,4,4,2,4,4,2,4,1
  }; 
  for (int thisNote = 0; thisNote <24; thisNote++) {

    double noteDuration = 500 / noteDurations[thisNote];

    tone(pin_buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(pin_buzzer);

  }
  delay(1000);
}
/*
//5雪
void snow(){
  int melody[] = {
    NOTE_G5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_C6,NOTE_E6,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_C6,NOTE_D6,
    NOTE_D6,0,NOTE_D6,NOTE_E6,NOTE_D6,NOTE_D6,NOTE_C6,NOTE_C6,NOTE_C6,NOTE_C6,NOTE_C6,NOTE_G6,NOTE_E6
  };

  double noteDurations[] = {

  4,4,4,4,4,4/3,4,4,4,4,4,4,
  1,4,4,4,4,4,4,4,4,4,4,4,2,1
  };

  for (int thisNote = 0; thisNote <25; thisNote++) {

    double noteDuration = 800/ noteDurations[thisNote];

    tone(pin_buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(pin_buzzer);

  }
}*/

//melody for alarm
void alarm(){
  int melody[] = {
    NOTE_G3,NOTE_G3,NOTE_G3,NOTE_DS3,NOTE_AS3,NOTE_G3,NOTE_DS3,NOTE_AS3,NOTE_G3,
    NOTE_D4,NOTE_D4,NOTE_D4,NOTE_DS4,NOTE_AS3,NOTE_FS3,NOTE_DS3,NOTE_AS3,NOTE_G3
  };

  double noteDurations[] = {
    4,4,4,16/3,16,4,16/3,16,2,
    4,4,4,16/3,16,4,16/3,16,2
  };

  for (int thisNote = 0; thisNote <18; thisNote++) {

    double noteDuration = 800/ noteDurations[thisNote];

    tone(pin_buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 2.0;

    delay(pauseBetweenNotes);

    noTone(pin_buzzer);

  }
  delay(1000);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(input, INPUT);
  pinMode(pin_buzzer, OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
}
/*
void rgb(int r,int g,int b){
  analogWrite(red, 255 - r);
  analogWrite(green, 255 - g);
  analogWrite(blue, 255 - b);
}

void weatherswitch(int weather){
  switch(weather){
    case 1://
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
*/
void loop(){
    //read pwm high time
    float val = pulseIn(input, HIGH);
    //convert to 10-bit and remove last 2 digits
    switch (int(val * 10.23 / 2000))
    {
    case 1:
      sunny();
      break;
    case 2:
      cloudy();
      break;
    case 3:
      rain();
      break;
    case 4:
      sunny();
      break;
    case 5:
      //snow();
      break;
    case 6:
      rain();
      break;
    case 7:
      cloudy();
      break;
    case 8:
      sunny();
      break;
    case 9:
      alarm();
      break;
    case 10:
      alarm();
      break;
    default:
      noTone(pin_buzzer);
      digitalWrite(red, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(blue, HIGH);
      break;
    }
}
