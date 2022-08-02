#pragma once

#include "Include.h"


class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString	ConvertRelativePath(CString	strFullPath);

	static int	DirFileCount(const wstring& wstrPath); //파일의 개수
	static void	DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);	//파일을 찾는 함수
};

