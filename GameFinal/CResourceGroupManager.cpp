#include "stdafx.h"
#include "CResourceGroupManager.h"
#include "CResourceLoader.h"
#include "gfUtil.h"

CResourceGroupManager::CResourceGroupManager(ITextureManager* TextureManager,
	IShaderManager* ShaderManager,
	IInputLayoutManager* InputlayoutManager,
	IRenderStateManager* RenderStateManager,
	IPipelineManager* PipelineManager,
	IMaterialManager* MaterialManager,
	IMeshManager* MeshManager)
{
	// init shader file exts.
	mResourceFileExtensions[ERFT_SHADER].push_back("vs");
	mResourceFileExtensions[ERFT_SHADER].push_back("ps");
	mResourceFileExtensions[ERFT_SHADER].push_back("hs");
	mResourceFileExtensions[ERFT_SHADER].push_back("ds");
	mResourceFileExtensions[ERFT_SHADER].push_back("gs");
	mResourceFileExtensions[ERFT_SHADER].push_back("hlsl");
	mResourceFileExtensions[ERFT_SHADER].push_back("shader");

	// init texture file exts.
	mResourceFileExtensions[ERFT_TEXTURE].push_back("jpg");
	mResourceFileExtensions[ERFT_TEXTURE].push_back("jpeg");
	mResourceFileExtensions[ERFT_TEXTURE].push_back("png");
	mResourceFileExtensions[ERFT_TEXTURE].push_back("bmp");

	// init pipeline file exts
	mResourceFileExtensions[ERFT_PIPELINE].push_back("pipeline");
	mResourceFileExtensions[ERFT_PIPELINE].push_back("pipe");
	mResourceFileExtensions[ERFT_PIPELINE].push_back("pipeline.xml");
	mResourceFileExtensions[ERFT_PIPELINE].push_back("pipe.xml");

	// init material file exts
	mResourceFileExtensions[ERFT_MATERIAL].push_back("material");
	mResourceFileExtensions[ERFT_MATERIAL].push_back("mtrl");
	mResourceFileExtensions[ERFT_MATERIAL].push_back("material.xml");
	mResourceFileExtensions[ERFT_MATERIAL].push_back("mtrl.xml");
	
	// init mesh file exts.
	mResourceFileExtensions[ERFT_MESH].push_back("mesh");


	// create the resource loader object.
	mResourceLoader = new CResourceLoader(this, TextureManager, ShaderManager, InputlayoutManager,
		RenderStateManager, PipelineManager, MaterialManager, MeshManager);
}

CResourceGroupManager::~CResourceGroupManager()
{
	ReleaseReferenceCounted(mResourceLoader);
}


bool CResourceGroupManager::init(const std::string& configFileName)
{
	std::ifstream infile;
	infile.open(configFileName, std::ios::in);
	if (infile.fail())
	{
		//throw std::runtime_error("Can't open the resource config file!");
		GF_PRINT_CONSOLE_INFO("Can't open the resource config file!");
		return false;
	}

	std::string groupName = "";
	std::string line;
	E_RESOURCE_STORE_METHOD storeMethod;
	std::string dirPath;
	SResourceGroup* currentGroup;

	
	while (!infile.eof())
	{
		infile >> line;
		if (isResourceGroupLexical(line, groupName))
		{
			SResourceGroup group;
			group.Name = groupName;
			mResourceGroups.push_back(group);
			currentGroup = &mResourceGroups.back();
		}
		else
		{
			/* if the first group has not found. */
			if (groupName == "")
				continue;

			if (isResourceDirectoryLexical(line, storeMethod, dirPath))
			{
				SResourceDirectory dir;
				dir.Path = dirPath;
				dir.StoreMethod = storeMethod;
				currentGroup->Directories.push_back(dir);
			}
		}

		std::cout << line << std::endl;
	}

	infile.close();

	/* build the full paths of resources files. */
	for (u32 i = 0; i < mResourceGroups.size(); i++)
	{
		for (u32 j = 0; j < mResourceGroups[i].Directories.size(); j++)
		{
			setupFilePathsInDirectory(i, j);
		}
	}

	/* sort the resource files in each group according to the type. */
	for (u32 i = 0; i < mResourceGroups.size(); i++)
	{
		sort(mResourceGroups[i].Files.begin(), 
			mResourceGroups[i].Files.end(),  [](const SResourceFile& file1, const SResourceFile& file2){
			return file1.Type < file2.Type;
		});
	}

	return true;

}

bool CResourceGroupManager::isResourceGroupLexical(const std::string& s, std::string& groupname)
{
	u32 p1 = s.find('[');
	if (p1 == std::string::npos)
		return false;

	u32 p2 = s.find(']');
	if (p2 == std::string::npos || p1 + 1 > p2)
		return false;

	groupname = s.substr(p1 + 1, p2 - p1 - 1);

	return true;
}

bool CResourceGroupManager::isResourceDirectoryLexical(const std::string& s, E_RESOURCE_STORE_METHOD& storeMethod, std::string& dirstr)
{
	/* 
	 p1     p2 pe p3						   p4 
	 FileSystem=../media/textures/start_menu.jpg
	
	 ps: p1 is the first letter not space after start
	     p2 is the last letter not space before '='
		 p3 is the first letter not space after '='
		 p4 is the last letter not space before end
	*/

	u32 pe = s.find('=');
	if (pe == std::string::npos)
		return false;
	
	u32 p1 = s.find_first_not_of(' ');
	u32 p2 = s.find_last_not_of(' ', pe - 1);

	if (p1 == std::string::npos || p2 == std::string::npos || p1 >= p2)
		return false;

	std::string s1 = s.substr(p1, p2 - p1 + 1);

	// convert all the letters of s1 to upper.
	std::for_each(s1.begin(), s1.end(), [](char& c){
		if (c >= 'A' && c <= 'Z')
			c += ('a' - 'A');
	});

	if (s1 == "filesystem")
		storeMethod = ERSM_FILESYSTEM;
	else if (s1 == "zip")
		storeMethod = ERSM_ZIP;
	else
		return false;

	u32 p3 = s.find_first_not_of(' ', pe + 1);
	u32 p4 = s.find_last_not_of(' ');

	if (p3 == std::string::npos || p4 == std::string::npos || p3 >= p4)
		return false;
	
	dirstr = s.substr(p3, p4 - p3 + 1);
	return true;
}

bool CResourceGroupManager::setupFilePathsInDirectory(u32 groupIndex, u32 dirIndex)
{
	SResourceGroup& group = mResourceGroups[groupIndex];
	SResourceDirectory& dir = group.Directories[dirIndex];
	std::string& path = dir.Path;
	if (dir.StoreMethod == ERSM_FILESYSTEM)
	{
		//if the dir itself is a file, not a directory.
		if (!(GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY))
		{
			/* 先从完整路径中剥离出目录路径和文件名，
			然后将文件名存在SResourceFile的Name字段，
			将dir.Path修改为单纯的目录路径.
			这样将配置文件中指定文件路径和目录路径两种处理方式统一。*/
			
			std::string filename;
			std::string dirpath;
			seperateDirAndName(path, dirpath, filename);
			dir.Path = dirpath;
			registerResourceFileLocation(groupIndex, dirIndex, filename);
		}
		else
		{
			if (path.back() != '/')
				path.insert(path.end(), '/');

			/* 给路径后面加上"*"通配符 */
			std::string dirPathWithWildcard = path;
			dirPathWithWildcard.insert(dirPathWithWildcard.end(), '*');

			WIN32_FIND_DATAA ffd;
			HANDLE hFind;
			hFind = FindFirstFileA(dirPathWithWildcard.c_str(), &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
			{
				return false;
			}

			do
			{
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					registerResourceFileLocation(groupIndex, dirIndex, ffd.cFileName);
				}

			} while (FindNextFileA(hFind, &ffd) != 0);

			FindClose(hFind);
		}
		
	}

	return true;
}

E_RESOURCE_FILE_TYPE CResourceGroupManager::getResourceFileType(const std::string& path)
{
	u32 dotpos = path.find_last_of('.');
	if (dotpos == std::string::npos)
		return ERFT_UNKNOWN;

	std::string extstr = path.substr(dotpos + 1);
	const char* ext = extstr.c_str();
	for (u32 i = 0; i < ERFT_TYPE_COUNT; i++)
	{
		for (u32 j = 0; j < mResourceFileExtensions[i].size(); j++)
		{
			if (stricmp(ext, mResourceFileExtensions[i][j]) == 0)
				return (E_RESOURCE_FILE_TYPE)i;
		}
	}
	
	/* 判断是否是xxx.pipeline.xml这种文件名，需要获取文件名倒数第二个.的位置 */
	dotpos = path.find_last_of('.', dotpos - 1);
	if (dotpos == std::string::npos)
		return ERFT_UNKNOWN;

	extstr = path.substr(dotpos + 1);
	ext = extstr.c_str();
	for (u32 i = 0; i < ERFT_TYPE_COUNT; i++)
	{
		for (u32 j = 0; j < mResourceFileExtensions[i].size(); j++)
		{
			if (stricmp(ext, mResourceFileExtensions[i][j]) == 0)
				return (E_RESOURCE_FILE_TYPE)i;
		}
	}

	return ERFT_UNKNOWN;
}

bool CResourceGroupManager::registerResourceFileLocation(u32 groupIndex, s32 dirIndex, const std::string& filename)
{
	/* if the filename is already full path, just set the dirIndex to -1.*/
	/* if the resource file is a shader, add to the 'mShaderResourceFiles' member */

	E_RESOURCE_FILE_TYPE type = getResourceFileType(filename);
	if (type == ERFT_UNKNOWN)
		return false;

	switch (type)
	{
	case ERFT_TEXTURE:
	case ERFT_PIPELINE:
	case ERFT_MATERIAL:
	case ERFT_MESH:
		{
			SResourceFile file;
			file.Name = filename;
			file.DirectoryIndex = dirIndex;
			file.Type = type;
			mResourceGroups[groupIndex].Files.push_back(file);
		}
		break;
	}

	u32 locationIndex = (groupIndex << 16) | (dirIndex);
	mResourceFilesLocationInfos.insert(std::make_pair(filename, locationIndex));

	return true;
}

/* 从完整路径中剥离出目录路径和文件名，调用函数必须保证fullpath参数是一个'文件'的完整路径*/
void CResourceGroupManager::seperateDirAndName(const std::string& fullpath, std::string& dirpath, std::string& filename)
{
	u32 lastSlashPos = fullpath.find_last_of('/');
	if (lastSlashPos == std::string::npos)
	{
		dirpath = "";
		filename = fullpath;
	}
	else
	{
		dirpath = fullpath.substr(0, lastSlashPos + 1);
		filename = fullpath.substr(lastSlashPos + 1);
	}
}


bool CResourceGroupManager::loadResourceGroup(u32 groupid)
{
	if (groupid >= mResourceGroups.size())
		return false;

	const SResourceGroup& group = mResourceGroups[groupid];
	u32 fileCount = group.Files.size();


	for (u32 i = 0; i < fileCount; i++)
	{
		const SResourceFile& file = group.Files[i];
		switch (file.Type)
		{
		case ERFT_TEXTURE:
			mResourceLoader->loadTexture(file.Name);
			break;
		case ERFT_PIPELINE:
			mResourceLoader->loadPipelinesFromFile(file.Name);
			break;
		case ERFT_MATERIAL:
			mResourceLoader->loadMaterialsFromFile(file.Name);
			break;
		case ERFT_MESH:
			mResourceLoader->loadMeshFromFile(file.Name);
			break;
		}
	}


	return true;
}

bool CResourceGroupManager::loadResourceGroup(const std::string& groupname)
{
	for (u32 i = 0; i < mResourceGroups.size(); i++)
	{
		if (mResourceGroups[i].Name == groupname)
			return loadResourceGroup(i);
	}

	return false;
}

bool CResourceGroupManager::getFullPath(const std::string& resourceFileName, std::string& fullpath) const
{
	auto it = mResourceFilesLocationInfos.find(resourceFileName);
	if (it == mResourceFilesLocationInfos.end())
		return false;

	u32 locationIndex = it->second;
	u32 groupId = (locationIndex >> 16) & 0x0000FFFF;
	u32 directoryId = locationIndex & 0x0000FFFF;

	const std::string& dirpath = mResourceGroups[groupId].Directories[directoryId].Path;
	
	fullpath = dirpath + resourceFileName;

	return true;
}
