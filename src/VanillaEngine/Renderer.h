#ifndef RENDERER_H
#define RENDERER_H

class ShaderProgram;
class VertexArray;
class Texture;
class PBR_Material;
class Model;

class Renderer : public Component
{
private:
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_va;
	std::shared_ptr<PBR_Material> m_pbrMat;
	std::shared_ptr<Camera> m_cam;
	std::shared_ptr<Model> m_assimpModel;
	float m_texCoordScale = 1.0f;

	bool m_parallax_clipBorders = true;
	float m_parallax_height = 0.1f;
	int m_parallax_minLayers = 8;
	int m_parallax_maxLayers = 32;


	void BindPBRValues();
	void BindIBLMaps();

public:
	Renderer();
	~Renderer();

	void OnInit() {}
	void OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<PBR_Material> _mat);
	void OnInit(std::string _path, std::shared_ptr<ShaderProgram> _shader);
	void OnTick();
	void OnDisplay();

	void SetMesh(std::shared_ptr<VertexArray> _va) { m_va = _va; }
	void SetPBRMaterial(std::shared_ptr<PBR_Material> _mat) { m_pbrMat = _mat; }
	void SetShader(std::shared_ptr<ShaderProgram> _shader) { m_shaderProgram = _shader; }
	void SetCamera(std::shared_ptr<Camera> _cam) { m_cam = _cam; }
	void SetTexCoordScale(float _value) { m_texCoordScale = _value; }

	void SetParallaxClipBorders(bool value) { m_parallax_clipBorders = value; }

};
#endif