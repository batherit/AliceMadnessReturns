#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ  | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	// D3DFVF_TEXCOORDSIZE3 : uv값이 float형 3개의 크기 만큼이며 괄호 안의 숫자 0의 의미는 
	// 본래 버텍스에 텍스쳐 uv값이 여러 개가 올 수 있는데 그 중 0번째 값을 지정하겠다는 의미
	
	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;
		
	typedef struct tagPickingRayInfo {
		_vec3 vRayDir;
		_vec3 vRayPos;
	}PICKINGRAYINFO;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;


	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;	// x파일을 로드했을 때 최초의 상태를 가지고 있는 컴 객체(불변)

		_ulong					dwNumBones;	// 뼈의 개수를 보관하는 변수


		_matrix*				pFrameOffsetMatrix;	// 애니메이션을 포함한 최초의 뼈 상태
		_matrix**				ppFrameCombinedMatrix;	// frame 마다 가지고 있는 CombinedTransformationMatrix의 주소값들을 보관하기 위한 포인터
		_matrix*				pRenderingMatrix;	//pFrameOffsetMatrix * (*ppFrameCombinedMatrix) 결과값으로 최종적으로 렌더링을 하기 위한 행렬


	}D3DXMESHCONTAINER_DERIVED;
}

#endif // Engine_Struct_h__
