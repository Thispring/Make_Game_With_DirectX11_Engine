#include "pch.h"

#include "AMaterial.h"
#include "Device.h"
#include "AssetMgr.h"

AMaterial::AMaterial()
	: Asset(ASSET_TYPE::MATERIAL)
	, m_Const{}
	, m_Domain(RENDER_DOMAIN::DOMAIN_NONE)	// 기본 값은 미지정으로
{
}

AMaterial::~AMaterial()
{
}

void AMaterial::Binding()
{
	m_Shader->Binding();

	for (int i = 0; i < TEX_END; i++)
	{
		if (m_Tex[i] == nullptr)
		{
			m_Const.IsTex[i] = 0;
			continue;
		}

		m_Tex[i]->Binding(i);
		m_Const.IsTex[0] = 1;
	}
	Device::GetInst()->GetCB(CB_TYPE::MATERIAL)->SetData(&m_Const); 
	Device::GetInst()->GetCB(CB_TYPE::MATERIAL)->Binding();
}

void AMaterial::Clear()
{
	for (int i = 0; i < TEX_END; i++)
	{
		if (m_Tex[i] == nullptr)
			continue;

		m_Tex[i]->Clear();
	}
}

AMaterial* AMaterial::Clone()
{
	// 새로 동적할당한 객체 주소를 반환
	return new AMaterial(*this);
}

int AMaterial::Save(const wstring& _FilePath)
{
	// 파일 스트림 생성
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	// 재질이 사용하는 쉐이더 파이프라인 정보
	SaveAssetRef(pFile, m_Shader.Get());

	// 파이프라인 동작 시, 어떤 텍스쳐를 전달하기로 했었는지
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; i++)
	{
		SaveAssetRef(pFile, m_Tex[i].Get());
	}

	// 파이프라인 동작 시, 전달할 상수 데이터
	fwrite(&m_Const, sizeof(MtrlConst), 1, pFile);

	// 렌더링 시점, 도메인
	fwrite(&m_Domain, sizeof(RENDER_DOMAIN), 1, pFile);

	fclose(pFile);

	return 0;
}

int AMaterial::Load(const wstring& _FilePath)
{
	// 파일 스트림 생성
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 재질이 사용하는 쉐이더 파이프라인 정보
	m_Shader = LoadAssetRef<AGraphicShader>(pFile);

	// 파이프라인 동작 시, 어떤 텍스쳐를 전달하기로 했었는지
	for (UINT i = 0; i < TEX_END; i++)
	{
		m_Tex[i] = LoadAssetRef<ATexture>(pFile);
	}

	// 파이프라인 동작 시, 전달할 상수 데이터
	fread(&m_Const, sizeof(MtrlConst), 1, pFile);

	// 렌더링 시점, 도메인
	fread(&m_Domain, sizeof(RENDER_DOMAIN), 1, pFile);

	fclose(pFile);

	return 0;
}
