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

void ALevel::DeleteObject(Ptr<GameObject> _Object)
{
	_Object->Destroy();
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

int ALevel::Save(const wstring& _FilePath)
{
	// 파일 스트림 생성
	FILE_OPEN(_FilePath, L"wb");
	
	// Level 이름 불러와 저장
	wstring LevelName = GetName();
	SaveWString(pFile, LevelName);

	// 충돌 체크 정보 저장
	fwrite(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);

	// Layer 정보 저장
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer의 이름 정보 저1장
		SaveWString(pFile, m_arrLayer[i].GetName());

		// GameObject 저장은 최상위 부모기준으로 저장하여
		// 불러왔을 때, 최상위 부모의 자식 오브젝트들이 알맞은 Layer에 등록되도록 설계
		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[i].GetParentObjects();

		// GameObject 개수 저장
		size_t parentCount = vecParents.size();
		fwrite(&parentCount, sizeof(size_t), 1, pFile);
		
		for (const auto& Object : vecParents)
		{
			// GameObject 클래스에 GameObject 정보를 저장하는
			// SaveToLevelFile 함수를 추가
			Object->SaveToLevelFile(pFile);
		}
	}

	fclose(pFile);

	return S_OK;
}

int ALevel::Load(const wstring& _FilePath)
{
	FILE_OPEN(_FilePath, L"rb");

	// 파일에 저장된 Level 이름을 읽어 불러오기
	wstring LevelName = LoadWString(pFile);
	SetName(LevelName);

	// 충돌 체크 정보 불러오기
	fread(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);

	// Layer 정보 불러오기
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 이름 불러오기
		wstring LayerName = LoadWString(pFile);
		m_arrLayer[i].SetName(LayerName);

		// Layer 소속 최상위 부모 오브젝트를 계층구조로 불러옵니다.
		size_t parentCount = 0;
		fread(&parentCount, sizeof(size_t), 1, pFile);
		
		for (size_t j = 0; j < parentCount; ++j)
		{
			Ptr<GameObject> pObject = NEW GameObject;
			pObject->LoadFromLevelFile(pFile);
			AddObject(i, pObject);
		}
	}

	fclose(pFile);

	return S_OK;
}
