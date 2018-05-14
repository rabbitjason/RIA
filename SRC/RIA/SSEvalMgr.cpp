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
    fseek(fp, 44, 0);//����wav�ļ���ͷ
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
        message = L"����¼�����쳣��";
        break;
    case 10000:
        message = L"�÷���Ч��δ¼����";
        break;
    case 10001:
        message = L"�÷���Ч������������������û�ж������䡣";
        break;
    case 10002:
        message = L"�÷���Ч��δ���յ���������������1.ϵͳ�����Ƿ��� 2.¼��ʱ��̫�� 3.����������";
        break;
    case 10003:
        message = L"�÷���Ч��δ���յ���������������1.ϵͳ�����Ƿ��� 2.¼��ʱ��̫�� 3.����������";
        break;
    case 10004:
        message = L"�÷���Ч������ƫ�ͻ���˷�λ��̫Զ���������˷硣";
        break;
    case 10005:
        message = L"�÷���Ч����˷�λ��̫�����������˷�λ��";
        break;
    case 10006:
        message = L"�÷���Ч��¼��Ч��̫������ǻ���̫���ӻ�����˷����������⡣";
        break;
    default:
        message = L"�÷���Ч�����������쳣���������Ա��ϵ��";
        break;
    }
    return message;
}