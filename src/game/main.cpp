#include <VanillaEngine/VanillaEngine.h>

#include "FirstPersonCameraControls.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(int argc, char *argv[])
{
	std::shared_ptr<Application> application = Application::Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/pbrShader.txt", "pbr_shader");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/test.txt", "test_shader");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/NormalDebug.txt", "normal_debug_shader");

	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Cube.obj", "cube");
	application->GetResourceManager()->CreateResource<VertexArray>("plane", "plane");
	application->GetResourceManager()->CreateResource<VertexArray>("sphere", "sphere");

	application->GetResourceManager()->CreateResource<PBR_Material>("../src/resources/Textures/Cobblestone/cobblestone.txt", "cobble_mat");
	application->GetResourceManager()->CreateResource<PBR_Material>("../src/resources/Textures/metal_plate/metal_plate.txt", "metal_plate_mat");

	//application->GetSkybox()->CreateSkybox("bg",
	//	"../src/resources/textures/right.jpg",
	//	"../src/resources/textures/left.jpg",
	//	"../src/resources/textures/top.jpg",
	//	"../src/resources/textures/bottom.jpg",
	//	"../src/resources/textures/back.jpg",
	//	"../src/resources/textures/front.jpg");

	application->GetSkybox()->CreateSkybox("bg", "../src/resources/Textures/MonValley_A_LookoutPoint_2k.hdr");
	application->GetSkybox()->CreateSkybox("bg2", "../src/resources/Textures/Mt-Washington-Gold-Room_Ref.hdr");
	application->GetSkybox()->SetSkybox("bg");

	std::shared_ptr<PBR_Material> dielectric = std::make_shared<PBR_Material>();
	dielectric->SetAlbedo(glm::vec3(0.2f, 0.0f, 0.0f));
	dielectric->SetMetallic(0.0f);
	dielectric->SetRoughness(0.3f);
	std::shared_ptr<PBR_Material> metal = std::make_shared<PBR_Material>();
	metal->SetAlbedo(glm::vec3(0.2f, 0.3f, 0.5f));
	metal->SetMetallic(1.0f);
	metal->SetRoughness(0.1f);

	application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));

	std::shared_ptr<Entity> camera = application->AddEntity();
	camera->AddComponent<FirstPersonCameraControls>();
	application->GetCamera()->SetCurrentCamera(camera);
	camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetCamera()->SetFPSCamera(true);

	// Test Object from assimp
	//std::shared_ptr<Entity> assimpTest = application->AddEntity();
	//assimpTest->AddComponent<Renderer>("../src/resources/backpack/backpack.obj",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));
	//assimpTest->AddComponent<Renderer>("../src/resources/Old_Antique_Standing_Globe_OBJ/Old_Antique_Standing_Globe_.obj",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));
	//assimpTest->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	
	//test spheres
	std::shared_ptr<Entity> testSphere = application->AddEntity();
	testSphere->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
										application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
										application->GetResourceManager()->LoadFromResources<PBR_Material>("metal_plate_mat"));
	testSphere->GetComponent<Renderer>()->SetTexCoordScale(2.0f);
	std::shared_ptr<Entity> testSphere2 = application->AddEntity();
	testSphere2->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
		dielectric);
	testSphere2->GetTransform()->SetPos(glm::vec3(2.0f, 0.0f, 0.0f));	
	std::shared_ptr<Entity> testSphere3 = application->AddEntity();
	testSphere3->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
		metal);
	testSphere3->GetTransform()->SetPos(glm::vec3(-2.0f, 0.0f, 0.0f));
	
	std::shared_ptr<Entity> testPlane = application->AddEntity();
	testPlane->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("plane"),
		application->GetResourceManager()->LoadFromResources<PBR_Material>("cobble_mat"));
	testPlane->GetTransform()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	testPlane->GetTransform()->SetPos(glm::vec3(0.0f, -1.5f, 0.0f));
	testPlane->GetTransform()->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
	testPlane->GetComponent<Renderer>()->SetTexCoordScale(4.0f);

	//std::shared_ptr<Entity> light = application->AddEntity();
	//light->AddComponent<PointLight>();
	//light->GetTransform()->SetPos(glm::vec3(2.0f, 3.0f, 1.0f));

	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}