#include "StdAfx.h"
#include "SSEvalMgr.h"
#include "Utility.h"

#pragma comment(lib, "ssound.lib")

const char SSENGINE_CFG[]="\
           {\
           \"appKey\": \"a138\",\
           \"secretKey\": \"2ceb63b8a5124854a92d046dca1e54a1\",\
           \"cloud\": {\
           \"server\": \"ws://api.cloud.ssapi.cn:8080\",\
           \"enable\": 0\
           },\
           \"native\": {\
           \"en.word.score\": {\
           \"res\": \"resource/eval/bin/eng.wrd.pydnn.16bit\"\
           },\
           \"en.sent.score\": {\
           \"res\": \"resource/eval/bin/eng.snt.pydnn.16bit\"\
           },\
           \"en.pred.score\": {\
           \"res\": \"resource/eval/bin/eng.pred.pydnn.16bit\"\
           }\
           }\
           }";

const char SSENGINE_PARAMS[] = "\
                {\
                \"coreProvideType\": \"native\",\
                \"app\": {\
                \"userId\": \"aidemo\"\
                },\
                \"audio\": {\
                \"audioType\": \"wav\",\
                \"sampleRate\": 16000,\
                \"channel\": 1,\
                \"sampleBytes\": 2\
                },\
                \"request\": {\
                \"coreType\": \"%s\",\
                \"refText\": \"%s\",\
                \"rank\": 100\
                }\
                }";

static int SSOUND_CALL _ssound_callback(const void *usrdata, const char *id, int type, const void *message, int size)
{
    if (usrdata != NULL)
    {
        SSEvalMgr *pThis = (SSEvalMgr*)(usrdata);
        vector<IEvalSink*>::const_iterator it;
        it = pThis->m_vecEvalSink.begin();
        while (it != pThis->m_vecEvalSink.end())
        {
            (*it)->OnEvalReply(id, type, message, size);
            it++;
        }
        //if (SSOUND_MESSAGE_TYPE_JSON == type)
        //{
        //    //printf("%s - %.*s\n", id, size, (char *)message);
        //    pThis->m_strResult.clear();
        //    pThis->m_strResult.append((char *)message);
        //    SetEvent(pThis->m_hRecvEvent);
        //}

        pThis->m_nState = SSEvalMgr::E_STATE_IDLE;
    }
    
    return 0;
}


SSEvalMgr::SSEvalMgr(void)
: m_pEngine(NULL),
  m_nState(E_STATE_IDLE)
{
    //m_hRecvEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
}

SSEvalMgr::~SSEvalMgr(void)
{
    Close();
    //CloseHandle(m_hRecvEvent);
    //m_hRecvEvent = NULL;
    m_vecEvalSink.clear();
}

BOOL SSEvalMgr::IsOpen()
{
    if (m_pEngine != NULL)
    {
        return TRUE;
    }

    return FALSE;
}

int SSEvalMgr::Open()
{
    int nRet = 0;
    if (NULL == m_pEngine)
    {
        m_pEngine = ssound_new(SSENGINE_CFG);
    } 

    if (NULL == m_pEngine)
    {
       nRet = -1;
    }

    return nRet;
}

int SSEvalMgr::Close()
{
    int nRet = 0;
    if (m_pEngine != NULL && E_STATE_IDLE == m_nState)
    {
        nRet = ssound_delete(m_pEngine);
        m_pEngine = NULL;
    }

    return nRet;
}

int SSEvalMgr::EvalPrototype(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, EVALTYPE nType, LPTSTR sid)
{

    char params[4096] = {0};
    char szRefText[1024] = {0};
    char id[64] = {0};
    char buf[1024] = {0};
    int   bytes = 0;
    errno_t err = 0;
    FILE* fp = NULL;

    if (m_pEngine == NULL)
    {
        return -1;
    }

    fopen_s(&fp, CUtility::UnicodeToANSI(lpWavRecFile).c_str(), "rb");
    if (err != 0)
    {
        return -2;
    }

    if (E_TYPE_EVAL_WORD == nType)
    {
        sprintf_s(params, sizeof(params), 
            SSENGINE_PARAMS, "en.word.score", CUtility::UnicodeToANSI(lpRefText).c_str());
    } else if (E_TYPE_EVAL_SENT == nType)
    {
        sprintf_s(params, sizeof(params), 
            SSENGINE_PARAMS, "en.sent.score", CUtility::UnicodeToANSI(lpRefText).c_str());
    } else if (E_TYPE_EVAL_PRED == nType)
    {
        sprintf_s(params, sizeof(params), 
            SSENGINE_PARAMS, "en.pred.score", CUtility::UnicodeToANSI(lpRefText).c_str());
    }
    m_nState = E_STATE_BUSY;
    ssound_start(m_pEngine, params, id, _ssound_callback, this);
    fseek(fp, 44, 0);//跳过wav文件的头
    while ((bytes = (int)fread(buf, 1, 1024, fp))) {
        ssound_feed(m_pEngine, buf, bytes);
    }
    ssound_stop(m_pEngine);
    fclose(fp);
    _tccpy(sid, CUtility::ANSIToUnicode(id).c_str());

    //WaitForSingleObject(m_hRecvEvent, 60*1000);
    m_wstrResult = CUtility::ANSIToUnicode(id);

    return 0;

}
int SSEvalMgr::EvalSentence(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid)
{
    return EvalPrototype(lpWavRecFile, lpRefText, E_TYPE_EVAL_SENT, sid);
}

int SSEvalMgr::EvalWord(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid)
{
    return EvalPrototype(lpWavRecFile, lpRefText, E_TYPE_EVAL_WORD, sid);
}

int SSEvalMgr::EvalPred(LPCTSTR lpWavRecFile, LPCTSTR lpRefText, LPTSTR sid)
{
    return EvalPrototype(lpWavRecFile, lpRefText, E_TYPE_EVAL_PRED, sid);
}

void SSEvalMgr::SetEvalCallback(IEvalSink *pEvalSink)
{
    if (pEvalSink != NULL)
    {
        m_vecEvalSink.push_back(pEvalSink);
    }
}

BOOL SSEvalMgr::EvalIsCompleted()
{
    return (m_nState != E_STATE_BUSY) ? TRUE : FALSE;
}

wstring SSEvalMgr::GetErrMsg(UINT nId)
{
    wstring message;
    switch (nId)
    {
    case 0:
        message = L"本次录音无异常。";
        break;
    case 10000:
        message = L"得分无效，未录音。";
        break;
    case 10001:
        message = L"得分无效，发音不完整或您还没有读完整句。";
        break;
    case 10002:
        message = L"得分无效，未接收到您的语音，请检查1.系统设置是否静音 2.录音时间太短 3.发音不完整";
        break;
    case 10003:
        message = L"得分无效，未接收到您的语音，请检查1.系统设置是否静音 2.录音时间太短 3.发音不完整";
        break;
    case 10004:
        message = L"得分无效，音量偏低或麦克风位置太远，请调整麦克风。";
        break;
    case 10005:
        message = L"得分无效，麦克风位置太近，请调整麦克风位置";
        break;
    case 10006:
        message = L"得分无效，录音效果太差，可能是环境太嘈杂或者麦克风有质量问题。";
        break;
    default:
        message = L"得分无效，语音引擎异常，请与管理员联系。";
        break;
    }
    return message;
}