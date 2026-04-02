#pragma once
#include "contentEnum.h"

// 콘텐츠에 사용되는 전역함수 or 전역 변수를 정의합니다.


// ENEMY_TYPE에 따라 조건을 두어
// ENEMY_STATE를 반환하는 함수를 만들기
// ENEMY_TYPE과 ENEMY_STATE 를 파라미터로 받아서
// 해당 TYPE에 ENEMY_STATE에 맞는 인덱스 반환
// 만약 인덱스 범위가 안맞다면 assert로 크래시
ENEMY_STATE GetEnemyStateToParam(ENEMY_TYPE _Type, ENEMY_STATE _Common);