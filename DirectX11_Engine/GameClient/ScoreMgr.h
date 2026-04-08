#pragma once

class ScoreMgr
	: public Singleton<ScoreMgr>
{
	SINGLE(ScoreMgr)
private:
	static int		m_Score;
	static int		m_HighScore;

	static constexpr wchar_t HIGHSCORE_FILE[] = L"highscore.dat";

	void LoadHighScore();
	void SaveHighScore();

public:
	void Init();
	void Progress();

	void SaveScore(float _RemainingTime, int _DeathCount);
	void RenderScore();


};

