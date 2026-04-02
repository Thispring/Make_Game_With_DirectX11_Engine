#pragma once
// 엔진 요소 X, 콘텐츠 용으로 자주 사용될 Enum 값을 정의합니다.


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
//enum class ENEMY_STATE
//{
//    // ENEMY_STATE는 파생 클래스의 생성자에서 EnemyState의 ENEMY_STATE 멤버값을
//    // 변경하게 하는 구조로 되어 있기에, 명시적으로 '타입_상태' 로 네이밍 + EnemyStateManager에 
//    // 할당한 index 순서대로 값을 고정 시킵니다.
//
//    // DEMON 타입 상태
//    DEMON_IDLE = 0,
//    DEMON_MOVE = 1,
//    DEMON_JUMP = 2,
//    DEMON_ATTACK = 3,
//    DEMON_HIT = 4,
//    DEMON_DEAD = 5,
//
//    // SKULL 타입 상태
//    SKULL_IDLE = 0,
//    SKULL_MOVE = 1,
//    SKULL_JUMP = 2,
//    SKULL_ATTACK = 3,
//    SKULL_HIT = 4,
//    SKULL_DEAD = 5,
//
//    // FLYING 타입 상태
//    FLYING_IDLE = 0,
//    FLYING_MOVE = 1,
//    FLYING_JUMP = 2,
//    FLYING_ATTACK = 3,
//    FLYING_HIT = 4,
//    FLYING_DEAD = 5,
//
//    // FLOWER 타입 상태
//    FLOWER_IDLE = 0,
//    FLOWER_MOVE = 1,
//    FLOWER_JUMP = 2,
//    FLOWER_ATTACK = 3,
//    FLOWER_HIT = 4,
//    FLOWER_DEAD = 5,
//
//    // BOSS 타입 상태
//    BOSS_IDLE = 0,
//    BOSS_MOVE = 1,
//    BOSS_JUMP = 2,
//    BOSS_ATTACK = 3,
//    BOSS_HIT = 4,
//    BOSS_DEAD = 5,
//
//    END,
//};

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
};


enum class ENEMY_FLIPBOOK
{
    // 게임오브젝트에 직접 할당한
    // Flipbook 순서대로 enum class 번호를 부여합니다.

    // DEMON 타입 상태
    DEMON_IDLE = 0,
    DEMON_MOVE = 1,
    DEMON_JUMP = 2,
    DEMON_ATTACK = 3,
    DEMON_HIT = 4,
    DEMON_DEAD = 5,

    // SKULL 타입 상태
    SKULL_IDLE = 0,
    SKULL_MOVE = 1,
    SKULL_ATTACK = 2,
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

    END,
};

// 기존 enum class ENEMY_FLIPBOOK 전체를 삭제하고 아래로 교체

// 게임오브젝트에 직접 할당한 Flipbook 순서대로 번호를 부여합니다.
// 타입별로 namespace가 분리되어 있으므로, 번호 충돌이 없습니다.
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
    }

    namespace FLYING
    {
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int JUMP = 2;
        constexpr int ATTACK = 3;
        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }

    namespace FLOWER
    {
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int JUMP = 2;
        constexpr int ATTACK = 3;
        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }

    namespace BOSS
    {
        constexpr int IDLE = 0;
        constexpr int MOVE = 1;
        constexpr int JUMP = 2;
        constexpr int ATTACK = 3;
        constexpr int HIT = 4;
        constexpr int DEAD = 5;
    }
}