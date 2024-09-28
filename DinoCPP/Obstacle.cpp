/* 
* Obstacle.cpp
*
* Created: 2023-11-28 오후 10:21:16
* Author: SON
*/


#include "Obstacle.h"

int Obstacle::obstacleSpeed;
int Obstacle::position;

int Obstacle::obstacleTimer;

int Obstacle::spawnCounter;

int Obstacle::score;

bool Obstacle::enable;

bool Obstacle::collision;

void Obstacle::initialize(){
	// 초기화 로직
	position = 3;
	
	enable = false;
	obstacleTimer = 0;
	
	obstacleSpeed = 0; 
	
	spawnCounter = 0;
	
	score = 0;
	
	collision = false;
}

void Obstacle::spawn(){
	//장애물 생성
	if(enable == false){
		enable = true;
		position = 3;
		
		obstacleTimer = 0;
		
		spawnCounter = 0;
	}
	
}

void Obstacle::update(){
	// 업데이트 로직
	if(enable){
		//활성화 상태라면
		obstacleTimer++;
		
		if(obstacleTimer == (100-obstacleSpeed)){
			//이동 속도 증가
			obstacleTimer = 0;
			move();
			//한 칸 왼쪽으로 이동
		}
	}else{
		//비활성화 상태라면
		spawnCounter++;
		if (spawnCounter == 2*(100-obstacleSpeed))
		{
			//생성 빈도 증가
			spawnCounter = 0;
			spawn();
			//생성
		}
	}
	
}

void Obstacle::move(){
	//왼쪽으로 움직이기
	if (position == 1) {
		//공룡 위치를 통과할 경우
		checkCollision();
		//충돌 감지
	}
	
	if(position ==0){
		//가장 왼쪽 칸에 위치할 경우
		disable();
		//비활성화
	}else{
		//가장 왼쪽 칸이 아니라면
		position--;
		//왼쪽으로 한 칸 이동
	}
	
	
}

void Obstacle::disable(){
	enable = false;
	//비활성화
	obstacleSpeed+=8;
	//장애물을 넘길 때 마다 장애물 속도 증가
	score++;
	//점수 추가
	if(score == 10){
		//10점 획득 시
		collision = true;
		//게임오버를 위해 충돌 처리
		SoundManager::playSFX(SoundManager::gameOver);
		//게임오버 효과 재생
	}
}

void Obstacle::checkCollision() {
	//충돌 감지
	if (Dino::isOnGround() && position == 1) {
		//공룡이 지상에 있다면
		collision = true;
		//충돌 - 게임 오버 처리
		SoundManager::playSFX(SoundManager::gameOver);
		//게임 오버 효과 재생
	}
}

bool Obstacle::isGameOver(){
	return collision;
	//충돌 - 게임 오버 처리
}

unsigned char Obstacle::displaySeg(int segNum){
	// 디스플레이 세그먼트 로직
	unsigned char seg = 0;
	// 비어있는 세그먼트 정보
	if(segNum == position && enable){
		// 자신의 위치에 대한 세그먼트 요청 및 활성화 시
		seg = 0x8C;
		// 세그먼트 정보 덮어씌우기
	}
	return seg;
	// 세그먼트 정보 출력
}