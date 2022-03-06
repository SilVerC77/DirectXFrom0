#include "CMyException.h"
#include <sstream>

CMyException::CMyException(int _line, const char* _file)noexcept
	:iLine(_line)
	, sFile(_file)
{}

const char* CMyException::what()const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	sWhatBuffer = oss.str();
	return sWhatBuffer.c_str();
}

const char* CMyException::GetType()const noexcept
{
	return "CMyException";
}

int CMyException::GetLine()const noexcept
{
	return iLine;
}

const std::string& CMyException::GetFile() const noexcept
{
	return sFile;
}

std::string CMyException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << sFile << std::endl << "[Line] " << iLine;
	return oss.str();
}

