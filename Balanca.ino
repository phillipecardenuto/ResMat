#include<Wire.h>
#include<math.h> 
//Endereco I2C do MPU6050



const int MPU=0x68;  //pino aberto 0X68 , pino ligado em 3,3V 0x69

//Variaveis globais
int accel_center_x=0, accel_center_y=0,accel_center_z=0;
long int acelX,acelY,acelZ,temperatura,giroX,giroY,giroZ;
long int mediax=0,mediay=0,mediaz=0;
int i=0;

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

  //Para manter que a variavel não exceda o máximo de valores que ela possa receber é preciso dividi-la por 250
  if(i<250){
   mediax+=acelX/250;
   mediay+=acelY/250;
   mediaz+=acelZ/250;
   i++;
  }
  else{
    i=0;
    Serial.print("X: ");Serial.print(mediax);
    Serial.print("\tY: ");Serial.print(mediay);
    Serial.print("\tZ: ");Serial.print(mediaz);
    Serial.print("\n");

      Serial.print("(Alpha) =");
     Serial.print(-(96.5-acos(mediax/sqrt(mediax*mediax+mediay*mediay+mediaz*mediaz))*180/3.1415));

     //Próxima linha comentada server para achar o zero das coordenadas no referencia novo do acelerometro
     //Serial.print(-(-acos(mediax/sqrt(mediax*mediax+mediay*mediay+mediaz*mediaz))*180/3.1415));
    Serial.print("\n\n\n");
    mediax=0;
    mediay=0;
    mediaz=0;
  }
  
   
  //Aguarda 500 ms
  delay(1);
}
