#include "pch.h"
#include "GameObject.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "TaskMgr.h"
#include "LevelMgr.h"
#include "Source/ScriptMgr.h"

#include "CTransform.h"

GameObject::GameObject()
	: m_Com{}
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
}

GameObject::GameObject(const GameObject& _Origin)
	: Entity(_Origin)
	, m_Com{}
	, m_Parent(nullptr)

	// GameObject가 어떤 Layer 소속인지, Dead상태인지는 생성 이후 결정되기에
	// -1, false로 초기화
	, m_LayerIdx(-1)
	, m_Dead(false)
{
	/*************************************************************
	* 복사 생성자로 가지고 있던 Component들을 깊은 복사
	*  => 원본 주소가 아닌, 동일한 세팅의 Component를 
	*	  새롭게 만들어서 가리키게 한다.
	*************************************************************/
	// 컴포넌트 복사
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Origin.m_Com[i] == nullptr)
			continue;

		AddComponent(_Origin.m_Com[i]->Clone());
	}

	// 스크립트 복사
	for (const auto& Script : _Origin.m_vecScripts)
	{
		AddComponent(Script->Clone());
	}

	// 자식 오브젝트 복사
	for (const auto& Child : _Origin.m_vecChild)
	{
		AddChild(Child->Clone());
	}
}

GameObject::~GameObject()
{
}

void GameObject::RegisterLayer()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	pLayer->RegisterObject(this);
}

void GameObject::Begin()
{
	// 처음 한 번 실행 되었을 때
	// 스크립트의 Begin과
	for (size_t i = 0; i < m_vecScripts.size(); i++)
	{
		m_vecScripts[i]->Begin();
	}
	// Component의 Begin 실행
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Begin();
	}
	// 자식오브젝트의 Begin 실행
	for (size_t i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Begin();
	}
}

void GameObject::Tick()
{
	// Scripts의 개수만큼 Tick 실행
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Tick();
	}

	/********************************************************************************************************
	* Tick에서 TRANSFORM 을 이동하거나, 크기를 변경하는 기능은 따로 분리
	* 유니티와 언리얼의 GameObject 설계 스타일이 각자 다르다.
	* 
	* 게임 오브젝트에는 TRANSFORM이 무조건 들어있어야함
	* 헷갈리면 유니티에서 GameObject 생성을 생각하기
	*  -> 빈 게임 오브젝트라도 TRANSFORM이 무조건 있음
	*
	* 다운 캐스팅
	* 사용자가 다형성을 이용할 때, 가리켜야할 상속받은 자식을 정확하게 알고있다면,
	* 명시적으로 자식 포인터를 가리킨다. 다운 캐스팅을 하기 위해서는 설계에 대한 확신이 있어야 한다.
	* Ptr<CTransform> pTrans = nullptr;
	* if (COMPONENT_TYPE::TRANSFORM == m_Com[(UINT)COMPONENT_TYPE::TRANSFORM]->GetType())
	* {
	*		pTrans = (CTransform*)m_Com[(UINT)COMPONENT_TYPE::TRANSFORM].Get();
	* }
	*
	* C++ 에서 제공하는 다운 캐스팅 관련 기능인 dynamic_cast
	* 다운 캐스팅이 실패하면 nullptr을 반환
	* 상속 관계에서 부모 클래스에 가상함수가 1개라도 있으면(가상함수가 없으면 안됨 -> 가상함수가 있어야 타입정보를 생성), 
	* C++에서는 타입 정보를 자료형 별로 생성한다. 이것을 기반으로 다운캐스팅 성공 여부를 판단할 수 있다.
	********************************************************************************************************/
	for (size_t i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Tick();
	}
}

void GameObject::FinalTick()
{
	// Component의 FinalTick을 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// 자신이 소속된 Layer에 자기자신을 알림(등록)
	RegisterLayer();

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void GameObject::FinalTick_Editor()
{
	// Editor 오브젝트 전용 FinalTick

	// Component의 FinalTick을 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void GameObject::Render()
{
	/****************************************************
	* MeshRender에 대한 예외처리
	* GameObject는 Transform은 필수로 가지고 있어야하지만
	* 나머지 Component는 의도에 맞게 추가하는 사항이기 때문에,
	* MeshRender가 null인지를 검사
	****************************************************/

	// 렌더링 관련 기능을 보유한 컴포넌트가 없으면 GameObejct 는 Rendering 될 수 없다.
	if (nullptr != m_RenderCom)
	{
		Transform()->Binding();
		m_RenderCom->Render();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}
}

void GameObject::Destroy()
{
	if (m_Dead)
		return;

	TaskInfo info = {};
	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)this;
	TaskMgr::GetInst()->AddTask(info);
}

// 입력인자로 부모 포인터를 받아, 상속받은 객체로 접근이 가능
void GameObject::AddComponent(Ptr<Component> _Com)
{
	/*******************************************************************
	* + 렌더링 컴포넌트는 하나의 매쉬렌더만 가질 수 있음
	* 입력받은 컴포넌트를 렌더로 다운캐스팅 하여, 매쉬렌더가 포함되어있는지 확인
	*******************************************************************/
	if (dynamic_cast<CRenderComponent*>(_Com.Get()))
	{
		assert(!m_RenderCom.Get());

		m_RenderCom = (CRenderComponent*)_Com.Get();
	}

	// 입력으로 들어온 컴포넌트가 스크립트면, vector로 관리
	if (_Com->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScripts.push_back((CScript*)_Com.Get());
	}
	else
	{
		// 예외처리
		// 해당 컴포넌트를 이미 가지고 있지 않아야 함
		assert(nullptr == m_Com[(UINT)_Com->GetType()]);
		m_Com[(UINT)_Com->GetType()] = _Com;
	}

	_Com->m_Owner = this;

	// 컴포넌트 생성 시, Init을 호출하여,
	// Init에 작성된 Render 관련 기능을 실행
	_Com->Init();
}

// AddChild 함수는 다른 GameObject를 자식으로 추가
// TaskMgr에서 CREATE_OBJECT할 시, 최상위 부모로 등록
void GameObject::AddChild(Ptr<GameObject> _Child)
{
	// 부모 오브젝트가 있는지 확인
	if (_Child->GetParent().Get())
	{
		// 기존의 부모 오브젝트와의 관계를 해제
		_Child->DisconnectWithParent();
	}
	// 최상위 부모 오브젝트라면
	else
	{
		// 레벨 내부에 있는 오브젝트인지 확인
		if (_Child->m_LayerIdx != -1)
		{
			// Layer에서 최상위 부모로 가리키던 포인터를 제거
			_Child->DeregisterAsParent();
		}
	}

	m_vecChild.push_back(_Child);

	_Child->m_Parent = this;

	// Level에 없었던 오브젝트라면, 레이어를 등록
	if (_Child->m_LayerIdx == -1)
	{
		_Child->m_LayerIdx = m_LayerIdx;

		// 부모가 될 오브젝트는 Level 내부 소속이고
		// Level이 Play 모드라면
		if (m_LayerIdx != -1 && LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState())
		{
			// Play 중 Level에 어떤 오브젝트가 자식으로 합류한 상황이기에
			// 자식 오브젝트의 Begin 호출
			_Child->Begin();
		}
	}

	// 부모 오브젝트가 Level에 있던 오브젝트라면
	if (m_LayerIdx != -1)
		LevelMgr::GetInst()->GetCurLevel()->SetChanged();	// Level의 변경 발생을 알림
}

void GameObject::DisconnectWithParent()
{
	// 부모가 nullptr이라면 자신이 최상위 이므로 함수 리턴
	if (m_Parent == nullptr)
		return;

	// Level에 없었다면 변경점을 알림
	if (m_LayerIdx != -1)
		LevelMgr::GetInst()->GetCurLevel()->SetChanged();

	// 자식 오브젝트 vector를 순회하여,
	// 계층관계를 해제
	vector<Ptr<GameObject>>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void GameObject::RegisterAsParent()
{
	if (m_LayerIdx == -1)
		return;

	// LevelMgr에 자기자신을 등록
	LevelMgr::GetInst()->GetCurLevel()->GetLayer(m_LayerIdx)->AddObject(this);
}

void GameObject::DeregisterAsParent()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	// Layer에서 부모 관계를 해제
	pLayer->DeregisterAsParent(this);
}

void GameObject::ReleaseContentScript(const wstring* _ScriptName)
{
}

void GameObject::ReleaseContentScript(const char* _ScriptName)
{
}

void GameObject::ReleaseContentScript(SCRIPT_TYPE _Type)
{
	// 이 함수를 호출한 GameObject의 script vector 전체 목록을 확인하고
	// SCRIPT_TYPE으로 전달받은 인덱스를 가리키게 하지 않도록 해제
	for (UINT i = 0; i < m_vecScripts.size(); ++i)
	{
		if (_Type == m_vecScripts[i]->GetScriptType())
		{
			// 해제한 벡터 원소자리를 지워야
			// 다음 벡터 추가 시, 문제가 없음
			m_vecScripts[i] = nullptr;
			if (i < m_vecScripts.size()) 
			{
				m_vecScripts.erase(m_vecScripts.begin() + i); // O(N) — 뒤 원소들이 앞으로 당겨짐
			}
		}
	}

}

void GameObject::SaveToLevelFile(FILE* _File)
{
	// 이름 
	SaveWString(_File, GetName());

	// 컴포넌트
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_Com[i] == nullptr)
			continue;

		// enum으로 정의된 COMPONENT_TYPE 값을 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 컴포넌트 내용 저장
		m_Com[i]->SaveToLevelFile(_File);
	}
	// 컴포넌트 끝
	// GameObject가 들고있는 컴포넌트가 더이상 없다면
	// 마침표 처럼 COMPONENT_TYPE의 END를 저장합니다.
	UINT ComEnd = (UINT)COMPONENT_TYPE::END;
	fwrite(&ComEnd, sizeof(UINT), 1, _File);
	

	// 스크립트
	size_t ScriptCount = m_vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (const auto& Script : m_vecScripts)
	{
		wstring ScriptName = ScriptMgr::GetScriptName(Script.Get());
		SaveWString(_File, ScriptName);

		Script->SaveToLevelFile(_File);
	}

	// 자식 오브젝트가 있다면 저장
	size_t ChildCount = m_vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (const auto& Child : m_vecChild)
	{
		// 재귀호출
		Child->SaveToLevelFile(_File);
	}
}

void GameObject::LoadFromLevelFile(FILE* _File)
{
	// 이름
	SetName(LoadWString(_File));

	// 컴포넌트
	UINT ComType = 0;

	while (true)
	{
		fread(&ComType, sizeof(UINT), 1, _File);

		// COMPONENT_TYPE가 END를 만났다면, 해당 오브젝트의
		// 컴포넌트를 모두 읽었다는 뜻
		if (ComType == (UINT)COMPONENT_TYPE::END)
			break;

		// 어떤 Component를 저장했는지에 따라
		// 해당 타입에 알맞은 Component를 생성
		Ptr<Component> pComponent = nullptr;

		switch ((COMPONENT_TYPE)ComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			//
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case COMPONENT_TYPE::SPRITE_RENDER:
			pComponent = new CSpriteRender;
			break;
		case COMPONENT_TYPE::BILLBOARD_RENDER:
			pComponent = new CBillboardRender;
			break;
		case COMPONENT_TYPE::FLIPBOOK_RENDER:
			pComponent = new CFlipbookRender;
			break;
		case COMPONENT_TYPE::PARTICLE_RENDER:
			//
			break;
		case COMPONENT_TYPE::TILE_RENDER:
			pComponent = new CTileRender;
			break;
		}

		AddComponent(pComponent);
		pComponent->LoadFromLevelFile(_File);
	}


	// 스크립트
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = LoadWString(_File);
		Ptr<CScript> pScript = ScriptMgr::GetScript(ScriptName);
		AddComponent(pScript.Get());

		pScript->LoadFromLevelFile(_File);
	}

	// 자식 오브젝트 불러오기
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		Ptr<GameObject> pChild = new GameObject;
		AddChild(pChild);
		pChild->LoadFromLevelFile(_File);
	}
}
