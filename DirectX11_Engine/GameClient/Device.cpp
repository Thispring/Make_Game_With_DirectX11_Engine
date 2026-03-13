#include "pch.h"
#include "Device.h"

Device::Device()
	: m_hWnd(nullptr)
{
}

Device::~Device()
{
}

int Device::Init(HWND _hwnd, Vec2 _Resolution)
{
	/***********************************************************
	* Dx11 라이브러리는 동적 라이브러리 입니다.
	* Dx11 관련 객체 생성함수를 통해서 생성된 객체의 주소를 받은 경우,
	* 메모리 해제 시 Dx11 쪽 함수를 이용해 해제해줘야 합니다.
	***********************************************************/
	m_hWnd = _hwnd;
	m_RenderResol = _Resolution;
	g_Global.Resolution = m_RenderResol;

#ifdef _DEBUG
	UINT iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL* pLevel = nullptr;

	/******************************************************************************************************
	* 이중 포인터
	* ID3D11Device, ID3D11DeviceContext
	* D3D11CreateDevice 함수는 는 Dx11 라이브러리에서 제공하는 함수로,
	* ID3D11Device 객체와, ID3D11DeviceContext 객체를 생성해서, 주소를 알려주는 함수
	* ID3D11Device, ID3D11DeviceContext 객체를 생성할때 레퍼런스 카운트를 이미 1을 주고 시작합니다.
	* 따라서 스마트 포인터가 해당 객체를 가리키게 되면, 레퍼런스카운트가 2가 되는 문제가 발생합니다.
	* 스마트 포인터가 대상을 가리키는 맴버 포인터의 주소(이중포인터)를 받아와서 가리킬 주소값을 강제로 바로 세팅해주려고
	* 맴버의 주소(이중포인터) 를 요구합니다.
	******************************************************************************************************/
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, 0, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), pLevel, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}
	
	/*****************************************************************************************
	* [SwapChain]:
	* 윈도우에서 이미지를 표시하기 위해, 비트맵 데이터를 이용해야 하며,
	* 비트맵 데이터는 SystemMemory 영역(Ram)이 가지고 있습니다.
	* 이 비트맵에 어떤 신호를 주고 받는지에 따라 윈도우 화면 영역에 표시가 됩니다.
	* EX) 특정 비트맵의 2진수 묶음인 RGBA 값을 변경하여 색을 표현합니다.
	* 
	* SwapChain 과정은 
	* GPU에게 명령한 VRAM에 생성된 RenderTarget(픽셀 데이터 집합체 버퍼)를 생성하고, 
	* 버퍼에 그림(texture(비트맵 데이터))을 그린다음 그림를 컴퓨터의 SystemMemory인
	* RAM으로 옮기는(Swap) 작업을 수행합니다.
	*	=> 과거의 GPU는 직접적으로 SystemMemory 영역에 접근할 수 없기 때문에 이러한 연산과정이 필요합니다.
	*****************************************************************************************/
	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}

	/********************************************************************************
	* [RenderTarget(픽셀 데이터 집합체 버퍼)]:
	* 그림을 그릴 목적지 타겟에 해당하는 RenderTarget은 SwapChain 생성 시 같이 만들어집니다.
	********************************************************************************/
	 

	/**********************************************************************************************************************
	* [DepthStencilTarget]:
	* 3차원 정보, 가상의 카메라(사용자가 컨트롤할 수 있음)를 두고 픽셀단위로 깊이 단위를 계산하는 과정입니다.
	* 
	* RenderTarget이 물체의 그림이 그려진다면, DepthStencilTarget에는 카메라와 물체의 거리값이 픽셀에 기록 됩니다.
	* 더 가까운 깊이값을 가지는 물체가 그려진다면, 픽셀단위로 RenderTarget에 의해 덮여 그려지며, 자신의 깊이를 깊이타겟에 덮어 쓰여집니다.
	* EX) 같은 거리 기준, 먼저 기록된 물체보다 현재 물체의 깊이값이 더 멀면, RenderTarget에도 그려지지 않고, 깊이타겟도 갱신하지 않음
	* 
	* RenderTarget과 동일한 해상도여야 합니다.
	* EX) 해상도가 다를 시 왜곡된 정보 기록, 출력
	**********************************************************************************************************************/

	
	/*********************************************************************************************
	* {View]:
	* 리소스의 전달자, 매니징 역할
	* 연결된 리소스의 무결성을 보증, 실제 리소스가 필요한 곳에다가 연결된  담당 view를 전달해서 리소스를 연결
	*********************************************************************************************/
	if (FAILED(CreateBuffer()))
	{
		return E_FAIL;
	}

	// 윈도우의 출력 위치
	D3D11_VIEWPORT ViewPort = {};
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = (UINT)m_RenderResol.x;
	ViewPort.Height = (UINT)m_RenderResol.y;
	ViewPort.MinDepth = 0.f;
	ViewPort.MaxDepth = 1.f;

	// RenderTarget을 목적지에 출력시킬 영역 설정
	m_Context->RSSetViewports(1, &ViewPort);


	// GPU는 float 정보를 정규화 하여 사용
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	m_Context->ClearRenderTargetView(m_RTV.Get(), clearColor);

	// 기본 샘플러 생성
	if (FAILED(CreateSampler()))
		return E_FAIL;

	// 앞으로 사용할 DepthStencilState 제작
	if (FAILED(CreateDepthStencilState()))
		return E_FAIL;

	// 앞으로 사용할 BlendState 제작
	if (FAILED(CreateBlendState()))
		return E_FAIL;

	// Rasterizer 상태 제작
	if (FAILED(CreateRasterizerState()))
		return E_FAIL;

	// 각 b 레지스터 번호별로 대응하는 상수버퍼 제작
	CreateConstBuffer();

	return S_OK;
}

void Device::ClearTarget() 
{
	Vec4 vColor = Vec4(0.f, 0.f, 0.f, 0.f);
	CONTEXT->ClearRenderTargetView(m_RTV.Get(), vColor);
	CONTEXT->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Device::OMSetTarget()
{
	// RenderTarget, DST 출력 설정

	// 렌더타겟, 깊이 타겟 출력 설정
	// 렌더링 파이프라인 과정에서 마지막에 그림을 출력시킬 목적지 설정
	m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());
}

int Device::CreateSampler()
{
	//========================================
	// 이방성 필터링(샘플링 기법 중 하나) + 랩 모드
	//========================================
	D3D11_SAMPLER_DESC Desc = {};

	Desc.Filter = D3D11_FILTER_ANISOTROPIC;
	
	// Adress Mode
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	DEVICE->CreateSamplerState(&Desc, m_arrSam[0].GetAddressOf());

	// s0 레지스터에 바인딩 (모든 파이프라인 단계에서 사용 가능)
	CONTEXT->VSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSam[0].GetAddressOf());
	
	//==========================================
	// POINT 필터링 + 랩 모드(2D 게임에서 자주 사용)
	//==========================================
	Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	// Adress Mode
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	DEVICE->CreateSamplerState(&Desc, m_arrSam[1].GetAddressOf());

	// s0 레지스터에 바인딩 (모든 파이프라인 단계에서 사용 가능)
	CONTEXT->VSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSam[1].GetAddressOf());

	return S_OK;
}

int Device::CreateSwapChain() 
{
	DXGI_SWAP_CHAIN_DESC m_Desc = {};

	m_Desc.BufferCount = 1;	// RanderTarget 버퍼의 개수

	// 생성 시킬 버퍼의 해상도 옵션 == 윈도우 비트맵 해상도
	m_Desc.BufferDesc.Width = (UINT)m_RenderResol.x;		// 넓이
	m_Desc.BufferDesc.Height = (UINT)m_RenderResol.y;		// 높이

	// 버퍼의 사용 용도 => RanderTarget
	m_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 생성되는 버퍼의 픽셀 종류(타입)
	m_Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// SwapChain이 RanderTarget을 출력시킬 목적지 윈도우(비트맵)
	m_Desc.OutputWindow = m_hWnd;

	// 창모드 설정 (false => 전체화면, true => 창 모드)
	// 둘은 윈도우 점유율 차이가 존재
	m_Desc.Windowed = true;

	// 기존에 그렸던 그림을 유지하지 않고 지운다.
	// 복사본을 들고 있을 필요가 없기 때문에
	m_Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	m_Desc.BufferDesc.RefreshRate.Numerator = 60;
	m_Desc.BufferDesc.RefreshRate.Denominator = 1;
	m_Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;
	m_Desc.Flags = 0;

	// SwapChain 생성하기
	// IDXGIFactory가 필요
	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &m_Desc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int Device::CreateBuffer() 
{
	/************************************************************
	* RenderTarget
	* SwapChain 생성과정에서 이미 만들어짐
	* __uuidof은 자료형의 고유의 ID를 붙여, 자료형 Type 중복을 막는다.
	* 준비한 포인터에 주소만 받아오면 된다.
	************************************************************/
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RenderTarget.GetAddressOf());
	

	// DepthStencilTarget 생성
	D3D11_TEXTURE2D_DESC Desc = {};

	// 텍스쳐 1장
	Desc.ArraySize = 1;	
	
	// DST는 RenderTarget과 해상도가 동일해야함
	Desc.Width = (UINT)m_RenderResol.x;
	Desc.Height = (UINT)m_RenderResol.y;
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// CPU 메모리 접근 옵션
	// 아래 값들로 CPU 접근 불가 옵션으로 설정
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	// 용도
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	Desc.MipLevels = 1;	// 저화질 버전 텍스쳐 추가 생성, 1 == 원본
	Desc.MiscFlags = 0;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr/*초기 전달할 데이터(깊이값을 0으로 설정)*/
		, m_DepthStencilTarget.GetAddressOf())))
	{
		return E_FAIL;
	}

	/*******************************************
	* View
	* RenderTaragetView, DepthStencil View 생성
	* Get => 
	* GetAddressOf => 
	*******************************************/
	if (FAILED(m_Device->CreateRenderTargetView(m_RenderTarget.Get(), nullptr, m_RTV.GetAddressOf())))
	{
		return E_FAIL;
	}
	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilTarget.Get(), nullptr, m_DSV.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int Device::CreateBlendState() 
{
	// 1. Defalut
	// Context에 nullptr을 전달하면, 기본 블렌드스테이트를 사용한다는 뜻
	m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;	// 일부로 nullptr을 선택, 기본 옵션이기 때문에

	// 2. AlphaBlend: (ScrRGB * SrcA) + (DestRGB * (1 - SrcA))
	D3D11_BLEND_DESC Desc = {};

	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	// 블랜딩 결과를 타켓에 출력
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 노출되는 색상설정이라, 아래 3옵션 설정이 중요함
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	// GameViewportUI 추가로 인한 설정 변경
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

	DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());

	// 3. One_One: (ScrRGB * 1) + (DestRGB * 1)
	Desc = {};

	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	// 블랜딩 결과를 타켓에 출력
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 노출되는 색상설정이라, 아래 3옵션 설정이 중요함
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());
	
	return 0;
}

int Device::CreateDepthStencilState()
{
	// Less
	m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

	// LessEqual
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	Desc.StencilEnable = false;

	if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUL].GetAddressOf())))
		return E_FAIL;

	// No Test
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	Desc.StencilEnable = false;

	if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
		return E_FAIL;

	// No Test No Write
	Desc.DepthEnable = false;
	if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

int Device::CreateRasterizerState()
{
	// CULL_BACK
	m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc = {}; 
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc = {};
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

void Device::CreateConstBuffer()
{
	// 앞으로 사용할 상수버퍼 미리 생성	
	m_CB[(UINT)CB_TYPE::TRANSFORM] = new ConstBuffer;
	m_CB[(UINT)CB_TYPE::TRANSFORM]->Create(CB_TYPE::TRANSFORM, sizeof(TransformMatrix));
	
	m_CB[(UINT)CB_TYPE::MATERIAL] = new ConstBuffer;
	m_CB[(UINT)CB_TYPE::MATERIAL]->Create(CB_TYPE::MATERIAL, sizeof(MtrlConst));

	m_CB[(UINT)CB_TYPE::GLOBAL] = new ConstBuffer;
	m_CB[(UINT)CB_TYPE::GLOBAL]->Create(CB_TYPE::GLOBAL, sizeof(GlobalData));
}

void Device::Resize(UINT width, UINT height)
{
	if (m_SwapChain == nullptr || m_Context == nullptr)
		return;

	// 너비/높이 유효성 검사
	if (width == 0 || height == 0)
		return;

	//// 1) 기존 렌더 타겟(및 관련 뷰)을 해제
	//if (m_RenderTarget)
	//{
	//	m_RTV->Release();
	//	m_RTV = nullptr;
	//}
	// 1) 기존 뷰/관련 리소스 해제 (ComPtr 방식)
	if (m_RTV) m_RTV.Reset();
	if (m_DSV) m_DSV.Reset();


	// (선택) ImGui에 디바이스 리소스 무효화 알림 — imgui_impl_dx11 내부에서 리소스 재생성에 사용
	// ImGui_ImplDX11_InvalidateDeviceObjects();

	// 2) SwapChain 크기 변경
	HRESULT hr = m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr))
	{
		// 실패 처리 (로그 등)
		return;
	}

	// 3) 백버퍼 얻고 RenderTargetView 재생성
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr) || pBackBuffer == nullptr)
	{
		// 실패 처리
		return;
	}

	hr = m_Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_RTV);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		// 실패 처리
		m_RTV = nullptr;
		return;
	}

	// 4) 뷰포트 업데이트
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_Context->RSSetViewports(1, &vp);

	// (선택) ImGui 디바이스 오브젝트 재생성 — ImGui에서 DX 자원을 다시 생성
	// ImGui_ImplDX11_CreateDeviceObjects();
}