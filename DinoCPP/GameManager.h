/* 
* GameManager.h
*
* Created: 2023-11-28 오후 11:15:25
* Author: SON
*/


#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__


class GameManager
{
//variables
public:
protected:
private:
	static GameManager* instance;
	GameManager();
//functions
public:
	GameManager(GameManager const&)= delete;
	void operator=(GameManager const&) = delete;
	
	static GameManager* getInstance();
	
	void playGame();
	
	~GameManager();
protected:
private:
	GameManager( const GameManager &c );
	GameManager& operator=( const GameManager &c );

}; //GameManager


#endif //__GAMEMANAGER_H__
