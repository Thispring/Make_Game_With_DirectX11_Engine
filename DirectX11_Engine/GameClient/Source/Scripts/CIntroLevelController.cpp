#include "pch.h"
#include "CIntroLevelController.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CIntroLevelController::CIntroLevelController()
	: CScript(SCRIPT_TYPE::INTROLEVELCONTROLLER)
{
}

CIntroLevelController::~CIntroLevelController()
{
}

void CIntroLevelController::Begin()
{
	// 0번째, 1번째 자식 가져오기
	Ptr<GameObject> child0 = GetOwner()->GetChild(0);
	Ptr<GameObject> child1 = GetOwner()->GetChild(1);

	if (child0 == nullptr || child1 == nullptr)
		return;

	// 0번째 자식 동적 재질 생성 후 초기화
	Ptr<CMeshRender> mesh0 = child0->MeshRender();
	if (mesh0 != nullptr)
	{
		mesh0->CreateDynamicMaterial(); // Shared material 복제 -> 개별 재질 사용
		Ptr<AMaterial> mat0 = mesh0->GetMaterial();
		if (mat0 != nullptr)
		{
			// VEC2_0.x = 1 => 페이드 제어 활성화, VEC2_0.y = 0 => 경과시간 0으로 시작(투명)
			Vec2 start0 = Vec2(1.f, 0.f);
			mat0->SetScalar<Vec2>(VEC2_0, start0);
			mat0->SetScalar<int>(INT_0, 1); // 진행 중
		}
	}

	// 1번째 자식 동적 재질 생성 후 초기화
	Ptr<CMeshRender> mesh1 = child1->MeshRender();
	if (mesh1 != nullptr)
	{
		mesh1->CreateDynamicMaterial(); // Shared material 복제 -> 개별 재질 사용
		Ptr<AMaterial> mat1 = mesh1->GetMaterial();
		if (mat1 != nullptr)
		{
			// 두 자식 모두 투명으로 시작: 플래그는 켜두고 시간은 0으로
			Vec2 start1 = Vec2(1.f, 0.f);
			mat1->SetScalar<Vec2>(VEC2_0, start1);
			mat1->SetScalar<int>(INT_0, 0); // 대기
		}
	}
}

void CIntroLevelController::Tick()
{
	// 0번째, 1번째 자식 가져오기
	Ptr<GameObject> child0 = GetOwner()->GetChild(0);
	Ptr<GameObject> child1 = GetOwner()->GetChild(1);

	if (child0 == nullptr || child1 == nullptr)
		return;

	Ptr<CMeshRender> mesh0 = child0->MeshRender();
	Ptr<CMeshRender> mesh1 = child1->MeshRender();

	if (mesh0 == nullptr || mesh1 == nullptr)
		return;

	Ptr<AMaterial> mat0 = mesh0->GetMaterial();
	Ptr<AMaterial> mat1 = mesh1->GetMaterial();

	if (mat0 == nullptr || mat1 == nullptr)
		return;

	const float DURATION = 3.0f;

	// 상태 확인 (INT_0): 0=대기, 1=진행중, 2=완료, 3=완료후 처리됨(중복방지)
	int state0 = mat0->GetScalar<int>(INT_0);

	if (state0 == 1) // 진행중이면 시간 누적 (vec2.y에 누적)
	{
		Vec2& v0 = mat0->GetScalar<Vec2>(VEC2_0);
		v0.y += DT;

		// 완료 검사
		if (v0.y >= DURATION)
		{
			mat0->SetScalar<int>(INT_0, 2); // 완료
			// 다음(1번째) 자식 시작 신호는 아래에서 처리
		}
	}

	// 1번째 자식 처리 (단, 0번째가 완료되어야 시작)
	int state1 = mat1->GetScalar<int>(INT_0);

	// 만약 0번째가 완료(2)이고 1번째가 아직 대기(0)라면 1번째를 시작(1)으로 전환
	bool startSecond = false;
	state0 = mat0->GetScalar<int>(INT_0);
	if (state0 == 2 && state1 == 0)
	{
		startSecond = true;
	}

	if (startSecond)
	{
		mat1->SetScalar<int>(INT_0, 1); // 진행중으로 설정
	}

	// 진행중이면 시간 누적
	if (mat1->GetScalar<int>(INT_0) == 1)
	{
		Vec2& v1 = mat1->GetScalar<Vec2>(VEC2_0);
		v1.y += DT;

		// 완료 검사
		if (v1.y >= DURATION)
		{
			mat1->SetScalar<int>(INT_0, 2); // 완료
		}
	}

	// 두 자식이 모두 완료(2) 상태이면 씬 전환 호출, 한 번만 호출하도록 상태를 3으로 변경
	bool bothDone = false;

	state0 = mat0->GetScalar<int>(INT_0);
	state1 = mat1->GetScalar<int>(INT_0);

	if (state0 == 2 && state1 == 2)
		bothDone = true;

	// 중복 호출 방지: 이미 3으로 설정되어있으면 처리 안 함
	if (bothDone)
	{
		// 상태를 변경하여 이 분기가 다시 실행되지 않도록 함
		mat0->SetScalar<int>(INT_0, 3);
		mat1->SetScalar<int>(INT_0, 3);

		LevelMgr::GetInst()->ChangeMainMenu();
	}
}

void CIntroLevelController::SaveToLevelFile(FILE* _File)
{
}

void CIntroLevelController::LoadFromLevelFile(FILE* _File)
{
}
