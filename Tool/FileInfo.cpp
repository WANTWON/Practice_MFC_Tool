#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	// 상대 경로를 저장할 문자 배열
	TCHAR		szRelativePath[MAX_PATH] = L"";

	// 현재의 절대경로(디렉토리 경로)를 저장할 배열
	TCHAR		szCurDirPath[MAX_PATH] = L"";

	// 현재 프로젝트가 설치된 절대 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath 에서 strFullPath로 가는 상대 경로를 구해서 szRelativePath에 저장해줌(단, 같은 드라이브 내에서만 가능함)

	PathRelativePathTo(szRelativePath, szCurDirPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	CFileFind			Find;
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iFileCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	// CFileFind : mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind			Find;

	// 경로 상의 모든 파일(*.*)을 찾기 위해 파일 경로를 수정
	wstring		wstrFilePath = wstrPath + L"//*.*";

	// FindFile : 경로에 파일이 존재하는지 유무 관계를 판단하는 함수, 파일이 존재하지 않으면 false, 존재하면 true를 리턴

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : FindFile함수 호출 이후 함수를 무조건 호출해야지만 실제 파일에 접근이 가능
		// 동일 경로 안에서 다음 파일을 찾기 위해 호출함, 더이상 찾을게 없거나 마지막 대상을 찾았을 때 0을 리턴

		bContinue = Find.FindNextFile();

		// 사실 디렉토리 내부에는 . 또는 ..과 같은 마커가 존재, 우리가 찾고자 하는 파일이 아니기 때문에 재 탐색을 지시
		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			//GetFilePath : 현재 find 객체가 가리키는 파일의 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			// "파일명.확장자"를 잘라냄
			PathRemoveFileSpec(szPathBuf);

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : 현재 Find객체가 가리키는 파일명만 추출하는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(시작, 끝) : 시작에서부터 끝에 해당하는 문자열을 얻어오는 함수
			// AKIHA_AKI13_000.png ->	AKIHA_AKI13_00%d.png
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());

			// D:\유준환\130기\Frame130\Texture\Stage\Player\Dash
			PathRemoveFileSpec(szBuf);

			// PathCombine(out, str1, str2); str1 + str2를 이어 붙여 out에 저장, str1과 str2사이에 \\가 자동으로 삽입 

			// D:\유준환\130기\Frame130\Texture\Stage\Player\Dash\\KIHA_AKI13_00%d.png
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());

			// ..\Texture\Stage\Player\Dash\\KIHA_AKI13_00%d.png
			pImgPath->wstrPath = ConvertRelativePath(szBuf);

			// ..\Texture\Stage\Player\Dash

			PathRemoveFileSpec(szBuf);

			// PathFindFileName : 파일명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			// Dash
			pImgPath->wstrStateKey = PathFindFileName(szBuf);


			// ..\Frame130\Texture\Stage\Player\
						
			PathRemoveFileSpec(szBuf);

			// PathFindFileName : 파일명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			// Player


			pImgPath->wstrObjKey = PathFindFileName(szBuf);
			rPathInfoList.push_back(pImgPath);

			bContinue = 0;
		}
	}
}
