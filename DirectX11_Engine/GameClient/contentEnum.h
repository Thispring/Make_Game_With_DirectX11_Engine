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

enum class ENEMY_STATE
{
    // ENEMY_STATE는 파생 클래스의 생성자에서 EnemyState의 ENEMY_STATE 멤버값을
    // 변경하게 하는 구조로 되어 있기에, 명시적으로 '타입_상태' 로 네이밍 + EnemyStateManager에 
    // 할당한 index 순서대로 값을 고정 시킵니다.

    // DEMON 타입 상태 (인덱스는 EnemyStateManager의 등록 순서와 일치)
    DEMON_IDLE = 0,
    DEMON_MOVE = 1,
    DEMON_JUMP = 2,
    DEMON_ATTACK = 3,
    DEMON_HIT = 4,
    DEMON_DEAD = 5,

    // SKULL 타입 상태 (같은 인덱스 규칙을 사용)
    SKULL_IDLE = 0,
    SKULL_MOVE = 1,
    SKULL_JUMP = 2,
    SKULL_ATTACK = 3,
    SKULL_HIT = 4,
    SKULL_DEAD = 5,

    // FLYING 타입 상태
    FLYING_IDLE = 0,
    FLYING_MOVE = 1,
    FLYING_JUMP = 2,
    FLYING_ATTACK = 3,
    FLYING_HIT = 4,
    FLYING_DEAD = 5,

    // FLOWER 타입 상태
    FLOWER_IDLE = 0,
    FLOWER_MOVE = 1,
    FLOWER_JUMP = 2,
    FLOWER_ATTACK = 3,
    FLOWER_HIT = 4,
    FLOWER_DEAD = 5,

    // BOSS 타입 상태
    BOSS_IDLE = 0,
    BOSS_MOVE = 1,
    BOSS_JUMP = 2,
    BOSS_ATTACK = 3,
    BOSS_HIT = 4,
    BOSS_DEAD = 5,

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
