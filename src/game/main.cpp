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

	//application->GetSkybox()->CreateSkybox("bg",
	//	"../src/resources/textures/right.jpg",
	//	"../src/resources/textures/left.jpg",
	//	"../src/resources/textures/top.jpg",
	//	"../src/resources/textures/bottom.jpg",
	//	"../src/resources/textures/back.jpg",
	//	"../src/resources/textures/front.jpg");

	application->GetSkybox()->CreateSkybox("bg", "../src/resources/Textures/MonValley_A_LookoutPoint_2k.hdr");
	application->GetSkybox()->CreateSkybox("bg2", "../src/resources/Textures/Mt-Washington-Gold-Room_Ref.hdr");
	application->GetSkybox()->SetSkybox("bg2");



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
	metalPlate->SetTexCoordScale(2.0f);

	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/Cobblestone/cobblestone_floor_08_diff_1k.png", "cobblestone_diff"); 
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/Cobblestone/cobblestone_floor_08_nor_1k.png", "cobblestone_normal");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/Cobblestone/cobblestone_floor_08_rough_1k.png", "cobblestone_rough");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/Textures/Cobblestone/cobblestone_floor_08_ao_1k.png", "cobblestone_ao");
	std::shared_ptr<PBR_Material> cobble = std::make_shared<PBR_Material>();
	cobble->SetAlbedoTex(application->GetResourceManager()->LoadFromResources<Texture>("cobblestone_diff"));
	cobble->SetNormalTex(application->GetResourceManager()->LoadFromResources<Texture>("cobblestone_normal"));
	cobble->SetMetallic(0.0f);
	cobble->SetRoughnessTex(application->GetResourceManager()->LoadFromResources<Texture>("cobblestone_rough"));
	cobble->SetAOTex(application->GetResourceManager()->LoadFromResources<Texture>("cobblestone_ao"));
	cobble->SetTexCoordScale(8.0f);


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
										metalPlate);
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
		cobble);
	testPlane->GetTransform()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	testPlane->GetTransform()->SetPos(glm::vec3(0.0f, -1.5f, 0.0f));
	testPlane->GetTransform()->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

	//std::shared_ptr<Entity> light = application->AddEntity();
	//light->AddComponent<PointLight>();
	//light->GetTransform()->SetPos(glm::vec3(2.0f, 3.0f, 1.0f));

	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}