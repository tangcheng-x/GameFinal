#include<Windows.h>
#include<GameFinal.h>

#pragma comment(lib, "GameFinal.lib")
#pragma comment(lib, "winmm.lib")

const u32 SCREEN_WIDTH = 800;
const u32 SCREEN_HEIGHT = 600;
const f32 CAMERA_MOVE_UNIT = 5.0f;
const f32 CAMERA_ROTATE_UNIT = 1.0f;

void updateCamera(ICameraNode* camera, f32 delta);

f32 getFps(float dt)
{
	static u32 frameCount = 0;
	static f32 elapseTime = 0.0f;
	static f32 fps = 1.0f;
	elapseTime += dt;
	frameCount++;
	if (elapseTime > 0.1f)
	{
		fps = static_cast<f32>(frameCount) / elapseTime;
		frameCount = 0;
		elapseTime = 0.0f;
	}
	return fps;
}

int main()
{
	SDeviceContextSettings settings;
	settings.MultiSamplingCount = 4;
	settings.MultiSamplingQuality = 32;

	IDevice* device = createDevice(EDT_DIRECT3D11, 800, 600, EWS_NONE, true, settings);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IMeshManager* meshManager = driver->getMeshManager();
	IMaterialManager* materialManager = driver->getMaterialManager();

	IResourceGroupManager* resourceGroupManager = driver->getResourceGroupManager();
	resourceGroupManager->init("Resources.cfg");
	resourceGroupManager->loadResourceGroup("General");

	ISimpleMesh* cubeMesh = meshManager->createCubeMesh("cube1");
	IMeshNode* cubeMeshNode = smgr->addMeshNode(cubeMesh, nullptr, nullptr, XMFLOAT3(0, 3.0f, 0));
	cubeMeshNode->setMaterialName("test/material01");
	//cubeMeshNode->remove();

	ISimpleMesh* planeMesh = meshManager->createPlaneMesh("plane1", 10.0, 10.0f, 50, 50, 1.0f, 1.0f);
	IMeshNode* planeMeshNode = smgr->addMeshNode(planeMesh, nullptr);
	planeMeshNode->setMaterialName("test/ground_material");

	IAnimatedMesh* animMesh = meshManager->getAnimatedMesh("lxq.mesh");
	IAnimatedMeshNode* animNode = smgr->addAnimatedMeshNode(animMesh);
	animNode->scale(0.02f, 0.02f, 0.02f);
	IModelMesh* heroMesh = meshManager->getModelMesh("hero.mesh");
	IMeshNode* heroNode = smgr->addModelMeshNode(heroMesh);	
	
	heroNode->scale(0.01f, 0.01f, 0.01f);
	heroNode->translate(2.0f, 0.5f, 0);

	ILightNode* light = smgr->addLightNode(1);
	light->setType(ELT_POINT);
	light->setAmbient(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	light->setPosition(2.0f, 5.0f, -3.0f);
	light->setSpecular(XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f));
	light->setDiffuse(XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
	light->setAttenuation(1.0f, 0.0f, 0.0f);
	light->setRange(100.0f);

	materialManager->destroy(std::string("test/material02"));

	//ICameraNode* camera = smgr->addFpsCameraNode(1, nullptr, XMFLOAT3(0, 1.0f, -4.0f), XMFLOAT3(0, 1.0f, 0.0f));
	ICameraNode* camera = smgr->addFpsCameraNode(1, nullptr, XMFLOAT3(0, 1.0f, -4.0f), XMFLOAT3(0, 1.0f, 0.0f));

	f32 rotx = 0;
	f32 roty = 0;
	f32 rotz = 0;

	char caption[200];

	//FILE* fp = fopen("log.txt", "w");

	ITimer* timer = device->createTimer();
	timer->reset();

	while (device->run())
	{
		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		driver->beginScene(true, true, clearColor);

		float dt = timer->tick();
		
		rotx += dt * 2.0f;
		roty += dt * 1.0f;
		rotz += dt * 0.5f;
		if (rotx > XM_2PI) rotx -= XM_2PI;
		if (roty > XM_2PI) roty -= XM_2PI;
		if (rotz > XM_2PI) rotz -= XM_2PI;

		XMMATRIX Mx = XMMatrixRotationX(rotx);
		XMMATRIX My = XMMatrixRotationY(roty);
		XMMATRIX Mz = XMMatrixRotationZ(rotz);
		XMMATRIX rotM = Mx * My * Mz;

		cubeMeshNode->setOrientation(rotM);
	//	heroNode->yaw(dt);
		animNode->addTime(dt * 3000.0f);

		updateCamera(camera, dt);
	//	std::cout << dt << std::endl;

		smgr->drawAll();

		driver->endScene();

		sprintf(caption, "FPS:%f", getFps(dt));
		device->setWindowCaption(caption);
	}

	device->drop();

	return 0;
}

void updateCamera(ICameraNode* camera, f32 delta)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		camera->walk(CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		camera->walk(-CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		camera->strafe(-CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		camera->strafe(CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState('R') & 0x8000)
	{
		camera->fly(CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState('F') & 0x8000)
	{
		camera->fly(-CAMERA_MOVE_UNIT * delta);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		camera->pitch(-CAMERA_ROTATE_UNIT * delta);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		camera->pitch(CAMERA_ROTATE_UNIT * delta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		camera->yaw(-CAMERA_ROTATE_UNIT * delta);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		camera->yaw(CAMERA_ROTATE_UNIT * delta);
	}

	if (GetAsyncKeyState('N') & 0x8000)
	{
		camera->roll(CAMERA_ROTATE_UNIT * delta);
	}

	if (GetAsyncKeyState('M') & 0x8000)
	{
		camera->roll(-CAMERA_ROTATE_UNIT * delta);
	}
}

