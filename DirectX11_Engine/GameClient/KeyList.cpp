#include "pch.h"
#include "KeyList.h"
#include "PathMgr.h"

KeyList::KeyList()
	: EditorUI("KeyList")
{
	GetStringFromFile();
}

KeyList::~KeyList()
{
}

void KeyList::GetStringFromFile()
{
	wstring txtPath = CONTENT_PATH;
	txtPath += L"Key_info.txt";

	m_fopen.open(txtPath);
	
	// 파일이 열리지 않았다면
	if (!m_fopen.is_open())
		return;

	while (!m_fopen.eof())	// 파일의 끝을 만날 때 까지
	{
		// 계산을 위한 임시 string
		string tmp;
		getline(m_fopen, tmp);
		m_keyList.push_back(tmp);
	}
	
	// 읽기가 끝났다면 파일 닫기
	m_fopen.close();
}

void KeyList::Tick_UI()
{
	ImGui::Text("KeyList");
	vector<string>::iterator iter(m_keyList.begin());

	for (; iter < m_keyList.end(); ++iter)
	{
		ImGui::Text((*iter).c_str());
	}
	
	// 추후 List를 filter로 검색하는 방법 찾아보기
	//static ImGuiTextFilter filter;

	//filter.Draw("Search");

	//for (auto& item : m_keyList) 
	//{
	//	if (filter.PassFilter(item.c_str())) {
	//		ImGui::Text("%s", item.c_str());
	//	}
	//}
}