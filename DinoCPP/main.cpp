/*
 * Dinor.c
 * 버튼을 눌러 장애물을 뛰어넘는 간단한 공룡게임 입니다.
 * Created: 2023-11-28 오후 8:35:08
 * Author: SON
 */
#define F_CPU 16000000 //클럭 속도


    
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "SoundManager.h"
#include "GameObject.h"
#include "Dino.h"
#include "Obstacle.h"



void Show2Digit(int n);
void ShowDigit(int i,int digit);
	
void display();
void showCharacters();

const unsigned char Segment_Data[] =	//0~9까지의 세그먼트 정보
{0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};

enum GameMode
{
	MAINGAME,	//게임을 진행합니다.
	GAMEOVER	//게임을 종료하고 점수를 출력합니다.
};

GameMode gameMode;

ISR(INT0_vect){	//푸시 버튼을 누르면 호출됩니다. 
	if(gameMode == MAINGAME){
		if(PIND & 1){
			//버튼 누름
			EICRA &=0;	//버튼을 땔 때 인터럽트가 작동하도록 설정
			Dino::jump();
			}else{
			//버튼 땜
			EICRA |=1;	//버튼을 누를 때 인터럽트가 작동하도록 설정
			Dino::land();
		}
	}
}

int main(void)	//필요한 세팅과 변수를 초기화한 뒤, 게임 루프를 시작합니다. 
{
	// 세븐세그먼트 세팅
	DDRC = 0xff;
	DDRA = 0xff;
	PORTC = 0x00;
	
	// 버튼 인터럽트 세팅
	EICRA = 0x03;
	EIMSK = 0x01;
	SREG |= 0x80; 

	gameMode = MAINGAME;
	SoundManager::initialize();
	Dino::initialize();
	Obstacle::initialize();
    
	SoundManager::playSFX(SoundManager::gameOver);
    while (1) 
    {
		if(gameMode == MAINGAME){
			Dino::update();
			Obstacle::update();
			bool isGameOver = Obstacle::isGameOver();
			if(isGameOver){
				gameMode = GAMEOVER;
			}
				
		}
		display();
    }
}

void display(){	//모드에 따라 필요한 세그먼트를 출력합니다.
	
	if(gameMode == MAINGAME){	//Dino, Obstacle 클래스로부터 출력할 세그먼트를 받아옵니다.
		showCharacters();
	}else{
		Show2Digit(Obstacle::score);	//게임모드가 GAMEOVER이므로, 점수를 출력합니다.
	}
	
}

void showCharacters(){
	unsigned char segment;
	for(int i = 0; i<4;i++){	//네 7세그먼트에서 각각 출력할 내용을 bitwise OR 연산으로 겹쳐 출력합니다.
		segment = 0;
		segment |= Dino::displaySeg(i);
		segment |= Obstacle::displaySeg(i);
		
		PORTC=~(1<<i);		//출력할 7세그먼트 옯기기
		
		PORTA = segment;	//출력
		
		_delay_ms(2);
	}	
};
	
void Show2Digit(int n) {	//예제의 함수를 약간 변형하여 2자리 수를 출력합니다.
	unsigned int COLUMN[2]={0,0};
	
	COLUMN[0] = (n%100)/10;
	COLUMN[1] = (n%10);
	
	for(int i=0;i<2;i++) {
		ShowDigit(COLUMN[i],i+2);
	
		_delay_ms(2);// wait for a second
	}
}

void ShowDigit(int i,int digit) {
	PORTC=~(0x01<<digit);
	PORTA=Segment_Data[i];
}
