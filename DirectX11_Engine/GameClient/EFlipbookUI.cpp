#include "pch.h"
#include "EFlipbookUI.h"
#include "AssetMgr.h"

EFlipbookUI::EFlipbookUI()
	: EAssetUI(ASSET_TYPE::FLIPBOOK)
	, m_CurFrame(0)
	, m_Elapsed(0.f)
	, m_FrameInterval(0.1f)
	, m_PrevFlipbookKey{}
{
}

EFlipbookUI::~EFlipbookUI()
{
}

void EFlipbookUI::Tick_UI()
{
	OutputTitle();


	// Flipbook에 접근해 UV값과 Sprite 개수를 알아낸 뒤,
	// 이미지를 순차적으로 보여줍니다.

	Ptr<AFlipbook> pFlipbook = (AFlipbook*)GetTargetAsset().Get();

	string Key = string(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##FlipbookName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);
	

	if (pFlipbook == nullptr || pFlipbook->GetSpriteCount() == 0)
		return;

	// Flipbook이 바뀌었는지 확인
	if (m_PrevFlipbookKey != pFlipbook->GetKey())
	{
		m_CurFrame = 0;
		m_Elapsed = 0.f;
		m_PrevFlipbookKey = pFlipbook->GetKey();
	}

	// 현재 프레임이 SpriteCount보다 크면 보정
	if (m_CurFrame >= (int)pFlipbook->GetSpriteCount())
		m_CurFrame = 0;

	// 시간 누적 (ImGui::GetIO().DeltaTime은 프레임당 경과 시간)
	m_Elapsed += ImGui::GetIO().DeltaTime;
	if (m_Elapsed >= m_FrameInterval)
	{
		m_Elapsed = 0.f;
		m_CurFrame = (m_CurFrame + 1) % pFlipbook->GetSpriteCount();
	}

	// 현재 프레임의 Sprite만 출력
	Ptr<ASprite> sprite = pFlipbook->GetSprite(m_CurFrame);
	Vec2 LeftTopUV = sprite->GetLeftTopUV();
	Vec2 SliceUV = sprite->GetSliceUV();

	ImGui::ImageWithBg(
		(ImTextureRef)sprite->GetAtlas()->GetSRV().Get(),
		ImVec2(200, 200),
		LeftTopUV, LeftTopUV + SliceUV,
		ImVec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	float frameInterval = GetFrameInterval();
	ImGui::Text("FrameInterval");
	if (ImGui::DragFloat("##FRAMEINTERVAL", &frameInterval, 1.0f, 0.f, FLT_MAX))
	{
		SetFrameInterval(frameInterval);
	}


	// (선택) 현재 프레임 표시
	ImGui::Text("Frame: %d / %d", m_CurFrame + 1, pFlipbook->GetSpriteCount());
}
