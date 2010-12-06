#include <MP3Trigger.h>
#include <NewSoftSerial.h>

NewSoftSerial softSerial(2, 3);
MP3Trigger trigger;

void setup() {
//start serial communication with the trigger (over Serial)
trigger.setup(softSerial);

//start looping TRACK001.MP3
trigger.setLooping(true,1);
}

void loop()
{
//necessary to receive signals from trigger
trigger.update();
}