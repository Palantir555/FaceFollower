#include <string.h>
#include <Servo.h>
#define STEP 1
#define DELAY 12

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

Servo Hservo;
Servo Vservo;

int Hpos; //Servo position
int Vpos;
int Hint; //Input value
int Vint;
char Hch;
char Vch;

void setup() 
{ 
    Serial.begin(9600);
    // reserve 200 bytes for the inputString:
    inputString.reserve(800);

    Hservo.attach(8);  // attaches the servo on pin 9 to the servo object 
    Vservo.attach(9);

    Hpos = 90;
    Vpos = 150;
    Hservo.write(Hpos); //Default H
    delay(600);
    Vservo.write(Vpos); //Default V
    delay(600);
} 


void loop() 
{
    if((Hpos <= 180) && (Hpos >= 0))
    {
        switch (Hch)
        {
        case 'R':
            Hpos-=STEP;
            Hservo.write(Hpos);
            delay(DELAY);
            break;
        case 'L':
            Hpos+=STEP;
            Hservo.write(Hpos);
            delay(DELAY);
            break;
        default:
            delay(DELAY);
            break;
        }
    }
    else if(Hpos > 180)
    {
        Hpos = 180;
        Hservo.write(Hpos);
        delay(DELAY);
    }
    else if(Hpos < 0)
    {
        Hpos = 0;
        Hservo.write(Hpos);
        delay(DELAY);
    }

    if((Vpos <= 180) && (Vpos >= 0))
    {
        switch (Vch)
        {
        case 'U':
            Vpos-=STEP;
            Vservo.write(Vpos);
            delay(DELAY);
            break;
        case 'D':
            Vpos+=STEP;
            Vservo.write(Vpos);
            delay(DELAY);
            break;
        default:
            delay(DELAY);
            break;
        }
    }
    else if(Vpos > 180)
    {
        Vpos = 180;
        Vservo.write(Vpos);
        delay(DELAY);
    }
    else if(Hpos < 0)
    {
        Vpos = 0;
        Vservo.write(Vpos);
        delay(DELAY);
    }
} 

void serialEvent() 
{
    while (Serial.available()) 
    {
        char inChar = (char)Serial.read();
        inputString += inChar;
        if (inChar == '\n')
        {
            int hmod=0;
            int vmod=0;
            for(int i=0; i<inputString.length(); i++)
            {
                if(inputString.charAt(i) == 'H')
                {
                    Hch = inputString.charAt(i+1);
                    hmod=1;
                }
                if(inputString.charAt(i) == 'V')
                {
                    Vch = inputString.charAt(i+1);
                    vmod=1;
                }
                if(hmod==1 && vmod==1)
                    break;
            }
            inputString = "";
            break;
        }
    }
}












