#include<Wire.h>
#include<math.h> 
//Endereco I2C do MPU6050



const int MPU=0x68;  //pino aberto 0X68 , pino ligado em 3,3V 0x69

//Variaveis globais
int accel_center_x=0, accel_center_y=0,accel_center_z=0;
long int acelX,acelY,acelZ,temperatura,giroX,giroY,giroZ;
long int mediax=0,mediay=0,mediaz=0;
int i=0;

//configurações iniciais
void calc_xy_angles(int accel_value_x,int accel_value_y,int accel_value_z){
   // Using x y and z from accelerometer, calculate x and y angles
   int x_val, y_val, z_val;
   double result,accel_angle_x,accel_angle_y, accel_angle_z;
   unsigned short  x2, y2, z2; //24 bit
   double modulo;

   // Lets get the deviations from our baseline
   x_val = accel_value_x-accel_center_x;
   y_val = accel_value_y-accel_center_y;
   z_val = accel_value_z-accel_center_z;

   // Work out the squares 
   x2 = (x_val*x_val);
   y2 =(y_val*y_val);
   z2 = (z_val*z_val);
   modulo=sqrt(x2+y2+z2);

   //X Axis
   //result=sqrt(y2+z2);
   result=acos(x_val/modulo)*180/3.1415;
   //accel_angle_x = atan(result)*180/3.1415;
   
//   Serial.print("AgX: ");Serial.print(result);
  
   //Y Axis
//   result=sqrt(x2+z2);
//  result=acos(y_val/modulo)*180/3.1415;
//  Serial.print(y_val/modulo);
 //  accel_angle_y = atan(result)*180/3.1415;
//   Serial.print("\t AgY: ");Serial.print(result);
   //Z axis
//   result=sqrt(x2+y2);
//   result=z_val/result;
    result=acos(z_val/modulo)*180/3.1415;
  // accel_angle_z = atan(result)*180/3.1415;
  Serial.print("\t AgZ: ");Serial.print(result);
  
//   Serial.print("AngX: ");Serial.print(result);
//    Serial.print("\tAngY: ");Serial.print(result);
//    Serial.print("\tAngZ: ");Serial.print(result);
   Serial.print("\n");
}



void setup()
{

  Serial.begin(9600);     //inicia a comunicação serial
  Wire.begin();                 //inicia I2C
  Wire.beginTransmission(MPU);  //Inicia transmissão para o endereço do MPU
  Wire.write(0x6B);             
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
}

//loop principal
void loop()
{
  
  Wire.beginTransmission(MPU);      //transmite
  Wire.write(0x3B);                 // Endereço 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);     //Finaliza transmissão
  
  Wire.requestFrom(MPU,14,true);   //requisita bytes
   
  //Armazena o valor dos sensores nas variaveis correspondentes
  acelX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  acelY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acelZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //temperatura=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  //giroX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  //giroY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  //giroZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  if(i<1000){
   mediax+=acelX/1000;
   mediay+=acelY/1000;
   mediaz+=acelZ/1000;
   i++;
  }
  else{
    i=0;
//    Serial.print("X: ");Serial.print(mediax);
//    Serial.print("\tY: ");Serial.print(mediay);
//    Serial.print("\tZ: ");Serial.print(mediaz);

//LINHA MAIS IMPORTANTE DO CODIGO
     Serial.print(-(97.85-acos(mediax/sqrt(mediax*mediax+mediay*mediay+mediaz*mediaz))*180/3.1415));
    Serial.print("\n\n\n");
//   calc_xy_angles(mediax,mediay,mediaz);
    mediax=0;
    mediay=0;
    mediaz=0;
  }
    //Envia valores lidos do acelerômetro
  //Serial.print("Acel:"); 
  //Serial.print("  X:");Serial.print(acelX);
  //Serial.print("\tY:");Serial.print(acelY);
  //Serial.print("\tZ:");Serial.print(acelZ);
  
  //Envia valores lidos do giroscópio
  //Serial.print("\tGiro:"); 
  //Serial.print("  X:");Serial.print(giroX);
  //Serial.print("\tY:");Serial.print(giroY);
  //Serial.print("\tZ:");Serial.print(giroZ);
   
  //Envia valor da temperatura em graus Celsius
  //Serial.print("\tTemperatura: ");    Serial.println(temperatura/340.00+36.53);
   
  //Aguarda 500 ms
  delay(1);
}
