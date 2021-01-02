#ifndef Engine_Struct_h__
#define Engine_Struct_h__

BEGIN(Engine)
class CGameObject;
class CColliderObject;
END

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE/* | D3DFVF_TEX0*/;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	// D3DFVF_TEXCOORDSIZE3 : uv���� float�� 3���� ũ�� ��ŭ�̸� ��ȣ ���� ���� 0�� �ǹ̴� 
	// ���� ���ؽ��� �ؽ��� uv���� ���� ���� �� �� �ִµ� �� �� 0��° ���� �����ϰڴٴ� �ǹ�
	
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

		LPD3DXMESH				pOriMesh;	// x������ �ε����� �� ������ ���¸� ������ �ִ� �� ��ü(�Һ�)

		_ulong					dwNumBones;	// ���� ������ �����ϴ� ����


		_matrix*				pFrameOffsetMatrix;	// �ִϸ��̼��� ������ ������ �� ����
		_matrix**				ppFrameCombinedMatrix;	// frame ���� ������ �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϱ� ���� ������
		_matrix*				pRenderingMatrix;	//pFrameOffsetMatrix * (*ppFrameCombinedMatrix) ��������� ���������� �������� �ϱ� ���� ���


	}D3DXMESHCONTAINER_DERIVED;

	typedef struct _collision_info {
		CGameObject* pCollidedObject = nullptr;
		CColliderObject* pCollidedCollider = nullptr;
		//_vec3 vCollidedPos{0.f, 0.f, 0.f};
	}CollisionInfo;

	typedef struct _box_info {
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3]; // �ڽ��� �������κ��� �� ������ ������� ����
		_vec3		vAxis[3];	 // ��ü�� ������ �� ����
	}BOXINFO;

	typedef struct _triangle_info {
		_vec3 vVertex[3];
		_int iTagIndex = 0;
	}TriangleInfo;

	typedef struct _texture_info
	{
		_texture_info() {
			ZeroMemory(this, sizeof(this));
		}
		//LPDIRECT3DTEXTURE9 pTexture;	// �̹��� ������ �����ϱ� ���� �İ�ü. 
		IDirect3DBaseTexture9* pTexture;
		D3DXIMAGE_INFO tImageInfo;		//�̹����� ������ ��Ƶ� ����ü. 
	}TextureInfo;
}

#endif // Engine_Struct_h__
