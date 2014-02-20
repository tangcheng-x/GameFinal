#include<stdio.h>
#include<Windows.h>
#include<GameFinal.h>
#include<iostream>
#include <map>
#include <regex>

#pragma comment(lib, "GameFinal.lib")
#pragma comment(lib, "winmm.lib")

const u32 SCREEN_WIDTH = 800;
const u32 SCREEN_HEIGHT = 600;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;

	Vertex(){}
	Vertex(XMFLOAT3 Pos, XMFLOAT4 Color)
		:Pos(Pos), Color(Color){}
};

f32 getFps(float dt)
{
	static u32 frameCount = 0;
	static f32 elapseTime = 0.0f;
	static f32 fps = 1.0f;
	elapseTime += dt;
	frameCount++;
	if (elapseTime > 1.0f)
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
	settings.MultiSamplingCount = 1;
	settings.MultiSamplingQuality = 0;

	IDevice* device = createDevice(EDT_DIRECT3D11, 800, 600, EWS_NONE, true, settings);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	IShaderManager* shaderManager = driver->getShaderManager();
	IInputLayoutManager* inputlayoutManager = driver->getInputLayoutManager();
	IPipelineManager* pipelineMgr = driver->getPipelineManager();
	ITextureManager* textureMgr = driver->getTextureManager();
	IMaterialManager* materialMgr = driver->getMaterialManager();
	IRenderStateManager* renderStateMgr = driver->getRenderStateManager();
	IMeshManager* meshManager = driver->getMeshManager();
	IResourceGroupManager* resourceGroupManager = driver->getResourceGroupManager();
	resourceGroupManager->init("Resources.cfg");

	std::string fullpath;
	resourceGroupManager->getFullPath("Desert.png", fullpath);
	std::cout << fullpath << std::endl;

	resourceGroupManager->loadResourceGroup("General");

	IShader* vs = shaderManager->load(EST_VERTEX_SHADER, "BoxShader.hlsl", "vs_main", "vs1");
	IShader* ps = shaderManager->load(EST_PIXEL_SHADER, "PS.hlsl", "ps_main", "ps1");

	std::vector<SInputLayoutElement> elements;
	elements.resize(4);
	elements[0].SemanticName = "POSITION";
	elements[0].SemanticIndex = 0;
	elements[0].Format = EGF_R32G32B32_FLOAT;
	elements[0].Offset = 0;
	elements[1].SemanticName = "NORMAL";
	elements[1].SemanticIndex = 0;
	elements[1].Format = EGF_R32G32B32_FLOAT;
	elements[1].Offset = 12;
	elements[2].SemanticName = "TANGENT";
	elements[2].SemanticIndex = 0;
	elements[2].Format = EGF_R32G32B32_FLOAT;
	elements[2].Offset = 24;
	elements[3].SemanticName = "TEXCOORD";
	elements[3].SemanticIndex = 0;
	elements[3].Format = EGF_R32G32_FLOAT;
	elements[3].Offset = 36;

	IInputLayout* layout = inputlayoutManager->create(elements, vs);

	IRenderState* pRenderState = renderStateMgr->get("common_render_state");
	/*
	pRenderState->setRenderState(ERS_FILL_MODE, E_FILL_WIREFRAME);
	pRenderState->setRenderState(ERS_BLEND_ENABLE, true);
	pRenderState->setRenderState(ERS_SRC_BLEND, E_BLEND_SRC_ALPHA);
	pRenderState->setRenderState(ERS_DEST_BLEND, E_BLEND_INV_SRC_ALPHA);
	pRenderState->setRenderState(ERS_CULL_MODE, E_CULL_NONE);
	pRenderState->confirm();
	*/

	IShader* shaders[2] = { vs, ps };
	IPipeline* pipeline = pipelineMgr->create("color", shaders, 2, layout, EPT_TRIANGLELIST, pRenderState);

	IMesh* cubeMesh = meshManager->createCubeMesh("cube1");

//	textureMgr->load("gta.jpg", "gta.jpg");
	ITexture* texture1 = textureMgr->get("gta.jpg");

	IMaterial material("material01", pipeline);
	material.Material.Ambient = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	material.Material.Diffuse = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	material.Material.Specular = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	material.Material.Emissive = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	material.setTexture(0, texture1);
	materialMgr->add(material);

	IMaterial* pMaterial = materialMgr->get("material01");

	IMeshNode* cubeMeshNode = smgr->addMeshNode(cubeMesh, pMaterial);

	ILightNode* light = smgr->addLightNode(1);
	light->setType(ELT_POINT);
	light->setAmbient(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	light->setPosition(2.0f, 5.0f, -3.0f);
	light->setSpecular(XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f));
	light->setDiffuse(XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
	light->setAttenuation(1.0f, 0.0f, 0.0f);
	light->setRange(100.0f);

	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR at = XMVectorZero();

	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * 3.14f,
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		1.0f, 1000.0f);

	cubeMeshNode->translate(0, 0, 1.0f);

	XMMATRIX viewProj = view * proj;

	pipeline->setMatrix("gViewProj", reinterpret_cast<f32*>(&viewProj));
	
	XMFLOAT3 eyePos(0, 0, -3.0f);
	pipeline->setRawValue("gEyePos", reinterpret_cast<f32*>(&eyePos), sizeof(XMFLOAT3));
	//pipeline->setTexture("gTexture1", texture1);

	SShaderAutoVariable var;
	var.Type = ESAVT_WORLD_MATRIX;
	var.ShaderType = EST_VERTEX_SHADER;
	var.VariableName = "gWorld";
	var.UpdateFrequency = EUF_PER_OBJECT;

	pipeline->addShaderAutoVariable(var);

	SShaderAutoVariable lightvar;
	lightvar.Type = ESAVT_LIGHT;
	lightvar.ShaderType = EST_PIXEL_SHADER;
	lightvar.VariableName = "gLight";
	lightvar.IndexParam = 1;
	lightvar.UpdateFrequency = EUF_PER_FRAME;
	
	pipeline->addShaderAutoVariable(lightvar);

	SShaderAutoVariable texvar;
	texvar.Type = ESAVT_TEXTURE;
	texvar.ShaderType = EST_PIXEL_SHADER;
	texvar.VariableName = "gTexture1";
	texvar.IndexParam = 0;
	texvar.UpdateFrequency = EUF_PER_OBJECT;

	pipeline->addShaderAutoVariable(texvar);

	SShaderAutoVariable mtrlvar;
	mtrlvar.Type = ESAVT_MATERIAL;
	mtrlvar.ShaderType = EST_PIXEL_SHADER;
	mtrlvar.VariableName = "gMaterial";
	mtrlvar.UpdateFrequency = EUF_PER_FRAME;

	pipeline->addShaderAutoVariable(mtrlvar);

	f32 rotx = 0;
	f32 roty = 0;
	f32 rotz = 0;
	bool stop = false;

	ITimer* timer = device->createTimer();
	timer->reset();
	char caption[100];
	while (device->run())
	{
		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		driver->beginScene(true, true, clearColor);

		if (GetAsyncKeyState(0x31) & 0x8000)
		{
			pRenderState->set(ERS_FILL_MODE, E_FILL_SOLID);
			pRenderState->confirm();
		}
		else if (GetAsyncKeyState(0x32) & 0x8000)
		{
			pRenderState->set(ERS_FILL_MODE, E_FILL_WIREFRAME);
			pRenderState->confirm();
		}

		if (GetAsyncKeyState(0x33) & 0x8000)
		{
			pRenderState->set(ERS_DEPTH_ENABLE, true);
			pRenderState->confirm();
		}
		else if (GetAsyncKeyState(0x34) & 0x8000)
		{
			pRenderState->set(ERS_DEPTH_ENABLE, false);
			pRenderState->confirm();
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			stop = true;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			stop = false;
		}
		
		f32 dt = timer->tick();
		if (!stop)
		{
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
		}
		

		

		//cubeMeshNode->setScale(rotx, roty, rotz);
		
		/*
		cubeMeshNode->yaw(2.0f * dt);
		cubeMeshNode->pitch(1.0f * dt);
		cubeMeshNode->roll(0.5f * dt);
		*/

		smgr->drawAll();

		XMMATRIX m = cubeMeshNode->getAbsoluteTransformation();
		XMFLOAT3 points[6];

		f32 sides[8][3] = {
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, -1.0f },
			{ 1.0f, -1.0f, 1.0f },
			{ 1.0f, -1.0f, -1.0f },
			{ -1.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, -1.0f },
			{ -1.0f, -1.0f, 1.0f },
			{ -1.0f, -1.0f, -1.0f },
		};

		for (u32 i = 0; i < 6; i++)
		{
			points[i].x = m._11 * sides[i][0] + m._21 * sides[i][1] + m._31 * sides[i][2];
			points[i].y = m._12 * sides[i][0] + m._22 * sides[i][1] + m._32 * sides[i][2];
			points[i].z = m._13 * sides[i][0] + m._23 * sides[i][1] + m._33 * sides[i][2];
			f32 s = points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z;

			if (s > 3.01f || s < 2.99f)
			{
				std::cout << "Error" << std::endl;
			}
		}

		f32 x0 = points[0].x;
		f32 y0 = points[0].y;
		f32 z0 = points[0].z;

		f32 x1 = points[2].x;
		f32 y1 = points[2].y;
		f32 z1 = points[2].z;

		f32 len = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) + (z0 - z1) * (z0 - z1);
		if (len > 4.01f || len < 3.99f)
		{
			std::cout << "Error" << std::endl;
		}


		//std::cout << getFps(dt) << std::endl;

		sprintf(caption, "FPS:%f", getFps(dt));
		device->setWindowCaption(caption);
	//	Sleep(10);

		driver->endScene();




	}

	return 0;
}
