#include <LiquidCrystal.h>

#define current       (PINB & 0x02)
#define voltage       (PINB & 0x01)
#define currentHIGH   2
#define voltageHIGH   1
#define pi            3.1415926535897932384626433832795


void pfCalculation();
void displayPF();


int x=0, state;
float pf, pfMAX;

// RS,EN,D4,D5,D6,D7
LiquidCrystal lcd(5, 4, 3, 2, 1, 0);

void setup() 
{

    lcd.begin(16,2); 

    pinMode(8,INPUT_PULLUP);
    pinMode(9,INPUT_PULLUP);
    pinMode(A0,OUTPUT);
    pinMode(A1,OUTPUT);
    pinMode(A2,OUTPUT);
  
    pfCalculation();
    pfMAX = pf;
    displayPF();
    delay(200);

    capacitorSelect();
    pf = pfMAX;
    
    lcd.clear();
    delay(200);
    displayPF();
}

void loop() 
{
}

void pfCalculation()
{
    int  i, temp, n;

    n = 3;
    temp = 0;
    for( i = 1; i <= n; i++ )
    {
      while( current != currentHIGH){}
      while( current == currentHIGH){}
    
      x = micros();
      state = voltage;
    
      if( voltage == voltageHIGH )
      {
        while( voltage == voltageHIGH){}
        x = micros() - x;
        //pf = cos((x/20000.0)*2*pi);
      }
      else
      {
        while( voltage != voltageHIGH){}
        x = micros() - x;
        //pf = cos(((10000-x)/20000.0)*2*pi);
      }

      temp = temp + x;
    }
    
    x = temp/n;
    if( state == 1 )
      pf = cos((x/20000.0)*2*pi);
    else
      pf = cos(((10000-x)/20000.0)*2*pi);
}

void capacitorSelect()
{
    digitalWrite(A0, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
    
    digitalWrite(A1, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
    
    digitalWrite(A2, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
      
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
      
    digitalWrite(A0, HIGH);
    digitalWrite(A2, HIGH);
    pfCalculation();
    
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
      
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
    pfCalculation();

    if( pf > pfMAX )
      pfMAX = pf;
}

void displayPF()
{
    lcd.setCursor(0, 0);
    lcd.print(pf,4);
    
    lcd.setCursor(0,1);
    if(state == 0)
      lcd.print("Lagging");
    else
      lcd.print("Leading");
}

