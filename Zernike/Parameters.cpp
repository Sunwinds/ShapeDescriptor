//
// Parameters.cpp
//

#include "Parameters.h"

void Parameters::Init(const std::string &filename)
{
    std::vector<std::string> fileLines = Utility::GetFileLines(filename, 3);
    for(auto lineIterator = fileLines.begin(); lineIterator != fileLines.end(); lineIterator++)
    {
        const std::string &curLine = *lineIterator;
        if(curLine.find("databaseDirectory") == 0)
        {
            databaseDirectory = Utility::PartitionString(curLine, "=")[1];
            if(databaseDirectory[databaseDirectory.length() - 1] != '/')
            {
                databaseDirectory.push_back('/');
            }
        }

        if(curLine.find("inputScenes") == 0)
        {
            std::string sceneSequence = Utility::PartitionString(curLine, "=")[1];
			inputScenes = Utility::PartitionString(sceneSequence, " ");
        }

        if(curLine.find("textQuery") == 0)
        {
            textQuery = Utility::PartitionString(curLine, "=")[1];
        }

		if(curLine.find("taskName") == 0)
		{
			taskName = Utility::PartitionString(curLine, "=")[1];
		}

		if(curLine.find("perspective") == 0)
		{
			perspective = Utility::PartitionString(curLine, "=")[1];
		}

		if (curLine.find("kernelType") == 0)
		{
			kernelType = Utility::PartitionString(curLine, "=")[1];
		}
    }
}
