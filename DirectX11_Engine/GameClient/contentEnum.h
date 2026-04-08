#pragma once
// 엔진 요소 X, 콘텐츠 용으로 자주 사용될 Enum 값을 정의합니다.

/***************************************************************************************
* NOTE(26-04-03):
* 기존 enum class ENEMY_FLIPBOOK 전체를 삭제하고 아래로 교체
*
* 게임오브젝트에 직접 할당한 Flipbook 순서대로 번호를 부여합니다.
* 타입별로 namespace가 분리되어 있으므로, 번호 충돌이 없습니다.
* 설계 의도는 코드의 가독성을 높이면서, 공통으로 사용되는 정수값을 할당할 수 있어야 하며,
* 자료구조로 관리 될 수 있어야 하기에 namespace 분리와 constexpr int를 사용했습니다.
*
* constexpr는 “컴파일 시점에 값(또는 계산)을 만들 수 있다”는 의미의 C++ 키워드입니다. 
* constexpr int는 그 변수가 상수 식(constant expression)으로 평가될 수 있음을 보장합니다. 
***************************************************************************************/
namespace FLIPBOOK
{
    namespace DEMON
    {
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int JUMP = 2;
        constexpr int ATTACK = 3;
        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }

    namespace SKULL
    {
        // SKULL은 JUMP가 없으므로 ATTACK부터 순번이 당겨집니다.
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int ATTACK = 2;
        constexpr int HIT = 3;   // 기존 4 → 3으로 수정
        constexpr int DEAD = 4;   // 기존 5 → 4로 수정

        constexpr int GHOST_SKULL = 5; 
        constexpr int GHOST_SKULL_MOVE = 6; 
    }

    namespace FLYING
    {
        constexpr int IDLE = 0;
        constexpr int ATTACK = 1;
        constexpr int HIT = 2;
        constexpr int DEAD = 3;
    }

    namespace FLOWER
    {
        constexpr int IDLE = 0;
        constexpr int JUMP = 1;
        constexpr int MELEE_ATTACK = 2;
        constexpr int RANGED_ATTACK = 3;
        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }

    namespace BOSS
    {
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int ATTACK_TORNADO_WHIRL = 2;
        constexpr int ATTACK_TORPEDO_WHIRL = 3;

        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }

    namespace PLAYER
    {
        constexpr int IDLE             = 0;
        constexpr int WALK             = 1;
        constexpr int JUMP             = 2;
        constexpr int PUNCH            = 3;
        constexpr int HIGH_KICK        = 4;
        constexpr int MIDDLE_KICK      = 5;
        constexpr int LOW_KICK         = 6;
        constexpr int ENERGYBLAST_SHOT = 7;
        constexpr int DEATH            = 8;
    }
}


//========
// PLAYER
//========
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

    DEATH,

    END,
};


//=======
// ENEMY
//=======
enum class ENEMY_TYPE
{
    // TYPE 네이밍은 Flipbook 이미지에 따름
    DEMON,
    SKULL,
    FLYING,
    FLOWER,

    BOSS,

    GHOST_SKULL,

    TEST,   // 테스트용

    END,
};

// 공통 상태 (모든 적 타입에 공통인 의미)
enum class ENEMY_STATE
{
    IDLE = 0,
    MOVE = 1,
    JUMP = 2,
    ATTACK = 3,
    HIT = 4,
    DEAD = 5,

    // NOTE(26-04-01):
    // 정찰 및 추적 상태 추가
    // 사용 시, 인덱스 번호 주의
    PATROL = 6,
    CHASE = 7,

    GHOST_SKULL = 8,
    GHOST_SKULL_MOVE = 9,

    RANGED_ATTACK = 10,
};


//=========================
// Level_0 Layer 이름 재정의
//=========================
enum class LEVEL_0_LAYER
{
    DEFAULT,
    BACKGROUND,
    TILE,

    PLAYER,
    PLAYER_MELEE_TRIGGER,
    PLAYER_PROJECTILE,

    ENEMY,
    ENEMY_MELEE_TRIGGER,
    ENEMY_PROJECTILE,
    ENEMY_EYES,
    ENEMY_PROJECT_ANCHOR,
    ENEMY_SPAWNER,
    ENEMY_WALL_COLLIDER,

    PORTAL = 14,
    SAVE_POINT = 15,
    BACK_GROUND_COLLIDER = 16,
    WALL_COLLIDER = 17,         
    OUT_OF_BOUNDS = 18,         // 추락 방지용 콜라이더 Layer

    DISABLE = 30,
    UI = 31,
    
};