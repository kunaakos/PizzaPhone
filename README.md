##Rotary Alarm Clock

####**Work in progress.**

Vintage **rotary phone** converted to a speaking **alarm clock**. Acts and feels like a ["wake-up call"/"morning call" service](http://en.wikipedia.org/wiki/Wake-up_call_%28service%29) : pick up the receiver, listen to a robot voice, dial in your settings, hang up, get a call when it's time to wake up. Clumsier and less reliable than any mobile phone in existence.

##What was used

#### Arduino

Well, duh. Final hardware will run on a custom-built board (needs to fit), but I'm keeping everything compatible with the Uno R3.

####Catalex Serial MP3 player
Cheapest sound module I could find, some of the documentation is Chinese-only. Easy to use, but has a lot of drawbacks (file/folder names, limited control options, poor sound). Get something else if you can.

####DS3231N based RTC module
Using [my fork](https://github.com/kunaakos/DS3232RTC) of [JChristensen's library](https://github.com/JChristensen/DS3232RTC), I'll see if I can get my code pulled. Added method to read alarm times from RTC.

####Phone Dial
Using [my fork](https://github.com/kunaakos/RotaryDialMM) of [markfickett's library](https://github.com/markfickett/Rotary-Dial). This is *not compatible with the original*, the dials are a bit different and I rewrote a lot of the original library.

####Phone Hook Switch
It's a button. You can replace it with a regular button.

#### Phone Bell
**Not yet implemented.** Using [my own library](https://github.com/kunaakos/PhoneBell), which is a mess. The bell needs at least 9V to function properly, has coils and stuff, so can't connect it directly to the microcontroller. I'm using a cheap motor controller board (it's an H-bridge, basically).