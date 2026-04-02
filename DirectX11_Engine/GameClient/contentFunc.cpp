#include "pch.h"

ENEMY_STATE GetEnemyStateToParam(ENEMY_TYPE _Type, ENEMY_COMMON_STATE _Common)
{
    // 범위 검증 (컴파일 타임 보장 불가한 경우 런타임 검사)
    int idx = static_cast<int>(_Common);

    assert(idx >= 0 && idx <= 5 && "ENEMY_COMMON_STATE out of range (0..5)");

    switch (_Type)
    {
    case ENEMY_TYPE::DEMON:
        switch (_Common)
        {
        case ENEMY_COMMON_STATE::IDLE:   return ENEMY_STATE::DEMON_IDLE;
        case ENEMY_COMMON_STATE::MOVE:   return ENEMY_STATE::DEMON_MOVE;
        case ENEMY_COMMON_STATE::JUMP:   return ENEMY_STATE::DEMON_JUMP;
        case ENEMY_COMMON_STATE::ATTACK: return ENEMY_STATE::DEMON_ATTACK;
        case ENEMY_COMMON_STATE::HIT:    return ENEMY_STATE::DEMON_HIT;
        case ENEMY_COMMON_STATE::DEAD:   return ENEMY_STATE::DEMON_DEAD;
        }
        break;

    case ENEMY_TYPE::SKULL:
        switch (_Common)
        {
        case ENEMY_COMMON_STATE::IDLE:   return ENEMY_STATE::SKULL_IDLE;
        case ENEMY_COMMON_STATE::MOVE:   return ENEMY_STATE::SKULL_MOVE;
        case ENEMY_COMMON_STATE::JUMP:   return ENEMY_STATE::SKULL_JUMP;
        case ENEMY_COMMON_STATE::ATTACK: return ENEMY_STATE::SKULL_ATTACK;
        case ENEMY_COMMON_STATE::HIT:    return ENEMY_STATE::SKULL_HIT;
        case ENEMY_COMMON_STATE::DEAD:   return ENEMY_STATE::SKULL_DEAD;
        }
        break;

    case ENEMY_TYPE::FLYING:
        switch (_Common)
        {
        case ENEMY_COMMON_STATE::IDLE:   return ENEMY_STATE::FLYING_IDLE;
        case ENEMY_COMMON_STATE::MOVE:   return ENEMY_STATE::FLYING_MOVE;
        case ENEMY_COMMON_STATE::JUMP:   return ENEMY_STATE::FLYING_JUMP;
        case ENEMY_COMMON_STATE::ATTACK: return ENEMY_STATE::FLYING_ATTACK;
        case ENEMY_COMMON_STATE::HIT:    return ENEMY_STATE::FLYING_HIT;
        case ENEMY_COMMON_STATE::DEAD:   return ENEMY_STATE::FLYING_DEAD;
        }
        break;

    case ENEMY_TYPE::FLOWER:
        switch (_Common)
        {
        case ENEMY_COMMON_STATE::IDLE:   return ENEMY_STATE::FLOWER_IDLE;
        case ENEMY_COMMON_STATE::MOVE:   return ENEMY_STATE::FLOWER_MOVE;
        case ENEMY_COMMON_STATE::JUMP:   return ENEMY_STATE::FLOWER_JUMP;
        case ENEMY_COMMON_STATE::ATTACK: return ENEMY_STATE::FLOWER_ATTACK;
        case ENEMY_COMMON_STATE::HIT:    return ENEMY_STATE::FLOWER_HIT;
        case ENEMY_COMMON_STATE::DEAD:   return ENEMY_STATE::FLOWER_DEAD;
        }
        break;

    case ENEMY_TYPE::BOSS:
        switch (_Common)
        {
        case ENEMY_COMMON_STATE::IDLE:   return ENEMY_STATE::BOSS_IDLE;
        case ENEMY_COMMON_STATE::MOVE:   return ENEMY_STATE::BOSS_MOVE;
        case ENEMY_COMMON_STATE::JUMP:   return ENEMY_STATE::BOSS_JUMP;
        case ENEMY_COMMON_STATE::ATTACK: return ENEMY_STATE::BOSS_ATTACK;
        case ENEMY_COMMON_STATE::HIT:    return ENEMY_STATE::BOSS_HIT;
        case ENEMY_COMMON_STATE::DEAD:   return ENEMY_STATE::BOSS_DEAD;
        }
        break;

    default:
        assert(false && "Unknown ENEMY_TYPE");
        break;
    }

    // assert가 활성화된 디버그에서는 여기 도달하지 않음
    return (ENEMY_STATE)0;
}
