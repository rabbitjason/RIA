#include "StdAfx.h"
#include <Wincrypt.h>
#include "WaveRecorder.h"
#include "CMciWave.h"
#include "main_frame.h"
#include "aiengine.h"

//const char* CAiEngine::m_appKey = "13934044080001ee";
//const char* CAiEngine::m_secretKey = "8f6477a656437c49d73ed7943e435512";

//正式key
const char* CAiEngine::m_appKey = "1448872271000004";
const char* CAiEngine::m_secretKey = "2faf6070a3d28c504d70ec3f64c7d5f2";

//测试用key，体验版本 2016-08-03
//const char* CAiEngine::m_appKey = "147028887100004f";
//const char* CAiEngine::m_secretKey = "8c7bc60e7ca4aa3784c4e5c9e056c660";

const char* CAiEngine::m_appKey0 = "1396575650000251";
const char* CAiEngine::m_secretKey0 = "aacdf7e2ac6388e78da5bdd39278ccd8";

HMODULE  CAiEngine::m_hModule = NULL;
HANDLE   CAiEngine::m_hEvent = NULL;

paiengine_cancel CAiEngine::m_paiengine_cancel = NULL;
paiengine_delete CAiEngine::m_paiengine_delete = NULL;
paiengine_feed CAiEngine::m_paiengine_feed = NULL;
paiengine_get_device_id CAiEngine::m_paiengine_get_device_id = NULL;
paiengine_new CAiEngine::m_paiengine_new = NULL;
paiengine_opt CAiEngine::m_paiengine_opt = NULL;
paiengine_start CAiEngine::m_paiengine_start = NULL;
paiengine_stop CAiEngine::m_paiengine_stop = NULL;

PRtlTimeToSecondsSince1970 CAiEngine::m_PRtlTimeToSecondsSince1970 = NULL;

static CAiEngine::AI_SCODE InitEnCodeMap()
{
#if 1
    CAiEngine::AI_SCODE as;
    as.insert(CAiEngine::SCODE_Pair(_T("ih"), _T("I")));       //I
    as.insert(CAiEngine::SCODE_Pair(_T("ax"), _T("E")));       //E
    as.insert(CAiEngine::SCODE_Pair(_T("oh"), _T("C")));       //C
    as.insert(CAiEngine::SCODE_Pair(_T("uh"), _T("J")));       //J
    as.insert(CAiEngine::SCODE_Pair(_T("ah"), _T("Q")));       //Q
    as.insert(CAiEngine::SCODE_Pair(_T("eh"), _T("e")));       //e
    as.insert(CAiEngine::SCODE_Pair(_T("ae"), _T("A")));      //A
    as.insert(CAiEngine::SCODE_Pair(_T("iy"), _T("I:")));      //I:
    as.insert(CAiEngine::SCODE_Pair(_T("er"), _T("\\:")));      //\:
    as.insert(CAiEngine::SCODE_Pair(_T("axr"), _T("L")));      //L
    as.insert(CAiEngine::SCODE_Pair(_T("ao"), _T("C:")));      //C:
    as.insert(CAiEngine::SCODE_Pair(_T("uw"), _T("U:")));      //U:
    as.insert(CAiEngine::SCODE_Pair(_T("aa"), _T("B:")));      //B:
    as.insert(CAiEngine::SCODE_Pair(_T("ey"), _T("eI")));      //eI
    as.insert(CAiEngine::SCODE_Pair(_T("ay"), _T("aI")));      //aI
    as.insert(CAiEngine::SCODE_Pair(_T("oy"), _T("CI")));      //CI
    as.insert(CAiEngine::SCODE_Pair(_T("aw"), _T("aJ")));      //aJ
    as.insert(CAiEngine::SCODE_Pair(_T("ow"), _T("EJ")));      //EJ
    as.insert(CAiEngine::SCODE_Pair(_T("ia"), _T("IE")));      //IE
    as.insert(CAiEngine::SCODE_Pair(_T("ea"), _T("ZE")));      //ZE
    as.insert(CAiEngine::SCODE_Pair(_T("ua"), _T("JE")));      //JE
    as.insert(CAiEngine::SCODE_Pair(_T("p"), _T("p")));
    as.insert(CAiEngine::SCODE_Pair(_T("b"), _T("b")));
    as.insert(CAiEngine::SCODE_Pair(_T("t"), _T("t")));
    as.insert(CAiEngine::SCODE_Pair(_T("d"), _T("d")));
    as.insert(CAiEngine::SCODE_Pair(_T("k"), _T("k")));
    as.insert(CAiEngine::SCODE_Pair(_T("g"), _T("g")));
    as.insert(CAiEngine::SCODE_Pair(_T("l"), _T("l")));
    as.insert(CAiEngine::SCODE_Pair(_T("r"), _T("r")));
    as.insert(CAiEngine::SCODE_Pair(_T("m"), _T("m")));
    as.insert(CAiEngine::SCODE_Pair(_T("n"), _T("n")));
    as.insert(CAiEngine::SCODE_Pair(_T("ng"), _T("N")));       //N
    as.insert(CAiEngine::SCODE_Pair(_T("hh"), _T("h")));
    as.insert(CAiEngine::SCODE_Pair(_T("s"), _T("s")));
    as.insert(CAiEngine::SCODE_Pair(_T("z"), _T("z")));
    as.insert(CAiEngine::SCODE_Pair(_T("th"), _T("W")));      //W
    as.insert(CAiEngine::SCODE_Pair(_T("dh"), _T("T")));       //T
    as.insert(CAiEngine::SCODE_Pair(_T("f"), _T("f")));
    as.insert(CAiEngine::SCODE_Pair(_T("v"), _T("v")));
    as.insert(CAiEngine::SCODE_Pair(_T("w"), _T("w")));
    as.insert(CAiEngine::SCODE_Pair(_T("y"), _T("j")));
    as.insert(CAiEngine::SCODE_Pair(_T("sh"), _T("F")));       //F
    as.insert(CAiEngine::SCODE_Pair(_T("zh"), _T("V")));       //V
    as.insert(CAiEngine::SCODE_Pair(_T("ch"), _T("tF")));      //tF
    as.insert(CAiEngine::SCODE_Pair(_T("jh"), _T("dV")));      //dV
    return as;
#else
    CAiEngine::AI_SCODE as;                                 //DFPhonetic字体，GB2312字符集下，对应的字符; 5是重音符号，7是低（次）重音
    as.insert(CAiEngine::SCODE_Pair(_T("ih"), L"ɪ"));       //I
    as.insert(CAiEngine::SCODE_Pair(_T("ax"), L"ə"));       //E
    as.insert(CAiEngine::SCODE_Pair(_T("oh"), L"ɒ"));       //C
    as.insert(CAiEngine::SCODE_Pair(_T("uh"), L"ʊ"));       //J
    as.insert(CAiEngine::SCODE_Pair(_T("ah"), L"ʌ"));       //Q
    as.insert(CAiEngine::SCODE_Pair(_T("eh"), L"e"));       //e
    as.insert(CAiEngine::SCODE_Pair(_T("ae"), L"æ "));      //A
    as.insert(CAiEngine::SCODE_Pair(_T("iy"), L"i:"));      //I:
    as.insert(CAiEngine::SCODE_Pair(_T("er"), L"ɜ:"));      //\:
    as.insert(CAiEngine::SCODE_Pair(_T("axr"), L"ɚ"));      //L
    as.insert(CAiEngine::SCODE_Pair(_T("ao"), L"ɔ:"));      //C:
    as.insert(CAiEngine::SCODE_Pair(_T("uw"), L"u:"));      //U:
    as.insert(CAiEngine::SCODE_Pair(_T("aa"), L"ɑ:"));      //B:
    as.insert(CAiEngine::SCODE_Pair(_T("ey"), L"eɪ"));      //eI
    as.insert(CAiEngine::SCODE_Pair(_T("ay"), L"aɪ"));      //aI
    as.insert(CAiEngine::SCODE_Pair(_T("oy"), L"ɔɪ"));      //CI
    as.insert(CAiEngine::SCODE_Pair(_T("aw"), L"aʊ"));      //aJ
    as.insert(CAiEngine::SCODE_Pair(_T("ow"), L"әʊ"));      //EJ
    as.insert(CAiEngine::SCODE_Pair(_T("ia"), L"ɪə"));      //IE
    as.insert(CAiEngine::SCODE_Pair(_T("ea"), L"ɛә"));      //ZE
    as.insert(CAiEngine::SCODE_Pair(_T("ua"), L"ʊə"));      //JE
    as.insert(CAiEngine::SCODE_Pair(_T("p"), L"p"));
    as.insert(CAiEngine::SCODE_Pair(_T("b"), L"b"));
    as.insert(CAiEngine::SCODE_Pair(_T("t"), L"t"));
    as.insert(CAiEngine::SCODE_Pair(_T("d"), L"d"));
    as.insert(CAiEngine::SCODE_Pair(_T("k"), L"k"));
    as.insert(CAiEngine::SCODE_Pair(_T("g"), L"g"));
    as.insert(CAiEngine::SCODE_Pair(_T("l"), L"l"));
    as.insert(CAiEngine::SCODE_Pair(_T("r"), L"r"));
    as.insert(CAiEngine::SCODE_Pair(_T("m"), L"m"));
    as.insert(CAiEngine::SCODE_Pair(_T("n"), L"n"));
    as.insert(CAiEngine::SCODE_Pair(_T("ng"), L"ŋ"));       //N
    as.insert(CAiEngine::SCODE_Pair(_T("hh"), L"h"));
    as.insert(CAiEngine::SCODE_Pair(_T("s"), L"s"));
    as.insert(CAiEngine::SCODE_Pair(_T("z"), L"z"));
    as.insert(CAiEngine::SCODE_Pair(_T("th"), L"θ"));      //W
    as.insert(CAiEngine::SCODE_Pair(_T("dh"), L"ð"));       //T
    as.insert(CAiEngine::SCODE_Pair(_T("f"), L"f"));
    as.insert(CAiEngine::SCODE_Pair(_T("v"), L"v"));
    as.insert(CAiEngine::SCODE_Pair(_T("w"), L"w"));
    as.insert(CAiEngine::SCODE_Pair(_T("y"), L"j"));
    as.insert(CAiEngine::SCODE_Pair(_T("sh"), L"ʃ"));       //F
    as.insert(CAiEngine::SCODE_Pair(_T("zh"), L"ʒ"));       //V
    as.insert(CAiEngine::SCODE_Pair(_T("ch"), L"tʃ"));      //tF
    as.insert(CAiEngine::SCODE_Pair(_T("jh"), L"dʒ"));      //dV
    return as;
#if 0
    Json::Reader jread;
    Json::Value jroot;
    static WCHAR sEnCode[] = L"{\
\"ih\": \"ɪ\",\
\"ax\": \"ə\",\
\"oh\": \"ɒ\",\
\"uh\": \"ʊ\",\
\"ah\": \"ʌ\",\
\"eh\": \"e\",\
\"ae\": \"æ \",\
\"iy\": \"i:\",\
\"er\": \"ɜ:\",\
\"axr\": \"ɚ\",\
\"ao\": \"ɔ:\",\
\"uw\": \"u:\",\
\"aa\": \"ɑ:\",\
\"ey\": \"eɪ\",\
\"ay\": \"aɪ\",\
\"oy\": \"ɔɪ\",\
\"aw\": \"aʊ\",\
\"ow\": \"әʊ\",\
\"ia\": \"ɪə\",\
\"ea\": \"ɛә\",\
\"ua\": \"ʊə\",\
\"p\": \"p\",\
\"b\": \"b\",\
\"t\": \"t\",\
\"d\": \"d\",\
\"k\": \"k\",\
\"g\": \"g\",\
\"l\": \"l\",\
\"r\": \"r\",\
\"m\": \"m\",\
\"n\": \"n\",\
\"ng\": \"ŋ\",\
\"hh\": \"h\",\
\"s\": \"s\",\
\"z\": \"z\",\
\"th\": \"θ\",\
\"dh\": \"ð\",\
\"f\": \"f\",\
\"v\": \"v\",\
\"w\": \"w\",\
\"y\": \"j\",\
\"sh\": \"ʃ\",\
\"zh\": \"ʒ\",\
\"ch\": \"tʃ\",\
\"jh\": \"dʒ\"}";
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, sEnCode, -1, NULL, 0, NULL, NULL);
    string sJson;
    sJson.resize(nLen);
    nLen = ::WideCharToMultiByte(CP_UTF8, 0, sEnCode, -1, &sJson[0], nLen, NULL, NULL);

    //TRACE(_T("EnCode Json:%s\n\n"), (LPTSTR)CA2T(sJson.c_str()));
    bool bOK = jread.parse(sJson, jroot);
    //Json::Value jua = jroot["ua"];
    //if (jua.isNull())
    //{
    //    TRACE(_T("us is null\n"));
    //}
    //else
    //{
    //    TRACE(_T("type is:%d\n"), jua.type());
    //    WCHAR wua[100] = {UNICODE_NULL,};
    //    int wsz =MultiByteToWideChar(CP_UTF8, 0, jua.asString().c_str(), jua.asString().size(), wua, 100);
    //    TRACE(L"value:%s\n", wua);
    //}
    return jroot;
#endif

#endif
}

CAiEngine::AI_SCODE CAiEngine::m_SpeechEncode = InitEnCodeMap();
#ifdef _DEBUG
CStdioFile* CAiEngine::m_flog;
#endif

CAiEngine::CAiEngine()
{
}

CAiEngine::~CAiEngine()
{
}

DWORD CAiEngine::LoadFunc()
{

    DWORD dwErr = ERROR_SUCCESS;
    do
    {
        if (NULL != m_hModule)
        {
            break;
        }
        CString strTmp;
        strTmp.Format(_T("%saiengine.dll"), CPaintManagerUI::GetInstancePath());
        m_hModule = ::LoadLibrary(strTmp);
        if (NULL == m_hModule)
        {
            dwErr = GetLastError();
            TRACE(_T("load %s failed:%s\n"), strTmp, MyFormatMessage(dwErr));
            break;
        }

        m_paiengine_cancel = (paiengine_cancel)GetProcAddress(m_hModule, "aiengine_cancel");
        m_paiengine_delete = (paiengine_delete)GetProcAddress(m_hModule, "aiengine_delete");
        m_paiengine_feed = (paiengine_feed)GetProcAddress(m_hModule, "aiengine_feed");
        m_paiengine_get_device_id = (paiengine_get_device_id)GetProcAddress(m_hModule, "aiengine_get_device_id");
        m_paiengine_new = (paiengine_new)GetProcAddress(m_hModule, "aiengine_new");
        m_paiengine_opt = (paiengine_opt)GetProcAddress(m_hModule, "aiengine_opt");
        m_paiengine_start = (paiengine_start)GetProcAddress(m_hModule, "aiengine_start");
        m_paiengine_stop = (paiengine_stop)GetProcAddress(m_hModule, "aiengine_stop");
#ifdef _DEBUG
        CFileException fe;
        strTmp.Format(_T("%sData\\%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("aiengine.txt"));
        m_flog = new CStdioFile;
        if (!m_flog->Open(strTmp, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::typeText, &fe))
        {
            dwErr = fe.m_lOsError;
            fe.ReportError();
        }
#endif
        m_hEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);
        if (NULL == m_hEvent)
        {
            dwErr = GetLastError();
            break;
        }
    }while(0);

    if (ERROR_SUCCESS != dwErr)
    {
        Release();
    }
    return dwErr;
}

void CAiEngine::Release()
{
    m_paiengine_cancel = NULL;
    m_paiengine_delete = NULL;
    m_paiengine_feed = NULL;
    m_paiengine_get_device_id = NULL;
    m_paiengine_new = NULL;
    m_paiengine_opt = NULL;
    m_paiengine_start = NULL;
    m_paiengine_stop = NULL;

    if (m_hModule)
    {
        ::FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
#ifdef _DEBUG
    if (m_flog)
    {
        m_flog->Close();
        delete m_flog;
        m_flog = NULL;
    }
#endif
    if (m_hEvent)
    {
        //SetEventState(TRUE);
        ::CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }
}

int CAiEngine::aiengine_cancel(HANDLE hEngine)
{
    SetEventState(TRUE);
    if (m_paiengine_cancel)
    {
        return m_paiengine_cancel(hEngine);
    }
    return -1;
}

int CAiEngine::aiengine_delete(HANDLE hEngine)
{
    SetEventState(TRUE);
    if (m_paiengine_delete)
    {
        return m_paiengine_delete(hEngine);
    }
    return -1;
}

int CAiEngine::aiengine_feed(HANDLE hEngine, LPBYTE lpData, int nSize)
{
    if (m_paiengine_feed)
    {
        return m_paiengine_feed(hEngine, lpData, nSize);
    }
    return 1;
}

int CAiEngine::aiengine_get_device_id(LPBYTE lpDeviceId)
{
    if (m_paiengine_get_device_id)
    {
        return m_paiengine_get_device_id(lpDeviceId);
    }
    return -1;
}

HANDLE CAiEngine::aiengine_new(LPCSTR strCfg)
{
    if (m_paiengine_new)
    {
        return m_paiengine_new(strCfg);
    }
    return NULL;
}

int CAiEngine::aiengine_opt(HANDLE hEngine, int nOpt, LPBYTE lpData, int nSize)
{
    if (m_paiengine_opt)
    {
        return m_paiengine_opt(hEngine, nOpt, lpData, nSize);
    }
    return -1;
}

int CAiEngine::aiengine_start(HANDLE hEngine, LPCSTR strParam, LPBYTE lpID,
                              paiengine_callback callback, PVOID lpUsrdata)
{
    int rt = -1;
    if (m_paiengine_start)
    {
        rt = m_paiengine_start(hEngine, strParam, lpID, callback, lpUsrdata);
    }
    if (rt >= 0)
    {
        SetEventState(FALSE);
    }
    return rt;
}

int CAiEngine::aiengine_stop(HANDLE hEngine)
{
    //dont't
    //SetEventState(TRUE);
    if (m_paiengine_stop)
    {
        return m_paiengine_stop(hEngine);
    }
    return -1;
}

int __stdcall CAiEngine::aiengine_callback(PVOID lpUsrdata, LPCSTR strID, int nType, LPBYTE lpMessage, int nSize)
{
    MainFrame* pFrame = (MainFrame*)lpUsrdata;
    TRACE("type:%d, size:%d\n", nType, nSize);
    if (nType == AIENGINE_MESSAGE_TYPE_JSON)
    {
        pFrame->m_strRecognitionCompare[8].Format(_T("%d"), -1);
        pFrame->m_strRecognitionCompare[7].Empty();
        pFrame->m_strRecognitionCompare[11].Empty();

        do 
        {
            Json::Reader jreader;
            Json::Value jroot;
            //CA2W wMsg((LPCSTR)lpMessage);
            //int nLen = AtlUnicodeToUTF8(wMsg, -1, NULL, 0);//::WideCharToMultiByte(CP_UTF8, 0, wMsg, -1, NULL, 0, NULL, NULL);
            //string sJson;
            //sJson.resize(nLen);
            //nLen = AtlUnicodeToUTF8(wMsg, -1, &sJson[0], nLen);//::WideCharToMultiByte(CP_UTF8, 0, wMsg, -1, &sJson[0], nLen, NULL, NULL);
            string sJson = (LPCSTR)lpMessage;
#ifdef _DEBUG
            m_flog->WriteString(_T("Reftext:\n"));
            m_flog->WriteString(pFrame->m_strAiRefText);
            m_flog->WriteString(_T("\n"));
            m_flog->WriteString(_T("Result:\n"));
            m_flog->WriteString((LPTSTR)CA2T((LPCSTR)lpMessage));
            m_flog->WriteString(_T("\n\n"));
#endif
            TRACE("Json:%s\n", sJson.c_str());
            if (!jreader.parse(sJson.c_str(), jroot))
            {
                pFrame->m_strAiErrMsg = _T("语音识别返回数据格式错误。");
                break;
            }
            //TRACE("version:%s,recordId:%s\n", jroot["version"].asCString(), jroot["tokenId"].asCString());
            Json::Value jresult = jroot["result"];
            if (jresult.isNull())
            {
                //{"errId": 70200, "error": "Network abnormal.", "tokenId": "565fd9ec02dbc2158c000001" }
                Json::Value jerrid = jroot["errId"];
                if (!jerrid.isNull())
                {
                    pFrame->m_strRecognitionCompare[8].Format(_T("%d"), -2);
                    TRACE(_T("errorid:%d\n"), jerrid.asInt());
                }
                Json::Value jerror = jroot["error"];
#ifdef _DEBUG
                if (!jerror.isNull())
                {
                    TRACE("Json error:%s\n", jerror.asCString());
                }
#endif

                if (!jerrid.isNull() && !jerror.isNull())
                {
                    //UTF8==>unicode
                    pFrame->m_strAiErrMsg.Format(_T("语音识别失败，错误码：%d，错误描述：%s。请联系客服人员。"),
                        jerrid.asInt(), (LPTSTR)CA2T(jerror.asCString()));
                    if (jerrid.asInt() == 60015)
                    {
                        pFrame->m_strAiErrMsg = _T("语音识别失败，请重新启动程序。");
                        pFrame->RegKeySetValue(REG_SpeechSerial, _T(""));
                        //并删除%ProgramData%/aiengine/udidinfo
                        TCHAR szPath[1024] = {_T('\0'),};
                        if (SUCCEEDED(::SHGetFolderPath(pFrame->GetHWND(),
                                CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath)))
                        {
                            ::PathAppend(szPath, _T("aiengine"));
                            TRACE(_T("sn failed, delete %s\n"), szPath);
                            MyRemoveDir(szPath);
                        }
                    }
                }
                break;
            }
            //得分，在识别结果为0（成功情况下）
            Json::Value jOverall = jresult["overall"];
            if (jOverall.isNull())
            {
                pFrame->m_strAiErrMsg = _T("语音识别失败，无结果。");
                break;
            }
            TRACE("\noverall:%d\n", jOverall.asInt());
            pFrame->m_strRecognitionCompare[8].Format(_T("%d"), jOverall.asInt());
#ifdef _DEBUG
            //if (jOverall.asInt() < 50)
            {
                Json::Value jinfo = jresult["info"];
                if (!jinfo.isNull())
                {
                    TRACE("tipId:%d\n", jinfo["tipId"].asUInt());
                    if (jinfo["tipId"].asUInt() != 0)
                    {
                        TRACE("tips:%S\n", GetErrMsg(jinfo["tipId"].asUInt()).c_str());
                        //break;
                    }
                }
            }
#endif

            Json::Value jdetails = jresult["details"];
            if (jdetails.isNull())
            {
                TRACE(_T("has no details!!\n"));
                break;
            }
            Json::UInt i = 0, count = jdetails.size();
            if (count <= 0)
            {
                TRACE(_T("has %d details!!\n"), count);
                break;
            }
            CString strWords;
            Json::Value ele;
            Json::Value score;
            Json::Value jchar;
            Json::Value jtext;
            pFrame->m_aReadbadWord.clear();
            if (MainFrame::ct_pred == pFrame->m_nAiCoreType)
            {
                for (i = 0; i < count; i++)
                {
                    ele = jdetails[i];
                    score = ele["score"];
                    jtext = ele["text"];
                    if (score.isNull() ||
                        jtext.isNull())
                    {
                        continue;
                    }
                    pFrame->m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jtext.asCString())));
                }
            }
            else if (MainFrame::ct_sent == pFrame->m_nAiCoreType)
            {
                for (i = 0; i < count; i++)
                {
                    //得分<=80情况下，单词未识别
                    ele = jdetails[i];
                    score = ele["score"];
                    jchar = ele["char"];
                    if (jchar.isNull() ||
                        score.isNull())
                    {
                        continue;
                    }
                    //TRACE("%d: score:%d, char:%s\n", i+1, ele["score"].asUInt(), ele["char"].asCString());
                    if (ele["score"].asUInt() <= 80)
                    {
                        strWords += (LPTSTR)CA2T(jchar.asCString());
                        strWords += _T(" ");
                        TRACE(_T("%s is bad:%d\n"), (LPTSTR)CA2T(jchar.asCString()), ele["score"].asUInt());
                    }
                    pFrame->m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jchar.asCString())));
                }
                strWords.TrimRight(_T(' '));
                pFrame->m_strRecognitionCompare[7] = strWords;
            }
            else if (MainFrame::ct_word == pFrame->m_nAiCoreType)
            {
                i = 0;
                Json::Value ele = jdetails[i];
                Json::Value phone = ele["phone"];
                if (!phone.isNull())
                {
                    count = phone.size();
                    for (i = 0; i < count; i++)
                    {
                        score = phone[i]["score"];
                        jchar = phone[i]["char"];
                        if (jchar.isNull() ||
                            score.isNull())
                        {
                            continue;
                        }
                        pFrame->m_aReadbadWord.push_back(MyBadWord(score.asUInt(), (LPTSTR)CA2T(jchar.asCString())));
                    }
                }
#ifdef _DEBUG
                else
                {
                    TRACE(_T("单词没有音素评分结果！\n"));
                }
#endif
            }
            else
            {
                ;
            }

#if 0       //只有单词才有音素的评分
            Json::Value jstatics = jresult["statics"];
            count = jstatics.size();
            strWords.Empty();
            for (i = 0; i < count; i++)
            {
                //得分<=80情况下，音标未识别
                Json::Value ele = jstatics[i];
                //TRACE("%d: score:%d, char:%s\n", i+1, ele["score"].asUInt(), ele["char"].asCString());
                if (ele["score"].asUInt() <= 80)
                {
                    strWords += (LPTSTR)CA2T(ele["char"].asCString());
                    strWords += _T(",");
                }
            }
            strWords.TrimRight(_T(','));
            pFrame->m_strRecognitionCompare[11] = strWords;
#endif
        } while (FALSE);

        //pFrame->m_bAICompleted = true;
        SetEventState(TRUE);
        TRACE(_T("callback out, set m_bAICompleted %d\n"), IsCallbackQuit());
    }

    return 0;
}

string CAiEngine::GetTimeStamp()
{
    if (NULL == m_PRtlTimeToSecondsSince1970)
    {
        HMODULE hNtdll = ::LoadLibrary(_T("ntdll.dll"));
        ASSERT(hNtdll);
        m_PRtlTimeToSecondsSince1970 = (PRtlTimeToSecondsSince1970)GetProcAddress(hNtdll, "RtlTimeToSecondsSince1970");
        ASSERT(m_PRtlTimeToSecondsSince1970);
    }
    FILETIME ft;
    ::GetSystemTimeAsFileTime(&ft);
    ULONG ulSeconds = 0;
    m_PRtlTimeToSecondsSince1970((PLARGE_INTEGER)&ft, &ulSeconds);
    //CTimeSpan tspan(CTime(st) - CTime(1970, 1, 1, 0, 0, 0));
    CString strTmp;
    strTmp.Format(_T("%lu"), ulSeconds);
    return string((LPCSTR)CT2A(strTmp));
}

string CAiEngine::GetDeviceId()
{
    BYTE device_id[AI_DAT_LEN+1] = {0};
    aiengine_get_device_id(device_id);
    string strTmp((char*)device_id);
    return strTmp.substr(0, 12);
}

string CAiEngine::GetSAH(const string& strTimeStamp, const string& strId)
{
    string strDat = m_appKey + strTimeStamp + m_secretKey + strId;
    HCRYPTPROV hProv = NULL;
    HCRYPTHASH hHash  = NULL;
    DWORD dwHashLen;
    BYTE* pbHash = NULL;

    do 
    {
        if (!CryptAcquireContext(
                &hProv,                   // handle of the CSP
                NULL,                     // key container name
                NULL,                     // CSP name
                PROV_RSA_FULL,            // provider type
                CRYPT_VERIFYCONTEXT))     // no key access is requested
        {
            TRACE("Error in AcquireContext 0x%08x \n",
                GetLastError());
            break;
        }

        if (!CryptCreateHash(
                hProv,                    // handle of the CSP
                CALG_SHA1,                // hash algorithm to use
                0,                        // hash key
                0,                        // reserved
                &hHash))                  // address of hash object handle
        {
            TRACE("Error in CryptCreateHash 0x%08x \n",
                GetLastError());
            break;
        }

        if (!CryptHashData(
                hHash,                    // handle of the hash object
                (const BYTE*)strDat.c_str(),                  // password to hash
                strDat.length(),            // number of bytes of data to add
                0))                       // flags
        {
            TRACE("Error in CryptHashData 0x%08x \n", 
                GetLastError());
            break;
        }

        if(!CryptGetHashParam(
                hHash, 
                HP_HASHVAL, 
                NULL, 
                &dwHashLen, 
                0)) 
        {
            TRACE("CryptGetHashParam failed to get length. 0x%08x\n", GetLastError());
            break;
        }

        pbHash = new BYTE[dwHashLen];
        if(!CryptGetHashParam(
                hHash, 
                HP_HASHVAL, 
                pbHash, 
                &dwHashLen, 
                0)) 
        {
            TRACE("CryptGetHashParam failed to get value. 0x%08x\n", GetLastError());
            break;
        }
        // Print the hash value.
        DWORD i;
        strDat.clear();
        CString strTmp;
        TRACE("The hash is:  ");
        for(i = 0 ; i < dwHashLen ; i++) 
        {
            TRACE("%2.2x ",pbHash[i]);
            strTmp.Format(_T("%02x"), pbHash[i]);
            strDat += CT2A(strTmp);
        }
        TRACE("\n");
    } while (0);

    if (pbHash)
    {
        delete[] pbHash;
    }
    if (hHash)
    {
        CryptDestroyHash(hHash);
    }
    if (hProv)
    {
        CryptReleaseContext(hProv, 0);
    }
    return strDat;
}

wstring CAiEngine::GetErrMsg(UINT nId)
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

CString CAiEngine::GetSpeechChar(const CString& strCode)
{
#if 1
    CAiEngine::AI_SCODE::const_iterator p = m_SpeechEncode.find(strCode);
    return p == m_SpeechEncode.end() ? _T("") : p->second;
#else
    ASSERT(m_SpeechEncode.size() > 0);
    wstring wChar;
    Json::Value jChar = m_SpeechEncode[strCode];
    wChar.empty();

    do 
    {
        if (jChar.isNull())
        {
            wChar = CA2W(strCode.c_str());
            break;
        }
        ASSERT(jChar.type() == Json::stringValue);
        WCHAR wua[256] = {UNICODE_NULL,};
        int wsz = ::MultiByteToWideChar(CP_UTF8, 0, jChar.asCString(), -1, wua, _countof(wua));
        wChar = wua;
    } while (false);

    return wChar;
#endif
}

void CAiEngine::SetEventState(BOOL bSigned)
{
    if (NULL == m_hEvent)
    {
        return;
    }

    if (bSigned)
    {
        ::SetEvent(m_hEvent);
    }
    else
    {
        ::ResetEvent(m_hEvent);
    }
}

BOOL CAiEngine::IsCallbackQuit()
{
    if (NULL == m_hEvent)
    {
        TRACE(_T("hey, m_hEvent is NULL!!\n"));
        return TRUE;
    }
    DWORD dwRet = ::WaitForSingleObject(m_hEvent, 0);
    return (WAIT_OBJECT_0 == dwRet) ? TRUE : FALSE;
}

LPSTR CAiEngine::CheckRefText(__inout LPSTR lpstrReftext, __in DWORD dwBufCharsLen)
{
    CString strTxt = (LPTSTR)CA2T(lpstrReftext);
    CheckRefText(strTxt);
    StringCchCopyA(lpstrReftext, dwBufCharsLen, (LPSTR)(CT2A)strTxt);
    return lpstrReftext;
}

LPWSTR CAiEngine::CheckRefText(__inout LPWSTR lpstrReftext, __in DWORD dwBufCharsLen)
{
    CString strTxt = (LPTSTR)CW2T(lpstrReftext);
    CheckRefText(strTxt);
    StringCchCopyW(lpstrReftext, dwBufCharsLen, (LPWSTR)(CT2W)strTxt);
    return lpstrReftext;
}

void CAiEngine::CheckRefText(__inout CString& strReftext)
{
    /*
    支持文本符号
    [a-z][A-Z]-'.

    支持的单词边界分割字符
    ,;?!"

    文本中常见的其他符号处理
    数字：转换成相应英文发音实词，如 100 -> one hundred
    时间：转换成相应英文发音实词，如 1999 -> nineteen ninety; 8:30 -> eight thirty
    非 UTF-8 编码：转换成 UTF-8 编码，常见主要是 ASCII 编码中前 128 个字符。
    其他与发音、停顿或者语速相关的符号：替换成上述支持单词边界分割字符,;?!"。
    其他与发音、停顿或者语速非相关的符号：直接去除，因为不影响口语评分。
    */
    if (IncludeChinese(strReftext))
    {
        TRACE(_T("include chinese!\n"));
        TRACE(_T("%s\n"), strReftext);
    }
    strReftext.Replace(_T("\""), _T(""));
    strReftext.Replace(_T("\r"), _T(""));
    strReftext.Replace(_T("\n"), _T(""));
    strReftext.Replace(_T("\\"), _T(""));
    strReftext.Replace(_T("("), _T(" "));
    strReftext.Replace(_T(")"), _T(" "));
    strReftext.Replace(_T("["), _T(" "));
    strReftext.Replace(_T("]"), _T(" "));
    strReftext.Replace(_T("{"), _T(" "));
    strReftext.Replace(_T("}"), _T(" "));
    ////不能删除这几个，xml
    strReftext.Replace(_T("<"), _T(" "));
    strReftext.Replace(_T(">"), _T(" "));
    strReftext.Replace(_T("/"), _T(" "));
    ////Hyphen,U+2010
    //strReftext.Replace((LPTSTR)CW2T(L"‐"), _T(""));
    ////en dash, U+2013
    //strReftext.Replace((LPTSTR)CW2T(L"–"), _T(""));
    ////em dash,U+2014
    //strReftext.Replace((LPTSTR)CW2T(L"—"), _T(" "));
    //strReftext.Replace((LPTSTR)CW2T(L"-"), _T(""));
    //strReftext.Trim();

    TString sTxt(strReftext);
    TString::const_iterator p = unique(sTxt.begin(), sTxt.end(), s_equal);
    strReftext = sTxt.substr(0, p - sTxt.begin()).c_str();
}
