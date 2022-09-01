#pragma once
#include "Timer.h"
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Shader.h"

class CScene
{
public:
	CScene();
	~CScene();

	ID3D12RootSignature* GetGraphicsRootSignature();

	//그래픽 루트 시그너쳐를 생성한다. 
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	void ReleaseUploadBuffers();

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseObjects();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool ProcessInput();

	void AnimateObjects(float fTimeElapsed);

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	



protected:
	//배치(Batch) 처리를 하기 위하여 씬을 셰이더들의 리스트로 표현한다. 
	CInstancingShader* m_pShaders = NULL;
	int m_nShaders = 0;

	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;	//루트 시그너쳐를 나타내는 인터페이스 포인터이다. 
	//ID3D12PipelineState *m_pd3dPipelineState = NULL; //파이프라인 상태를 나타내는 인터페이스 포인터이다. : Shader.h 로 넘어간거 같음
};

