#include <Servo.h> 

#define SERVOPINH  6 //horizontal servo
#define SERVOPINV  5 //vertical servo


int tol=50;      //A faixa de resposta de iluminância: quanto menor o valor, mais sensível é a resposta; caso contrário, a resposta será mais lenta.
                //(O valor varia de 10 a 100, e a sensibilidade depende da intensidade da luz ambiente. Em ambientes internos,
                // onde a fonte de luz varia significativamente, a sensibilidade será maior. Sob luz solar, as mudanças são menores.)

int dtime=50;    //Parâmetro de atraso: quanto menor o valor, mais rápida será a velocidade de resposta.
                //Por outro lado, quanto maior o valor, mais lenta será a velocidade de resposta. Unidade: milissegundos. Valor geral (10~100). 
            
// Configurações do servo horizontal
Servo horizontal;            //horizontal servo
int servoh = 90;             //Inicializar ângulo
int servohLimitHigh = 180;   //O ângulo máximo de rotação na direção horizontal
int servohLimitLow = 0;      //O ângulo mínimo de rotação na direção horizontal


// Configurações do servo vertical
Servo vertical;              //vertical servo
int servov = 45;             //Inicializar ângulo
int servovLimitHigh = 150;   // ângulo máximo de rotação na direção vertical
int servovLimitLow = 10;     // ângulo mínimo de rotação na direção vertical

// 4 portas de conexão para módulos de fotoresistores  
const int ldrlt = A0;   //top left
const int ldrrt = A1;   //top right
const int ldrld = A2;   //down left
const int ldrrd = A3;   //down right

void setup()
{
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);
  Serial.begin(9600);
}

void loop() 
{
  //Ler os valores de iluminância dos 4 módulos de fotoresistores, respectivamente.
  int lt = analogRead(ldrlt); //upper left
  int rt = analogRead(ldrrt); //top right
  int ld = analogRead(ldrld); //down left
  int rd = analogRead(ldrrd); //down right
  Serial.print("lt:");Serial.print(lt);Serial.print(" ");
  Serial.print("rt:");Serial.print(rt);Serial.print(" ");
  Serial.print("ld:");Serial.print(ld);Serial.print(" ");
  Serial.print("rd:");Serial.println(rd);
  //Average readings from adjacent photoresistor modules    
  int avt = (lt + rt) / 2; 
  int avd = (ld + rd) / 2; 
  int avl = (lt + ld) / 2; 
  int avr = (rt + rd) / 2;

  int veg=(avt + avd + avl + avr) / 4;

 //Ajustar os parâmetros de resposta de acordo com diferentes intensidades de luz.
 if(0<veg&&veg<300)
 { tol=map(veg,10,300,5,100);
   dtime=map(veg,10,300,100,50);
 }
 else
 { tol=20;
   dtime=20;
 }

  Serial.print("veg= ");Serial.println(veg);
  Serial.print("tol= ");Serial.println(tol);
  Serial.print("dtime= ");Serial.println(dtime);
  

  
  //Em seguida, calcule a diferença entre as linhas superior e inferior e o valor médio das linhas esquerda e direita.
  int dvert = avt - avd;    //upper and lower rows 
  int dhoriz = avl - avr;   //left and right rows

   //Verifique se a diferença está dentro da tolerância; caso contrário, altere o ângulo vertical.   
   if (-1*tol > dvert || dvert > tol) 
   {
      if (avt < avd)
      {
        servov = ++servov;
         if (servov > servovLimitHigh) 
         { 
           servov = servovLimitHigh;
         }
      }
      else if (avt > avd)
      {
        servov= --servov;
        if (servov < servovLimitLow)
        {
          servov = servovLimitLow;
        }
      }
      vertical.write(servov); //Se o ângulo de rotação do servo for oposto à luz, use (180- servov) ou (servov) para mudar a direção.
    }

  //Verifique se a diferença está dentro da tolerância; caso contrário, altere o ângulo horizontal.
  if (-1*tol > dhoriz || dhoriz > tol) 
  {
    if (avl < avr)
    {
      servoh = --servoh;
      if (servoh < servohLimitLow)
      {
        servoh = servohLimitLow;
      }
    }
    else if (avl > avr)
    {
       servoh = ++servoh;
       if (servoh > servohLimitHigh)
       {
         servoh = servohLimitHigh;
       }
    }
    horizontal.write(servoh); //Se o ângulo de rotação do servo for oposto à luz, use (180- servoh) ou (servoh) para mudar a direção.
  }
   delay(dtime);
   
}
