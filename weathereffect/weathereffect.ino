int R=9,G=10,B=11;
void setup() {
  // put your setup code here, to run once:
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
}

void rgb(int r,int g,int b){
  analogWrite(R,r);
  analogWrite(G,g);
  analogWrite(B,b);
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
    default: break;
  }
}

void loop(){}
