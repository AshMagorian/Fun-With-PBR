#include "PBR_Material.h"

void PBR_Material::SetTextures(std::shared_ptr<Texture> _albedo,
								std::shared_ptr<Texture> _normal,
								std::shared_ptr<Texture> _metallic,
								std::shared_ptr<Texture> _roughness,
								std::shared_ptr<Texture> _ao)
{
	m_albedo = _albedo;
	m_normal = _normal;
	m_metallic = _metallic;
	m_roughness = _roughness;
	m_ao = _ao;
}