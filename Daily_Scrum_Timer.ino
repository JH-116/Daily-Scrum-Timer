#include "SevSeg.h"
#define BUTTON_STARTMEETING 30
#define BUZZERMEETING 45
#define BUZZERTALKTIME 47
#define BUTTON_STARTTALKTIME 44
#define BUTTON_RESET 48



SevSeg meetingDisplay;
SevSeg talktimeDisplay;


//long Startwert = //1500000;//Startwert des Timers
long talkTime = 300000;//maximale Redezeit
long StartValueInSeconds = 60; //900; //15min
long TalkTimeStartValueInSeconds = 30; //180;

long startpunkt = 0;//Speicher des aktuellen Milli-Wertes
long talkTimeStart= 0;//Speicher des aktuellen Milli-Wertes
bool timerIsStarted = false;//flag ob Timer gestartet
bool talktimerIsStarted = false;//flag ob talktimer gestartet ist





void setup() {

  pinMode(BUTTON_STARTMEETING, INPUT_PULLUP);
  pinMode(BUZZERMEETING, OUTPUT);
  pinMode(BUZZERTALKTIME, OUTPUT);
  pinMode(BUTTON_STARTTALKTIME, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);
  
  
 //Setup MeetingTimer 
  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  meetingDisplay.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  meetingDisplay.setBrightness(90);

  //Setup TalktimeTimer 
  byte numDigits_2 = 1;
  byte digitPins_2[] = {24};
  byte segmentPins_2[] = {38, 37, 34, 35, 36, 39, 40, 41};
  bool resistorsOnSegments = true;
  talktimeDisplay.begin(COMMON_CATHODE, numDigits_2, digitPins_2, segmentPins_2);
  talktimeDisplay.setBrightness(90);
  
  Serial.begin(9600);

  
 
}
 

void loop() {

  if(isResetButtonPressed())
  {
    timerIsStarted = false;
    talktimerIsStarted = false;
    meetingDisplay.blank();
    talktimeDisplay.blank();
  }

  if (!timerIsStarted && isStartButtonPressed())
  {
    timerIsStarted = true;
    startpunkt = millis();//speichern des aktuellen Milli-Wertes  
  }
  

  if (timerIsStarted && isTalkTimeButtonPressed())//Starten der TalkTime nur möglich wenn timerIsStarted
  {
    talktimerIsStarted = true;
    talkTimeStart = millis();//speichern des aktuellen Milli-Wertes    
  }

  if(timerIsStarted)
  {
    
    long meetingDurationInSeconds = (millis() - startpunkt) / 1000;//dauer des Meetings ergibt sich aus dem Milliwert minus startpunkt
     
    int restTimeInSeconds = StartValueInSeconds - meetingDurationInSeconds;//übrige Zeit ist der Startwert minus der aktuellen Meetingdauer

    long seconds = restTimeInSeconds % 60;
     
    long minutes = restTimeInSeconds / 60;

    
    meetingDisplay.setNumber(minutes * 100 + seconds, 2);
    
   
    if(restTimeInSeconds <= 0) 
    {
      timerIsStarted = false;
      meetingDisplay.setNumber(0,2);
      tone(BUZZERMEETING, 1000);
      delay(1000);
      noTone(BUZZERMEETING);
      
    }
   if(talktimerIsStarted)
      {
      long durationTalkTimeInSeconds = (millis() - talkTimeStart) / 1000;

      int TalkTimerestTimeInSeconds = TalkTimeStartValueInSeconds - durationTalkTimeInSeconds;

      long TalkTimeMinutes = TalkTimerestTimeInSeconds / 60;

      
      talktimeDisplay.setNumber(TalkTimeMinutes + 1);
        
      

      if(TalkTimerestTimeInSeconds <= 0)
      {
        talktimerIsStarted = false;
        talktimeDisplay.setNumber(0);
        tone(BUZZERTALKTIME, 100);
        delay(1000);
        noTone(BUZZERTALKTIME);
        
      }
    }
    meetingDisplay.refreshDisplay();
    talktimeDisplay.refreshDisplay();
  }
  
}

bool isStartButtonPressed() {
  return digitalRead(BUTTON_STARTMEETING) == 0;
}
bool isTalkTimeButtonPressed(){
  return digitalRead(BUTTON_STARTTALKTIME) == 0;
}
bool isResetButtonPressed(){
  return digitalRead(BUTTON_RESET) == 0;
}
