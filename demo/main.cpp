#include<stdio.h>
#include<Windows.h>
#include<GameFinal.h>
//#include<CD3D11ShaderManager.h>
#include<iostream>
#include <map>

#pragma comment(lib, "GameFinal.lib")
#pragma comment(lib, "winmm.lib")

const u32 SCREEN_WIDTH  = 800;
const u32 SCREEN_HEIGHT = 600;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;

	Vertex(){}
	Vertex(XMFLOAT3 Pos, XMFLOAT4 Color)
		:Pos(Pos), Color(Color){}
};

int main()
{

	int d = 1;
	GF_PRINT_CONSOLE_INFO("Hello:%d\n", d);

	SDeviceContextSettings settings;
	settings.MultiSamplingCount = 4;
	settings.MultiSamplingQuality = 32;

	IDevice* device = createDevice(EDT_DIRECT3D11, 800, 600, EWS_NONE, true, settings);
	IVideoDriver* driver = device->getVideoDriver();
	IShaderManager* shaderManager = driver->getShaderManager();
	IInputLayoutManager* inputlayoutManager = driver->getInputLayoutManager();
	IPipelineManager* pipelineMgr = driver->getPipelineManager();

	IShader* vs = shaderManager->load(EST_VERTEX_SHADER, "color.hlsl", "ColorVertexShader");
	IShader* ps = shaderManager->load(EST_PIXEL_SHADER, "PixelShader.hlsl", "ColorPixelShader");

	std::vector<SInputLayoutElement> elements;
	elements.resize(2);
	elements[0].SemanticName = "POSITION";
	elements[0].SemanticIndex = 0;
	elements[0].Format = EGF_R32G32B32_FLOAT;
	elements[0].Offset = 0;
	elements[1].SemanticName = "COLOR";
	elements[1].SemanticIndex = 0;
	elements[1].Format = EGF_R32G32B32A32_FLOAT;
	elements[1].Offset = 12;

	IInputLayout* layout = inputlayoutManager->create(elements, vs);


	IShader* shaders[2] = { vs, ps };
	IPipeline* pipeline = pipelineMgr->create("color", shaders, 2, layout, EPT_TRIANGLELIST);

	ISceneManager* smgr = device->getSceneManager();
	
	Vertex vertices[3];
	vertices[0] = Vertex(XMFLOAT3(-1.0f, -0.6f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[1] = Vertex(XMFLOAT3(0.0f,   0.6f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[2] = Vertex(XMFLOAT3(1.0f,  -0.6f, 0.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
	//IMesh* mesh = smgr->createSimpleMesh(&vertices, 3, sizeof(Vertex), nullptr, 0, 0);
	IMesh* mesh = smgr->createCubeMesh();
	IMeshNode* meshNode = smgr->addMeshNode(mesh, pipeline);

	//CD3D11ShaderManager* s = new CD3D11ShaderManager(nullptr);

	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR at = XMVectorZero();

	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * 3.14f,
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		1.0f, 1000.0f);


	meshNode->translate(0, 0, 5.0f);
	XMMATRIX world = meshNode->getAbsoluteTransformation();
	//XMMATRIX world = XMMatrixIdentity();

	
	pipeline->setMatrix("viewMatrix", reinterpret_cast<f32*>(&view));
	pipeline->setMatrix("projectionMatrix", reinterpret_cast<f32*>(&proj));

	SShaderAutoVariable var;
	var.Type = ESAVT_WORLD_MATRIX;
	var.ShaderType = EST_VERTEX_SHADER;
	var.VariableName = "worldMatrix";

	pipeline->addShaderAutoVariable(var);

	std::cout << "Hello World" << std::endl;

	ITimer* timer = device->createTimer();
	timer->reset();
	while (device->run())
	{
		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		driver->beginScene(true, true, clearColor);

		f32 dt = timer->tick();
		//std::cout << dt << std::endl;

		meshNode->setPosition(0, 0, -2.0f);
		meshNode->yaw(1.0f * dt);

		smgr->drawAll();

		//XMMATRIX world = meshNode->getAbsoluteTransformation();
		//pipeline->setMatrix("worldMatrix", reinterpret_cast<f32*>(&world));

		driver->endScene();
	}

	return 0;
}
