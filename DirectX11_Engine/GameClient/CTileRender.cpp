#include "pch.h"
#include "CTileRender.h"
#include "AssetMgr.h"
#include "CTransform.h"


CTileRender::CTileRender()
	: CRenderComponent(COMPONENT_TYPE::TILE_RENDER)
{
	m_Buffer = new StructuredBuffer;
}

CTileRender::CTileRender(const CTileRender& _Origin)
	: CRenderComponent(_Origin)
	, m_TileMap(_Origin.m_TileMap)
	, m_vecSpriteInfo(_Origin.m_vecSpriteInfo)
	, m_Buffer(nullptr)
{
	// 복사 시 동일한 구조체 버퍼를 가리키면 안되기 때문에 복사생성자 수동 구현
	m_Buffer = new StructuredBuffer;
}

CTileRender::~CTileRender()
{
}

void CTileRender::Init()
{
	// 부모 클래스의 Init도 같이 호출
	// CreateMaterial 함수를 호출해야 출력되기 때문
	CRenderComponent::Init();
	SetTileMap(m_TileMap);
}

void CTileRender::FinalTick()
{
	// 크기조정
	if (m_TileMap != nullptr)
	{

	}
}

void CTileRender::Render()
{
	// TileMap 설정이 안되어있거나, Sprite vector가 비어있다면 리턴
	if (m_TileMap == nullptr && m_vecSpriteInfo.empty())
		return;

	m_Buffer->Binding(20);

	GetMaterial()->SetTexture(TEX_0, m_TileMap->GetAtlas());
	// NOTE(26-03-20): Col, Row를 .fx로 전달할때 값이 어떻게 전달되며,
	// .fx에서 어떻게 값이 쓰이는지 질문하기
	GetMaterial()->SetScalar(INT_0, m_TileMap->GetCol());
	GetMaterial()->SetScalar(INT_1, m_TileMap->GetRow());
	GetMaterial()->Binding();
		
	GetMesh()->Render();

	m_Buffer->Clear();


	//// m_TileMap에 전달받은 Row, Col값 만큼 for반복을하여,
	//// SpriteInfo 인덱스별로 UV 정보를 전달합니다.
	//UINT Row = m_TileMap->GetRow();
	//UINT Col = m_TileMap->GetCol();
	//int Count = 0;
	////GetMaterial()->SetScalar(INT_0, m_TileMap->GetRow());
	////GetMaterial()->SetScalar(INT_1, m_TileMap->GetCol());
	//GetMaterial()->SetTexture(TEX_0, m_TileMap->GetAtlas());
	//GetMaterial()->SetScalar(INT_0, m_TileMap->GetCol());
	//GetMaterial()->SetScalar(INT_1, m_TileMap->GetRow());
	//for (int i = 0; i < Col; ++i)
	//{
	//	for (int j = 0; j < Row; ++j)
	//	{
	//		GetMaterial()->SetScalar(VEC2_0, m_vecSpriteInfo[Count].LeftTop);
	//		GetMaterial()->SetScalar(VEC2_1, m_vecSpriteInfo[Count].Slice);
	//		GetMaterial()->Binding();
	//		GetMesh()->Render();
	//		Count++;
	//	}
	//}
	//m_Buffer->Clear();
}


void CTileRender::SetTileMap(Ptr<ATileMap> _TileMap)
{
	m_TileMap = _TileMap;

	if (m_TileMap == nullptr)
		return;

	// 이전 정보 리셋
	m_vecSpriteInfo.clear();

	// 크키 조정
	UINT Row = m_TileMap->GetRow();
	UINT Col = m_TileMap->GetCol();
	Vec2 TileSize = m_TileMap->GetTileSize();

	// 수정: X는 타일 가로 * 열(Col), Y는 타일 세로 * 행(Row)
	//Vec3 vScale = Vec3(TileSize.x * (float)Col, TileSize.y * (float)Row, 1.f);
	Vec3 vScale = Vec3(TileSize.x * (float)Row, TileSize.y * (float)Col, 1.f);
	Transform()->SetRelativeScale(vScale);

	// TileMap 의 Sprite 의 UV 정보를 받아옴
	vector<Ptr<ASprite>> vecSprites = m_TileMap->GetSprites();

	for (int i = 0; i < vecSprites.size(); ++i)
	{
		SpriteInfo info = {};

		if (nullptr != vecSprites[i])
		{
			info.LeftTop = vecSprites[i]->GetLeftTopUV();
			info.Slice = vecSprites[i]->GetSliceUV();
		}

		m_vecSpriteInfo.push_back(info);
	}

	// 구조화버퍼의 크기가 모자라거나 한번도 할당할 적이 없으면 메모리 확장 or 재확장
	if (m_Buffer->GetBufferSize() < sizeof(SpriteInfo) * m_vecSpriteInfo.size())
	{
		m_Buffer->Create(sizeof(SpriteInfo), m_vecSpriteInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// Sprite 들의 데이터를 구조화버퍼로 보내기
	m_Buffer->SetData(m_vecSpriteInfo.data(), sizeof(SpriteInfo) * m_vecSpriteInfo.size());

}

void CTileRender::CreateMaterial()
{
	wstring MeshName = L"SquareMesh";
	wstring MtrlName = L"Material\\TileMtrl.mtrl";
	wstring ShaderName = L"TileShader";
	wstring FilePath = L"Shader\\tile.fx";
	string VS = "VS_Tile";
	string PS = "PS_Tile";

	// RectMesh 설정
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(MeshName));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(MtrlName);

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = new AMaterial;
		pMtrl->SetName(MtrlName);

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(ShaderName);

		// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다
		if (nullptr == pShader)
		{
			pShader = new AGraphicShader;
			pShader->SetName(ShaderName);
			pShader->CreateVertexShader(FilePath, VS);
			pShader->CreatePixelShader(FilePath, PS);
			pShader->SetBSType(BS_TYPE::DEFAULT);
			pShader->SetRSType(RS_TYPE::CULL_NONE);
			AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());
		}

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		// 재질의 도메인 등록
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
		// 생성한 재질을 파일로 저장
		pMtrl->Save(CONTENT_PATH + pMtrl->GetKey());
	}

	SetMaterial(pMtrl);
}

void CTileRender::SaveToLevelFile(FILE* _File)
{
	/******************************************************
	* SpriteInfo는 Load 과정에서 SetTileMap으로
	* 파일에 저장된 TileMap 정보로 멤버 벡터에 저장하기 때문에
	* m_vecSpriteInfo는 파일에 저장하지 않습니다.
	******************************************************/
	CRenderComponent::SaveToLevelFile(_File);

	SaveAssetRef(_File, m_TileMap.Get());
}

void CTileRender::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	m_TileMap = LoadAssetRef<ATileMap>(_File);

	SetTileMap(m_TileMap);
}
