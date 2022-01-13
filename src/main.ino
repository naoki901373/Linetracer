/**本番要のプログラム*/
/**モータドライバ変更後*/
/**エラー処理なし（止まる）*/

/**センサ*/
int photo_right = A1;     //右のフォトセンサのピン番号
int photo_left = A2;      //左のフォトセンサのピン番号

/**右のモータ*/
int APHASE_right  = 2;
int AENBL_right   = 3;
int BPHASE_right  = 12;
int BENBL_right   = 11;

/**左のモータ*/
int APHASE_left  = 4;
int AENBL_left   = 5;
int BPHASE_left  = 9;
int BENBL_left   = 8;

int m = 800;              //フォトセンサの閾値
int pulse_num = 10;      //1周期でパルス波を与える回数
int pulse_width = 2000;     //パルス波のHIGHの長さ[μs]

int temp = "s";           //前の状態を保存する変数（初期値：s）

void setup() {
  /**モータの端子を出力に*/
  pinMode(APHASE_right,OUTPUT);
  pinMode(AENBL_right,OUTPUT);
  pinMode(BPHASE_right,OUTPUT);
  pinMode(BENBL_right,OUTPUT);
  pinMode(APHASE_left,OUTPUT);
  pinMode(AENBL_left,OUTPUT);
  pinMode(BPHASE_left,OUTPUT);
  pinMode(BENBL_left,OUTPUT);
  
  /**enblピン*/
  digitalWrite(AENBL_right,HIGH);
  digitalWrite(BENBL_right,HIGH);
  digitalWrite(AENBL_left,HIGH);
  digitalWrite(BENBL_left,HIGH);

  /**シリアル通信の開始・パルス波の波長を送る*/
  Serial.begin(9600);
  
  Serial.println("【情報】");
  Serial.print("1周期のパルス波の数＝");
  Serial.print(pulse_num);
  Serial.println("回");
  Serial.print("パルス波の長さ=");
  Serial.print(pulse_width);
  Serial.println("μ秒");
  Serial.println("");
  
}

/**右に曲がる指令*/
void go_right(int n){
  for(int i=0; i<n; i++){
    digitalWrite(APHASE_left,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_left,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(APHASE_left,LOW);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_left,LOW);
    delayMicroseconds(pulse_width);
  }
}

/**左に曲がる指令*/
void go_left(int n){
  for(int i=0; i<n; i++){
    digitalWrite(APHASE_right,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_right,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(APHASE_right,LOW);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_right,LOW);
    delayMicroseconds(pulse_width);
  }
}

/**直進する指令*/
void go_straight(int n){
  for(int i=0; i<n; i++){
    digitalWrite(APHASE_right,HIGH);
    digitalWrite(APHASE_left,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_right,HIGH);
    digitalWrite(BPHASE_left,HIGH);
    delayMicroseconds(pulse_width);
    digitalWrite(APHASE_right,LOW);
    digitalWrite(APHASE_left,LOW);
    delayMicroseconds(pulse_width);
    digitalWrite(BPHASE_right,LOW);
    digitalWrite(BPHASE_left,LOW);
    delayMicroseconds(pulse_width);
  }
}

void loop() {
  int u_right = analogRead(photo_right);  //右のフォトセンサの出力
  int u_left = analogRead(photo_left);    //左のフォトセンサの出力

  /**（白の時、センサの値は大きい）*/
  /**（線幅17mm）*/
  if( u_right<m && u_left<m ){
    go_straight(pulse_num);               //両方が黒の時は、直進
    Serial.print("s");
    temp = "s";
  }else if(u_left<m){
    go_left(pulse_num);                   //左だけ黒を感知したら、左に曲がる
    Serial.print("l");
    temp = "l";
  }else if(u_right<m){
    go_right(pulse_num);                  //右だけ黒を感知したら、右に曲がる
    Serial.print("r");
    temp = "r";
  }else{
    //go_back(pulse_num);                   //両方が白の時は、例外処理
    Serial.print("e");
  }
}