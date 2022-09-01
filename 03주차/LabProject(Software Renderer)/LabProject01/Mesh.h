#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPoint3D
{
public:
	CPoint3D() { }
	CPoint3D(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	~CPoint3D() { }

	float						x = 0.0f;
	float						y = 0.0f;
	float						z = 0.0f;
};

class CVertex
{
public:
	CVertex() { m_f3Position = CPoint3D(0.0f, 0.0f, 0.0f); }
	CVertex(float x, float y, float z) { m_f3Position = CPoint3D(x, y, z); }
	~CVertex() { }

	CPoint3D					m_f3Position;
};

class CPolygon
{
public:
	CPolygon() { }
	CPolygon(int nVertices);
	~CPolygon();

	int							m_nVertices = 0;
	CVertex						*m_pVertices = NULL;

	void SetVertex(int nIndex, CVertex vertex);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMesh
{
public:
	CMesh() { }
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int							m_nReferences = 1;

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

private:
	int							m_nPolygons = 0;
	CPolygon					**m_ppPolygons = NULL;

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);

	virtual void Render(HDC hDCFrameBuffer);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};

