#include "PCH.h"

using namespace DirectX;
using namespace DX;

#pragma warning(push)
// enum class�� ����϶�� ��� �����Ѵ�. DirectX::SpriteEffects���� enum�� ����ϹǷ� ȣ���� ������ ��� �߱� ����.
#pragma warning(disable:26812)

/// <summary>
/// �ִϸ��̼� ������ �ִ� ������Ʈ�� �����մϴ�.
/// </summary>
/// <param name="textureName">: Sprite�� ��� �ִ� �̹����� ���</param>
/// <param name="sheetName">: Sprite Sheet�� ������ ��� �ִ� json ���� ���. (���� �̹����� ��� nullptr)</param>
/// <returns></returns>
Actor::Actor(const LPCWSTR textureName, const LPCWSTR sheetName) :
	m_texture{},
	m_textureName{},
	m_jsonSheet{},
	m_sourceRect{ 0, 0, 1, 1 },
	m_pivot{ 0.f, 0.f },
	m_size{ 1.f, 1.f },
	m_position{},
	m_rotation{},
	m_scale{ 1.f },
	m_color{ Colors::White },
	m_flip{ SpriteEffects_None },
	m_layer{},
	m_isActive{ true }
{
	LoadSpriteSheet(textureName, sheetName);
}

Actor::~Actor()
{
}

/// <summary>
/// �� ������Ʈ�� Sprite ������ �ε��մϴ�.
/// ���� ���� �̹����� ��� �ִ� ������ ���, �ش� ������ ������ ��� �ִ� json������ �߰��� �ε��ؾ� �մϴ�.
/// </summary>
/// <param name="textureFile">: �ҷ��� �̹��� ������ ���</param>
/// <param name="jsonFile">: Sprite Sheet�� ������ ��� �ִ� json������ ���</param>
void Actor::LoadSpriteSheet(const LPCWSTR textureFile, const LPCWSTR jsonFile)
{
	if (textureFile == nullptr)
		return;
	
	m_textureName = textureFile;
	m_texture = TextureManager::Instance().CreateShaderResourceView(m_textureName.c_str());

	if (jsonFile != nullptr)
	{
		m_jsonSheet = JsonManager::Instance().CreateDocument(jsonFile);
	}
	else
	{
		// Frame ������ ��� ������, ȭ�鿡 ����Ϸ��� �ؽ����� w, h ������ �ʿ��ϴ�.
		auto pTexture = Utility::GetTextureFromSRV(m_texture);

		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);

		m_size.x = static_cast<float>(desc.Width);
		m_size.y = static_cast<float>(desc.Height);
		SetCenterPivot();

		m_sourceRect.left = m_sourceRect.top = 0;
		m_sourceRect.right = static_cast<LONG>(m_size.x);
		m_sourceRect.bottom = static_cast<LONG>(m_size.y);
	}
}

/// <summary>
/// ����� �̹����� ������ �����մϴ�. ��������Ʈ ��Ʈ������ ���Ǹ� ���� �̹��� ������ ��� �ƹ� ���۵� ���� �ʽ��ϴ�.
/// </summary>
/// <param name="frameName">: json���Ͽ� ����� ���ϸ��� �Է��մϴ�. TexturePacker ��ŷ�� �������� �ϸ�, "frames" �迭�� ������Ʈ���� "filename"�� ����Ű�� �̸��� �Է��ؾ� �մϴ�.</param>
void Actor::SetFrame(const LPCWSTR frameName)
{
	if (m_jsonSheet == nullptr)
		return;

	// rapidjson�� �������� �Ѱ��ֹǷ�, ������ ������ �����Ϸ��� const Ÿ���̾�� �Ѵ�.
	const auto& frames = (*m_jsonSheet)[L"frames"].GetArray();

	for (auto& e : frames)
	{
		if (std::wcscmp(e[L"filename"].GetString(), frameName) == 0)
		{
			const auto& obj = e[L"frame"].GetObject();
			m_size.x = obj[L"w"].GetFloat();
			m_size.y = obj[L"h"].GetFloat();
			SetCenterPivot();

			m_sourceRect.left = static_cast<LONG>(obj[L"x"].GetInt());
			m_sourceRect.top = static_cast<LONG>(obj[L"y"].GetInt());
			m_sourceRect.right = m_sourceRect.left + static_cast<LONG>(m_size.x);
			m_sourceRect.bottom = m_sourceRect.top + static_cast<LONG>(m_size.y);

			return;
		}
	}

	Utility::DebugString(L"[Actor::SetFrame] Can't fine frame %s", frameName);
}

void Actor::OnDeviceLost()
{
	m_texture = nullptr;	// �޸� ������ Texture Manager���� ó��
}

void Actor::OnDeviceRestored()
{
	m_texture = TextureManager::Instance().CreateShaderResourceView(m_textureName.c_str());
}

// ���� �� ������Ʈ�� ���� ������ ȭ�鿡 ����մϴ�. �� ������Ʈ�� ���� �ؽ��� ������ ���ٸ� ������� �ʽ��ϴ�.
void Actor::Render(SpriteBatch* batch)
{
	// �� �� �׸� �ʿ䰡 ���� ��� ���������� �ʴ´�. (����Ƽ�� �� GameObject�� ���� ������ ����� ������Ʈ ��)
	if (batch == nullptr || m_texture == nullptr || !m_isActive)
		return;

	// �̹����� ���� �Ǵ� ���� �����Ǿ� ���� ���, �ǹ� ���� �������ش�. (�밢�� ������ ��� �� �� �����Ѵ�)
	XMFLOAT2 pivot = m_pivot;
	if (m_flip & SpriteEffects::SpriteEffects_FlipHorizontally)
	{
		pivot.x = m_size.x - pivot.x;
	}
	if (m_flip & SpriteEffects::SpriteEffects_FlipVertically)
	{
		pivot.y = m_size.y - pivot.y;
	}

	batch->Draw(m_texture, m_position, &m_sourceRect, m_color, m_rotation, pivot, m_scale, m_flip, m_layer);
}

#pragma warning(pop)
