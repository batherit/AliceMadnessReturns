#include "InputProcessor.h"
#include "InputProcessorMgr.h"

USING(Engine)

CInputProcessor::CInputProcessor(CInputProcessorMgr* _pInputProcessorMgr)
	:
	m_pInputProcessorMgr(_pInputProcessorMgr)
{
}

CInputProcessor::~CInputProcessor()
{
	//Free();
}


void CInputProcessor::Free(void)
{
}
