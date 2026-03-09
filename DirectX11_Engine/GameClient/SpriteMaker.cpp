#include "pch.h"
#include "SpriteMaker.h"

#include "AssetMgr.h"

SpriteMaker::SpriteMaker()
	: EditorUI("SpriteMaker")
{
}

SpriteMaker::~SpriteMaker()
{
}

void SpriteMaker::Tick_UI()
{
	ImGui::Text("SpriteMaker");

	// SliceUV는 멤버 변수로 빼야함,
	// Tick_UI에서 계속 초기화됨
	Vec2 SliceUV = {};

	ImGui::Text("SliceUV");
	ImGui::SameLine(150);
	if (ImGui::DragFloat2("##SLICEUV", SliceUV))
	{
		
	}

	// 아래 함수는 ImGui 버튼이 눌렸을 때, 델리게이트로 전달합니다.
	// 
	// 오버로딩된 함수 호출
	// void AssetMgr::CreateEngineSprite(wstring _Name, Vec2 _Slice, int _Loop, wstring _Path, float _Origin)
	// Path의 경우 다음과 같이 전달합니다: L"Sprite\\name_Action_"
	//AssetMgr::GetInst()->CreateEngineSprite(L"Name", Vec2(0.f, 0.f), 1, L"Path", 0.f);

}