#pragma once

// windows api or ImGui의 윈도우를 만들어서
// 실제 게임에 UI를 표시하는 매니저 클래스입니다.
class UIMgr
	: public Singleton<UIMgr>
{
	SINGLE(UIMgr)
private:
	bool		m_ShowOptions;
	bool		m_ShowCredit;
	bool		m_isFullScreen;
	bool		m_ShowExitConfirm;  // ← 추가: Exit? 팝업 유지 플래그

	UINT		m_OpenFrame;        // 팝업이 열린 ImGui 프레임 번호

	float		BGMvolume;
	float		SFXvolume;


public:
	void Init();
	void Progress();    // EditorMgr::Tick() 안 ImGui 프레임에서 호출

	void RenderOptionsWindow();
	void RenderCreditWindow();



	//=========
	// Get, Set
	//=========
	bool IsFullScreen() { return m_isFullScreen; }
};
