#include "pch.h"

#include "TaskMgr.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "GameMgr.h"
#include "TimeMgr.h"

#include "GameObject.h"

TaskMgr::TaskMgr()
{
}

TaskMgr::~TaskMgr()
{
}

void TaskMgr::SetActiveDeferred(GameObject* _Object, bool _IsActive)
{
	TaskInfo info = {};
	info.Type = TASK_TYPE::SET_ACTIVE_OBJECT;
	info.Param_0 = (DWORD_PTR)_Object;
	info.Param_1 = (DWORD_PTR)_IsActive;

	AddTask(info);
}

void TaskMgr::Progress()
{
	// 이전 프레임에 있던 가비지 컬렉터에 있는 요소를 제거
	m_Garbage.clear();

	// m_ChangeGameObjects에 등록된 오브젝트가 있다면
	if (m_ChangeGameObjects.size() > 0)
	{
		// 변경점 파일로 저장
		Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
		pCurLevel->Save(CONTENT_PATH + pCurLevel->GetKey());
		
		m_ChangeGameObjects.clear();
	}


	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
			// Level안에 있는 오브젝트에 변경점이 생기면
			// SetChanged 호출하기

			// TaskMgr에서 CREATE_OBJECT할 시, 최상위 부모로 등록
			case TASK_TYPE::CREATE_OBJECT:
			{
				Ptr<GameObject> pNewObj = (GameObject*)m_vecTask[i].Param_0;

				// 스마트 포인터에 있던 GameObject를 레벨에 등록, pCurLevel은 for문이 끝나면 소멸됨
				Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
				pCurLevel->AddObject(m_vecTask[i].Param_1, pNewObj);
				pCurLevel->SetChanged();
				
				// 변경점 파일로 저장
				pCurLevel->Save(CONTENT_PATH + pCurLevel->GetKey());
				/**********************************************************************
				* 레벨에 추가된 오브젝트는, 레벨 시작 지점 때, Begin을 호출받지 못하기 때문에,
				* 레벨에 스폰될 때 Begin을 호출받는다.
				* 
				* 생성된 오브젝트의 Begin 호출
				**********************************************************************/
				if (LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState())
					pNewObj->Begin();
			}
				break;

			case TASK_TYPE::DESTROY_OBJECT:
			{
				Ptr<GameObject> pObj = (GameObject*)m_vecTask[i].Param_0;
				// 같은 프레임에 같은 삭제 요청이 여러번 들어왔더라도
				// m_Garbage에 하나만 등록하게 예외처리
				if (pObj->m_Dead == false)
				{
					pObj->m_Dead = true;
					m_Garbage.push_back(pObj);	// 가비지 컬렉터에 삭제할 오브젝트 등록

					// list에도 등록하여, 다음 프레임에 list에 Object가 들어있다면
					// 변경된 Level을 저장하고 list 삭제
					m_ChangeGameObjects.push_back(pObj);

					Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurLevel();
					pCurLevel->SetChanged();

				}
			}
				break;

			case TASK_TYPE::CHANGE_LEVEL:
			{
				const wchar_t* pLevelName = (const wchar_t*)m_vecTask[i].Param_0;
				Ptr<ALevel> pLevel = AssetMgr::GetInst()->Find<ALevel>(pLevelName);
				LevelMgr::GetInst()->ChangeLevel(pLevel);
				TimeMgr::GetInst()->InitPlayTime();
				TimeMgr::GetInst()->InitGoalTime();
			}
				break;

			case TASK_TYPE::CHANGE_LEVEL_STATE:
			{
				LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_0;
				LevelMgr::GetInst()->ChangeLevelState(NextState);
				TimeMgr::GetInst()->InitPlayTime();
				TimeMgr::GetInst()->InitGoalTime();
			}
				break;

			case TASK_TYPE::SET_ACTIVE_OBJECT:
			{
				GameObject* pObj = (GameObject*)m_vecTask[i].Param_0;
				bool bActive = (bool)m_vecTask[i].Param_1;

				if (pObj && !pObj->IsDead())
				{
					pObj->SetIsActive(bActive);
				}
			}
				break;
		}
	}

	m_vecTask.clear();
}
