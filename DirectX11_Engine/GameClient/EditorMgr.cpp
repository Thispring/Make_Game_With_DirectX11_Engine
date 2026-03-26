#include "pch.h"
#include "EditorMgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
// imgui에서 문자열 길이에 따라 버퍼가 자동조절하게 해주는 해더
//#include "imgui/imgui_stdlib.h"

#include "Engine.h"
#include "Device.h"

#include "KeyMgr.h"

#include "Inspector.h"
#include "Outliner.h"
#include "Menu.h"
#include "KeyList.h"
#include "ListUI.h"
#include "ContentUI.h"
#include "TreeUI.h"

#include "SpriteMaker.h"
#include "FlipbookMaker.h"
#include "TileMapMaker.h"
#include "MaterialMaker.h"
#include "GameObjectMaker.h"
#include "PrefabMaker.h"

#include "CEditorCamMoveScript.h"
#include "RenderMgr.h"


EditorMgr::EditorMgr()
    : m_ShowDemo(false)     // ImGui Demo UI 활성/비활성
{
}

EditorMgr::~EditorMgr()
{
    // imgui에서 동적할당 해제 관련 함수를
    // EditorMgr의 소멸자에서 호출
    
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Window 해제 부분은
    // 이 프로젝트에서 담당하기 때문에 사용 X
}

void EditorMgr::Tick()
{
    //==============
    // Editor Object
    //==============
    // Editor Object 함수 호출
    for (const auto& Object : m_vecEditorObj)
        Object->Tick();

    for (const auto& Object : m_vecEditorObj)
        Object->FinalTick_Editor();


    //==========
    // Editor UI
    //==========
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_FocusedUI = nullptr;
    
    // DemoUI 활성/비활성화
    // Enable/Disable DemoUI
    if (KEY_TAP(KEY::F8))
        m_ShowDemo ? m_ShowDemo = false : m_ShowDemo = true;

    // Enter를 누르면 현재 선택된 ImGui의 포커스를 해제시킴
    // Clears the focus of the currently selected ImGui item when Enter is pressed.
    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);

    // DemoUI, ImGui의 기능을 살펴보기 위해 사용
    // Used to explore the features of DemoUI and ImGui.
    if (m_ShowDemo)
        ImGui::ShowDemoWindow(&m_ShowDemo);

    // EditorUI
    for (const auto& pair : m_mapUI)
    {
        if (pair.second->IsActive())
            pair.second->Tick();
    }

    if (nullptr != m_FocusedUI)
        KeyMgr::GetInst()->SetActive(false);
    else
        KeyMgr::GetInst()->SetActive(true);

    
    /*******************************************************
    * ImGui는 Begin과 End 사이에 원하는 기능을 추가하여,
    * 매 프레임 호출되어, 해당 내용에 대한 Render를 진행합니다.
    *******************************************************/
}

void EditorMgr::Render()
{
    // Main Window 렌더링
    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Main Window를 벗어났을 때의 렌더링
    // ImGui 자신만의 SwapChain을 생성해 렌더링을 진행
    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void EditorMgr::CreateEditorUI()
{
    // 파생 클래스를 동적할당으로 생성하고 등록
    Ptr<EditorUI> pUI = nullptr;
    
    //===========================
    // 새로운 ImGui 창은 여기서 등록
    //===========================
    pUI = new Menu;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new Inspector;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new Outliner;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new KeyList;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new ListUI;
    pUI->SetModal(true);    // Modal 방식으로 설정하려면 여기에서 true로 설정
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new ContentUI;
    AddUI(pUI->GetUIName(), pUI);

    // Maker UI들은 디폴트로 비활성화 처리
    pUI = new SpriteMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new FlipbookMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new TileMapMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new MaterialMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new GameObjectMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

    pUI = new PrefabMaker;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);

}

void EditorMgr::CreateEditorObject()
{
    // Editor에서 사용하는 카메라 오브젝트 생성
    Ptr<GameObject> pObject = nullptr;
    pObject = new GameObject;
    pObject->SetName(L"EditorCamera");

    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCamera);
    pObject->AddComponent(new CEditorCamMoveScript);
    
    pObject->Camera()->LayerCheckAll();

    pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
    pObject->Camera()->SetFar(1000.f);
    pObject->Camera()->SetFOV(90.f);
    pObject->Camera()->SetOrthoScale(1.f);

    Vec2 vResolution = Device::GetInst()->GetRenderResolution();
    pObject->Camera()->SetAspectRatio(vResolution.x / vResolution.y);	// 종횡비(AspectRatio)
    pObject->Camera()->SetWidth(vResolution.x);							// height 멤버가 없는대신, width 정보만 설정하고 종횡비를 사용

    m_vecEditorObj.push_back(pObject);

    // Editor 용 카메라를 RenderMgr에 등록
    RenderMgr::GetInst()->RegisterEditorCamera(pObject->Camera());
}

void EditorMgr::Init()
{
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(Engine::GetInst()->GetMainWndHwnd());
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // EditorUI 생성
    CreateEditorUI();

    // Editor용 Object 생성
    CreateEditorObject();
}

void EditorMgr::Progress()
{
    Tick();
    Render();
}

void EditorMgr::AddUI(const string& _UIName, Ptr<EditorUI> _UI)
{
    Ptr<EditorUI> pUI = FindUI(_UIName);
    assert(nullptr == pUI);
    m_mapUI.insert(make_pair(_UIName, _UI));
}

Ptr<EditorUI> EditorMgr::FindUI(const string& _UIName)
{
    map<string, Ptr<EditorUI>>::iterator iter = m_mapUI.find(_UIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}
