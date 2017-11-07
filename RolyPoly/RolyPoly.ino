#include <Wire.h>
#include <SFE_MMA8452Q.h>
#include <math.h>

#define moveRate 100
#define timeLimit 15000
#define gamePauseTime 4000

#define cframeRate 125

MMA8452Q accel;

int rPin[8];
int cPin[6];

int prey[2];
int predator[2];
bool predLed = 0;
bool winner;
int preyS = 0, predS = 0;

int frameCount = 0;
unsigned long frameTime = 0;
unsigned long pmoveTime = 0;
unsigned long gameTime = 0;
unsigned long pdmoveTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 1; i < 8; i++)
  {
    rPin[i] = i + 1;
    pinMode(rPin[i], OUTPUT);
  }

  for (int i = 1; i < 6; i++)
  {
    cPin[i] = i + 8;
    pinMode(cPin[i], OUTPUT);
  }

  assignPlaces();
  accel.init();
}

void loop()
{
  //print prey
  reset();
  digitalWrite(cPin[prey[0]], LOW);
  digitalWrite(rPin[prey[1]], HIGH);
  //check for move - prey
  if (millis() - pmoveTime > moveRate && accel.available())
  {
    accel.read();
    byte pl = accel.readPL();
    switch(pl)
    {
      case PORTRAIT_U:
        if (prey[1] != 7)
          prey[1]++;
        break;
      case PORTRAIT_D:
        if (prey[1] != 1)
          prey[1]--;
        break;
      case LANDSCAPE_R:
        if (prey[0] != 5)
          prey[0]++;
        break;
      case LANDSCAPE_L:
        if (prey[0] != 1)
          prey[0]--;
        break;
    }
    pmoveTime = millis();
  }

  //check for move - predator
  if (Serial.available() > 0)// && millis() - pdmoveTime > 2 * moveRate)
  {
    char c = Serial.read();
    switch (c)
    {
      case 'u':
        if (predator[1] != 1)
          predator[1]--;
        break;
      case 'd':
        if (predator[1] != 6)
          predator[1]++;
        break;
      case 'l':
        if (predator[0] != 1)
          predator[0]--;
        break;
      case 'r':
        if (predator[0] != 4)
          predator[0]++;
        break;
    }
  }
//  //clear buffer
//  while (Serial.available() > 0)
//    Serial.read();
  
  //print predator
  reset();
  digitalWrite(cPin[predator[0]], !predLed);
  digitalWrite(cPin[predator[0] + 1], !predLed);
  digitalWrite(rPin[predator[1]], predLed);
  digitalWrite(rPin[predator[1] + 1], predLed);
  if (millis() - frameTime > cframeRate)
  {
    predLed = !predLed;
    frameTime = millis();
  }

  //check for victory
  if (millis() - gameTime > timeLimit)
  {
    winner = 0;
    preyS++;
    showVictor(preyS, predS);
    gameTime = millis();

    assignPlaces();
  }
  else if ((prey[0] == predator[0] || prey[0] == predator[0] + 1) && (prey[1] == predator[1] || prey[1] == predator[1] + 1))
  {
    winner = 1;
    predS++;
    showVictor(predS, preyS);
    gameTime = millis();

    assignPlaces();
  }
}

void reset()
{
  for (int i = 1; i < 8; i++)
    digitalWrite(rPin[i], LOW);

  for (int i = 1; i < 6; i++)
    digitalWrite(cPin[i], HIGH);
}

void showVictor(int score1, int score2)
{
  int s = score1 - score2;
  int scoreType;
  if (s == 0)
    scoreType = 0;
  else if (s > 0 && s <=2)
    scoreType = 1;
  else if (s > 2)
    scoreType = 2;
  else if (s < 0 && s >= -2)
    scoreType = 3;
  else 
    scoreType = 4;

  gameTime = millis();
  while (millis() - gameTime < gamePauseTime)
  {
    //show victor
    if (winner == 0)
    {
      reset();
      digitalWrite(rPin[4], HIGH);
      digitalWrite(cPin[3], LOW);
    }

    else
    {
      reset();
      digitalWrite(rPin[4], HIGH);
      digitalWrite(rPin[5], HIGH);
      digitalWrite(cPin[3], LOW);
      digitalWrite(cPin[4], LOW);
    }
    
    //show score
    reset();
    digitalWrite(rPin[7], HIGH);
    switch(scoreType)
    {
      case 0:
        digitalWrite(cPin[3], LOW);
        break;

      case 1:
        digitalWrite(cPin[3], LOW);
        digitalWrite(cPin[4], LOW);
        break;

      case 2:
        digitalWrite(cPin[3], LOW);
        digitalWrite(cPin[4], LOW);
        digitalWrite(cPin[5], LOW);
        break;

      case 3:
        digitalWrite(cPin[2], LOW);
        digitalWrite(cPin[3], LOW);
        break;

      case 4:
        digitalWrite(cPin[1], LOW);
        digitalWrite(cPin[2], LOW);
        digitalWrite(cPin[3], LOW);
        break;
    }
  }
}

void assignPlaces()
{ 
  prey[0] = random(5) + 1;
  prey[1] = random(7) + 1;
  predator[0] = random(4) + 1;
  predator[1] = random(6) + 1;

  if ((prey[0] == predator[0] || prey[0] == predator[0] + 1) && (prey[1] == predator[1] || prey[1] == predator[1] + 1))
  {
    if (predator[1] == 6)
      prey[1] = 5;
    else
      prey[1] = predator[1] + 2;
  }
  //clear buffer
  while (Serial.available() > 0)
    Serial.read();
}

