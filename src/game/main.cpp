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
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Plane.obj", "plane");
	application->GetResourceManager()->CreateResource<VertexArray>("sphere", "sphere");

	//application->GetSkybox()->CreateSkybox("bg",
	//	"../src/resources/textures/right.jpg",
	//	"../src/resources/textures/left.jpg",
	//	"../src/resources/textures/top.jpg",
	//	"../src/resources/textures/bottom.jpg",
	//	"../src/resources/textures/back.jpg",
	//	"../src/resources/textures/front.jpg");

	//application->GetSkybox()->CreateSkybox("bg", "../src/resources/Textures/MonValley_A_LookoutPoint_2k.hdr");
	application->GetSkybox()->CreateSkybox("bg", "../src/resources/Textures/Mt-Washington-Gold-Room_Ref.hdr");
	

	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/metal_plate_diff.png", "metal_plate_diff");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/metal_plate_nor.png", "metal_plate_normal");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/metal_plate_spec.png", "metal_plate_metallic");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/metal_plate_rough.png", "metal_plate_rough");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/metal_plate_AO.png", "metal_plate_ao");
	std::shared_ptr<PBR_Material> metalPlate = std::make_shared<PBR_Material>();
	metalPlate->SetTextures(application->GetResourceManager()->LoadFromResources<Texture>("metal_plate_diff"),
							application->GetResourceManager()->LoadFromResources<Texture>("metal_plate_normal"),
							application->GetResourceManager()->LoadFromResources<Texture>("metal_plate_metallic"),
							application->GetResourceManager()->LoadFromResources<Texture>("metal_plate_rough"),
							application->GetResourceManager()->LoadFromResources<Texture>("metal_plate_ao"));
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

	//std::shared_ptr<Entity> testBox = application->AddEntity();
	//testBox->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
	//								application->GetResourceManager()->LoadFromResources<VertexArray>("cube") ,
	//								metalPlate);
	//
	//std::shared_ptr<Entity> floor = application->AddEntity();
	//floor->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
	//								application->GetResourceManager()->LoadFromResources<VertexArray>("plane"),
	//								metalPlate);
	//floor->GetTransform()->SetPos(glm::vec3(0.0f, -4.0f, 0.0f));
	//floor->GetTransform()->SetScale(glm::vec3(3.0f, 1.0f, 3.0f));

	// Test Object froi assimp
	//std::shared_ptr<Entity> assimpTest = application->AddEntity();
	//assimpTest->AddComponent<Renderer>("../src/resources/backpack/backpack.obj",
	//	application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));

	//test sphere
	std::shared_ptr<Entity> testSphere = application->AddEntity();
	testSphere->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
										application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
										metalPlate);

	//std::shared_ptr<Entity> testSphere2 = application->AddEntity();
	//testSphere2->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
	//	application->GetResourceManager()->LoadFromResources<VertexArray>("sphere"),
	//	gold);
	//testSphere2->GetTransform()->SetPos(glm::vec3(2.0f, 0.0f, 0.0f));

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


	//std::shared_ptr<Entity> light = application->AddEntity();
	//light->AddComponent<PointLight>();
	//light->GetTransform()->SetPos(glm::vec3(2.0f, 3.0f, 1.0f));

	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}