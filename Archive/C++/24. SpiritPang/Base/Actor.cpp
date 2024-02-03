#include "PCH.h"

using namespace DirectX;
using namespace DX;

#pragma warning(push)
// enum class를 사용하라는 경고를 무시한다. DirectX::SpriteEffects에서 enum을 사용하므로 호출할 때마다 경고가 뜨기 때문.
#pragma warning(disable:26812)

/// <summary>
/// 애니메이션 동작이 있는 오브젝트를 생성합니다.
/// </summary>
/// <param name="textureName">: Sprite를 담고 있는 이미지의 경로</param>
/// <param name="sheetName">: Sprite Sheet의 정보를 담고 있는 json 파일 경로. (단일 이미지일 경우 nullptr)</param>
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
/// 이 오브젝트의 Sprite 정보를 로딩합니다.
/// 여러 장의 이미지가 들어 있는 파일일 경우, 해당 파일의 정보를 담고 있는 json파일을 추가로 로딩해야 합니다.
/// </summary>
/// <param name="textureFile">: 불러올 이미지 파일의 경로</param>
/// <param name="jsonFile">: Sprite Sheet의 정보를 담고 있는 json파일의 경로</param>
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
		// Frame 정보가 비어 있으니, 화면에 출력하려면 텍스쳐의 w, h 정보가 필요하다.
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
/// 출력할 이미지의 영역을 세팅합니다. 스프라이트 시트에서만 사용되며 단일 이미지 파일일 경우 아무 동작도 하지 않습니다.
/// </summary>
/// <param name="frameName">: json파일에 기재된 파일명을 입력합니다. TexturePacker 패킹을 기준으로 하며, "frames" 배열의 오브젝트에서 "filename"이 가리키는 이름을 입력해야 합니다.</param>
void Actor::SetFrame(const LPCWSTR frameName)
{
	if (m_jsonSheet == nullptr)
		return;

	// rapidjson은 우측값을 넘겨주므로, 참조형 변수에 저장하려면 const 타입이어야 한다.
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
	m_texture = nullptr;	// 메모리 해제는 Texture Manager에서 처리
}

void Actor::OnDeviceRestored()
{
	m_texture = TextureManager::Instance().CreateShaderResourceView(m_textureName.c_str());
}

// 현재 이 오브젝트가 가진 정보를 화면에 출력합니다. 이 오브젝트가 가진 텍스쳐 파일이 없다면 출력하지 않습니다.
void Actor::Render(SpriteBatch* batch)
{
	// 씬 상에 그릴 필요가 없는 경우 렌더링하지 않는다. (유니티의 빈 GameObject와 같이 데이터 저장용 오브젝트 등)
	if (batch == nullptr || m_texture == nullptr || !m_isActive)
		return;

	// 이미지가 가로 또는 세로 반전되어 있을 경우, 피벗 값도 변경해준다. (대각선 반전일 경우 둘 다 동작한다)
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
