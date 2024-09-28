/* 
* GameManager.cpp
*
* Created: 2023-11-28 오후 11:15:25
* Author: SON
*/


#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::getInstance() {
	if (instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}


// default constructor
GameManager::GameManager()
{
} //GameManager

// default destructor
GameManager::~GameManager()
{
} //~GameManager

void GameManager::playGame() {
	// 게임 플레이 로직
}