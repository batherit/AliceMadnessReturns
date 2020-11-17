#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	DWORD Safe_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if (dwRefCnt == 0)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T>
	DWORD Safe_AddRef(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
			dwRefCnt = pointer->AddRef();
				
		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destory(T& pointer)
	{
		if (NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}
	//////////////////////////////////////////////////////////////////
	/////////////////////////////////Functor(�Լ� ��ü)//////////////////////////

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag)
			: m_pTargetTag(pTag) 
		{		
		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// ���������̳� ������
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};

	// Ŭ���̾�Ʈ Ŀ�� ��ǥ���
	inline POINT GetClientCursorPoint(const HWND& _hWND)
	{
		POINT pt;
		GetCursorPos(&pt);				// ����ũž ���� ��ǥ
		ScreenToClient(_hWND, &pt);	// ������â ���� ��ǥ

		return pt;
	}
	
	// Ŀ�� ��ǥ�� ȭ�� �� �����ϴ���?
	inline _bool IsPointInClient(const HWND& _hWND, const POINT& _ptClientPoint) {
		RECT rectView;
		GetClientRect(_hWND, &rectView);

		if (_ptClientPoint.x < 0.f) return false;
		if (_ptClientPoint.x > rectView.right - rectView.left) return false;
		if (_ptClientPoint.y < 0.f) return false;
		if (_ptClientPoint.y > rectView.bottom - rectView.top) return false;

		return true;
	}
	
	// ȭ�鿡�� ���ƴٴϴ� ��ǥ���
	inline POINT GetToroidClientPoint(const HWND& _hWND, const POINT& _ptClientPoint) {
		RECT rectView;
		GetClientRect(_hWND, &rectView);

		LONG iWinCX = rectView.right - rectView.left;
		LONG iWinCY = rectView.bottom - rectView.top;

		POINT ptClientPoint = _ptClientPoint;
		if (_ptClientPoint.x < 0.f) ptClientPoint.x += iWinCX;
		if (_ptClientPoint.x > iWinCX) ptClientPoint.x -= iWinCX;
		if (_ptClientPoint.y < 0.f) ptClientPoint.y += iWinCY;
		if (_ptClientPoint.y > iWinCY) ptClientPoint.y -= iWinCY;

		return ptClientPoint;
	}

	template<typename T>
	void Clamp(T* _pValue, T _Min, T _Max) {
		if (*_pValue < _Min)		*_pValue = _Min;
		else if (*_pValue > _Max)	*_pValue = _Max;
	}

	template<typename T>
	T Clamp(T _Value, T _Min, T _Max) {
		Clamp(&_Value, _Min, _Max);
		return _Value;
	}

	// 2�� ������ �
	template<typename T>
	T GetQuadraticBezierCurve(float _fT, T _p1, T _p2, T _p3) {
		Clamp(&_fT, 0.f, 1.f);
		return _p1 * (1.f - _fT) * (1.f - _fT) + _p2 * 2.f * _fT * (1.f - _fT) + _p3 * _fT * _fT;
	}

	// ���� ��ƿ
	inline _float GetRandomFloat(void) {
		return static_cast <_float> (rand()) / static_cast <_float> (RAND_MAX);
	}
	inline _int GetNumberMinBetweenMax(_int _iMin, _int _iMax) {
		if (_iMin > _iMax) abort();

		return rand() % (_iMax - _iMin + 1) + _iMin;
	}
	inline _float GetNumberMinBetweenMax(_float _fMin, _float _fMax) {
		if (_fMin > _fMax) abort();

		float fT = GetRandomFloat();
		return _fMin * (1.f - fT) + _fMax * fT;
	}
	inline void ToggleBoolean(_bool& _bValue) {
		if (_bValue) _bValue = false;
		else _bValue = true;
	}

	// ȸ���� ���
	inline _vec3 GetRotationAxis(const _vec3& vU, const _vec3& vV) {
		_vec3 vRotAxis;
		D3DXVec3Cross(&vRotAxis, &vU, &vV);
		D3DXVec3Normalize(&vRotAxis, &vRotAxis);
		return vRotAxis;
	}

	// ȸ���� ���
	inline _float GetRotationAngle(const _vec3& vU, const _vec3& vV) {
		_vec3 vUnitU;
		_vec3 vUnitV;
		_float fDot;
		D3DXVec3Normalize(&vUnitU, &vU);
		D3DXVec3Normalize(&vUnitV, &vV);
		fDot = Clamp(D3DXVec3Dot(&vUnitU, &vUnitV), -1.f, 1.f);
		return acosf(fDot);
	}

	// ���ʹϾ� ������ ȸ�� ��� ���
	inline _matrix* GetRotationMatrixSlerp(_matrix *_pOut, const _matrix* _pFromM, const _matrix* _pToM, const _float& _fT) {
		D3DXQUATERNION _FromQ, _ToQ, _LerpQ;
		D3DXQuaternionRotationMatrix(&_FromQ, _pFromM);
		D3DXQuaternionRotationMatrix(&_ToQ, _pToM);

		D3DXQuaternionSlerp(&_LerpQ, &_FromQ, &_ToQ, _fT);

		D3DXMatrixRotationQuaternion(_pOut, &_LerpQ);

		return _pOut;
	}

	// ���� ��Ŀ��� ȸ�� ��� ���� �����ϱ�
	inline _matrix* ExtractRotationMatrix(_matrix* _pOut, const _matrix* _pWorldM) {
		_vec3 vRight = _vec3(_pWorldM->_11, _pWorldM->_12, _pWorldM->_13);
		_vec3 vUp = _vec3(_pWorldM->_21, _pWorldM->_22, _pWorldM->_23);
		_vec3 vLook = _vec3(_pWorldM->_31, _pWorldM->_32, _pWorldM->_33);

		D3DXMatrixIdentity(_pOut);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		_pOut->_11 = vRight.x;	_pOut->_12 = vRight.y;	_pOut->_13 = vRight.z;
		_pOut->_21 = vUp.x;		_pOut->_22 = vUp.y;		_pOut->_23 = vUp.z;
		_pOut->_31 = vLook.x;	_pOut->_32 = vLook.y;	_pOut->_33 = vLook.z;

		return _pOut;
	}

	// ����ġ
	inline float GetWeightByDegree(float _fDegree) {
		return cosf(D3DXToRadian(_fDegree)) * 0.5f + 0.5f;
	}

	// ����Ʈ ��� ���
	inline _matrix GetViewportMatrix(const D3DVIEWPORT9& _vp) {
		_matrix matViewport;
		D3DXMatrixIdentity(&matViewport);

		matViewport._11 = _vp.Width * 0.5f;
		matViewport._22 = -(_vp.Height * 0.5f);
		matViewport._33 = _vp.MaxZ - _vp.MinZ;
		matViewport._41 = _vp.X + _vp.Width * 0.5f;
		matViewport._42 = _vp.Y + _vp.Height * 0.5f;
		matViewport._43 = _vp.MinZ;

		return matViewport;
	}

	// ��������
	inline PICKINGRAYINFO GetPickingRayInfo(LPDIRECT3DDEVICE9 _pGraphicDev, const POINT& _ptClient) {
		_vec3 vRay = _vec3(static_cast<FLOAT>(_ptClient.x), static_cast<FLOAT>(_ptClient.y), 0.f);
		_matrix matInvViewport, matInvProj, matInvView;
		D3DVIEWPORT9 vp;
		_pGraphicDev->GetViewport(&vp);

		// ����Ʈ ����� �� ���ϱ�
		matInvViewport = GetViewportMatrix(vp);
		D3DXMatrixInverse(&matInvViewport, NULL, &matInvViewport);

		// ���� ����� �� ���ϱ�
		_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matInvProj);
		D3DXMatrixInverse(&matInvProj, NULL, &matInvProj);

		// �� ����� �� ���ϱ�
		_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
		D3DXMatrixInverse(&matInvView, NULL, &matInvView);

		D3DXVec3TransformCoord(&vRay, &vRay, &(matInvViewport * matInvProj * matInvView));		// ī�޶� ����
		_vec3 vCameraPos = _vec3(matInvView._41, matInvView._42, matInvView._43);
		vRay -= vCameraPos;
		D3DXVec3Normalize(&vRay, &vRay);

		return PICKINGRAYINFO{ vRay, vCameraPos };
	}

	// ���� �浹 ���� ���
	inline _vec3 GetHitPos(const _vec3& _vV1, const _vec3& _vV2, const _vec3& _vV3, const _float& _fU, const _float& _fV) {
		return _vV1 + _fU * (_vV2 - _vV1) + _fV * (_vV3 - _vV1);
	}
}


#endif // Engine_Function_h__