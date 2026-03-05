#pragma once
// 프로젝트에서 생성한 모든 Component들을 모아서 관리

#include "CTransform.h"
#include "CCamera.h"

#include "CRenderComponent.h"
#include "CMeshRender.h"
#include "CSpriteRender.h"
#include "CFlipbookRender.h"
#include "CBillboardRender.h"
#include "CTileRender.h"

#include "CLight2D.h"

#include "CCollider2D.h"

// 다른 Script.h 는 엔진의 기본 요소가 아닌
// 사용자가 추가하는 콘텐츠이기 때문에 components.h에서 분리


// #include "CCollider3D.h"
// #include "CLight3D.h"
// #include "CParticleRender.h"

// #include "CMScript.h"
#include "CScript.h"