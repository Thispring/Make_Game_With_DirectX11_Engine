#include "pch.h"
#include "RenderMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Device.h"
#include "Engine.h"

RenderMgr::RenderMgr()
	: m_IsDebugRender(true)
	, m_CurrentGameResolution(0.f, 0.f)
{

}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::Init()
{
	// RenderMgr 초기화 함수에서 디버그 랜더에
	// 관한 변수들을 초기화
	m_DbgObj = new GameObject;
	m_DbgObj->AddComponent(new CTransform);
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(FIND(AMaterial, L"Material\\DbgMtrl.mtrl"));

	m_Light2DBuffer = new StructuredBuffer;

	// Game View 렌더 타겟 설정
	Vec2 resolution = Engine::GetInst()->GetResolution();
	CreateGameRenderTarget(resolution);
}

void RenderMgr::Progress()
{
	if (KEY_TAP(KEY::F9))
		m_IsDebugRender ? m_IsDebugRender = false : m_IsDebugRender = true;


	// 렌더링 시작 전에 할 일
	Render_Start();

	// 카메라 기반 렌더링
	// Level의 상태에 따라 렌더링 카메라를 선택

	// Level이 Play 상태라면 MainCam으로 렌더링
	if (LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
	{
		if (m_MainCam == nullptr)
			return;
		
		// 카메라가 렌더링 하기 전에, Render Domain을 정렬
		m_MainCam->SortObejct();
		// 카메라를 이용해서 레벨안에 있는 물체들을 렌더링
		m_MainCam->Render();
	}
	// Level이 Pause, Stop 상태라면, EditorCam으로 렌더링
	else
	{
		if (m_EditorCam == nullptr)
			return;

		m_EditorCam->SortObejct();
		m_EditorCam->Render();
	}

	// 디버그 렌더링 요청
	if (m_IsDebugRender) Render_Debug();

	// 렌더링 마무리 시, 할 일
	Render_End();
}

void RenderMgr::Render_Start()
{
	// Game View 렌더 타겟 설정
	CONTEXT->OMSetRenderTargets(1, m_GameRTV.GetAddressOf(), m_GameDSV.Get());

	// ALPHABLEND 상태를 강제로 적용
	//CONTEXT->OMSetBlendState(
	//	Device::GetInst()->GetBSState(BS_TYPE::ALPHABLEND).Get(),
	//	nullptr,
	//	0xffffffff
	//);

	// 렌더타겟 클리어
	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	CONTEXT->ClearRenderTargetView(m_GameRTV.Get(), clearColor);
	CONTEXT->ClearDepthStencilView(m_GameDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	// 뷰포트 설정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_CurrentGameResolution.x;
	viewport.Height = m_CurrentGameResolution.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	CONTEXT->RSSetViewports(1, &viewport);

	//// 타겟 설정
	//Device::GetInst()->OMSetTarget();

	//// 렌더타겟 클리어
	//Device::GetInst()->ClearTarget();

	// 등록받은 Light2D의 광원 정보를 구조화 버퍼에 담는다.
	// 구조화버퍼를 특정 t 레지스터에 바인딩 한다.
	vector<Light2DInfo> vecInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); i++)
	{
		vecInfo.push_back(m_vecLight2D[i]->GetInfo());
	}

	// 등록된 광원이 최소 1개 이상인 경우에만 데이터를 구조화버퍼로 보낸다.
	// 이전에 등록된 광원이 0개 였을 때, else로 조건 분기가 되어
	// nullptr 주소를 바인딩 시도하는 오류가 있었기에, vector가 비어있는지 조건 체크
	if (!vecInfo.empty())
	{
		// 구조화버퍼 공간이 모자라면 재확장 및 데이터 전달
		if (vecInfo.size() > m_Light2DBuffer->GetElementCount())
			m_Light2DBuffer->Create(sizeof(Light2DInfo), vecInfo.size(), SB_TYPE::SRV_ONLY, true, vecInfo.data());

		// 공간이 여유가 있으면 바로 광원데이터 전달
		else
			m_Light2DBuffer->SetData(vecInfo.data(), sizeof(Light2DInfo) * vecInfo.size());

		// t12 레지스터로 바인딩 
		m_Light2DBuffer->Binding(12);
	}

	g_Global.Light2DCount = m_vecLight2D.size();

	// Globa 데이터를 상수버퍼를 통해서 b2 레지스터에 바인딩
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->SetData(&g_Global);
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->Binding();
}

void RenderMgr::Render_End()
{
	// 구조화버퍼 클리어
	// 등록받았던 광원들 해제
	// (매 프레임 마다 호출 되므로)
	m_Light2DBuffer->Clear();
	m_vecLight2D.clear();
}


void RenderMgr::Render_Debug()
{
	list<DbgInfo>::iterator iter = m_DbgInfoList.begin();
	for (; iter != m_DbgInfoList.end();)
	{
		// Mesh 설정
		switch ((*iter).Shape)
		{
		case DBG_SHAPE::RECT:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh_LineStrip"));
			break;
		case DBG_SHAPE::SQUARE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"SquareMesh_LineStrip"));
			break;
		case DBG_SHAPE::CIRCLE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CircleMesh_LineStrip"));
			break;
		case DBG_SHAPE::CUBE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CubeMesh"));
			break;
		case DBG_SHAPE::SPHERE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"SphereMesh"));
			break;
		}

		// Transform 설정
		if ((*iter).matWorld == XMMatrixIdentity())
		{
			m_DbgObj->Transform()->SetRelativePos((*iter).Pos);
			m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
			m_DbgObj->Transform()->SetRelativeRot((*iter).Rotation);
			m_DbgObj->FinalTick();
		}
		else
		{
			// 여기 조건에 걸린다면 world 행렬 값만 전달
			m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
		}

		// Material 설정
		m_DbgObj->MeshRender()->GetMaterial()->SetScalar(VEC4_0, (*iter).Color);

		// Depth 설정 분기
		if ((*iter).DepthTest)
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// Render 요청
		if (m_IsDebugRender)
			m_DbgObj->Render();

		// 렌더링 시간 누적
		(*iter).Age += E_DT;

		// 최대 수명에 도달하면 정보 삭제
		if ((*iter).Life < (*iter).Age)
			iter = m_DbgInfoList.erase(iter);	// erase시 연결되어있는 다음 iter 주소 반환
		else
			++iter;	// iter삭제 시, 다음 iter의 다음을 가리키게 하지 않기 위해 else에서 증가
	}
}

void RenderMgr::CreateGameRenderTarget(Vec2 _Size)
{
	m_CurrentGameResolution = _Size;

	// 기존 리소스 해제
	m_GameSRV.Reset();
	m_GameRTV.Reset();
	m_GameDSV.Reset();
	m_GameDepthStencil.Reset();
	m_GameRenderTarget.Reset();

	// 렌더 타겟 텍스처 생성
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = (UINT)_Size.x;
	texDesc.Height = (UINT)_Size.y;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	DEVICE->CreateTexture2D(&texDesc, nullptr, m_GameRenderTarget.GetAddressOf());

	// RTV 생성
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	DEVICE->CreateRenderTargetView(m_GameRenderTarget.Get(), &rtvDesc,
		m_GameRTV.GetAddressOf());

	// SRV 생성 (ImGui에서 사용)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	DEVICE->CreateShaderResourceView(m_GameRenderTarget.Get(), &srvDesc,
		m_GameSRV.GetAddressOf());

	// 깊이 스텐실 버퍼 생성
	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Width = (UINT)_Size.x;
	dsDesc.Height = (UINT)_Size.y;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	DEVICE->CreateTexture2D(&dsDesc, nullptr, m_GameDepthStencil.GetAddressOf());

	// DSV 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = dsDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	DEVICE->CreateDepthStencilView(m_GameDepthStencil.Get(), &dsvDesc,
		m_GameDSV.GetAddressOf());
}

void RenderMgr::ResizeGameRenderTarget(Vec2 newSize)
{
	// 최소 크기 제한
	if (newSize.x < 100 || newSize.y < 100)
		return;

	if (m_CurrentGameResolution == newSize)
		return;

	CreateGameRenderTarget(newSize);
}