namespace Client{
	// 애니메이션 관련
	typedef struct _anim_info_line {
		//int iState = -1;				// 상태를 나타내는 열거형값 0은 기본적으로 Idle
		int iStartFrameIndex = 0;			// 애니메이션 시작 인덱스
		int iFrameCount = 0;				// 시작 인덱스에서부터 출력할 총 프레임.
		float fTotalTime = 0.f;			// 한 애니메이션 총 진행 시간
		int iCountToRepeat = 0;			// 한 애니메이션을 몇 번 진행? (0 - 무한, 1 - 1회하고 끝(별 일 없으면 IDLE로 전환))
		bool bIsReversePlay = false;
	}AnimInfoLine;

	typedef struct _anim_info {
	public:
		_anim_info(int _iAnimIndex = 0, int _iCol = 1, int _iStartFrameIndex = 0, int _iFrameCount = 0, float _fTotalTime = 0.f, int _iCountToRepeat = 0, bool _bIsReversePlay = false) :
			iAnimIndex(_iAnimIndex),
			iCol(_iCol),
			iStartFrameIndex(_iStartFrameIndex),
			iFrameCount(_iFrameCount),
			fTotalTime(_fTotalTime),
			iCountToRepeat(_iCountToRepeat),
			bIsReversePlay(_bIsReversePlay)
		{}

	public:
		int iAnimIndex = 0;				// 실행할 애님 인덱스
		int iCol = 1;					// 애님 아틀라스 이미지에서 열의 수 (실행할 프레임의 행과 열을 구하는데 쓰임.)
		int iStartFrameIndex = 0;		// 애니메이션 시작 인덱스
		int iFrameCount = 1;			// 시작 인덱스에서부터 출력할 총 컷수
		float fTotalTime = 0.f;			// 애님 총 진행시간
		int iCountToRepeat = 0;			// 애님 반복 횟수 (0 - 무한)
		bool bIsReversePlay = false;	// 거꾸로 재생할 것인지?
	}AnimInfo;

	// 애님 재생 처리 구조체
	typedef struct _anim_processing_info {
		float fAnimElapsedTime = 0.f;
		int iCurrentIndex = 0;
		int iRepeatedCount = 0;
	}AnimProcessingInfo;

	//typedef struct _texture_info
	//{
	//	LPDIRECT3DTEXTURE9 pTexture;	// 이미지 한장을 제어하기 위한 컴객체. 
	//	D3DXIMAGE_INFO tImageInfo;		//이미지의 정보를 담아둘 구조체. 
	//}TextureInfo;
}
