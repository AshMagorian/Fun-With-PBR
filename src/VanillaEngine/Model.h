#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
private:
	std::vector<AssimpTexture> textures_loaded;
	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<AssimpTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

public:
	Model(char *path)
	{
		LoadModel(path);
	}
	void Draw(std::shared_ptr<ShaderProgram> shader);
};
#endif