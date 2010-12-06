/*
MP3Trigger.cpp
@author David Wicks
@url	sansumbrella.com
*/

#include "WProgram.h"
#include "NewSoftSerial.h"
#include "MP3Trigger.h"

MP3Trigger::MP3Trigger()
{
	mDoLoop = false;
	mPlaying = false;
}

MP3Trigger::~MP3Trigger()
{
	s->flush();
	s = NULL;
}

void MP3Trigger::setup(NewSoftSerial &serial)
{
	s = &serial;
	s->begin(38400);
}

// 
// Looping functions
// 
void MP3Trigger::setLooping(bool doLoop, byte track)
{
	mDoLoop = doLoop;
	mLoopTrack = track;
	
	if(!mPlaying && mDoLoop)
	{
		loop();
	}
}

void MP3Trigger::setLoopingTrack(byte track)
{
	mLoopTrack = track;
}

void MP3Trigger::update()
{
	if( s->available() )
	{
		int data = s->read();
		if(char(data) == 'X' || char(data) == 'x')
		{
			if(mDoLoop)
			{	
				loop();
			} else
			{
				mPlaying = false;
			}
		} else if(char(data) == 'E')
		{
			mPlaying = false;
		}
	}
}

void MP3Trigger::loop()
{
	trigger(mLoopTrack);
}

void MP3Trigger::stop()
{
	bool wasPlaying = mPlaying;
	mDoLoop = false;
	mPlaying = false;
	
	if(wasPlaying)
	{
		play();
	}
}

// 
// Single-byte built-in functions
// 

void MP3Trigger::play()
{
	s->print('O');
	mPlaying = !mPlaying;
}

void MP3Trigger::forward()
{
	s->print('F');
}

void MP3Trigger::reverse()
{
	s->print('R');
}

//
// Built-in two-byte functions
// 

void MP3Trigger::trigger(byte track)
{
	s->print('t');
	s->print(track);
	mPlaying = true;
}

void MP3Trigger::play(byte track)
{
	s->print('p');
	s->print(track);
	mPlaying = true;
}

void MP3Trigger::setVolume(byte level)
{
	// level = level ^ B11111111;	//flip it around, so the higher number > higher volume
	s->print('v');
	s->print(level);
}

// 
// Response functions
// 

void MP3Trigger::statusRequest()
{
	s->println("MP3Trigger::statusRequest is not yet implemented");
	s->flush();
	s->print('S');
	s->print('1');
	delay(5);
	s->read();
	//will need to work on this one to make it useful
	// if (Serial.available() > 0)
	// {
	// 	// read the incoming byte:
	// 	int incomingByte = Serial.read();
	// 
	// 	// say what you got:
	// 	Serial.print("I received: ");
	// 	Serial.println(incomingByte, DEC);
	// }
}