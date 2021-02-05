#ifndef RENDERER_H
#define RENDERER_H

class ShaderProgram;
class VertexArray;
class Texture;
class PBR_Material;

class Renderer : public Component
{
private:
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_va;
	std::shared_ptr<PBR_Material> m_mat;
	std::shared_ptr<Camera> m_cam;

public:
	Renderer();
	~Renderer();

	void OnInit() {}
	void OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<PBR_Material> _mat);
	void OnTick();
	void OnDisplay();

	void SetMesh(std::shared_ptr<VertexArray> _va) { m_va = _va; }
	void SetPBRMaterial(std::shared_ptr<PBR_Material> _mat) { m_mat = _mat; }
	void SetShader(std::shared_ptr<ShaderProgram> _shader) { m_shaderProgram = _shader; }
	void SetCamera(std::shared_ptr<Camera> _cam) { m_cam = _cam; }

};
#endif