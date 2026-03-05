#include "pch.h"
#include "ALevel.h"

ALevel::ALevel()
	: Asset(ASSET_TYPE::LEVEL)
	, m_Matrix{}
	, m_Changed(false)
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		m_arrLayer[i].m_LayerIdx = i;
	}
}

ALevel::~ALevel()
{
}

void ALevel::AddObject(int _LayerIdx, Ptr<GameObject> _Object)
{
	m_arrLayer[_LayerIdx].AddObject(_Object);
}

void ALevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].DeregisterObject();
	}
}

void ALevel::CheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2)
{
	UINT Row = _LayerIdx1;
	UINT Col = _LayerIdx2;

	// 더 작은 레이어 인덱스를 행으로 사용한다.
	// NOTE(2026-02-04): 3항 연산자로 되는지 확인해보기
	if (_LayerIdx2 < _LayerIdx1)
	{
		Row = _LayerIdx2;
		Col = _LayerIdx1;
	}

	m_Matrix[Row] ^= (1 << Col);
}

void ALevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Begin();
	}
}

// ALevel의 Tick, FinalTick, Render는 Layer의 함수들을 호출
void ALevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Tick();
	}
}

void ALevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].FinalTick();
	}
}

Ptr<GameObject> ALevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		// GetAllObjects 함수는 호출 시점이, 1프레임이 지난 FinalTick 이후 이므로
		// 첫 프레임에서 찾을 수가 없다.
		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[i].GetParentObjects();

		// 그렇기에 최상위 부모오브젝트만을 건내주는 GetParentObjects를 사용하여
		// 여기에서 자식 오브젝트가 있는지 검사하는 방식을 사용
		for (size_t i = 0; i < vecParents.size(); i++)
		{
			// tree 방식의 list를 사용해서 level 순회
			list<Ptr<GameObject>> queue;
			queue.push_back(vecParents[i]);
			
			while (!queue.empty())
			{
				Ptr<GameObject> pObject = queue.front();
				queue.pop_front();

				// 찾은 경우
				if (pObject->GetName() == _Name)
					return pObject;

				const vector<Ptr<GameObject>>& vecChild = pObject->GetChild();


				for (size_t j = 0; j < vecChild.size(); ++j)
				{
					// 찾으려면 오브젝트 이름과, 현재 오브젝트의 이름이 다르다면 자식 등록 X
					//if (pObject->GetName() == _Name)
						queue.push_back(vecChild[j]);
				}
			}
		}
	}

	return nullptr;
}
