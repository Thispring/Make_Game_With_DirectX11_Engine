#pragma once

/******************************************************
* TaskMgr는 프레임 마다 요청한 처리를 저장하고
* 해당 작업을 다음 프레임에 수행시키게 하는 역할을 수행합니다.
******************************************************/
class TaskMgr
	: public Singleton<TaskMgr>
{
	SINGLE(TaskMgr)
private:
	vector<TaskInfo>			m_vecTask;
	vector<Ptr<GameObject>>		m_Garbage;	// 가비지 컬렉터 기능

public:
	//=========
	// 멤버 함수
	//=========
	void AddTask(const TaskInfo& _Info) { m_vecTask.push_back(_Info); }
	void Progress();
};
