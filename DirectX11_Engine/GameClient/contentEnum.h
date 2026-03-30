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
    IDLE,

    MOVE,
    JUMP,

    // ENEMY_TYPE 별 공격 Flipbook index는 다를 수 있으니
    // ATTACK의 경우 원하는 값을 대입하여 사용합니다.
    // index가 겹치지 않도록 값대입을 합니다.
    DEMON_ATTACK = 3,

    DEAD,


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
