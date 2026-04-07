#include "pch.h"
#include "CMainMenuController.h"

#include "GameObject.h"
#include "CSpriteRender.h"

#include "TimeMgr.h"
#include "RandomMgr.h"
#include "LevelMgr.h"

CMainMenuController::CMainMenuController()
	: CScript(SCRIPT_TYPE::MAINMENUCONTROLLER)
	, m_Time(0.f)
	, m_StartKey((KEY)0)
	, m_OptionKey((KEY)0)
	, m_CreditKey((KEY)0)
{
}

CMainMenuController::~CMainMenuController()
{
}

void CMainMenuController::Start(KEY _Key)
{
	if (KEY_PRESSED(_Key))
	{
		// Level 변경 및 시작
		LevelMgr::GetInst()->GameStart();
	}
}

void CMainMenuController::Option(KEY _Key)
{
	if (KEY_PRESSED(_Key))
	{
		// Option 창 활성화
	}
}

void CMainMenuController::Credit(KEY _Key)
{
	if (KEY_PRESSED(_Key))
	{
		// Credit 창 활성화
	}
}

void CMainMenuController::ShuffleKey()
{
	KEY arryKey[3] = {};

	RandomMgr::GetInst()->ShuffleKeyNum();

	for (UINT i = 0; i < 3; ++i)
	{
		arryKey[i] = RandomMgr::GetInst()->GetRandomKey(3);
	}

	m_StartKey = arryKey[0];
	m_OptionKey = arryKey[1];	
	m_CreditKey = arryKey[2];
	return;
}

void CMainMenuController::ChangeKeyUI()
{
	vector<Ptr<GameObject>> vecChild = GetOwner()->GetChild();
	if (vecChild.size() < 3)
		return;

	// vecChild[0] -> Start, [1] -> Option, [2] -> Credit
	KEY keys[3] = { m_StartKey, m_OptionKey, m_CreditKey };

	for (int i = 0; i < 3; ++i)
	{
		Ptr<ASprite> pSprite = vecChild[i]->SpriteRender()->GetSprite();
		float fAtlasW = pSprite->GetAtlas()->GetWidth();
		float fAtlasH = pSprite->GetAtlas()->GetHeight();

		// 키 셀은 아틀라스에서 16×16px 고정
		const Vec2 vCellUV = Vec2(16.f / fAtlasW, 16.f / fAtlasH);
		const Vec2 vZeroUV = Vec2(0.f, 0.f);
		pSprite->SetSliceUV(vCellUV);
		pSprite->SetBackgroundUV(vCellUV);
		pSprite->SetOffsetUV(vZeroUV);

		float u = (float)((int)keys[i]) * 16.f / fAtlasW;
		float v = (KEY_TAP(keys[i]) || KEY_PRESSED(keys[i])) ? 0.f : 16.f / fAtlasH;
		pSprite->SetLeftTopUV(Vec2(u, v));
	}
}

void CMainMenuController::Begin()
{
	m_StartKey = KEY::A;
	m_OptionKey = KEY::B;
	m_CreditKey = KEY::C;


	/*********************************************************************
	* NOTE(26-04-06): 자식들이 같은 Sprite를 가리키고 있어서 생긴 문제
	* 공유 에셋으로 변경했지만, 추후 여기에서 문제가 생기면, SprtieRender의
	* 공유 제질 관련 함수, Clone 지우고, 각자 다른 Sprite를 가리키게 설정하기
	*********************************************************************/
	// 자식 0~2 슬롯의 공유 ASprite를 독립 복사본으로 교체 — 공유 에셋 오염 방지
	vector<Ptr<GameObject>> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < 3 && i < (int)vecChild.size(); ++i)
	{
		if (vecChild[i]->SpriteRender() != nullptr)
			vecChild[i]->SpriteRender()->CreateDynamicSprite();
	}
}

void CMainMenuController::Tick()
{
	Start(m_StartKey);
	Option(m_OptionKey);
	Credit(m_CreditKey);

	ChangeKeyUI();

	// 게임 시작 후, 5초마다 KEY를 RandMgr을 통해
	// 무작위로 반환
	m_Time += DT;

	if (m_Time >= 5.f)
	{
		ShuffleKey();
		m_Time = 0;
		return;
	}

}

void CMainMenuController::SaveToLevelFile(FILE* _File)
{
}

void CMainMenuController::LoadFromLevelFile(FILE* _File)
{
}
