#pragma once
#include "CCamera.h"
#include "GameObject.h"
#include "StructuredBuffer.h"

class RenderMgr
	: public Singleton<RenderMgr>
{
	SINGLE(RenderMgr);
private:
	Ptr<CCamera>			m_MainCam;
	Ptr<CCamera>			m_EditorCam;				// Engine Editor 모드에서 사용할 카메라 오브젝트

	Ptr<GameObject>			m_DbgObj;					// 디버그 렌더링을 위한 Dummy GameObject
	list<DbgInfo>			m_DbgInfoList;				// 디버그 요청 정보(디버그 렌더 수명은 일정하지가 않기에 정보를 추가 삭제하는데 list가 적합)

	vector<Ptr<CLight2D>>	m_vecLight2D;				// 레벨 안에있는 모든 광원
	Ptr<StructuredBuffer>	m_Light2DBuffer;			// 광원의 데이터를 입력받을 구조화 버퍼
	
	bool					m_IsDebugRender;			// 디버그 렌더 기능 On / Off
	
	//=================
	// private 멤버 함수
	//=================
	void Render_Start();
	void Render_End();
	void Render_Debug();				// 디버그 랜더를 설정하는 함수

public:
	//=========
	// 멤버 함수
	//=========
	void Init();
	void Progress();
	
	void RegisterCamera(Ptr<CCamera> _Cam) { m_MainCam = _Cam; };
	void RegisterEditorCamera(Ptr<CCamera> _Cam) { m_EditorCam = _Cam; };
	void AddDebugInfo(const DbgInfo& _Info) { if (m_IsDebugRender) m_DbgInfoList.push_back(_Info); }
	void RegisterLight2D(Ptr<CLight2D> _Light2D) { m_vecLight2D.push_back(_Light2D); }


	//=========
	// Get, Set
	//=========
	Ptr<CCamera> GetPOVCamera() { return m_MainCam; }
};
