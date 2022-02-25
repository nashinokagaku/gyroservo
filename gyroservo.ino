#include <Wire.h>
#include <Servo.h>

#define MPU6050_ADDR 0x68
#define MPU6050_AX  0x3B
#define MPU6050_AY  0x3D
#define MPU6050_AZ  0x3F
#define MPU6050_TP  0x41
#define MPU6050_GX  0x43
#define MPU6050_GY  0x45
#define MPU6050_GZ  0x47

short int AccX, AccY, AccZ;
short int Temp;
short int GyroX, GyroY, GyroZ;
float AccX_f = 0.0f, AccY_f = 0.0f, AccZ_f = 0.0f;
float GyroX_f = 0.0f, GyroY_f = 0.0f, GyroZ_f = 0.0f;
static String key;
Servo servo1;
Servo servo2;
const int SV_PIN1 = 6;
const int SV_PIN2 = 7;
const float g = 9.81;

void setup(){
  Serial.begin( 9600 );
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
  Serial.print(""); Serial.print("AccX");
  Serial.print(","); Serial.print("AccY");
  Serial.print(","); Serial.print("AccZ");
//  Serial.print(","); Serial.print("GyroX");
//  Serial.print(","); Serial.print("GyroY");
//  Serial.print(","); Serial.print("GyroZ"); 
  Serial.println("");
  servo1.attach(SV_PIN1);
  servo2.attach(SV_PIN2);
}

void interpolation(float d_pos1, float d_pos2, int d_time, int d_num){
  int i = 0;
  float pos[2];
  float tpos[2];
  float hpos[2];
  float dpos[2];
  hpos[0] = 60;
  hpos[1] = 60;
  dpos[0] = d_pos1;
  dpos[1] = d_pos2;
  tpos[0] = hpos[0] + dpos[0];
  tpos[1] = hpos[1] + dpos[1];
  pos[0] = servo1.read();
  pos[1] = servo2.read();
  tpos[0] -= pos[0];
  tpos[1] -= pos[1];
  tpos[0] /= d_num;
  tpos[1] /= d_num;
  for(i = 0; i < d_num; i++){
  	pos[0] = pos[0] + tpos[0];
  	pos[1] = pos[1] + tpos[1];
  	if (pos[0] >= 120){
		  pos[0] = 120;
	  }
    else if (pos[0] <= 0){
      pos[0] = 0;
    }
    if (pos[1] >= 120){
      pos[1] = 120;
    }
    else if (pos[1] <= 0){
      pos[1] = 0;
    }
  	servo1.write(pos[0]);
  	servo2.write(pos[1]);
    // Serial.println(pos[0]);
    delay(d_time);
  }
}

void loop(){
  if ( Serial.available() ){
    key = Serial.readString();
//    Serial.write( key );
  }
//  Serial.println( key );
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_AX);
  Wire.endTransmission();  
  Wire.requestFrom(MPU6050_ADDR, 14);
  AccX = Wire.read() << 8;  AccX |= Wire.read();
  AccY = Wire.read() << 8;  AccY |= Wire.read();
  AccZ = Wire.read() << 8;  AccZ |= Wire.read();
  Temp = Wire.read() << 8;  Temp |= Wire.read();
  GyroX = Wire.read() << 8; GyroX |= Wire.read();
  GyroY = Wire.read() << 8; GyroY |= Wire.read();
  GyroZ = Wire.read() << 8; GyroZ |= Wire.read();
//  AccX_f = 0.9 * AccX_f + 0.95 * 9.81 * 4.0 / 65536 * 0.1 * (AccX + 520.2591200);
//  AccY_f = 0.9 * AccY_f + 0.95 * 9.81 * 4.0 / 65536 * 0.1 * (AccY - 522.8294176);
//  AccZ_f = 0.9 * AccZ_f + 0.95 * 9.81 * 4.0 / 65536 * 0.1 * (AccZ + 1052.728278);
//  GyroX_f = 0.9 * GyroX_f + 500.0 / 65536 * 0.1 * (GyroX + 182.20048312);
//  GyroY_f = 0.9 * GyroY_f + 500.0 / 65536 * 0.1 * (GyroY - 375.927516203);
//  GyroZ_f = 0.9 * GyroZ_f + 500.0 / 65536 * 0.1 * (GyroZ - 213.864375375);
  AccX_f = 0.8 * AccX_f + 0.95 * 9.81 * 4.0 / 65536 * 0.2 * (AccX + 520.2591200);
  AccY_f = 0.8 * AccY_f + 0.95 * 9.81 * 4.0 / 65536 * 0.2 * (AccY - 522.8294176);
  AccZ_f = 0.8 * AccZ_f + 0.95 * 9.81 * 4.0 / 65536 * 0.2 * (AccZ + 1052.728278);
  GyroX_f = 0.8 * GyroX_f + 500.0 / 65536 * 0.2 * (GyroX + 182.20048312);
  GyroY_f = 0.8 * GyroY_f + 500.0 / 65536 * 0.2 * (GyroY - 375.927516203);
  GyroZ_f = 0.8 * GyroZ_f + 500.0 / 65536 * 0.2 * (GyroZ - 213.864375375);
  Serial.print(""); Serial.print(AccX_f, 5);
  Serial.print(","); Serial.print(AccY_f, 5);
  Serial.print(","); Serial.print(AccZ_f, 5);
//  Serial.print(","); Serial.print(GyroX_f, 5);
//  Serial.print(","); Serial.print(GyroY_f, 5);
//  Serial.print(","); Serial.print(GyroZ_f, 5); 
//  Serial.println("");
  if (key == "O"){
    Serial.print(","); Serial.print( "ON" ); Serial.println("");
    interpolation(90 * AccZ_f / g, 90 * AccY_f / g, 5, 5);
  }
  else{
    Serial.print(","); Serial.print( "OFF" ); Serial.println("");
    servo1.write(60);
    servo2.write(60);
    delay(1000);
  }
}
