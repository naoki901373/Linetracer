/**本番用のプログラム*/
/**モータドライバ変更後*/
/**エラー処理あり（止まり、そこまでの経路を表示）*/
/**ゴールテープを感知したら停止し、fと送信*/

/**（白の時、センサの値は大きい）*/
/**（線幅17mm）*/    

/**センサ*/
int photo_right = A1;     //右のフォトセンサのピン番号
int photo_left = A2;      //左のフォトセンサのピン番号
int photo_finish = A3;    //一週判定用のフォトセンサ

/**右のモータ*/
int APHASE_right  = 2;
int AENBL_right   = 3;
int BPHASE_right  = 12;
int BENBL_right   = 11;

/**左のモータ*/
int APHASE_left  = 7;
int AENBL_left   = 6;
int BPHASE_left  = 5;
int BENBL_left   = 4;

int m = 800;              //フォトセンサの閾値
int pulse_num = 10;      //1周期でパルス波を与える回数
int pulse_width = 10000;     //パルス波のHIGHの長さ[μs]

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
  /*
  Serial.println("【情報】");
  Serial.print("1周期のパルス波の数＝");
  Serial.print(pulse_num);
  Serial.println("回");
  Serial.print("パルス波の長さ=");
  Serial.print(pulse_width);
  Serial.println("μ秒");
  Serial.println("");
  */
}

/**右に曲がる指令*/
void go_right(int n){
  for(int i=0; i<n; i++){
    digitalWrite(APHASE_left,HIGH);
    delayMicroseconds(pulse_width/2);
    digitalWrite(BPHASE_left,HIGH);
    delayMicroseconds(pulse_width/2);
    digitalWrite(APHASE_left,LOW);
    delayMicroseconds(pulse_width/2);
    digitalWrite(BPHASE_left,LOW);
    delayMicroseconds(pulse_width/2);
    int u_finish = analogRead(photo_finish);  //一週判定
    if( u_finish<m ){
      go_stop();
    }
  }
}

/**左に曲がる指令*/
void go_left(int n){
  for(int i=0; i<n; i++){
    digitalWrite(APHASE_right,HIGH);
    delayMicroseconds(pulse_width/2);
    digitalWrite(BPHASE_right,HIGH);
    delayMicroseconds(pulse_width/2);
    digitalWrite(APHASE_right,LOW);
    delayMicroseconds(pulse_width/2);
    digitalWrite(BPHASE_right,LOW);
    delayMicroseconds(pulse_width/2);
    int u_finish = analogRead(photo_finish);  //一週判定
    if( u_finish<m ){
      go_stop();
    }
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
    int u_finish = analogRead(photo_finish);  //一週判定
    if( u_finish<m ){
      go_stop();
    }
  }
}

void go_stop(){
  Serial.println("f");
  for(;;){
    delay(100);
  }
}

void loop() {
  int u_right = analogRead(photo_right);  //右のフォトセンサの出力
  int u_left = analogRead(photo_left);    //左のフォトセンサの出力
  
  int u_finish = analogRead(photo_finish);  //一週判定
  if( u_finish<m ){
    go_stop();
  }
  
  if( u_right<m && u_left<m ){
    go_straight(pulse_num);               //両方が黒の時は、直進
    Serial.println("s");
    temp = "s";
  }else if(u_left<m){
    go_left(pulse_num);                   //左だけ黒を感知したら、左に曲がる
    Serial.println("l");
    temp = "l";
  }else if(u_right<m){
    go_right(pulse_num);                  //右だけ黒を感知したら、右に曲がる
    Serial.println("r");
    temp = "r";
  }else{
    //go_back(1);                   //両方が白の時は、例外処理
    //delayMicroseconds(pulse_width*4*pulse_num);
    Serial.println("e");                  //線からはみ出たら、止めて、その時点までの経路を表示
    for(;;){
      delay(100);
    }
  }
}