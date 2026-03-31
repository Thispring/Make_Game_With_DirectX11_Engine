#include "pch.h"
#include "Layer.h"


Layer::Layer()
	: m_LayerIdx(-1)
{
}

Layer::Layer(const Layer& _Origin)
	: Entity(_Origin)
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	// Layer에 소속되어 있는 최상위 부모 게임 오브젝트만 복사합니다.
	for (const auto& Object : _Origin.m_vecParents)
	{
		AddObject(Object->Clone());
	}
}

Layer::~Layer()
{
}

void Layer::DeregisterAsParent(Ptr<GameObject> _Object)
{
	vector<Ptr<GameObject>>::iterator iter = m_vecParents.begin();

	for (; iter != m_vecParents.end(); ++iter)
	{
		if (*iter == _Object)
		{
			m_vecParents.erase(iter);
			return;
		}
	}

	assert(nullptr);
}

void Layer::AddObject(Ptr<GameObject> _Object)
{
	/*********************************
	* queue 자료구조를 사용하여
	* 자식 오브젝트가 있다면 Layer에 등록
	*********************************/

	m_vecParents.push_back(_Object);

	list<GameObject*> queue;

	queue.push_back(_Object.Get());

	while (!queue.empty())
	{
		GameObject* pObject = queue.front();
		queue.pop_front();
		
		pObject->m_LayerIdx = m_LayerIdx;

		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
		{
			queue.push_back(pObject->m_vecChild[i].Get());
		}
	}
}

void Layer::Begin()
{
	for (size_t i = 0; i < m_vecParents.size(); ++i)
	{
		m_vecParents[i]->Begin();
		/******************************************************************
		* NOTE(26-02-25): RelativeScale에 3을 곱하는 함수 발견,
		* Level 오브젝트 생성 시 설정한 크기가 모두 작아서, 직교 투영일때 
		* 작게보이는 현상을 해결하기 위해, 모든 게임 오브젝트의 크기에 3을 곱했음
		* 
		* Level에서 코드로 설정한 크기와, 실제 크기와 차이가 발생해서, 코드로 크기를
		* 지정할 때 불편함이 발생하기에 아래 코드를 주석처리하고, 크기 변경을 원하는
		* 오브젝트는 직접 LevelMgr Init 단계에서 설정하는 방식으로 변경
		******************************************************************/
		//m_vecParents[i]->Transform()->MulRelativeScale(3.f);
	}
}

/************************************************
* Layer의 Tick, FinalTick, Render는
* GameObject의 함수를 호출
* ALevel -> Layer -> GameObject 순서로 작동합니다.
************************************************/
void Layer::Tick()
{
	for (size_t i = 0; i < m_vecParents.size(); ++i)
	{
		m_vecParents[i]->Tick();
	}
}

void Layer::FinalTick()
{
	vector<Ptr<GameObject>>::iterator iter = m_vecParents.begin();

	for (; iter != m_vecParents.end();)
	{
		(*iter)->FinalTick();

		// 이전 프레임에 isDead가 켜져있는 오브젝트인지 확인
		//  => Level 안에 있는 Layer가 담당
		if ((*iter)->IsDead())
		{
			iter = m_vecParents.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}

void Layer::Render()
{
	for (size_t i = 0; i < m_vecParents.size(); ++i)
	{
		m_vecParents[i]->Render();
	}
}
