//
// Utility.cpp
//

#include "Utility.h"

bool Utility::FileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return (!file.fail());
}

std::vector<std::string> Utility::GetFileLines(const std::string &filename, unsigned int minLineLength)
{
    if(!Utility::FileExists(filename))
    {
        std::cout << "Required file not found: " << filename << '\n';
        exit(1);
    }
    std::ifstream file(filename);
    std::vector<std::string> result;
    std::string curLine;

	// to speed up the reading
	//std::ios_base::sync_with_stdio(false);

    while(!file.fail())
    {
        std::getline(file, curLine);
        if(!file.fail() && curLine.length() >= minLineLength)
        {
        	if (curLine.at(curLine.length()-1) == '\r')
        		curLine = curLine.substr(0,curLine.size()-1);
            result.push_back(curLine);
        }
    }
    return result;
}

std::vector<std::string> Utility::PartitionString(const std::string &s, const std::string &separator)
{
    std::vector<std::string> result;
    std::string curEntry;
    for(unsigned int outerCharacterIndex = 0; outerCharacterIndex < s.length(); outerCharacterIndex++)
    {
        bool isSeperator = true;
        for(unsigned int innerCharacterIndex = 0; innerCharacterIndex < separator.length() && outerCharacterIndex + innerCharacterIndex < s.length() && isSeperator; innerCharacterIndex++)
        {
            if(s[outerCharacterIndex + innerCharacterIndex] != separator[innerCharacterIndex])
            {
                isSeperator = false;
            }
        }

        if(isSeperator)
        {
            if(curEntry.length() > 0)
            {
                result.push_back(curEntry);
                curEntry.clear();
            }
            outerCharacterIndex += separator.length() - 1;
        }
        else
        {
            curEntry.push_back(s[outerCharacterIndex]);
        }
    }
    if(curEntry.length() > 0)
    {
        result.push_back(curEntry);
    }
    return result;
}

int Utility::StringToInt(const std::string &s)
{
    std::stringstream stream(std::stringstream::in | std::stringstream::out);
    stream << s;
    
    int result;
    stream >> result;
    return result;
}

float Utility::StringToFloat(const std::string &s)
{
    std::stringstream stream(std::stringstream::in | std::stringstream::out);
    stream << s;

    float result;
    stream >> result;
    return result;
}

std::vector<int> Utility::StringToIntegerList(const std::string &s, const std::string &prefix)
{
    std::string subString = Utility::PartitionString(s, prefix)[0];
    auto parts = Utility::PartitionString(subString, " ");

    std::vector<int> result(parts.size());
    for(unsigned int resultIndex = 0; resultIndex < result.size(); resultIndex++)
    {
        result[resultIndex] = Utility::StringToInt(parts[resultIndex]);
    }
    return result;
}

std::vector<float> Utility::StringToFloatList(const std::string &s, const std::string &prefix)
{
    std::string subString = Utility::PartitionString(s, prefix)[0];
    auto parts = Utility::PartitionString(subString, " ");

    std::vector<float> result(parts.size());
    for(unsigned int resultIndex = 0; resultIndex < result.size(); resultIndex++)
    {
        result[resultIndex] = Utility::StringToFloat(parts[resultIndex]);
    }
    return result;
}

//GLuint Utility::MakeOpenGLBitmap(const std::string &filename)
//{
//    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
//	FIBITMAP *dib = NULL;
//	BYTE* bits = NULL;
//	unsigned int width = 0, height = 0;
//
//	fif = FreeImage_GetFileType(filename.c_str(), 0);
//	if (fif == FIF_UNKNOWN)
//		fif = FreeImage_GetFIFFromFilename(filename.c_str());
//	if (fif == FIF_UNKNOWN)
//		return 0;
//
//	if (FreeImage_FIFSupportsReading(fif))
//		dib = FreeImage_Load(fif, filename.c_str());
//	if (!dib)
//		return 0;
//
//    FIBITMAP *convertedDib;
//    if(FreeImage_GetBPP(dib) == 24)
//    {
//        convertedDib = dib;
//    }
//    else
//    {
//        convertedDib = FreeImage_ConvertTo24Bits(dib);
//    }
//
//    bits = FreeImage_GetBits(convertedDib);
//	width = FreeImage_GetWidth(convertedDib);
//	height = FreeImage_GetHeight(convertedDib);
//
//    GLuint texture;
//    glGenTextures( 1, &texture );
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
//
//    if(FreeImage_GetBPP(dib) != 24)
//    {
//        FreeImage_Unload(convertedDib);
//    }
//
//	FreeImage_Unload(dib);
//
//	return texture;
//}

//void Utility::checkGLError(std::string str)
//{
//	GLenum error;
//	if ((error = glGetError()) != GL_NO_ERROR)
//		printf("GL Error: %s (%s)\n", gluErrorString(error), str.c_str());
//}

std::string convertInt(int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}