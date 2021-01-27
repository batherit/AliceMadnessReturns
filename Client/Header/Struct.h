namespace Client{
	// �ִϸ��̼� ����
	typedef struct _anim_info_line {
		//int iState = -1;				// ���¸� ��Ÿ���� �������� 0�� �⺻������ Idle
		int iStartFrameIndex = 0;			// �ִϸ��̼� ���� �ε���
		int iFrameCount = 0;				// ���� �ε����������� ����� �� ������.
		float fTotalTime = 0.f;			// �� �ִϸ��̼� �� ���� �ð�
		int iCountToRepeat = 0;			// �� �ִϸ��̼��� �� �� ����? (0 - ����, 1 - 1ȸ�ϰ� ��(�� �� ������ IDLE�� ��ȯ))
		bool bIsReversePlay = false;
	}AnimInfoLine;

	typedef struct _anim_info {
	public:
		_anim_info(_int _iAnimIndex = 0, _int _iCol = 1, _int _iRow = 1, _int _iStartFrameIndex = 0, _int _iFrameCount = 0, _float _fTotalTime = 0.f, _int _iCountToRepeat = 0, _bool _bIsReversePlay = false) :
			iAnimIndex(_iAnimIndex),
			iCol(_iCol),
			iRow(_iRow),
			iStartFrameIndex(_iStartFrameIndex),
			iFrameCount(_iFrameCount),
			fTotalTime(_fTotalTime),
			iCountToRepeat(_iCountToRepeat),
			bIsReversePlay(_bIsReversePlay)
		{}

	public:
		_int iAnimIndex = 0;				// ������ �ִ� �ε���
		_int iCol = 1;					// �ִ� ��Ʋ�� �̹������� ���� ��
		_int iRow = 1;					// �ִ� ��Ʋ�� �̹������� ���� �� 
		_int iStartFrameIndex = 0;		// �ִϸ��̼� ���� �ε���
		_int iFrameCount = 1;			// ���� �ε����������� ����� �� �Ƽ�
		_float fTotalTime = 0.f;			// �ִ� �� ����ð�
		_int iCountToRepeat = 0;			// �ִ� �ݺ� Ƚ�� (0 - ����)
		_bool bIsReversePlay = false;	// �Ųٷ� ����� ������?
	}AnimInfo;

	// �ִ� ��� ó�� ����ü
	typedef struct _anim_processing_info {
		_float fAnimElapsedTime = 0.f;
		_int iCurrentIndex = 0;
		_int iRepeatedCount = 0;
	}AnimProcessingInfo;

	// ����� ����
	typedef struct _routing_info {
		_routing_info(_float _fX, _float _fY, _float _fZ, _float _fSecToReach)
			:
			vPoint(_vec3(_fX, _fY, _fZ)),
			fSecToReach(_fSecToReach)
		{}
		_routing_info(_vec3 _vPoint, _float _fSecToReach)
			:
			vPoint(_vPoint),
			fSecToReach(_fSecToReach)
		{}
		_vec3 vPoint;
		_float fSecToReach;
	}ROUTING_INFO;

	//typedef struct _texture_info
	//{
	//	LPDIRECT3DTEXTURE9 pTexture;	// �̹��� ������ �����ϱ� ���� �İ�ü. 
	//	D3DXIMAGE_INFO tImageInfo;		//�̹����� ������ ��Ƶ� ����ü. 
	//}TextureInfo;
}
