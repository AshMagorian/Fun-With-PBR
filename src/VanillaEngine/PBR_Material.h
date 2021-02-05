#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <memory>

class Texture;

class PBR_Material
{
private:
	std::shared_ptr<Texture> m_albedo;
	std::shared_ptr<Texture> m_normal;
	std::shared_ptr<Texture> m_metallic;
	std::shared_ptr<Texture> m_roughness;
	std::shared_ptr<Texture> m_ao;

public:
	std::shared_ptr<Texture> GetAlbedo() { return m_albedo; }
	std::shared_ptr<Texture> GetNormal() { return m_normal; }
	std::shared_ptr<Texture> GetMetallic() { return m_metallic; }
	std::shared_ptr<Texture> GetRoughness() { return m_roughness; }
	std::shared_ptr<Texture> GetAO() { return m_ao; }

	void SetTextures(std::shared_ptr<Texture> _albedo,
					std::shared_ptr<Texture> _normal,
					std::shared_ptr<Texture> _metallic,
					std::shared_ptr<Texture> _roughness,
					std::shared_ptr<Texture> _ao);
};

#endif