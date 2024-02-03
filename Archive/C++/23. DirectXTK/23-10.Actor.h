#pragma once

namespace DX
{
	class Actor : public IDeviceNotify
	{
	public:
		Actor();
		virtual ~Actor();

		void LoadSpriteSheet(const LPCWSTR textureFile, const LPCWSTR jsonFile = nullptr);
		void SetFrame(const LPCWSTR frameName);

		void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; }
		DirectX::XMFLOAT2 XM_CALLCONV GetPosition() const { return m_position; }

		void SetColor(float r, float g, float b, float a) { m_color.f[0] = r; m_color.f[1] = g; m_color.f[2] = b; m_color.f[3] = a; }
		const float* GetColorF() const { return m_color.f; }
		DirectX::XMVECTOR XM_CALLCONV GetColor() const { return m_color.v; }

		void SetRotation(float angle) { m_rotation = angle; }
		float GetRotation() const { return m_rotation; }

		void SetScale(float scale) { m_scale = scale; }
		float GetScale() const { return m_scale; }

		void SetFlip(DirectX::SpriteEffects mode) { m_flip = mode; }
		DirectX::SpriteEffects GetFlip() const { return m_flip; }

		void SetLayer(int layer) { m_layer = layer; }
		int GetLayer() const { return m_layer; }

		void SetPivot(float x, float y) { m_pivot.x = x; m_pivot.y = y; }
		void SetCenterPivot() { SetPivot(m_size.x / 2, m_size.y / 2); }
		DirectX::XMFLOAT2 XM_CALLCONV GetPivot() { return m_pivot; }

		DirectX::XMFLOAT2 XM_CALLCONV GetCurrentSize() const { return DirectX::XMFLOAT2(m_size.x * m_scale, m_size.y * m_scale); }
		DirectX::XMFLOAT2 XM_CALLCONV GetOriginSize() const { return m_size; }

		virtual void OnDeviceLost() override;
		virtual void OnDeviceRestored() override;

		virtual void Update(double delta) {};
		virtual void Render(DirectX::SpriteBatch* batch);

	protected:
		DirectX::XMFLOAT2		m_position;
		DirectX::XMVECTORF32	m_color;
		float					m_rotation;
		float					m_scale;
		DirectX::SpriteEffects	m_flip;
		int						m_layer;

	private:
		ID3D11ShaderResourceView*	m_texture;		// 메모리 해제는 Texture Manager에서 처리
		rapidjson::WDocument*		m_jsonSheet;	// 메모리 해제는 Json Manager에서 처리

		std::wstring				m_textureName;
		RECT						m_sourceRect;
		DirectX::XMFLOAT2			m_pivot;		// 회전의 중심점
		DirectX::XMFLOAT2			m_size;			// 원본 크기 (scale 1 기준)
	};
}

