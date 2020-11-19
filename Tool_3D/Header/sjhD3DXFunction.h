#ifndef sjhD3DXFunction_h__
#define sjhD3DXFunction_h__

namespace Client
{
	inline _vec3* sjhVec3Cross(_vec3 *pOut, const _vec3 *pV1, const _vec3 *pV2) {
		pOut->x = pV1->y * pV2->z - pV1->z * pV2->y;
		pOut->y = pV1->z * pV2->x - pV1->x * pV2->z;
		pOut->z = pV1->x * pV2->y - pV1->y * pV2->x;
		
		return pOut;
	}

	inline _float sjhVec3Dot(const _vec3* pV1, const _vec3* pV2) {
		return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
	}

	inline _float sjhVec3Length(const _vec3 *pV) {
		return sqrtf(sjhVec3Dot(pV, pV));
	}

	inline _vec3* sjhVec3Normalize(_vec3 *pOut, const _vec3 *pV) {
		_float fVec3Length = sjhVec3Length(pV);

		*pOut = *pV / fVec3Length;
		
		return pOut;
	}

	inline _vec3* sjhVec3TransformCoord(_vec3 *pOut, const _vec3 *pV, const _matrix *pM) {
		pOut->x = pV->x * pM->_11 + pV->y * pM->_21 + pV->z * pM->_31 + pM->_41;
		pOut->y = pV->x * pM->_12 + pV->y * pM->_22 + pV->z * pM->_32 + pM->_42;
		pOut->z = pV->x * pM->_13 + pV->y * pM->_23 + pV->z * pM->_33 + pM->_43;

		return pOut;
	}

	inline _vec3* sjhVec3TransformNormal(_vec3 *pOut, const _vec3 *pV, const _matrix *pM) {
		pOut->x = pV->x * pM->_11 + pV->y * pM->_21 + pV->z * pM->_31;
		pOut->y = pV->x * pM->_12 + pV->y * pM->_22 + pV->z * pM->_32;
		pOut->z = pV->x * pM->_13 + pV->y * pM->_23 + pV->z * pM->_33;

		return pOut;
	}

	inline _matrix* sjhMatrixIdentity(_matrix *pOut) {
		pOut->_11 = 1.f;	pOut->_12 = 0.f;	pOut->_13 = 0.f;	pOut->_14 = 0.f;
		pOut->_21 = 0.f;	pOut->_22 = 1.f;	pOut->_23 = 0.f;	pOut->_24 = 0.f;
		pOut->_31 = 0.f;	pOut->_32 = 0.f;	pOut->_33 = 1.f;	pOut->_34 = 0.f;
		pOut->_41 = 0.f;	pOut->_42 = 0.f;	pOut->_43 = 0.f;	pOut->_44 = 1.f;

		return pOut;
	}

	inline _matrix* sjhMatrixLookAtLH(_matrix *pOut, const _vec3 *pEye, const _vec3 *pAt, const _vec3 *pUp) {
		sjhMatrixIdentity(pOut);

		_vec3 vLook, vRight, vUp;
		// Look축을 구합니다.
		sjhVec3Normalize(&vLook, &(*pAt - *pEye));
		// Right축을 구합니다.
		sjhVec3Cross(&vRight, pUp, &vLook);
		sjhVec3Normalize(&vRight, &vRight);
		// Up축을 구합니다.
		sjhVec3Cross(&vUp, &vLook, &vRight);
		
		pOut->_11 = vRight.x;					pOut->_12 = vUp.x;						pOut->_13 = vLook.x;					pOut->_14 = 0.f;
		pOut->_21 = vRight.y;					pOut->_22 = vUp.y;						pOut->_23 = vLook.y;					pOut->_24 = 0.f;
		pOut->_31 = vRight.z;					pOut->_32 = vUp.z;						pOut->_33 = vLook.z;					pOut->_34 = 0.f;
		pOut->_41 = -sjhVec3Dot(pEye, &vRight);	pOut->_42 = -sjhVec3Dot(pEye, &vUp);	pOut->_43 = -sjhVec3Dot(pEye, &vLook);	pOut->_44 = 1.f;

		return pOut;
	}

	inline _matrix* sjhMatrixPerspectiveFovLH(_matrix *pOut, _float fovy, _float Aspect, _float zn, _float zf) {
		sjhMatrixIdentity(pOut);

		pOut->_11 = 1.f / (Aspect * tanf(fovy * 0.5f));
		pOut->_22 = 1.f / tanf(fovy * 0.5f);
		pOut->_33 = zf / (zf - zn);
		pOut->_34 = 1.f;
		pOut->_43 = -zn * zf / (zf - zn);
		pOut->_44 = 0.f;

		return pOut;
	}
}

#endif