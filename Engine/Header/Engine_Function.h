#ifndef Engine_Function_h__
#define Engine_Function_h__

#include <list>

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

	// ��ȿȭ ��ü ����
	/*template <typename T>
	inline void CollectInvalidObjects(list<T*>& _list)
	{
		for (auto& pObj : _list)
		{
			if (!pObj->IsValid())
			{
				Safe_Release(pObj);
			}
		}
		_list.remove_if([](auto& pObj)
		{
			return pObj == nullptr;
		});
	}*/

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

	// Ŭ����
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

	// ���� �� ������ �Ÿ�
	inline _float GetDistanceBetweenPlaneAndPoint(const _plane& _plPlane, const _vec3& _vPos) {
		_vec3 vPlaneNormal = _vec3(_plPlane.a, _plPlane.b, _plPlane.c);
		return D3DXVec3Dot(&vPlaneNormal, &_vPos) + _plPlane.d;
	}

	// ��鿡 ������ �� ��ġ���
	inline _vec3 GetPointProjectedOntoPlane(const _plane& _plPlane, const _vec3& _vPos) {
		_vec3 vPlaneNormal = _vec3(_plPlane.a, _plPlane.b, _plPlane.c);
		_float fK = D3DXVec3Dot(&vPlaneNormal, &_vPos) + _plPlane.d;
		return _vPos - fK * vPlaneNormal;
	}

	// �ﰢ���� ������ �� ��ġ���
	inline _vec3 GetPointProjectedOntoTriangle(const _vec3& _vV1, const _vec3& _vV2, const _vec3& _vV3, const _vec3& _vPos) {
		_plane plPlane;
		return GetPointProjectedOntoPlane(*D3DXPlaneFromPoints(&plPlane, &_vV1, &_vV2, &_vV3), _vPos);
	}

	// ���� ���Ͱ� ���� ���迡 �ִ���? => ������� �浹�� ���ɼ��� ����. 
	inline _bool IsPlaneAndVectorOrthogonal(const _vec3& _vV, const _plane& _plPlane) {
		_vec3 vPlaneNormal = _vec3(_plPlane.a, _plPlane.b, _plPlane.c);
		return D3DXVec3Dot(&_vV, &vPlaneNormal) < 0.00001f;
	}

	// ������ ����� �浹�ߴ���?
	inline _bool IsRayAndPlaneCollided(const PICKINGRAYINFO& _stPickingInfo, const _plane& _plPlane, _float* pT = nullptr) {
		_vec3 vPlaneNormal = _vec3(_plPlane.a, _plPlane.b, _plPlane.c);
		_float fT = (-_plPlane.d - (D3DXVec3Dot(&vPlaneNormal, &_stPickingInfo.vRayPos)))
			/ D3DXVec3Dot(&vPlaneNormal, &_stPickingInfo.vRayDir);

		if (pT)
			*pT = fT;

		return  fT >= 0.f;
	}

	// ������ ���� �浹�ߴ���?
	inline _bool IsRayAndSphereCollided(const PICKINGRAYINFO& _stPickingInfo, 
		_float _fRadius, const _vec3& _vSpherePos, _float* pT = nullptr) {
		// �ٷ� �� �׷��Ƚ� å ����
		_vec3 vM = _stPickingInfo.vRayPos - _vSpherePos;
		_float fA = D3DXVec3Dot(&_stPickingInfo.vRayDir, &_stPickingInfo.vRayDir);		// u.u
		_float fB = 2.f * D3DXVec3Dot(&vM, &_stPickingInfo.vRayDir);					// 2(m.u)
		_float fC = D3DXVec3Dot(&vM, &vM) - _fRadius * _fRadius;						// m.m - r^2
		_float fD = fB * fB - 4.f * fA * fC;	// �Ǻ��� b^2 - 4ac

		if (fD < 0.f)	// �Ǻ����� �����̸� �浹���� ���� ���̴�.
			return false;

		_float fT1 = (-fB + sqrtf(fD)) / (2.f * fA);
		_float fT2 = (-fB - sqrtf(fD)) / (2.f * fA);
		_float fT = min(fT1, fT2);

		if (fT < 0.f)	// fT�� �����̸�, �浹���� ���� ���̴�.
			return false;

		if (pT)
			*pT = fT;

		return true;
	}

	// ���� ���� �浹
	inline _bool IsPointAndSphereCollided(const _vec3 _vPoint, _float _fRadius, const _vec3 _vSpherePos) {
		return D3DXVec3Length(&(_vPoint - _vSpherePos)) <= _fRadius;
	}

	// ���� ��� �����ϴ� ���� ��ǥ ���
	inline _vec3 GetPointBetweenRayAndPlane(const PICKINGRAYINFO& _stPickingInfo, const _plane& _plPlane) {
		_vec3 vPlaneNormal = _vec3(_plPlane.a, _plPlane.b, _plPlane.c);
		_float fT = (-_plPlane.d - (D3DXVec3Dot(&vPlaneNormal, &_stPickingInfo.vRayPos)))
			/ D3DXVec3Dot(&vPlaneNormal, &_stPickingInfo.vRayDir);
		return _stPickingInfo.vRayPos + fT * _stPickingInfo.vRayDir;
	}

	// ���� ��ƿ
	inline _float GetRandomFloat(void) {
		return static_cast <_float> (rand()) / static_cast <_float> (RAND_MAX);
	}
	inline _int GetNumberBetweenMinMax(_int _iMin, _int _iMax) {
		if (_iMin > _iMax) abort();

		return rand() % (_iMax - _iMin + 1) + _iMin;
	}
	inline _float GetNumberBetweenMinMax(_float _fMin, _float _fMax) {
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

	inline _vec3 GetRotatedVector(const _vec3& _vAxis, const _float& _fAngle, const _vec3& _vV) {
		_vec3 vRotatedVector;
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &_vAxis, _fAngle);
		D3DXVec3TransformNormal(&vRotatedVector, &_vV, &matRot);

		return vRotatedVector;
	}

	// ���ʹϾ� ������ ȸ�� ��� ���
	inline _matrix* GetRotationMatrixSlerp(_matrix *_pOut, const _matrix* _pFromM, const _matrix* _pToM, const _float& _fT) {
		_qt _FromQ, _ToQ, _LerpQ;
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
	inline _float GetWeightByCos(_float _fCosRadian) {	// 0~2pi�� ���� ��������� 0~1�� ���� ��ȯ
		return 0.5f * cosf(_fCosRadian) + 0.5f;
	}
	inline _float GetWeightByValue(_float _fValue, _float _fMin, _float _fMax) {	// _fMin�� �������� 0�� ����� ����, _fMax�� �������� 1�� ����� ���� ��ȯ
		if (_fMin > _fMax) abort();

		return Clamp((_fValue - _fMin) / (_fMax - _fMin), 0.f, 1.f);
	}
	inline _float GetValueByWeight(_float _fWeight, _float _fZero, _float _fOne) {	// 0~1�� fZero, fOne�� ������ ���� ���� => ����
		Clamp(&_fWeight, 0.f, 1.f);
		return _fZero * (1.f - _fWeight) + _fOne * _fWeight;
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

	// �� �������κ��� ������� 
	inline _vec3 GetCross(const _vec3& _vV1, const _vec3& _vV2, const _vec3& _vV3) {
		_vec3 vCross;
		D3DXVec3Cross(&vCross, &(_vV2 - _vV1), &(_vV3 - _vV1));
		return vCross;
	}

	inline _bool IsFacing(const _vec3& _vDir1, const _vec3& _vDir2) {
		_vec3 vNormalizedDir1, vNormalizedDir2;
		D3DXVec3Normalize(&vNormalizedDir1, &_vDir1);
		D3DXVec3Normalize(&vNormalizedDir2, &_vDir2);
		
		// �����̸� ���� ���� �ִ� ���̴�.
		return D3DXVec3Dot(&vNormalizedDir1, &vNormalizedDir2) < 0.f;
	}

	//�ݻ纤��
	inline _vec3 GetReflectionVector(const _vec3& _vIncident, const _vec3& _vNormal) {	
		_vec3 vNormal;
		D3DXVec3Normalize(&vNormal, &_vNormal);

		return _vIncident + 2.f * vNormal * (-D3DXVec3Dot(&_vIncident, &vNormal));
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

	// ���ʹϾ� -> YawPitchRoll
	inline _vec3 ConvQuaternionToYawPitchRoll(const _qt& pQt) {
		_vec3 vResult;

		_float sqw = pQt.w * pQt.w;
		_float sqx = pQt.x * pQt.x;
		_float sqy = pQt.y * pQt.y;
		_float sqz = pQt.z * pQt.z;

		vResult.x = asinf(2.f * (pQt.w * pQt.x - pQt.y * pQt.z));
		vResult.y = atan2f(2.f * (pQt.x * pQt.z + pQt.w * pQt.y), (-sqx - sqy + sqz + sqw));
		vResult.z = atan2f(2.f * (pQt.x * pQt.y + pQt.w * pQt.z), (-sqx + sqy - sqz + sqw));
		
		return vResult;					
	}

	// ���Ϸ���(YawPitchRoll) -> ���ʹϾ�
	inline _qt ConvAngleToQuaternion(const _vec3& vEulerAngle) {
		_qt qtResult;

		D3DXQuaternionRotationYawPitchRoll(&qtResult, vEulerAngle.y, vEulerAngle.x, vEulerAngle.z);

		return qtResult;
	}

	// YawPitchRoll -> ���ʹϾ�
	inline _qt ConvYawPitchRollToQuaternion(const _float& _fYaw, const _float& _fPitch, const _float& _fRoll) {
		_qt qtResult;

		D3DXQuaternionRotationYawPitchRoll(&qtResult, _fYaw, _fPitch, _fRoll);

		return qtResult;
	}
}


#endif // Engine_Function_h__
