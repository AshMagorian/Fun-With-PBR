#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture;
class ShaderProgram;

class PBR_Material
{
private:
	std::shared_ptr<Texture> m_albedo;
	std::shared_ptr<Texture> m_normal;
	std::shared_ptr<Texture> m_metallic;
	std::shared_ptr<Texture> m_roughness;
	std::shared_ptr<Texture> m_ao;

	glm::vec3 m_albedo_rgb = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_metallic_value = 0.0f;
	float m_roughness_value = 0.5f;

	static GLuint m_irradianceMapID;
	static GLuint m_prefilterMapID;

	static bool brdfCheck;
	static GLuint m_brdfID;

	static void MakeBRDFTex();
public:
	static void SetIrradiance(GLuint _id) { m_irradianceMapID = _id; }
	static GLuint GetIrradiance() { return  m_irradianceMapID; }

	static void SetPrefilter(GLuint _id);
	static GLuint GetPrefilter() { return  m_prefilterMapID; }
	static GLuint GetBRDF() { return m_brdfID; }

	std::shared_ptr<Texture> GetAlbedo() { return m_albedo; }
	std::shared_ptr<Texture> GetNormal() { return m_normal; }
	std::shared_ptr<Texture> GetMetallic() { return m_metallic; }
	std::shared_ptr<Texture> GetRoughness() { return m_roughness; }
	std::shared_ptr<Texture> GetAO() { return m_ao; }

	glm::vec3 GetAlbedoValue() { return m_albedo_rgb; }
	float GetMetallicValue() { return m_metallic_value; }
	float GetRoughnessValue() { return m_roughness_value; }

	void SetTextures(std::shared_ptr<Texture> _albedo,
					std::shared_ptr<Texture> _normal,
					std::shared_ptr<Texture> _metallic,
					std::shared_ptr<Texture> _roughness,
					std::shared_ptr<Texture> _ao);

	void SetAlbedo(glm::vec3 value) { m_albedo_rgb = value; }
	void SetMetallic(float value) { m_metallic_value = value; }
	void SetRoughness(float value) { m_roughness_value = value; }
};

#endif