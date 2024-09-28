/* 
* SoundManager.cpp
*
* Created: 2023-11-29 오후 10:23:21
* Author: SON
*/


#include "SoundManager.h"

volatile unsigned char SoundManager::Timer0Value = 0;
unsigned int SoundManager::noteCount = 0;
unsigned int SoundManager::noteTimeCounter = 0;
unsigned int SoundManager::noteTimeCount = 0;
bool SoundManager::isSoundOn = false;

ISR(TIMER0_OVF_vect){
	TCNT0 = SoundManager::Timer0Value;
	if (SoundManager::isSoundOn) {
		PORTF ^= 0x80; // isSoundOn이 true일 때만 토글
	}
}

ISR(TIMER2_OVF_vect) {
	SoundManager::noteTimeCounter++; // 음의 길이 카운팅

	if (SoundManager::noteTimeCounter == 15) {
		SoundManager::noteTimeCounter = 0; // 카운터 재설정

		if (SoundManager::noteCount  < 1) {
			// 다음 음 재생
			SoundManager::Timer0Value = 255-(1000000/(8*C1)); // 다음 음의 주파수 값 설정
			SoundManager::noteCount++; // 다음 음으로 넘어가기
		} else {
			// 모든 음 재생 완료
			SoundManager::isSoundOn = false; // 소리 재생 중지
			SoundManager::noteCount = 0; // 음 카운터 초기화
		}
	}
}
                    
void SoundManager::initialize(){
	DDRF = 0x80;	//소리 신호 출력 단자
	
	TCCR0 = 0x04;	//분주비 설정
	TCCR2 = 0x04;	
	
	TIMSK = 0x41;	// TOIE0, TOIE2 오버플로우 인터럽트 활성화
	Timer0Value = 0;
	
	
	isSoundOn = false;	// 소리 비활성화
	
	noteCount = 0;
};

void SoundManager::playSFX(SFX sfx){
	// SFX에 따라 시작음 설정

	switch (sfx) {
		case jump:
			noteCount = 1;	// C1 연주 후 중단
			SoundManager::Timer0Value = 255-(1000000/(8*C1));
			break;
		case gameOver:
			noteCount = 0;	// C2 -> C1 연주 후 중단
			SoundManager::Timer0Value = 255-(1000000/(8*C2));
			break;
	}
	isSoundOn = true; // 소리 활성화
}
	
