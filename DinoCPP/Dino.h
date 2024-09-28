/* 
* Dino.h
*
* Created: 2023-11-28 오후 9:47:01
* Author: SON
*/

#include "GameObject.h"
#include "SoundManager.h"

#ifndef __DINO_H__
#define __DINO_H__


class Dino : public GameObject
{
//variables
public:
	
	static unsigned int airTimer;
private:
	static Position dinoPosition;
protected:
//functions
public:
	
	
	static void update();
	static void jump();
	static void land();
	static bool isOnGround();
	static void initialize();
	static unsigned char displaySeg(int segNum);
protected:
private:

}; //Dino

#endif //__DINO_H__
