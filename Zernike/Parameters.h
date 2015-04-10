//
// Parameters.h
//

#include "Utility.h"

struct Parameters
{
    void Init(const std::string &filename);

    std::string databaseDirectory;
    std::vector<std::string> inputScenes;

    std::string textQuery;
	std::string taskName;
	std::string perspective;
	std::string kernelType;
};