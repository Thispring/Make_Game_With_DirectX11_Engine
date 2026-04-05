#pragma once
#include "EditorUI.h"
#include "imgui/imgui_stdlib.h"

class EditorMgr
	: public Singleton<EditorMgr>
{	
	SINGLE(EditorMgr)
private:

	map<string, Ptr<EditorUI>>	m_mapUI;		// map을 이용해 imgui를 이용하는 클래스들을 관리
	bool						m_ShowDemo;		// 데모 창을 보여줄지 여부
	Ptr<EditorUI>				m_FocusedUI;	// 현재 Focus하고 있는 UI
	bool						m_UIVisible;	// ImGui UI 창 표시 여부 (전체화면 시 false)

	vector<Ptr<GameObject>>		m_vecEditorObj; // Editor 모드에서 사용할 오브젝트 모음 (Editor 카메라)
	

	//=================
	// private 멤버 함수
	//=================
	void Tick();
	void Render();
	void CreateEditorUI();
	void CreateEditorObject();

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Progress();
	void AddUI(const string& _UIName, Ptr<EditorUI> _UI);
	Ptr<EditorUI> FindUI(const string& _UIName);
	void RegisterFocusedUI(Ptr<EditorUI> _UI) { m_FocusedUI = _UI; }


	//=========
	// Get, Set
	//=========
	bool IsShowDemo() { return m_ShowDemo; }
	void ShowDemo(bool _Show) { m_ShowDemo = _Show; }

	bool IsUIVisible() const { return m_UIVisible; }
	void SetUIVisible(bool _Visible) { m_UIVisible = _Visible; }
};
