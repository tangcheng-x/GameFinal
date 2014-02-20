#ifndef __RESOURCE_GROUP_MANAGER_CLASS_H__
#define __RESOURCE_GROUP_MANAGER_CLASS_H__

#include "IResourceGroupManager.h"
#include "IShaderManager.h"
#include "IInputLayoutManager.h"
#include "IMaterialManager.h"
#include "IMeshManager.h"
#include "ITextureManager.h"
#include "IRenderStateManager.h"
#include "IPipelineManager.h"
#include "IResourceXmlParser.h"


enum E_RESOURCE_STORE_METHOD
{
	ERSM_FILESYSTEM,
	ERSM_ZIP
};

enum E_RESOURCE_TYPE
{
	ERT_SHADER,
	ERT_INPUTLAYOUT,
	ERT_PIPELINE,
	ERT_MESH,
	ERT_MATERIAL
};

/* 按照加载的先后顺序排列 */
enum E_RESOURCE_FILE_TYPE
{
	ERFT_UNKNOWN = -1,
	ERFT_TEXTURE = 0,
	ERFT_SHADER,
	ERFT_INPUTLAYOUT,
	ERFT_PIPELINE,
	ERFT_MATERIAL,
	ERFT_MESH,
	ERFT_TYPE_COUNT
};

class CResourceGroupManager : public IResourceGroupManager
{
public:
	struct SResourceDirectory
	{
		std::string						Path;
		E_RESOURCE_STORE_METHOD			StoreMethod;
	};

	struct SResourceFile
	{
		std::string						Name;
		E_RESOURCE_FILE_TYPE			Type;
		s32								DirectoryIndex;
	};

	struct SResourceGroup
	{
		std::string							Name;
		std::vector<SResourceDirectory>		Directories;
		std::vector<SResourceFile>			Files;
	};

public:
	CResourceGroupManager(ITextureManager* TextureManager,
		IShaderManager* ShaderManager,
		IInputLayoutManager* InputlayoutManager,
		IRenderStateManager* RenderStateManager,
		IPipelineManager* PipelineManager,
		IMaterialManager* MaterialManager,
		IMeshManager* MeshManager);

	virtual ~CResourceGroupManager();

	virtual bool init(const std::string& configFileName);

	virtual bool loadResourceGroup(u32 groupid);

	virtual bool loadResourceGroup(const std::string& groupname);

	virtual bool getFullPath(const std::string& resourceFileName, std::string& fullpath) const;

private:
	bool isResourceGroupLexical(const std::string& s, std::string& groupname);

	bool isResourceDirectoryLexical(const std::string& s, E_RESOURCE_STORE_METHOD& storeMethod, std::string& dirstr);

	bool setupFilePathsInDirectory(u32 groupIndex, u32 dirIndex);

	bool registerResourceFileLocation(u32 groupIndex, s32 dirIndex, const std::string& filename);

	void seperateDirAndName(const std::string& fullpath, std::string& dirpath, std::string& filename);

	E_RESOURCE_FILE_TYPE getResourceFileType(const std::string& path);

	std::vector<SResourceGroup>							mResourceGroups;

	/* the u32 datatype is used to store both group id and directory id in the group.
	   the higher 16 bit is used to save group id, 
	   the lower 16 bit is used to save directory id. */
	std::map<std::string, u32>							mResourceFilesLocationInfos;
	std::vector<const char*>							mResourceFileExtensions[ERFT_TYPE_COUNT];
};


#endif