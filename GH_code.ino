#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define sensor A1                       //định nghĩa biến sensor ở chân A1 để đọc cảm biến độ ẩm đất
#define lightsensor A0                  //định nghĩa biến lightsensor ở chân A0 để đọc dữ liệu từ cảm biến ánh sáng

#define pinDC1 7                        //định nghĩa biến pinDC1 ở chân số 7
#define pinDC2 2                        //định nghĩa biến pinDC2 ở chân số 2  
#define CTHTTren 13                     //định nghĩa biến CTHTTren ở chân số 13
#define CTHTDuoi 12                     //định nghĩa biến CTHTDuoi ở chân số 12
#define motor1 8                        //định nghĩa biến motor1 ở chân số 8
//.................................................
void setup() {
  Serial.begin(9600);
  pinMode(lightsensor, INPUT);          //chế độ INPUT nhận dữ liệu cho biến lightsensor hoặc chân A0
  pinMode(sensor, INPUT);               //chế độ INPUT nhận dữ liệu cho biến sensor hoặc chân A1
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(7, OUTPUT);                   //chế độ OUTPUT xuất dữ liệu cho các chân 7 và 2
  pinMode(2, OUTPUT);
  pinMode(motor1, OUTPUT);
  lcd.init();                           //khởi động LCD
}
//.................................................
String Stringdata, Rem = "OFF", Status = "ON";
int quang,soil;

void loop(){                            //chương trình chính
  TinHieu();
  ThucThi();
  }
  
void TinHieu(){                         //đọc, lưu dữ liệu vào các biến
  quang = analogRead(lightsensor);
  quang = map(quang, 0, 1023, 0, 100);
  soil = analogRead(sensor);
  soil = map(soil, 0, 1023, 0, 100);
}
void ThucThi()                          //thực thi
{
  if(Status =="OFF")                    //mục đích để xem trạng thái của motor và Rèm
  {
    if (Rem == "ON") MoRem();           //nếu Status ở trạng thái OFF và Rem ở trạng thái ON thì gọi hàm MoRem để đảo trạng thái Status, tiết kiệm thời gian chạy cho chương trình
    else DongRem();
  }
  else if(Status == "ON")               //vì quy định biến Status ở giá trị "ON" từ đầu nên chương trình bắt đầu từ đây
  {
    if(quang > 60)                      //nếu quang > 60 thì mở rèm và ghi nhận trạng thái rèm ở giá trị "ON"
    { MoRem(); Rem = "ON";}             //nếu không thì đóng rèm và ghi nhận trạng thái rèm ở giá trị "OFF"
    else
    { DongRem(); Rem = "OFF";}
  }
  if (soil > 70) {
    Serial.println("Do am binh thuong"); //nếu độ ẩm đất trên 70 thì hiển thị LCD 
    lcd.setCursor(0, 0);                  
    lcd.print("    Do am dat ok    ");    
    lcd.setCursor(0, 1);
    lcd.print("                   ");
    digitalWrite(motor1, LOW);           //và gửi tín hiệu LOW ra chân 8
  }
  else {
    Serial.println("Dat thieu nuoc");     //nếu độ ẩm đất dưới 70 thì hiển thị LCD
    lcd.setCursor(0, 0);
    lcd.print(" Dat thieu nuoc ");
    digitalWrite(motor1, HIGH);           //và gửi tín hiệu mức HIGH ra chân 8
    lcd.setCursor(0, 1);
    lcd.print("   MOTOR chay  ");         
  }
}
void MoRem()                            
{
  while ( digitalRead(CTHTTren))        //đọc giá trị từ biến CTHTTren, khi CTHTTren = 1 thì cho motor quay thuận để mở rèm
  { digitalWrite(pinDC1, HIGH);         
    digitalWrite(pinDC2, LOW);          
    if ( digitalRead(CTHTTren == 0)) break; //nếu giá trị từ biến CTHTTren thay đổi về 0 thì thoát vòng lặp while
  }
  digitalWrite(pinDC1, LOW);            //và ngưng motor
  digitalWrite(pinDC2, LOW);            
  if(Status == "ON") Status = "OFF";    //đảo trạng thái biến Status
  else Status = "ON";
}
void DongRem()
{
  while(digitalRead(CTHTDuoi))
  { digitalWrite(pinDC2, HIGH);
    digitalWrite(pinDC1, LOW);
    if ( digitalRead(CTHTDuoi == 0)) break;
  }
  digitalWrite(pinDC1, LOW);
  digitalWrite(pinDC2, LOW);
  if(Status == "ON") Status = "OFF";
  else Status = "ON";
}
