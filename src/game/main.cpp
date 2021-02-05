#include <VanillaEngine/VanillaEngine.h>

#include "FirstPersonCameraControls.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(int argc, char *argv[])
{
	std::shared_ptr<Application> application = Application::Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/lightingShader.txt", "lighting_shader");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/pbrShader.txt", "pbr_shader");
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Cube.obj", "cube");
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Plane.obj", "plane");

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

	//application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));
	application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"));

	std::shared_ptr<Entity> camera = application->AddEntity();
	camera->AddComponent<FirstPersonCameraControls>();
	application->GetCamera()->SetCurrentCamera(camera);
	camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetCamera()->SetFPSCamera(true);

	std::shared_ptr<Entity> testBox = application->AddEntity();
	testBox->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("cube") ,
									metalPlate);
	
	std::shared_ptr<Entity> floor = application->AddEntity();
	floor->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("plane"),
									metalPlate);
	floor->GetTransform()->SetPos(glm::vec3(0.0f, -4.0f, 0.0f));
	floor->GetTransform()->SetScale(glm::vec3(3.0f, 1.0f, 3.0f));

	std::shared_ptr<Entity> light = application->AddEntity();
	light->AddComponent<PointLight>();
	light->GetTransform()->SetPos(glm::vec3(2.0f, 3.0f, 1.0f));

	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}