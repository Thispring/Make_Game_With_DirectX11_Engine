#include "pch.h"
#include "CUIOverlayController.h"

#include "GameObject.h"

#include "GameMgr.h"
#include "LevelMgr.h"
#include "KeyMgr.h"
#include "CPlayerController.h"
#include "CPlayerData.h"
#include "CMeshRender.h"
#include "AMaterial.h"

CUIOverlayController::CUIOverlayController()
	: CScript(SCRIPT_TYPE::UIOVERLAYCONTROLLER)
	, m_bInit(false)
{
}

CUIOverlayController::~CUIOverlayController()
{
}

void CUIOverlayController::ChangeKeyUI()
{
	if (m_vecUIObject.size() < 3)
		return;

	Ptr<GameObject> pPlayer = GameMgr::GetInst()->GetPlayer();
	if (pPlayer == nullptr) return;

	Ptr<CPlayerController> pController = pPlayer->GetScript<CPlayerController>();
	Ptr<CPlayerData> pData = pPlayer->GetScript<CPlayerData>();
	if (pController == nullptr || pData == nullptr) return;

	// m_vecUIObject[0] -> Punch, [1] -> Kick, [2] -> BlastShot
	KEY keys[3] = { pController->GetPunchKey(), pController->GetKickKey(), pController->GetBlastShotKey() };

	for (int i = 0; i < 3; ++i)
	{
		Ptr<ASprite> pSprite = m_vecUIObject[i]->SpriteRender()->GetSprite();
		float fAtlasW = pSprite->GetAtlas()->GetWidth();
		float fAtlasH = pSprite->GetAtlas()->GetHeight();

		// 키 셀은 아틀라스에서 16×16px 고정
		// SliceUV = BackgroundUV = 셀 크기, OffsetUV = 0으로 맞춰 렌더 결과를 일정하게 유지
		const Vec2 vCellUV   = Vec2(16.f / fAtlasW, 16.f / fAtlasH);
		const Vec2 vZeroUV   = Vec2(0.f, 0.f);
		pSprite->SetSliceUV(vCellUV);
		pSprite->SetBackgroundUV(vCellUV);
		pSprite->SetOffsetUV(vZeroUV);

		Vec2 uv;

		if (pData->GetIsDead())
		{
			uv = Vec2(416.f / fAtlasW, 16.f / fAtlasH);
			m_bInit = true;
		}
		else if (!pController->HasDied() && !m_bInit)
		{
			float u = (float)((int)keys[i]) * 16.f / fAtlasW;
			float v = (KEY_TAP(keys[i]) || KEY_PRESSED(keys[i])) ? 0.f : 16.f / fAtlasH;
			uv = Vec2(u, v);
		}
		else
		{
			if (!pController->IsKeyRevealed(i))
			{
				uv = Vec2(416.f / fAtlasW, 16.f / fAtlasH);
			}
			else
			{
				float u = (float)((int)keys[i]) * 16.f / fAtlasW;
				float v = (KEY_TAP(keys[i]) || KEY_PRESSED(keys[i])) ? 0.f : 16.f / fAtlasH;
				uv = Vec2(u, v);
			}
		}

		pSprite->SetLeftTopUV(uv);
	}
}

void CUIOverlayController::UpdateMoveKeyUI()
{
	if (m_vecUIObject.size() < 8)
		return;

	// 이동키 매핑 테이블 (자식 인덱스 3~7 대응)
	struct MoveKeyInfo
	{
		KEY  key;
		Vec2 vPressedPixel;  // 입력 시 LeftTop 픽셀 좌표
		Vec2 vReleasedPixel; // 입력 없을 때 LeftTop 픽셀 좌표
		Vec2 vCellPx;        // 셀 크기(픽셀) — 키마다 다를 수 있음
	};

	const MoveKeyInfo keyInfos[5] =
	{
		{ KEY::SPACE, Vec2(48.f, 32.f), Vec2(48.f, 16.f), Vec2(48.f, 16.f) }, // [3] Jump (Space) — 48×16 셀
		{ KEY::UP,    Vec2(16.f,  0.f), Vec2(16.f, 32.f), Vec2(16.f, 16.f) }, // [4] Up
		{ KEY::LEFT,  Vec2( 0.f, 16.f), Vec2( 0.f, 48.f), Vec2(16.f, 16.f) }, // [5] Left
		{ KEY::DOWN,  Vec2(16.f, 16.f), Vec2(16.f, 48.f), Vec2(16.f, 16.f) }, // [6] Down
		{ KEY::RIGHT, Vec2(32.f, 16.f), Vec2(32.f, 48.f), Vec2(16.f, 16.f) }, // [7] Right
	};

	for (int i = 0; i < 5; ++i)
	{
		int objIdx = i + 3;
		Ptr<ASprite> pSprite = m_vecUIObject[objIdx]->SpriteRender()->GetSprite();
		float fAtlasW = pSprite->GetAtlas()->GetWidth();
		float fAtlasH = pSprite->GetAtlas()->GetHeight();

		// 키별 셀 크기로 SliceUV = BackgroundUV 설정, OffsetUV = 0으로 렌더 결과 고정
		const Vec2 vCellUV = Vec2(keyInfos[i].vCellPx.x / fAtlasW, keyInfos[i].vCellPx.y / fAtlasH);
		pSprite->SetSliceUV(vCellUV);
		pSprite->SetBackgroundUV(vCellUV);
		pSprite->SetOffsetUV(Vec2(0.f, 0.f));

		bool bPressed = KEY_TAP(keyInfos[i].key) || KEY_PRESSED(keyInfos[i].key);
		const Vec2& vPixel = bPressed ? keyInfos[i].vPressedPixel : keyInfos[i].vReleasedPixel;
		pSprite->SetLeftTopUV(Vec2(vPixel.x / fAtlasW, vPixel.y / fAtlasH));
	}
}

void CUIOverlayController::Begin()
{
	Vec3 vCamPos = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetRelativePos();

	m_vecUIObject = GetOwner()->GetChild();

	m_vecUIOffset.resize(m_vecUIObject.size());
	for (int i = 0; i < (int)m_vecUIObject.size(); i++)
	{
		if (i < 3)
		{
			// Key UI 슬롯(0~2): 초기 위치·스케일 확정 후 오프셋 계산
			float posX = (i * 73.f) + (-4673.f);
			m_vecUIObject[i]->Transform()->SetRelativePos(Vec3(posX, -75.f, 100.f));
			m_vecUIObject[i]->Transform()->SetRelativeScale(Vec3(90.f, 90.f, 1.f));
			m_vecUIOffset[i] = Vec3(posX - vCamPos.x, -75.f - vCamPos.y, 0.f);
		}
		else if (i >= 3 && i < 8)
		{
			 //이동키 UI 슬롯(3~7): 위치·스케일은 직접 수정 후 주석 해제
			 Vec3 vMoveKeyPos[5] =
			 {
			     Vec3(-4300.f, -85.f, 100.f),  // [3] Jump (Space)
			     Vec3(-4070.f, -20.f, 100.f),  // [4] Up
			     Vec3(-4130.f, -85.f, 100.f),  // [5] Left
			     Vec3(-4070.f, -85.f, 100.f),  // [6] Down
			     Vec3(-4010.f, -85.f, 100.f),  // [7] Right
			 };
			 Vec3 vSpaceScale = Vec3(224.f, 64.f, 1.f);
			 Vec3 vArrowScale = Vec3(64.f, 64.f, 1.f);
			 m_vecUIObject[i]->Transform()->SetRelativePos(vMoveKeyPos[i - 3]);
			 m_vecUIObject[3]->Transform()->SetRelativeScale(vSpaceScale);
			 m_vecUIObject[4]->Transform()->SetRelativeScale(vArrowScale);
			 m_vecUIObject[5]->Transform()->SetRelativeScale(vArrowScale);
			 m_vecUIObject[6]->Transform()->SetRelativeScale(vArrowScale);
			 m_vecUIObject[7]->Transform()->SetRelativeScale(vArrowScale);
			 m_vecUIOffset[i] = Vec3(vMoveKeyPos[i - 3].x - vCamPos.x, vMoveKeyPos[i - 3].y - vCamPos.y, 0.f);

			// 주석 해제 전 — 오브젝트 현재 위치 기준 오프셋 유지
			//Vec3 vUIPos = m_vecUIObject[i]->Transform()->GetRelativePos();
			//m_vecUIOffset[i] = Vec3(vUIPos.x - vCamPos.x, vUIPos.y - vCamPos.y, 0.f);
		}
		else if (i >= 8 && i <= 9)
		{
			Vec3 vHPPos = Vec3(-4600.f, 20.f, 100.f);
			Vec3 vHPScale = Vec3(70.f, 70.f, 1.f);
			m_vecUIObject[i]->Transform()->SetRelativePos(vHPPos);
			m_vecUIObject[i]->Transform()->SetRelativeScale(vHPScale);
			//m_vecUIOffset[i] = Vec3(vHPPos.x - vCamPos.x, vHPPos.y - vCamPos.y, 0.f);

			// 주석 해제 전 — 오브젝트 현재 위치 기준 오프셋 유지
			Vec3 vUIPos = m_vecUIObject[i]->Transform()->GetRelativePos();
			m_vecUIOffset[i] = Vec3(vUIPos.x - vCamPos.x, vUIPos.y - vCamPos.y, 0.f);
		}
		else
		{
			// 나머지 UI: 기존 위치 기준 오프셋 계산
			Vec3 vUIPos = m_vecUIObject[i]->Transform()->GetRelativePos();
			m_vecUIOffset[i] = Vec3(vUIPos.x - vCamPos.x, vUIPos.y - vCamPos.y, 0.f);
		}
	}

	// 자식 8, 9번은 HP 바 MeshRender — 공유 재질 오염을 막기 위해 동적 재질 생성
	for (int i = 8; i < 9; ++i)
	{
		if ((int)m_vecUIObject.size() <= i) break;
		Ptr<CMeshRender> pMesh = m_vecUIObject[i]->MeshRender();
		if (pMesh != nullptr)
			pMesh->CreateDynamicMaterial();
	}
}

void CUIOverlayController::Tick()
{
	Vec3 vCamPos = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetRelativePos();

	for (int i = 0; i < (int)m_vecUIObject.size(); i++)
	{
		// 0~2 포함 전체 오브젝트 동일하게 카메라 오프셋 추적
		float fZ = m_vecUIObject[i]->Transform()->GetRelativePos().z;
		m_vecUIObject[i]->Transform()->SetRelativePos(
			Vec3(vCamPos.x + m_vecUIOffset[i].x,
				 vCamPos.y + m_vecUIOffset[i].y,
				 fZ));
	}

	// Key UI 스프라이트 업데이트
	ChangeKeyUI();

	// 이동키 UI 스프라이트 업데이트
	UpdateMoveKeyUI();

	// HP 바 Fill 갱신
	UpdateHPBarUI();
}

void CUIOverlayController::UpdateHPBarUI()
{
	// 자식 오브젝트가 인덱스 9번까지 없으면 처리 불필요
	if ((int)m_vecUIObject.size() < 10)
		return;

	Ptr<GameObject> pPlayer = GameMgr::GetInst()->GetPlayer();
	if (pPlayer == nullptr) return;

	Ptr<CPlayerData> pData = pPlayer->GetScript<CPlayerData>();
	if (pData == nullptr) return;

	float fFullHP = pData->GetFullHP();
	if (fFullHP <= 0.f) return;

	// HP 비율 [0, 1] 범위로 고정
	// 1.0 = 풀 HP (전체 표시), 0.0 = HP 없음 (전체 discard)
	float fFill = pData->GetCurHP() / fFullHP;
	if (fFill < 0.f) fFill = 0.f;
	if (fFill > 1.f) fFill = 1.f;

	for (int i = 8; i < 9; ++i)
	{
		Ptr<CMeshRender> pMeshRender = m_vecUIObject[i]->MeshRender();
		if (pMeshRender == nullptr) continue;

		Ptr<AMaterial> pMtrl = pMeshRender->GetMaterial();
		if (pMtrl == nullptr) continue;

		// g_float_0 (FILL) 에 HP 비율 전달 → std2d.fx 에서 오른쪽부터 discard
		pMtrl->SetScalar(FLOAT_0, fFill);
	}
}

void CUIOverlayController::SaveToLevelFile(FILE* _File)
{
}

void CUIOverlayController::LoadFromLevelFile(FILE* _File)
{
}

