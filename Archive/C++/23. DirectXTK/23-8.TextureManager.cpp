#include "PCH.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DX;

TextureManager::TextureManager()
	: m_pDeviceResources{ nullptr }
{}

TextureManager::~TextureManager()
{
	ReleaseAll();
}

void TextureManager::OnDeviceLost()
{
	for (auto& e : m_shaderResources)
	{
		e.second.Reset();
	}
}

void TextureManager::OnDeviceRestored()
{
	auto device = m_pDeviceResources->GetD3DDevice();
	assert(device);
	for (auto& e : m_shaderResources)
	{
		CreateWICTextureFromFile(
			device,
			e.first.c_str(),
			nullptr,
			e.second.ReleaseAndGetAddressOf()
		);
	}
}

void TextureManager::Initialize(DeviceResources* pDeviceResources)
{
	assert(pDeviceResources != nullptr);
	m_pDeviceResources = pDeviceResources;
}

void TextureManager::ReleaseAll()
{
	for (auto& e : m_shaderResources)
	{
		e.second.Reset();
	}
	m_shaderResources.clear();
}

/// <summary>
/// 텍스쳐 출력을 위한 정보를 담은 ShaderResourceView를 생성하여 반환합니다.
/// 한 번 생성된 요소라면 재생성하는 대신 찾아서 반환합니다. 게임 시작 시 생성해두는 것을 권장합니다.
/// </summary>
/// <param name="filename">: 생성할 이미지의 파일 경로 및 파일명을 입력합니다.</param>
/// <returns></returns>
ID3D11ShaderResourceView* TextureManager::CreateShaderResourceView(LPCWSTR filename)
{
	auto device = m_pDeviceResources->GetD3DDevice();
	assert(device != nullptr);

	// 빈 값을 insert로 넣어보고 해당 값이 있는지 알아본다. (반환형: pair<반복자, 삽입 성공 여부>)
	auto result = m_shaderResources.insert(std::make_pair(filename, nullptr));

	if (result.second == true)
	{
		auto srv = ComPtr<ID3D11ShaderResourceView>();
		CreateWICTextureFromFile(device, filename, nullptr, srv.GetAddressOf());
		result.first->second = srv.Detach();
	}

	return result.first->second.Get();
}

/// <summary>
/// 텍스쳐 출력을 위한 정보를 담은 ShaderResourceView를 찾아서 반환합니다. 탐색에 실패 시 생성하여 반환합니다.
/// (CreateShaderResourceView와 동일하게 동작합니다)
/// </summary>
/// <param name="filename">: 가져올 이미지의 파일 경로 및 파일명을 입력합니다.</param>
/// <returns></returns>
ID3D11ShaderResourceView* DX::TextureManager::GetShaderResourceView(LPCWSTR filename)
{
	return CreateShaderResourceView(filename);
}

// 해당 경로에 있는 파일을 Texture Manager에서 삭제합니다. 다시 호출할 경우 재생성이 필요합니다.
void TextureManager::Remove(LPCWSTR filename)
{
	m_shaderResources.erase(filename);
}
