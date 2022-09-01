#include "stdafx.h"
#include "stdafx.h"
#include "Scene.h"

//생성자
CScene::CScene()
{
}
//소멸자
CScene::~CScene()
{
}

ID3D12RootSignature* CScene::GetGraphicsRootSignature() {
	return(m_pd3dGraphicsRootSignature);
}

/*루트 시그너쳐 : 어떤 리소스들이 파이프라인의 쉐이더에 연결되는 가를 정의
루트 매개변수 : 하나의 루트 시그너쳐는 64개의 32비트(DWORD) 배열로 구성됨 (많이 사용하는 매개변수를 앞쪽에 배치)
루트 상수, 루트 서술자, 서술자 테이블*/
ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues = 16;
	pd3dRootParameters[0].Constants.ShaderRegister = 0;
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1;
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	// 루트 시그너처 생성(위의 변수들을 매개변수로 받을 수 있도록)
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;

	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

// 초기화
void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	//가로x세로x깊이가 12x12x12인 정육면체 메쉬를 생성한다. 
	CCubeMeshDiffused *pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);

	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	//메쉬와 오브젝트 생성
	CRotatingObject* pRotatingObject = new CRotatingObject();
	pRotatingObject->SetMesh(pCubeMesh);

	CDiffusedShader* pShader = new CDiffusedShader();
	pShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	pRotatingObject->SetShader(pShader);

	m_ppObjects[0] = pRotatingObject;
}
// Release
void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	
	if (m_ppObjects) {
		for (int j = 0; j < m_nObjects; j++) 
			if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}
// ReleaseUploadBuffers
void CScene::ReleaseUploadBuffers() {
	if (m_ppObjects)
		for (int j = 0; j < m_nObjects; j++) 
			if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

//마우스 입력 메시지 처리
bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}
//키보드 입력 메시지 처리
bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}
//입력 처리
bool CScene::ProcessInput()
{
	return(false);
}

//오브젝트들 애니메이션 실행
void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++) {
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}

//렌더링 준비(그래픽 루트 시그너쳐 Set, 파이프라인 상태 Set), 프리미티브 토폴로지(삼각형 리스트)를 설정
//void CScene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	//그래픽 루트 시그너쳐를 설정한다. 
//	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
//	//파이프라인 상태를 설정한다. 
//	pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
//	//프리미티브 토폴로지(삼각형 리스트)를 설정한다. 
//	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	//카메라 뷰포트와 시져렉트 셋팅
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	//그래픽 루트 시그너쳐를 파이프라인에 연결(설정)한다. 
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if (pCamera) pCamera->UpdateShaderVariables(pd3dCommandList);

	//씬을 렌더링하는 것은 씬을 구성하는 게임 객체(셰이더를 포함하는 객체)들을 렌더링하는 것이다. 
	for (int j = 0; j < m_nObjects; j++) {
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera);
	}
}

