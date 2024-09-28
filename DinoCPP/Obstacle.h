/* 
* Obstacle.h
*
* Created: 2023-11-28 오후 10:21:17
* Author: SON
*/


#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "SoundManager.h"
#include "GameObject.h"
#include "Dino.h"

class Obstacle : public GameObject
{
//variables
public:
	static bool enable;
	static int score;
	static int obstacleSpeed;
	
	static int obstacleTimer;
	
	static int spawnCounter;
	
	static int position;
	
	
	
	static bool collision;
	
	

protected:
private:

//functions
public:
	static void spawn();
	static void disable();
	static bool isGameOver();
	static void update();
	static void checkCollision();
	static void initialize();
	static void move();
	static unsigned char displaySeg(int segNum);
protected:
private:

}; //Obstacle

#endif //__OBSTACLE_H__
