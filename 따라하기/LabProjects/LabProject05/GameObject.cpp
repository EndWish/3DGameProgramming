#include "stdafx.h"
#include "Shader.h"	//
#include "GameObject.h"



//������
CGameObject::CGameObject() 
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}
//�Ҹ���
CGameObject::~CGameObject() 
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader) {
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}
//�� ���̴�
void CGameObject::SetShader(CShader* pShader) 
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}
//�� �޽�
void CGameObject::SetMesh(CMesh* pMesh) 
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}
//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
void CGameObject::ReleaseUploadBuffers() 
{
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��. 
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}
//�ִϸ��̼�
void CGameObject::Animate(float fTimeElapsed) 
{

}

void CGameObject::OnPrepareRender() 
{
}
//�������ϱ�
void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera) 
{
	OnPrepareRender();

	if (m_pShader) {
		//���� ��ü�� ���� ��ȯ ����� ���̴��� ��� ���۷� ����(����)�Ѵ�. 
		m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
		m_pShader->Render(pd3dCommandList, pCamera);
	}

	//���� ��ü�� �޽��� ����Ǿ� ������ �޽��� �������Ѵ�. 
	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}


void CGameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle) 
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

/// ///////////////////////////////////////////////////////////////////////////
/// 
CRotatingObject::CRotatingObject() 
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 90.0f;
}

CRotatingObject::~CRotatingObject() 
{
}

void CRotatingObject::Animate(float fTimeElapsed) 
{
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}

