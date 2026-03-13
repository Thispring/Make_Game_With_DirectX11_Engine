/*********************************************************************************
* 그래픽스 파이프라인(Graphics Pipeline) or 렌더링 파이프라인 (Rendering Pipeline) 원리:
* 시각 정보를 출력하는 전자기기인 '디스플레이'는 색상 정보인 RGBA(Red, Green, Blue, Alpha)를
* 담고 있는 수많은 '픽셀'로 이루어져 있습니다. 즉, 일반적으로 사용되는 1920 x 1080의 모니터는
* 2,073,600개의 픽셀을 가지고, 화면을 표현하게 됩니다.
* RGBA에는 한 채널당 1바이트(환경마다 설정이 다를 수 있음)를 할당받아, 0~255의 범위를 나타낼 수
* 있습니다. 이를 활용하여, 특정 위치에 있는 픽셀 중 어떤 색상 신호를 얼마나 활성화할지를 조절하여,
* 원하는 화면 출력을 할 수 있습니다.
* ex) (255, 0, 0, 0)은 빨간색, (0, 0, 0, 0)은 검은색 => 빛의 삼원색의 원리 활용
* 이처럼 디스플레이에 화면을 출력하는 것은 수백만 개 픽셀의 RGBA 값을 결정하는 대규모 연산 과정입니다.
* 각 픽셀의 색상 계산은 서로에게 영향을 주지 않는 독립적인 연산이므로, 병렬 처리에 매우 유리합니다.
* 그렇기에, 순차적인 연산(=직렬)에 최적화되어 설계된 CPU보다 독립적인 연산(=병렬)에 최적화되어
* 설계된 GPU를 사용하는게, 연산 속도와 효율 면에서 이득이 많기 때문에
* GPU를 제어하는 라이브러리인 Direct X, OpenGL 등을 사용해서 구현합니다.
*
* 컴퓨터는 그래픽 렌더링 시, 매 프레임마다 이전에 그려졌던 그림을 지우고, 명령된 그림을 그리는 작업을
* 반복합니다. 이를 이용하여, 다음 프레임에 연속적이지만 다른 그림을 그리도록 명령한다면, 현실의 동영상 작동 원리와
* 동일하게, 움직임을 구현할 수 있습니다. 
* 컴퓨터 연산속도는 1초에 약 2만번을 작업할 정도로 매우 빠르게 처리할 수 있습니다. 그렇기에 1초동안 몇 프레임 작업이 되는지
* 계산을 하여, 알맞은 값을 넣어 움직임을 구현합니다. 하지만 모든 컴퓨터의 성능이 똑같지 않기에, 위와 같이 구현을하게 되면
* 컴퓨터마다 움직이는 속도에 오차가 발생할 수 있으므로, DeltaTime(프레임이 다시 돌아왔을 때, 걸린 시간)을 이용해, 기기 성능과
* 상관없이 동일한 양을 동작하도록 해야합니다.
* 
* 그래픽스 파이프라인(Graphics Pipeline) or 렌더링 파이프라인 (Rendering Pipeline) 과정:
* Input Assembler -> Vertex Shader -> Rasterization -> Pixel Shader
* 
* 참고자료:
* https://learn.microsoft.com/ko-kr/windows/uwp/graphics-concepts/graphics-pipeline
* https://steadycodist.tistory.com/entry/%EB%A9%B4%EC%A0%91-%EC%A4%80%EB%B9%84Graphics-%EB%A0%8C%EB%8D%94%EB%A7%81-%ED%8C%8C%EC%9D%B4%ED%94%84%EB%9D%BC%EC%9D%B8Rendering-Pipeline#google_vignette
* 
* 
* 그래픽스 파이프라인(Graphics Pipeline) or 렌더링 파이프라인 (Rendering Pipeline) 용어:
* Vertex:
* '정점', '꼭짓점'을 뜻하는 영어 단어입니다.
* 컴퓨터 그래픽스에서는 3D 모델을 구성하는 위치, 색상, 법선 등의 정보를 가진 최소 단위 '점'을 의미합니다.
* 
*********************************************************************************/

/**********************************************
* typeinfo 헤더를 이용해
* 해당 type의 이름이나 고유 번호를 알 수 있습니다.
* EX) const type_info& info = typeid(Entity);
* const char* pStr = info.name();
* DWORD num = info.hash_code();
**********************************************/

#pragma once
#include "ConstBuffer.h"

//===================================
// Device는 Direct x11 라이브러리 함수를 
// 이용하여, GPU를 제어할 수 있는 클래스
//===================================
class Device
	: public Singleton<Device>	// 싱글턴 디자인: 템플릿 T 자리에 자신 객체를 넣음
{
	SINGLE(Device)
private:
	// ComPtr은 Dx11객체의 '스마트 포인터', 템플릿으로 제공되어 있습니다.
	ComPtr<ID3D11Device>				m_Device;				// GPU 메모리 관리 ex)GPU의 vRam에 공간 할당 명령
	ComPtr<ID3D11DeviceContext>			m_Context;				// GPU 제어 및 렌더링 GPGPU(General Purpose => 비트코인 채굴, AI연산) - ComputeShader 명령
	
	ComPtr<IDXGISwapChain>				m_SwapChain;
	ComPtr<ID3D11Texture2D>				m_RenderTarget;
	ComPtr<ID3D11Texture2D>				m_DepthStencilTarget;
	
	ComPtr<ID3D11RenderTargetView>		m_RTV;
	ComPtr<ID3D11DepthStencilView>		m_DSV;

	ComPtr<ID3D11SamplerState>			m_arrSam[2];
	ComPtr<ID3D11BlendState>			m_BSState[(UINT)BS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];

	HWND								m_hWnd;					// 출력 윈도우
	Vec2								m_RenderResol;			// 해상도
	
	// 상수버퍼를 가리키는 스마트 포인터 추가
	Ptr<ConstBuffer>					m_CB[(UINT)CB_TYPE::END];


	//================
	//private 멤버 함수
	//================
	int CreateSwapChain();
	int CreateBuffer();
	int CreateBlendState();
	int CreateDepthStencilState();
	int CreateRasterizerState();
	void CreateConstBuffer();

public:
	//=========
	// 멤버 함수
	//=========
	// GPU를 이용해, 윈도우 창 생성 및 초기화
	int Init(HWND _hwnd, Vec2 _Resolution);
	
	// 만들어진 윈도우 창을 출력하는 함수
	void Present() { m_SwapChain->Present(0, 0); }
	
	/**********************************************************
	* 이전 윈도우를 지우는 함수, 이전에 그려졌던 그림을 지우고
	* 새 그림을 출력하기 위해 필요하다.
	*  => 동영상에서 프레임마다 다른 사진을 보여주는 것과 비슷한 원리
	**********************************************************/
	void ClearTarget();
	/********************************************************************************************
	* ImGui 에서 화면을 벗어난 창의 경우 자신만의 SwapChain 과 RenderTarget 을 가지고 있어서, 
	* 렌더링 전에 자신의 Rendertarget 으로 출력을 변경한다. 따라서 우리 엔진도 렌더링 
	* 시작 전에 메인 윈도우와 연결된 SwapChain 의 렌더타겟을 출력 목적지로 재 지정하고 렌더링에 들어가야 한다.
	********************************************************************************************/
	void OMSetTarget();
	int CreateSampler();

	void Resize(UINT width, UINT height);


	//=========
	// Get, Set
	//=========
	Vec2 GetRenderResolution() { return m_RenderResol; }
	ComPtr<ID3D11Device> GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
	
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

	Ptr<ConstBuffer> GetCB(CB_TYPE _Type) { return m_CB[(UINT)_Type]; }

};

// 자주 호출되는 인라인 함수를 재정의하여 사용
#define DEVICE Device::GetInst()->GetDevice().Get()
#define CONTEXT Device::GetInst()->GetContext().Get()
