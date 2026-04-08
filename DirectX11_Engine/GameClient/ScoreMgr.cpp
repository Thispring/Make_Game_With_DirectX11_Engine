#include "pch.h"
#include "ScoreMgr.h"

#include "FontMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "Engine.h"

int ScoreMgr::m_Score = 0;
int ScoreMgr::m_HighScore = 0;

ScoreMgr::ScoreMgr()
{

}

ScoreMgr::~ScoreMgr()
{
	// 프로그램 종료 시 자동 저장
	SaveHighScore();
}

void ScoreMgr::LoadHighScore()
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, HIGHSCORE_FILE, L"rb");

	if (pFile == nullptr)
	{
		// 파일 없으면 최초 실행 → 0으로 유지
		m_HighScore = 0;
		return;
	}

	fread(&m_HighScore, sizeof(int), 1, pFile);
	fclose(pFile);
}

void ScoreMgr::SaveHighScore()
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, HIGHSCORE_FILE, L"wb");

	if (pFile == nullptr)
		return;

	fwrite(&m_HighScore, sizeof(int), 1, pFile);
	fclose(pFile);
}


void ScoreMgr::Init()
{
	// 프로그램 시작 시 저장된 HighScore 불러오기
	LoadHighScore();
}

void ScoreMgr::Progress()
{
	RenderScore();
}

void ScoreMgr::SaveScore(float _RemainingTime, int _DeathCount)
{
	// 게임 Ending Level에서 호출 or Ending Level 전환 전에
	// 남은 시간과 플레이어 DeathCount에 비례해서 점수 계산
	// 
	// 점수 계산 공식:
	// GoalTime이 많이 남아 있을 수록 빨리 클리어 했다는 것이므로 높은 점수
	// DeathCount가 적을 수록, 적게 죽은 것이므로 높은 점수
	// 1. 시간 점수 계산 (50% 비중)
	float timeRatio = _RemainingTime;
	int timeScore = static_cast<int>(timeRatio * 5000);

	// 2. 생존 점수 계산 (50% 비중) - 고정 차감 방식 예시
	int deathPenalty = 1000;
	int survivalScore = 4999 - (_DeathCount * deathPenalty);
	// 10번 이상 죽었을 시, 생존 보너스 X
	if (survivalScore < 10) survivalScore = 0;

	// 3. 최종 합산 및 클램핑
	int finalScore = timeScore + survivalScore;
	m_Score = finalScore;

	if (m_Score > m_HighScore)
	{
		m_HighScore = m_Score;
		SaveHighScore();
	}
}

void ScoreMgr::RenderScore()
{
	// 최고 점수가 아직 기록되지 않았고,
	// MainMenu가 아니라면 렌더 X
	if (m_HighScore <= 0)
		return;

	//// 메인 카메라를 통해 월드 좌표 → 스크린 픽셀 좌표 변환
	//Ptr<CCamera> pCam = RenderMgr::GetInst()->GetPOVCamera();
	//if (pCam == nullptr)
	//	return;

	//Vec2 screenPos = pCam->WorldToScreenPos(Vec3(100.f, 1000.f, 0.f));

	// 해상도에 비례한 고정 UI 위치 (좌상단에서 5%, 10% 지점)
	Vec2 res = Engine::GetInst()->GetResolution();
	float posX = res.x * 0.05f;   // 화면 너비의 5%
	float posY = res.y * 0.25f;   // 화면 높이의 10%

	wstring score = L"HighScore:  " + to_wstring(m_HighScore);

	FontMgr::GetInst()->DrawFontOutline(score.c_str(),
		//screenPos.x, screenPos.y, 96.f,
		posX, posY, 48.f,
		FONT_RGBA(255, 255, 0, 255),
		FONT_RGBA(0, 0, 0, 255),
		1.5f);
}

