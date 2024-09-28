/* 
* Dino.cpp
*
* Created: 2023-11-28 오후 9:47:01
* Author: SON
*/

#include "Dino.h"

GameObject::Position Dino::dinoPosition;
unsigned int Dino::airTimer;

void Dino::initialize(){
	//초기화
	dinoPosition = ground;
	airTimer = 0;
}
void Dino::update(){
	//업데이트 로직
	if(dinoPosition == air){
		airTimer++;
		if(airTimer == 120){
			airTimer = 0;
			land();
		}
	}
}

void Dino::jump() {
	//지상에 있다면 점프
	if(dinoPosition == ground){
		SoundManager::playSFX(SoundManager::jump);
		dinoPosition = air;
		airTimer = 0;
	}
}

void Dino::land() {
	// 착지
	dinoPosition = ground;
}

bool Dino::isOnGround() {
	//지상에 위치 여부
	return (dinoPosition == ground);
}

unsigned char Dino::displaySeg (int segNum){
	//세그먼트 정보 출력
	unsigned char seg = 0; 
	if(segNum == 1){
		if(dinoPosition == ground){
			//지상에 있다면
			seg |= 0x50;
		}else{
			//공중에 있다면
			seg |= 0x21;
		}
	}
	
	return seg;
}