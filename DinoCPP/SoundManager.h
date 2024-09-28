/* 
* SoundManager.h
*
* Created: 2023-11-29 오후 10:23:21
* Author: SON
*/



#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#define C1 523 // 낮은 도

#define C2 C1*2 // 높은 도

#include <avr/io.h>
#include <avr/interrupt.h>



class SoundManager
{
//variables
public:
	enum SFX
	{
		jump,
		gameOver,
	};
	
	static unsigned int noteTimeCounter;
	static unsigned int noteTimeCount;
	
	static volatile unsigned char Timer0Value;
	static unsigned int noteCount;
	static bool isSoundOn; // 소리 ON/OFF
	
protected:
private:

//functions
public:
	static void initialize();
	static void playSFX(SFX sfx);
	
protected:
private:

}; //SoundManager

#endif //__SOUNDMANAGER_H__
