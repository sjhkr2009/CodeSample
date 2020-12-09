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
/// �ؽ��� ����� ���� ������ ���� ShaderResourceView�� �����Ͽ� ��ȯ�մϴ�.
/// �� �� ������ ��Ҷ�� ������ϴ� ��� ã�Ƽ� ��ȯ�մϴ�. ���� ���� �� �����صδ� ���� �����մϴ�.
/// </summary>
/// <param name="filename">: ������ �̹����� ���� ��� �� ���ϸ��� �Է��մϴ�.</param>
/// <returns></returns>
ID3D11ShaderResourceView* TextureManager::CreateShaderResourceView(LPCWSTR filename)
{
	auto device = m_pDeviceResources->GetD3DDevice();
	assert(device != nullptr);

	// �� ���� insert�� �־�� �ش� ���� �ִ��� �˾ƺ���. (��ȯ��: pair<�ݺ���, ���� ���� ����>)
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
/// �ؽ��� ����� ���� ������ ���� ShaderResourceView�� ã�Ƽ� ��ȯ�մϴ�. Ž���� ���� �� �����Ͽ� ��ȯ�մϴ�.
/// (CreateShaderResourceView�� �����ϰ� �����մϴ�)
/// </summary>
/// <param name="filename">: ������ �̹����� ���� ��� �� ���ϸ��� �Է��մϴ�.</param>
/// <returns></returns>
ID3D11ShaderResourceView* DX::TextureManager::GetShaderResourceView(LPCWSTR filename)
{
	return CreateShaderResourceView(filename);
}

// �ش� ��ο� �ִ� ������ Texture Manager���� �����մϴ�. �ٽ� ȣ���� ��� ������� �ʿ��մϴ�.
void TextureManager::Remove(LPCWSTR filename)
{
	m_shaderResources.erase(filename);
}
