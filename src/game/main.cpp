#include <VanillaEngine/VanillaEngine.h>

#include "FirstPersonCameraControls.h"
#include "SceneSwitcher.h"

#define WINDOW_WIDTH 1920 
#define WINDOW_HEIGHT 1080 

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

	Lights::AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));

	//std::shared_ptr<Entity> light = application->AddEntity();
	//light->AddComponent<PointLight>();
	//light->GetTransform()->SetPos(glm::vec3(2.0f, 3.0f, 1.0f));

	//application->GetCamera()->SetFPSCamera(true);
	//application->GetCamera()->SetWASD(true);

	//*********************************************************************************************************************************

	std::shared_ptr<Scene> scene1 = SceneManager::CreateScene("Scene1");
	scene1->camera->AddComponent<FirstPersonCameraControls>();
	scene1->camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	scene1->cubemapName = "bg";

	std::shared_ptr<Entity> sphere = scene1->AddEntity("sphere");
	sphere->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
		application->GetResourceManager()->LoadFromResources<PBR_Material>("cobble_mat"));
	sphere->GetComponent<Renderer>()->SetTexCoordScale(2.0f);
	sphere->GetComponent<Renderer>()->SetParallaxClipBorders(false);
	std::shared_ptr<Entity> sphere2 = scene1->AddEntity("sphere2");
	sphere2->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
		dielectric);
	sphere2->GetTransform()->SetPos(glm::vec3(2.0f, 0.0f, 0.0f));
	std::shared_ptr<Entity> sphere3 = scene1->AddEntity("sphere3");
	sphere3->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
		metal);
	sphere3->GetTransform()->SetPos(glm::vec3(-2.0f, 0.0f, 0.0f));
	std::shared_ptr<Entity> plane = scene1->AddEntity("plane");
	plane->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
		application->GetResourceManager()->LoadFromResources<VertexArray>("plane"),
		application->GetResourceManager()->LoadFromResources<PBR_Material>("cobble_mat"));
	plane->GetTransform()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	plane->GetTransform()->SetPos(glm::vec3(0.0f, -1.5f, 0.0f));
	plane->GetTransform()->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
	plane->GetComponent<Renderer>()->SetTexCoordScale(4.0f);

	std::shared_ptr<Entity> switcher1 = scene1->AddEntity("switcher");
	switcher1->AddComponent<SceneSwitcher>();
	
	//**********************************************************************************************************************************

	std::shared_ptr<Scene> scene2 = SceneManager::CreateScene("Scene2");
	scene2->camera->AddComponent<FirstPersonCameraControls>();
	scene2->camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	scene2->cubemapName = "bg2";

	// Test Object from assimp
	std::shared_ptr<Entity> assimpTest = scene2->AddEntity("assimpTest");
	//assimpTest->AddComponent<Renderer>("../src/resources/backpack/backpack.obj",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));
	//assimpTest->AddComponent<Renderer>("../src/resources/Survival_BackPack_2/Survival_BackPack_2.fbx",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));
	assimpTest->AddComponent<Renderer>("../src/resources/Old_Antique_Standing_Globe_FBX/Old_Antique_Standing_Globe_.fbx",
		application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));
	//assimpTest->AddComponent<Renderer>("../src/resources/Old_Antique_Standing_Globe_OBJ/Old_Antique_Standing_Globe_.obj",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));

	assimpTest->GetTransform()->SetScale(glm::vec3(0.02f, 0.02f, 0.02f));

	std::shared_ptr<Entity> switcher2 = scene2->AddEntity("switcher");
	switcher2->AddComponent<SceneSwitcher>();

	SceneManager::SetStartupScene("Scene1");
	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}