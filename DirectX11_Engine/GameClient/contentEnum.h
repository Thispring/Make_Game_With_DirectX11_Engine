#pragma once
// 엔진 요소 제외, 콘텐츠 용으로 자주 사용될 Enum 값을 정의합니다.


// Player의 상태를 enum class로 정의합니다.
// enum class 순서와 Flipbook vector의 순서는 일치해야합니다.
// CPlayerStateManager의 Begin 시점에 이루어 지는 State 클래스 초기화 순서와도 일치해야 합니다.
enum class PLAYER_STATE
{
    IDLE,

    WALK,
    JUMP,

    PUNCH,
    HIGH_KICK,
    MIDDLE_KICK,
    LOW_KICK,

    ENERGYBLAST_SHOT,

    END,
};

// ENEMY_TYPE에 따라 조건을 두어
// ENEMY_STATE를 반환하는 함수를 만들기
// ENEMY_TYPE과 int 를 파라미터로 받아서
// 해당 TYPE에 int에 맞는 인덱스 반환
// 만약 인덱스 범위가 안맞다면 assert로 크래시
enum class ENEMY_STATE
{
    // ENEMY_STATE는 파생 클래스의 생성자에서 EnemyState의 ENEMY_STATE 멤버값을
    // 변경하게 하는 구조로 되어 있기에, 명시적으로 '타입_상태' 로 네이밍 + EnemyStateManager에 
    // 할당한 index 순서대로 값을 고정 시킵니다.

    // DEMON 타입 상태
    DEMON_IDLE,
    DEMON_MOVE,
    DEMON_JUMP,
    DEMON_ATTACK,
    DEMON_HIT,
    DEMON_DEAD,

    // SKULL 타입 상태
    SKULL_IDLE,
    SKULL_MOVE,
    SKULL_JUMP,
    SKULL_ATTACK,
    SKULL_HIT,
    SKULL_DEAD,

    // FLYING 타입 상태
    FLYING_IDLE,
    FLYING_MOVE,
    FLYING_JUMP,
    FLYING_ATTACK,
    FLYING_HIT,
    FLYING_DEAD,

    // FLOWER 타입 상태
    FLOWER_IDLE,
    FLOWER_MOVE,
    FLOWER_JUMP,
    FLOWER_ATTACK,
    FLOWER_HIT,
    FLOWER_DEAD,

    // BOSS 타입 상태
    BOSS_IDLE,
    BOSS_MOVE,
    BOSS_JUMP,
    BOSS_ATTACK,
    BOSS_HIT,
    BOSS_DEAD,

    END,
};


enum class ENEMY_TYPE
{
    // TYPE 네이밍은 Flipbook 이미지에 따름
    DEMON,
    SKULL,
    FLYING,
    FLOWER,

    BOSS,

    END,
};

// 공통 상태 (모든 적 타입에 공통인 의미)
enum class ENEMY_COMMON_STATE
{
    IDLE = 0,
    MOVE = 1,
    JUMP = 2,
    ATTACK = 3,
    HIT = 4,
    DEAD = 5,
};