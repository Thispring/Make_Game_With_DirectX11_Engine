#pragma once

// windows api or ImGui의 윈도우를 만들어서
// 실제 게임에 UI를 표시하는 매니저 클래스입니다.
class UIMgr
	: public Singleton<UIMgr>
{
	SINGLE(UIMgr)
private:

public:
	void Init();
	void Tick();    // EditorMgr::Tick() 안 ImGui 프레임에서 호출

};
