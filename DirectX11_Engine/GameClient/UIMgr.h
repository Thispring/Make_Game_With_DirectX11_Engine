#pragma once

// ImGui 버튼 하나의 위치·크기·레이블을 보관하는 구조체
struct ImGuiButtonData
{
    int    x, y;
    int    width, height;
    string label;           // UTF-8 (ImGui 전달용)
    bool   clicked = false; // 현재 프레임 클릭 여부
};

// windows api or ImGui의 윈도우를 만들어서
// 실제 게임에 UI를 표시하는 매니저 클래스입니다.
class UIMgr
	: public Singleton<UIMgr>
{
	SINGLE(UIMgr)
private:
	map<int, HWND>              m_mapButtons;       // Win32 버튼 ID → HWND
	map<int, ImGuiButtonData>   m_mapImGuiButtons;  // ImGui 버튼 ID → 데이터

public:
	void Init();
	void Tick();    // EditorMgr::Tick() 안 ImGui 프레임에서 호출

	// ── Win32 버튼 ──────────────────────────────────────────────
	// x, y       : 부모 윈도우 기준 좌상단 좌표 (픽셀)
	// width      : 버튼 너비  (픽셀)
	// height     : 버튼 높이 (픽셀)
	// _label     : 버튼에 표시할 문자열
	// _id        : WM_COMMAND 에서 구분할 버튼 ID
	// 반환값     : 생성된 버튼 HWND (실패 시 nullptr)
	HWND CreateWindowButton(int x, int y, int width, int height,
							const wstring& _label, int _id);
	HWND FindButton(int _id);
	void RemoveButton(int _id);

	// ── ImGui 버튼 ──────────────────────────────────────────────
	// _id        : 버튼 구분 ID (음수 사용 권장, Win32 ID와 충돌 방지)
	// 반환값     : 생성 성공 여부
	bool CreateImGuiButton(int x, int y, int width, int height,
						   const wstring& _label, int _id);
	bool IsImGuiButtonClicked(int _id) const;   // 현재 프레임 클릭 여부 조회
	void RemoveImGuiButton(int _id);
};
