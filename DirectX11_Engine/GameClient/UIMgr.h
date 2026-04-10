#pragma once
#include "imgui/imgui.h"
#include "KeyMgr.h"

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
	
	// MainMenu 클래스에서 Key 제어를 위한 플래그 추가
	bool		m_eventShowOption;
	bool		m_bCloseOptionsRequest;
	
	bool		m_eventShowCredit;
	bool		m_bCloseCreditRequest;
	
	KEY			m_PrevOptionKey;
	KEY			m_PrevCreditKey;
	ImGuiKey	m_PrevOptionImGuiKey;  // m_PrevOptionKey 에 대응하는 ImGuiKey 캐시
	ImGuiKey	m_PrevCreditImGuiKey;  // m_PrevCreditKey 에 대응하는 ImGuiKey 캐시

	UINT		m_OpenFrame;        // Options 팝업이 열린 ImGui 프레임 번호
	UINT		m_CreditOpenFrame;  // Credit 팝업이 열린 ImGui 프레임 번호

	float		BGMvolume;
	float		SFXvolume;

	
public:
	void Init();
	void Progress();    // EditorMgr::Tick() 안 ImGui 프레임에서 호출

	void RenderOptionsWindow();
	void RenderCreditWindow();

	// 외부 클래스에서 UI 창을 열기 위한 함수
	void IsShowOptions(KEY _key);
	void IsShowCredit(KEY _key);
	void ResetOptionKey();  // Level_0 진입 전 PrevOptionKey 초기화
	void ResetCreditKey();  // Level_0 진입 전 PrevCreditKey 초기화


	//=========
	// Get, Set
	//=========
	bool IsFullScreen() { return m_isFullScreen; }
};
