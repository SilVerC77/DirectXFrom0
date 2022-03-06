#pragma once
#include <exception>
#include <string>

class CMyException :public std::exception {
	//throw from
	int iLine;
	std::string sFile;

protected:
	//what()is const so make it mutable
	mutable std::string sWhatBuffer;
public:
	CMyException(int _line, const char* _file)noexcept;

	//print out information
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
};