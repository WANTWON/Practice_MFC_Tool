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
	// ��� ��θ� ������ ���� �迭
	TCHAR		szRelativePath[MAX_PATH] = L"";

	// ������ ������(���丮 ���)�� ������ �迭
	TCHAR		szCurDirPath[MAX_PATH] = L"";

	// ���� ������Ʈ�� ��ġ�� ���� ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath ���� strFullPath�� ���� ��� ��θ� ���ؼ� szRelativePath�� ��������(��, ���� ����̺� �������� ������)

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
	// CFileFind : mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind			Find;

	// ��� ���� ��� ����(*.*)�� ã�� ���� ���� ��θ� ����
	wstring		wstrFilePath = wstrPath + L"//*.*";

	// FindFile : ��ο� ������ �����ϴ��� ���� ���踦 �Ǵ��ϴ� �Լ�, ������ �������� ������ false, �����ϸ� true�� ����

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : FindFile�Լ� ȣ�� ���� �Լ��� ������ ȣ���ؾ����� ���� ���Ͽ� ������ ����
		// ���� ��� �ȿ��� ���� ������ ã�� ���� ȣ����, ���̻� ã���� ���ų� ������ ����� ã���� �� 0�� ����

		bContinue = Find.FindNextFile();

		// ��� ���丮 ���ο��� . �Ǵ� ..�� ���� ��Ŀ�� ����, �츮�� ã���� �ϴ� ������ �ƴϱ� ������ �� Ž���� ����
		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			//GetFilePath : ���� find ��ü�� ����Ű�� ������ ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			// "���ϸ�.Ȯ����"�� �߶�
			PathRemoveFileSpec(szPathBuf);

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : ���� Find��ü�� ����Ű�� ���ϸ� �����ϴ� �Լ�
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(����, ��) : ���ۿ������� ���� �ش��ϴ� ���ڿ��� ������ �Լ�
			// AKIHA_AKI13_000.png ->	AKIHA_AKI13_00%d.png
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());

			// D:\����ȯ\130��\Frame130\Texture\Stage\Player\Dash
			PathRemoveFileSpec(szBuf);

			// PathCombine(out, str1, str2); str1 + str2�� �̾� �ٿ� out�� ����, str1�� str2���̿� \\�� �ڵ����� ���� 

			// D:\����ȯ\130��\Frame130\Texture\Stage\Player\Dash\\KIHA_AKI13_00%d.png
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());

			// ..\Texture\Stage\Player\Dash\\KIHA_AKI13_00%d.png
			pImgPath->wstrPath = ConvertRelativePath(szBuf);

			// ..\Texture\Stage\Player\Dash

			PathRemoveFileSpec(szBuf);

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			// Dash
			pImgPath->wstrStateKey = PathFindFileName(szBuf);


			// ..\Frame130\Texture\Stage\Player\
						
			PathRemoveFileSpec(szBuf);

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			// Player


			pImgPath->wstrObjKey = PathFindFileName(szBuf);
			rPathInfoList.push_back(pImgPath);

			bContinue = 0;
		}
	}
}
