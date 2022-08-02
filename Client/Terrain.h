#pragma once
#include "Obj.h"
class CTerrain :
	public CObj
{
public:
	CTerrain();
	virtual ~CTerrain();


public:
	vector<TILE*>&				Get_VecTile(void) { return m_vecTile; }
	vector<list<TILE*>>&		Get_VecAdj(void) { return m_vecAdjancency; }

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Load_Tile(const TCHAR* pFilePath);

	void		Ready_Adjacency(void);

private:
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>>		m_vecAdjancency;	//������ Ÿ���� ��θ� �˾Ƴ��� ���ؼ� ���� Ÿ�ϵ��� �����Ϸ��� ����

};

