#pragma once
#include "Obj.h"
class CConstruction :
	public CObj
{
public:
	CConstruction();
	virtual ~CConstruction();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Load_Construct(const TCHAR* pFilePath);
	vector<CONSTRUCT*>  m_vecConstruct;
};

