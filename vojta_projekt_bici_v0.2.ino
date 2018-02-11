/*

verze0.2

Naprogramovány jsou zvuky Hi-hatky a crashe.

*/

//zvuky jsou vybrány pro zvuk. banku Roland GS Drumset

//////////////////////////////////////////   INITIAL VALUE   ////////////////////////////////////////////

//{threshold, sensitivity, note, flag, velocity, peakValue} 
//*** NOTICE *** peakValue and velocity must be zero.
//*** NOTICE *** Except for the notes, the hi-hat should have the same value for open and close.

int SNARE[6] = {150, 950, 38, 3, 0, 0}; //modrý buben
int HIHAT[6] = {100, 1000, 46, 1, 0, 0}; //žlutý činel (vlevo)
int KICK[6] = {200, 700, 36, 1, 0, 0}; //kopák
int CRASH[6] = {100, 800, 49, 1, 0, 0}; //oranžový činel (vpravo)
int TOMTOM[6] = {150, 950, 48, 1, 0, 0}; //červený buben 
int FLOORTOM[6] = {150, 950, 47, 1, 0, 0}; //zelený buben
int SETTING[4] = {4, 500 ,100 ,1};  //{scantime, snare/rim, pedalVelocity , masktime}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

boolean snareFlag = false;
boolean hihatFlag = false;
boolean crashFlag = false;
boolean kickFlag = false;
boolean buttonState = true;
boolean buttonState_set = true;

void setup() {
  MIDI.begin(1);
  Serial.begin(115200);
}

void loop() {

  /////////////////////////////////// CIRCUIT ///////////////////////////////////////

  int piezoValue_hihat = analogRead(A1); //žlutý čínel (hi-hat)
  int piezoValue_crash = analogRead(A0); //oranžový čínel (crash)
  int piezoValue_2 = analogRead(A2); //červený buben (tomtom)
  int piezoValue_3 = analogRead(A3); //modrý buben (snare)
  int piezoValue_4 = analogRead(A4); //zelený buben (floor tom)
  int piezoValue_5 = analogRead(A5); //kopák (kick)

  ////////////////////////////////// SNARE ///////////////////////////////////

  //////////////////////////////// HI-HAT /////////////////////////////////////

 if (piezoValue_hihat > HIHAT[0] && hihatFlag == false) {

    for (int i = 0; i < SETTING[0]; i++) {
      int peak2 = analogRead(A1);
      delay(1);

      if (peak2 > HIHAT[4]) {
        HIHAT[4] = peak2;
      }
    }

    HIHAT[5] = HIHAT[4];
    HIHAT[4] = map(HIHAT[4], HIHAT[0], HIHAT[1], 0, 127);
    HIHAT[4] = (HIHAT[4] * HIHAT[4]) / 127;

    if (HIHAT[4] <= 1) {
      HIHAT[4] = 1;
    }

    if (HIHAT[4] > 127) {
      HIHAT[4] = 127;
    }
    else {
     // MIDI.sendNoteOn(HIHAT[2], HIHAT[4], 10);   //(note, velocity, channel)
       MIDI.sendNoteOn(HIHAT[2], 127, 10);   //(note, velocity, channel)
       MIDI.sendNoteOn(HIHAT[2], 0, 10);
      hihatFlag = true;
    }
  }

   //////////////////////////////// CRASH /////////////////////////////////////

  if (piezoValue_crash > CRASH[0] && crashFlag == false) {

    for (int i = 0; i < SETTING[0]; i++) {
      int peak2 = analogRead(A0);
      delay(1);

      if (peak2 > CRASH[4]) {
        CRASH[4] = peak2;
      }
    }

    CRASH[5] = CRASH[4];
    CRASH[4] = map(CRASH[4], CRASH[0], CRASH[1], 0, 127);
    CRASH[4] = (CRASH[4] * CRASH[4]) / 127;

    if (CRASH[4] <= 1) {
      CRASH[4] = 1;
    }

    if (CRASH[4] > 127) {
      CRASH[4] = 127;
    }
    else {
       MIDI.sendNoteOn(CRASH[2], 127, 10);   //(note, velocity, channel)
       MIDI.sendNoteOn(CRASH[2], 0, 10);
      crashFlag = true;
    }
  }

  //////////////////////////////////// KICK //////////////////////////////////

  ////////////////////////////// FLAG /////////////////////////////

   if (piezoValue_hihat < (HIHAT[5] * (0.01 * HIHAT[3])) && hihatFlag == true) {
    delay(SETTING[3]);
    int sensorValue1 = analogRead(A1);
    if (piezoValue_hihat < (HIHAT[5] * (0.01 * HIHAT[3]))) {
      hihatFlag = false;
    }
  }
     if (piezoValue_crash < (CRASH[5] * (0.01 * HIHAT[3])) && crashFlag == true) {
    delay(SETTING[3]);
    int sensorValue0 = analogRead(A0);
    if (piezoValue_crash < (HIHAT[5] * (0.01 * HIHAT[3]))) {
      crashFlag = false;
    }
  }
  
}
