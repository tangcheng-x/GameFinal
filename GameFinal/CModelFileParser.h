#ifndef __MODEL_FILE_PARSER_CLASS_H__
#define __MODEL_FILE_PARSER_CLASS_H__

#include "IModelFileParser.h"

class CModelFileParser : public IModelFileParser
{
public:
	virtual bool parseModelFile(const std::string& filepath, SModelMeshCreateParams& createParams);
};

#endif
