#pragma once

extern const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END];

const char* ToString(ASSET_TYPE _Type);

// 중력 가속도 정의
// 상수임을 명시하며 타입을 강제함
constexpr float GRAVITY_CONSTANT = 980.0f;