#include <Servo.h> 

#define  servomaxx   180   
#define  servomaxy   180   
#define  screenmaxx   320   
#define  screenmaxy   240    
#define  servocenterx   90  
#define  servocentery   90  
#define  servopinx   9   
#define  servopiny   10  
#define  baudrate 57600  
#define distancex 1  
#define distancey 1  

int valx = 0;       
int valy = 0;       
int posx = 0;
int posy = 0;
int incx = 10;  
int incy = 10;  

Servo servox;
Servo servoy;

short MSB = 0;  
short LSB = 0;  
int   MSBLSB = 0;  

void setup() {

  Serial.begin(baudrate);        
  Serial.println("Starting Cam-servo Face tracker");

  pinMode(servopinx,OUTPUT);    
  pinMode(servopiny,OUTPUT);    

  servoy.attach(servopiny); 
  servox.attach(servopinx); 

  // center servos

  servox.write(servocenterx); 
  delay(200);
  servoy.write(servocentery); 
  delay(200);
}

void loop () {
  while(Serial.available() <=0); 
  if (Serial.available() >= 4)   
  {
    
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valx = MSBLSB; 
    delay(5);

    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valy = MSBLSB; 
    delay(5);

    
    posx = servox.read(); 
    posy = servoy.read();

    if(valx > (screenmaxx/2 - incx)){
      if( posx >= incx ) posx += distancex; 
    }
    
    else if(valx < screenmaxx/2 + incx){
      if(posx <= servomaxx-incx) posx -=distancex; 
    }

    
    if(valy > (screenmaxy/2 - incy)){
      if(posy >= 5)posy += distancey; 
    }
    
    else if(valy < (screenmaxy/2 + incy)){
      if(posy <= 175)posy -= distancey; 
    }

    
    servox.write(posx);
    //servoy.write(posy);

  }   
}














