//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "SSEvalMgr.h"

class CMyWndUI : public CControlUI
{
public:
    CMyWndUI(LPCTSTR pstrClassName, DWORD dwStyle, CPaintManagerUI& PaintManager) : m_hWnd(NULL)
    , m_strClassName(pstrClassName)
    , m_dwStyle(dwStyle)
    , m_PaintManager(PaintManager)
    , m_pChart(NULL)
    , m_pWMP(NULL){}
    ~CMyWndUI()
    {
        if (m_pChart)
        {
            //if (::IsWindow(m_pChart->m_hWnd))
            {
                m_pChart->DestroyWindow();
            }
            delete m_pChart;
        }
        if (m_pWMP)
        {
            m_pWMP->DestroyWindow();
            delete m_pWMP;
        }
    }
#define WMP_MODE    _T("mini")
public:
    virtual void SetPos(RECT rc)
    {
        __super::SetPos(rc);
        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
            SWP_NOZORDER | SWP_NOACTIVATE);
        if (m_strClassName == MSWMP_CLASS_NAME)
        {
            ASSERT(m_pWMP);
            m_pWMP->SetWindowPos(CWnd::FromHandle(m_PaintManager.GetPaintWindow()),
                rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
    virtual void SetInternVisible(bool bVisible = true) // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
    {
        __super::SetInternVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
        if (m_strClassName == MSWMP_CLASS_NAME)
        {
            if (m_pWMP)
            {
                m_pWMP->put_uiMode(bVisible ? WMP_MODE : _T("invisible"));
            }
        }
    }
    virtual LPCTSTR GetClass() const
    {
        return m_strClassName;
    }
    virtual LPVOID GetInterface(LPCTSTR pstrName)
    {
        if (_tcsicmp(pstrName, m_strClassName) == 0)
        {
            return this;
        }
        return __super::GetInterface(pstrName);
    }
    virtual void DoPaint(HDC hDC, const RECT& rcPaint)
    {
        //::InvalidateRect(m_hWnd, &rcPaint, TRUE);
        __super::DoPaint(hDC, rcPaint);
    }
    //virtual void DoEvent(TEventUI& event)
    //{
    //    ;
    //}
    virtual void DoInit();
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        __super::SetAttribute(pstrName, pstrValue);
    }
    virtual void SetVisible(bool bVisible = true)
    {
        __super::SetVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
        if (m_strClassName == MSWMP_CLASS_NAME)
        {
            if (m_pWMP)
            {
                //m_pWMP->ShowWindow(bVisible ? SW_SHOW : SW_HIDE);
                //"invisible":隐藏所有；"none":只显示视频或者音频效果；"mini":MINI模式；"full":完整模式；"custom":自定义效果 
                m_pWMP->put_uiMode(bVisible ? WMP_MODE : _T("invisible"));
            }
        }
    }

    operator HWND() const
    {
        return m_hWnd;
    }

    CMSChart* GetMSChart()
    {
        return m_pChart;
    }

    CWMPPlayer4* GetMSWMP()
    {
        return m_pWMP;
    }
protected:
    CMyWndUI(CPaintManagerUI& PaintManager) : m_PaintManager(PaintManager)
    , m_pChart(NULL){}
    BOOL Attach(HWND hWndNew)
    {
        if (!::IsWindow(hWndNew))
        {
            return FALSE;
        }
        m_hWnd = hWndNew;
        return TRUE;
    }
    HWND Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

    HWND m_hWnd;
    CPaintManagerUI& m_PaintManager;
    CString m_strClassName;
    DWORD   m_dwStyle;
    CMSChart    *m_pChart;
    CWMPPlayer4 *m_pWMP;
};
typedef vector<CMyWndUI*> MyWndContainer;

//朗读用的
class MyWord
{
public:
    MyWord() : m_nWordStart(0), m_nWordEnd(0), m_nWordId(0), m_nWordMatchIndex(-1), m_strWordText("")
    {}
    ~MyWord(){}
    int m_nWordStart;
    int m_nWordEnd;
    int m_nWordId;
    int m_nWordMatchIndex;
    CString m_strWordText;
};
typedef vector<MyWord>  MyWordVector;

//朗读的背诵用的
class MyBadWord
{
public:
    MyBadWord(UINT nScore = 0, LPCTSTR lpstrWord = NULL) : m_nScore(nScore)
    {
        if (lpstrWord)
        {
            m_strWord = lpstrWord;
        }
    }
    ~MyBadWord(){}
    UINT m_nScore;
    CString m_strWord;
};
typedef vector<MyBadWord>   MyBadWordVector;

//练听力用的
typedef vector<COptionUI*>      MyOptions;
typedef vector<MyOptions>       MyOptionVector;

//////////////////////////////////////////////////////////////////////////
typedef struct tagSkinInfo 
{
    const TCHAR* const  skin;
    const TCHAR* const  title;
}SkinInfo;

extern SkinInfo g_SkinInfo[];

typedef enum emSkinId
{
    sMainSkin = 0,
    sWordSelectSkin,
    sWordSkin,
    sWordSetupSkin,
    sWordExamSkin,
    sWordExamSetupSkin,
    sWordExamResultSkin,
    sWordAddNewSkin,
    sWordManageNewSkin,
    sWordToVoiceSkin,
    sReadSkin,
    sReadCfgSkin,
    sReadStatSkin,
    SReadTransSkin,
    sReadCnTextSkin,
    sDialogSkin,
    sDialogSelectSkin,
    sDialogEditSkin,
    sDialogSetupSkin,
    sDialogListSkin,
    sListenSkin,
    sListenSelectSkin,
    sListenSetupSkin,
    sListenResultSkin,
    sGrammarSkin,
    sGrammarResultSkin,
    sSymbolSkin,
    sJoySkin,
    sRecordSkin,
    sReadingbarSkin,
    sReadingbarCfgSkin,
    sReportSkin,
    sReportDetailSkin,
    sLoadingSkin,
    sExportWaveSkin,
    sDictSkin,
    sRegisterSkin,
    sPaperSelectSkin
};

typedef enum em_MY_TIMER_ID
{
    TIMER_ID_MIN_VALUE = 100,
    TIMER_ID_RECORD = TIMER_ID_MIN_VALUE,
    TIMER_ID_RECORD_PLAY,
    TIMER_ID_JOY_SONG,
    TIMER_ID_WORD_EXAM,
    TIMER_ID_READ_FOLLOW,
    TIMER_ID_READ_NEXT,
    TIMER_ID_LISTEN_NEXT,
    TIMER_ID_DLG_WRITE,
    TIMER_ID_DLG_WRITE_NEXT,
    TIMER_ID_READ_RECITE,
    TIMER_ID_READ_RECITETIP,
    TIMER_ID_WORD_FOLLOW_WORD,
    TIMER_ID_WORD_FOLLOW_EXAMPLE,
    TIMER_ID_MAX_VALUE
};

#define TIMER_INTERVAL_NEXT     3000

#define MAN_VOICE               _T("Man Voice")
#define WOMAN_VOICE             _T("Woman Voice")

//////////////////////////////////////////////////////////////////////////
//数据库中各条目，应该都放到此定义

const LPCTSTR s_strDlgSpeakers[] =
{
    _T("Spoken_Speaker"),
    _T("Talk_Speaker"),
    _T("SceneTalk_Speaker"),
};

const LPCTSTR s_strDlgSpeeds[] =
{
    _T("Spoken_Speed"),
    _T("Talk_Speed"),
    _T("SceneTalk_Speed"),
};

const LPCTSTR s_strDlgScenes[] =
{
    _T("Spoken_Scene"),
    _T("Talk_Scene"),
    _T("SceneTalk_Scene"),
};

const LPCTSTR s_strDlgLevels[] =
{
    _T("Spoken_Level"),
    _T("Talk_Level"),
    _T("SceneTalk_Level"),
};

const LPCTSTR s_strDlgSqlScenes[] =
{
    _T("跟读场景"),
    _T("人机对话场景"),
    _T("情景对话场景"),
};

const LPCTSTR s_strDlgSqlCotents[] =
{
    _T("跟读内容"),
    _T("人机对话内容"),
    _T("情景对话内容"),
};

const LPCTSTR s_strDlgSqlGroups[] =
{
    _T("跟读分组"),
    _T("人机对话分组"),
    NULL
};

const ULONG  MY_MAX_ALTERNATES = 5;

class CDialogLoading;
class CDialogSR;
class CMFCDlgSR;

typedef map<CDuiString,WORD>    SkinMap;

#if SKIN==SKN_SMART     // 博士老爸版本 [2016-1-16 Arthur]
    #ifdef APP_DEMO
        //基础版本
        #define _ProCode  "BYJC"
        #define _ProKey   "45944F72EBDF4EAD"
    #else
        #define _ProCode  "bsyy"
        #define _ProKey   "0F249099C6E04A6D"
    #endif
#else
    #ifdef APP_DEMO
        //基础版本
        #define _ProCode  "QYJC"
        #define _ProKey   "FBABA008D5164807"
    #else
        #define _ProCode  "QY"
        #define _ProKey   "5A9EB5B8C3E942AA"
        //3个月体验版本
        //#define _ProCode  "QY3T"
        //#define _ProKey   "A4C05C731AD24B2F"
#endif
#endif

class MainFrame : public WindowImplBase, IEvalSink //, public IListCallbackUI
{
public:

    MainFrame();
    ~MainFrame();
    static locale m_loc;
public:

    LPCTSTR GetWindowClassName() const;	
    virtual void OnFinalMessage(HWND hWnd);
    virtual void InitWindow();
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual CDuiString GetSkinFile();
    virtual CDuiString GetSkinFolder();
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LPCTSTR GetResourceID() const;
    virtual CDuiString GetZIPFileName() const;

    //////////////////////////////////////////////////////////////////////////
    BOOL CheckUpdate(BOOL bRun, const CString& strExe);

    //释放m_lpResourceZIPBuffer
    static void  ReleaseZIPBuffer()
    {
        if (m_lpResourceZIPBuffer)
        {
            delete[] m_lpResourceZIPBuffer;
            m_lpResourceZIPBuffer = NULL;
        }
        //m_PaintManager.m_pStrResourceZip = NULL;
    }
    CString ReadSetup(LPCTSTR strParam);
    BOOL SaveSetup(LPCTSTR strParam, LPCTSTR strValue);
    BOOL SaveSetup(LPCTSTR strParam, int nValue)
    {
        CString strTmp;
        strTmp.Format(_T("%d"), nValue);
        return SaveSetup(strParam, strTmp);
    }
    CString GetSingleField(LPCTSTR lpStrSql, ...);
    _RecordsetPtr GetTablesBySql(LPCTSTR pstrSql, ...);
    BOOL ExecuteSql(LPCTSTR pstrSql, ...);
    CString StudyGetSingleField(LPCTSTR lpStrSql, ...);
    _RecordsetPtr StudyGetTablesBySql(LPCTSTR pstrSql, ...);
    BOOL StudyExecuteSql(LPCTSTR pstrSql, ...);

    /*朗读者、朗读速度的控件名字设置成一样，可以用统一的函数处理*/
    //设置语音速度等
    void SetVoiceRateBySetting(LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed = NULL);
    void SetVoiceRateByValue(LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed = NULL);
    //从配置初始化控件
    void InitVoiceRateControl(CPaintManagerUI* PaintManager, LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed);
    //控件默认值
    static void DefaultVoiceRateControl(CPaintManagerUI* PaintManager);
    //将当前配置保存到数据库
    void SaveVoiceRate(CPaintManagerUI* PaintManager, LPCTSTR lpstrSpeaker, LPCTSTR lpstrSpeed);

    void StopSpeak()
    {
        m_cpVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
    }
    HRESULT Speak(LPCWSTR lpwText, DWORD dwFlags=SPF_ASYNC);
    HRESULT Speak(LPCSTR lpText, DWORD dwFlags=SPF_ASYNC)
    {
        return Speak(CA2W(lpText), dwFlags);
    }
    int OpenModalWnd(WindowImplBase* pWnd, LPCTSTR lpsTitle=NULL);

#define STUDY_TYPE_WORD     1
#define STUDY_TYPE_READ     2
#define STUDY_TYPE_DIALOG   3
#define STUDY_TYPE_LISTEN   4
#define STUDY_TYPE_GRAMMAR  5

#define LOG_TYPE_TIME   1
#define LOG_TYPE_ACHIEV 2
    /*
    intStudyType    g_StudyTypeString之一
    intLogType      1:intValue1为时间（分钟），intValue2忽略; 2:答题数统计，intValue1为答题总数, intValue2为正确数
    */
    BOOL StudyLog(int intStudyType, int intLogType, int intValue1, int intValue2);
    /*
    输出到文件的调用过程：
    SpeakBinfToFile，绑定到文件
    SpeakAndWait,输出到文件
    SpeakRestore,恢复
    */
    HRESULT SpeakBindToFile(LPCTSTR strWavFile);
    void SpeakRestore();
    HRESULT SpeakAndWait(LPCSTR lpText, DWORD dwFlags=SPF_ASYNC)
    {
        return SpeakAndWait(CA2W(lpText), dwFlags);
    }
    HRESULT SpeakAndWait(LPCWSTR lpwText, DWORD dwFlags=SPF_ASYNC);

    //CPaintManagerUI& GetPaintManager()
    //{
    //    return m_PaintManager;
    //}

    static void hlpWave2Mp3(LPCTSTR strWave, LPCTSTR strMp3);
    //返回当前使用的皮肤资源ID
    LPCTSTR GetCurrentResourceID() const;
    //设置当前使用的皮肤资源ID，返回以前的ID
    LPCTSTR SetCurrentResourceID(WORD nId);

    void ShowLoading();
    void HideLoading();
protected:	

    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //my functions here
    bool CheckVoice(LPCWSTR wstrMyVoice, LPCWSTR wstrSysVoice, ISpObjectToken* pToken);
    DWORD InstallVoice();
    void OpenChildModalWnd(emSkinId nSkinId);
    void StopWaveRecordReco();

    CDialogLoading*  m_pDlgLoading;
    friend class CDialogSR;
    friend class CMFCDlgSR;
    friend class CAppSetting;
    //CDialogSR*       m_pDlgSR;
    bool    m_bInitOK;

    CString m_strRegKey;
    HKEY    m_hRegKey;
    bool    m_bRegistered;
    CString RegKeyGetValue(LPCTSTR strKey);
    void RegKeySetValue(LPCTSTR strName, LPCTSTR strValue);

    IRegPtr m_pNetReg;
    //用户注册/验证用buffer，不知道应该多大，用很大的buffer
    char    m_chRegBuffer[4096*1024];

    friend class CAiEngine;
    HANDLE      m_hAiEngine;
    CAiEngine   m_AiEngine;
    CString     m_strAiRefText;
    CString     m_strAiErrMsg;
    typedef enum _SRCoreType
    {
        ct_pred = 0,
        ct_sent,
        ct_word,
        ct_max
    }SRCoreType;
    SRCoreType  m_nAiCoreType;
    //bool volatile m_bAICompleted;
    int GetAiEngineSN();

    bool    m_bMaterialInstalled;
    bool    CheckMaterial();

    WORD      m_nResourceId;
    CDuiString  m_strZipFileName;
    static SkinMap m_SkinInfo;

    emSkinId        m_nSkinId;
    CDbFile         m_DbFile;
    CComPtr<ISpVoice> m_cpVoice;
    //CComPtr<ISpAudio> m_cpOutAudio;
    CComPtr<ISpRecognizer>  m_cpRecognizer;
    CComPtr<ISpRecoContext> m_cpRecoCtxt;
    CComPtr<ISpRecoGrammar> m_cpGrammar;
    CComPtr<ISpStreamFormat> m_cpOldStream;
    CComPtr<ISpStream> m_cpWavStream;

    HWND    m_hSubWnd;
    //volatile bool m_bRecognizerInput;

    BOOL    MyReadAsciiFile(LPCTSTR strPath, CString& strContent);
    BOOL    MyWriteAsciiFile(LPCTSTR strPath, LPCTSTR strContent);
    void    GetSentencePosition(__in LPCTSTR strCtrlName, __in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition);
    void    GetSentencePosition(__in CRichEditUI* pREdt, __in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition);
    void    NoticePrompt();
    void    SetRichEditSelColor(CRichEditUI* pREdt, COLORREF rgb);
    void    ShowSymbolStatistic(const CString& strWord, const CString& strSymbol);
    //文件读取缓存
    char*           m_pchrFileBuffer;
    DWORD           m_dwFileBufferSize;
    bool            m_bHoverActived;

    MyWndContainer  m_AllMyWnd;
    void KillAllTimer();

    //先声测评接口
    SSEvalMgr m_ssEvalMgr;
    void OnEvalReply(const char *id, int type, const void *message, int size);
    int  SingSoundPronEval(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType = ct_sent);

    //主界面
    void OnPrepareMain();
    std::map<CDuiString,bool>   m_SkinMenuCheckInfo;

    //////////////////////////////////////////////////////////////////////////
    //注册界面
    void OnPrepareRegister();
    void InitRegisterPage(int nIndex);
    void RegisterNextClicked();
    void RegisterPreClicked();
    void RegisterTabSelected();
    void RegisterOkClick();
    void RegisterQRCode();
    void RegisterSuccessed();
    int  m_nRegisterIdx;
    CString m_strRegisterSN;
    CString m_strRegisterMobile;
    CString m_strRegisterAuthCode;
    CString m_strRegisterComputerId;
#define REG_QR_FILE _T("dim.png")
    CString m_strRegisterQRFile;

    //迷你朗读条
    void OnReadBarPrepare();
    //get text from Clipboard, if any
    bool ReadBarGetText();
    void ReadBarSaveTextToSpeech(LPCTSTR strWaveFile);
    //迷你朗读条的文本内容
    wstring         m_strReadbarContent;
    INPUT           m_ReadBarCtrlC[4];

    //录音机的临时文件
    CString         m_strRecordFile;
    CTime           m_tmRecorderStartTime;
    CWaveRecorder   m_WaveRecorder;
    DWORD           m_dwRecordSeconds;
    CWave           m_mciWave;
    
    friend class CListenResult;
    //初始化听力界面
    void OnListenPrepare();
    bool InitListenStatus();
    void ListenTypeCfg(int nType);
    void InitListenCfgControl();
    void InitListenQuestion(bool bPlayVoice);
    void ListenNextQuestion(/*const CString& strResult, const CString& strStuanswer*/);
    void ListenSpeakVoice();
    void ListenStartClick();
    void ListenSubmitClick();
    static bool OnListenSmallNotify(void* event);
    //返回1~4之间的数字;否则返回0
    int IsListenOpt(const TNotifyUI& msg);
    void ListenOptionClicked(int nIndx);
    int IsListenPicOpt(const TNotifyUI& msg);
    void ListenPicOptionClicked(int nIndx);
    void ListenTheOptionClicked_(int nIndx, bool bIsPic);
    void ListenGetText(int nIndx, bool bIsPic);
    void ListenGetSmallText();
    //退出界面
    void ListenNoQuestion();

    bool    m_bListenReview;
    CString m_strListenReviewQuestion;
    CString m_strListenQuestion;
    _RecordsetPtr   m_pListenQuestionPtr;
    _RecordsetPtr   m_pListenSmallQuestionPtr;
    int     m_nListenQuestionIdx;
    int     m_nListenAllQuestion;
    int     m_nListenSmallQuestionIdx;
    int     m_nListenAllSmallQuestion;
    int     m_nListenQuestionNumber;
    int     m_nListenTrueNumber;
    MyDictionary    m_ListenDic;
    CTime   m_tmListenLogFrom;
    MyOptionVector  m_ListenOptionVector;

    friend class CDialogList;
    //////////////////////////////////////////////////////////////////////////
    //对话选择界面函数
    void InitDialogSelectStatus();
    void DialogSelectExpandPage(int nIdx);
    int IsDialogSelectOpt(const TNotifyUI& msg);
    void DialogSelectFollowmeCmbChanged();
    void DialogSelectTalkCmbChanged();
    void DialogSelectSubmit();

#define DIALOG_FOLLOWME     1
#define DIALOG_TALK         2
#define DIALOG_SCENETALK    3
#define DIALOG_WRITTING     4
    int m_nDialogSelectPageId;

    //////////////////////////////////////////////////////////////////////////
    //对话界面函数
    friend class CDialogSetup;
    friend class CDialogWrittingSetup;
    void OnDialogPrepare();
    void InitDialogStatus();
    int IsDialogOpt(const TNotifyUI& msg);
    void DialogOptClicked(int nIdx);
    void DialogChangePage(int nIdx);
    void DialogDbPtrMovePre();

    void DialogSpeackCurrentSentence();
    void DialogRecordCurrentSentence();

    void DialogFollowLeftClicked();
    void DialogFollowStartClicked();
    void DialogFollowStopClicked();
    void DialogFollowRightClicked();
    void DialogFollowStatisClicked();
    void DialogFollowListClicked();
    void DialogFollowReplayClicked();

    void DialogTalkSpeakClicked();
    void DialogTalkStartClicked();
    void DialogTalkStopClicked();
    void DialogTalkSkipClicked();
    void DialogTalkListClicked();
    void DialogTalkStaticClicked();
    void DialogTalkEndOf();
    CString DialogTalkSetRequestText();
    CString DialogTalkSetAnswerText(int nMaxValue);
    CString DialogTalkSetText_(LPCTSTR pstrCtrlEN, LPCTSTR pstrCtrlCN);
    CString DialogTalkGetContent();

    int  DialogSceneTalkTextSeconds(LPCTSTR pstrSqlText);
    void DialogSceneTalkStartShow(bool bShow);
    void DialogSceneTalkEndOf();
    void DialogSceneTalkStartClicked();
    void DialogSceneTalkStopClicked();
    void DialogSceneTalkSkipClicked();
    void DialogSceneTalkSatisClicked();

    int IsDialogWrittingOpt(const TNotifyUI& msg);
    void DialogWrittingOptClicked(int nIdx);
    void DialogWrittingNext();
    void DialogWrittingStart();
    void DialogWrittingStartNext_();
    void DialogWrittingSource();
    void DialogWrittingBeginSpeak();
    void DialogWrittingStopSpeak();
    void DialogWrittingStartRead();
    void DialogWrittingStartSpeak_(int nFrom, int nTo);
    void DialogWrittingSpeakNext_();

    _RecordsetPtr   m_pDialogSpokenPtr;

    CString m_strDialogScene;
    CString m_strDialogGroup;

    //对话，播放标识
    bool    m_bDialogplaying;
    CString m_strDialogUnknowWord;
    CString m_strDialogUnknowSymbol;
    int     m_nDialogValue1;
    int     m_nDialogValue2;
    bool    m_bDialogCountTime;
    int     m_nDialogAllCount;
#define DLG_WRT_HUATI   0
#define DLG_WRT_KANTU   1
#define DLG_WRT_LANGDU  2
    int     m_nDialogWrittingType;
    CTime   m_tmDialogLogFrom;
    CTime   m_tmDialogSpeakFrom;
    int     m_nDialogSpokenIndex;

    int     m_nDialogWrittingQuestion;
    int     m_nDialogWrittingDefaultSecond;
    //对话-口头作文，话题描述和看图说话播放标识
    bool    m_bDialogWrittingPlaying;
    //用m_nDialogSpokenIndex
    //int     m_nDialogWrittingSentence;
    //话题、对话界面要用户朗读的句子组
    SplitResultVector m_sptDialogWrittingSentence;
    //答案
    SplitResultVector m_sptDialogWrittingAnswer;

    //对话-口头作文，朗读作文是否开始朗读标识
    bool    m_bDialogWrittingReading;
    //朗读结束的秒数
    int     m_nDialogWrittingLastSeconds;
    //朗读的句子
    CString m_strDialogSpokenText;
    //高亮当前句子可用
    //int     m_nDialogWrittingCurrentCursor;
    int     m_nDialogWrittingToPostion;

    //////////////////////////////////////////////////////////////////////////
    //记单词函数
    //课本生词库之间切换
    void WordSetCfgControl(bool bIsBook);
    void InitWordCfgControl();
    void InitWordCfgGradeCombo(LPCTSTR lpstrGrade=NULL);
    void InitWordCfgBookCombo(LPCTSTR lpstrBook=NULL);
    void InitWordCfgUnitCombo(LPCTSTR lpstrUnit=NULL);
    void InitWordCfgNewWordCombo(LPCTSTR lpstrNewWord=NULL);
    //单词
    void InitWordStatus();
    void InitWordGradeCombo(LPCTSTR lpstrGrade=NULL);
    void InitWordBookCombo(LPCTSTR lpstrBook=NULL);
    void InitWordUnitCombo(LPCTSTR lpstrUnit=NULL);
    void WordChangeAddNewWordBtn();
    void WordUpdateWordCount();
    void WordChangeWordParaphrase();
    void WordChangeBtnClicked();
    void WordMp3Clicked();
    void WordAutoPlayClicked();
    void WordStopAutoPlay();
    void WordAddNewClicked();
    void WordFollowWordClicked();
    void WordFollowExampleClicked();
    HRESULT WordSpeakCurretnWordAuto();
    HRESULT WordSpeakCurrentWord();
    bool        m_bWordAutoRead;
    bool        m_bWordIsPlayOn;
    int         m_nWordModeBase;
    //统计speak完成的次数，是m_nWordModeBase的整数倍则完成当前单词的朗读，进行下一个单词
    int         m_nWordSpeakCount;
    //bool        m_bWordIsClickWordList;
    //跟读单词还是例句
    bool        m_bWordFollowWord;
    //////////////////////////////////////////////////////////////////////////
    //记单词设置函数
    void InitWordSetupControls();
    //////////////////////////////////////////////////////////////////////////
    //练单词函数
    void OnWordPrepare();
    void InitWordExamStatus();
    void InitWordExamGradeCombo(LPCTSTR lpstrGrade=NULL);
    void InitWordExamBookCombo(LPCTSTR lpstrBook=NULL);
    void InitWordExamUnitCombo(LPCTSTR lpstrUnit=NULL);
    void WordExamChangeBtnClicked();
    HRESULT WordExamSpeakWord();
    //点击“查看答案”按钮
    void WordExamAnswerClicked();
    //点击“确定”按钮，选择题型
    void WordExamOKClicked();
    void InitWordExamQuestion();
    //“加入生词”按钮
    void WordExamAddWordClicked();
    //点击“完成”按钮
    void WordExamSubmitClicked();
    //点击4个答案之一
    void WordExamOptionClicked(int nIndx);
    //如果答案正确，返回true
    bool WordExamGetText(const CString& strAnswer);
    void WordExamSelectAlltype(bool bSelected);
    //返回1~4之间的数字;否则返回0
    int IsWordExamOpt(const TNotifyUI& msg);
    int IsWordExamTypeCheck(const TNotifyUI& msg);
    void WordExamTypeClicked(int nIdx);
    _RecordsetPtr m_pWordExamRecdPtr;
    //当前题
    int m_nWordExamQuestions;
    //所有题数
    int m_nWordExamQuestionsNumbers;
    //int m_nWordExamTrueQuestion;
    CString m_strWordExamAnswerResult;
    CTime   m_tmWordLogFrom;

    //////////////////////////////////////////////////////////////////////////
    //练单词结果函数
    void InitWordExamResultCtrls();
    void WordExamResultQuestionDetail();
    SplitResultVector m_strWordExamResultAll;
    void WordExamResultAddWrongWord();

    //////////////////////////////////////////////////////////////////////////
    //学语法函数
    void InitGrammarClassCmb();
    void InitGrammarListCmb();
    void InitGrammarContent();
    void InitGrammarExamSettingCtrls();
    void InitGrammarExamCtrls();
    void InitGrammarExamQuestion();
    void GrammarCheckAnswer(int nIndx);
    void GrammarNextClicked();
    //返回1~4之间的数字;否则返回0
    int IsGrammarOpt(const TNotifyUI& msg);
    //点击4个答案之一
    void GrammarOptionClicked(int nIndx);

    _RecordsetPtr m_pGrammarRecdPtr;
    int m_nGrammarQuestions;
    int m_nGrammarQuestionsNumbers;
    CString m_strGramAnswerResult;
    CTime   m_tmGrammarLogFrom;

    //////////////////////////////////////////////////////////////////////////
    //学语法结果函数
    void InitGrammarResultCtrls();
    void GrammarResultQuestionDetail();
    SplitResultVector m_strGramResultAll;
    //从语法结果界面返回，显示练习语法界面
    bool m_bGramBackFromResult;
    int  m_nGramLastLvel;
    int  m_nGramLastRand;
    int  m_nGramLastClass;

    //////////////////////////////////////////////////////////////////////////
    //音标函数
    void InitSymbolCtrls();
    void InitSymbolSymbol(LPCTSTR strId);
    void SymbolHideAllWord();
    bool IsSymbolWord(const TNotifyUI& msg);
    bool IsSymbolName(const TNotifyUI& msg);
    static bool OnSymbolWordNotify(void* event);

    //////////////////////////////////////////////////////////////////////////
    //成绩报告函数
    void InitReportCtrls();
    void InitReportMsChart(CMSChart* pChart, bool bIsTime);
    //返回1~5之间的数字，如果是详细信息按钮，否则返回0
    int IsReportDetail(const TNotifyUI& msg);
    
    //////////////////////////////////////////////////////////////////////////
    //成绩报告详细信息
    void InitReportDetailCtrls();
    int m_nReportDetailItem;

    //////////////////////////////////////////////////////////////////////////
    //娱乐函数
#define JOY_TYPE_SONG   0
#define JOY_TYPE_VEDIO  1
#define JOY_TYPE_GAME   2
    void InitJoyCtrls(UINT nIdx);
    void InitJoyContent();
    void InitJoyList(int nIndx);
    //按钮切换到停止状态
    void JoyPaneStop();
    void JoyPanePlay();
    void JoyPanePause();
    void JoyPlayClicked();
    void JoyStopClicked();
    void JoyStartClicked();
    void JoyEndClicked();
    void JoyPlaySong(CString& strPath);
    void JoyOpenClicked();
    void JoyAddClicked();
    void JoyhlpAddSongs(bool bIsOpen);
    void JoyDelClicked();
    void JoyOpenGame();
    //0, 歌曲 1,视频 2,游戏
    int m_nJoyType;
    //用于播放歌曲，图省事
    //CWMPPlayer4     m_JoyWMPlayer4;
    //当前正使用的WMP
    CWMPPlayer4*    m_pJoyWMPCur;
    CWMPControls    m_JoyWMPCurCtl;
    CWMPSettings    m_JoyWMPCurSettings;
    //总的时间
    CString m_strJoyDuration;
    double  m_dbJoyDuration;
    long    m_lJoyVolumn;

    //////////////////////////////////////////////////////////////////////////
    //朗读函数
    void OnReadWndPrepare();
    void OnReadWndClicked(TNotifyUI& msg);
    //如果在朗读，则停止朗读
    void ReadSendStopClick();
    void InitReadCtrls(int nIndex);
    void InitReadTreeView();
    void ReadTreeViewAddNode(LPCTSTR lpstrPath, CTreeNodeUI* pParent);
    void InitReadContent();
    static bool OnReadTreeViewNotify(void* event);
    void ReadDelMaterialFiles_(LPCTSTR strPath);
    //0: 默认，新建或打开;1:保存；2:另存为;返回1表示保存等继续操作，0取消操作
    int ReadChangedSave_(int bSaveAs);
    void ReadGetWordPosition_(__in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition);
    void ReadGetSentencePosition_(__in int nCurrentPos, __out int& intFromPosition, __out int& intToPosition);
    void ReadStartSpeak_(int nFrom, int nTo);
    void ReadSpeackCurrentSentence();
    HRESULT ReadKeyMouseEvent(MSGFILTER* pMsg);
    void ReadKeyDown(WPARAM wParam, LPARAM lParam);
    CString ReadHelpGetSubString(TCHAR chars[], int num);
    void ReadDoHelp(const CString& strValue, int vkValue);
    void ReadUpdateHelpList_(_RecordsetPtr& pRecdPtr);
    bool ReadUpdateHelpList(CString& strKeyWord);
    void ReadReLayoutHelp();
    void ReadSpeakNext_();
    int  ReadCompareWord(__inout CString& SourceText, const CString& WordText);
    void ReadStartRecite_();
    void ReadStartRecite();
    void ReadStopRecite();

#define READ_TYPE_READ  0
#define READ_TYPE_FOLLOW 1
#define READ_TYPE_DICTA 2
#define READ_TYPE_RECITE 3
    int     m_nReadType;
    bool    m_bReadChangeSpeaker;
    int     m_nReadCurrentSpeaker;
    CString m_strReadSpeaker;
    CString m_strReadSecondSpeaker;
    CString m_strReadLevel;
    CString m_strReadSpeed;

    CTime   m_tmReadLogFrom;
    CTime   m_tmReadSpokenFrom;
    int     m_nReadValue2;
    CString m_strReadSpokenText;
    CString m_strReadUnknowWord;
    CString m_strReadUnknowSymbol;

    //////////////////////////////////////////////////////////////////////////
    //单元测试函数
    void InitPaperCfgGradeCombo(LPCTSTR lpstrGrade=NULL);
    void InitPaperCfgBookCombo(LPCTSTR lpstrBook=NULL);
    void InitPaperCfgUnitCombo(LPCTSTR lpstrUnit=NULL);
    void PaperPrepareImage();
    void PaperCleanImage();
    void PaperCheckit();
    CLSID m_clsidpng;

    //实际上是共用部分
    void SRRecognition(CSpEvent& event);
    void SREndStream(CSpEvent& event);
#define READ_FOLLOW_WAVE_FILE   _T("ly.wav")
    DWORD SpeechRecognition_Start();
    DWORD SpeechRecognition_Start_();
    HRESULT SpeechRecognition_Stop();
    int     SpeechRecognitionCompare(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType = ct_sent);
    //采用新语音识别引擎
    int     SpeechRecognitionCompare2(const CString& strSpokenText, CString& strErrMsg, SRCoreType nCoreType = ct_sent);
    int     SpeechSpokenScoreToLevel();
    //[0]放原句, [1]放识别的句子，[2]~[6]放Alternates, [7]放UnknowWord, [8]放分数, [9]放Pronunciation(数字值), [10]放单词的音标, [11]放UnknowSymbol
    CString m_strRecognitionCompare[12];
    CString m_strCommonWaveFile;
#define CF_OPEN     _T("open")
#define CF_CLOSE    _T("close")
    CString m_strCloseFlag;

    int     m_nReadCurrentCursor;
    int     m_nReadToPostion;
    /*听写用的变量*/
    CString             m_strReadSentences;
    SplitResultVector   m_sptReadSentences;
    SplitResultVector::size_type     m_nReadCurrentSentence;

    CString m_strReadHelpKeyWord;

    std::map<CDuiString,bool>   m_ReadMenuCheckInfo;
    CComPtr<ITextDocument>      m_pReadTextDoc;
    /*记录状态*/
    CString m_strReadParentName;
    CString m_strReadContentPath;
    CString m_strReadDocumentPath;
    int     m_nReadDocument;
    int     m_iReadFontSize;
    bool    m_bReadSpeakLetter;
    bool    m_bReadSpeakWord;
    bool    m_bReadSpeakSentence;
    bool    m_bReadHelper;
    bool    m_bReadExporting;
    bool    m_bReadPlaying;
    //提示时备份原文变量
    //CString m_strReadReciteTextBak;
    int m_nReadTipS;
    //提示时计数
    int     m_nReadReciteTipeCount;
    //背诵各单词得分记录
    MyBadWordVector m_aReadbadWord;
#define RECITE_1st    0       //背诵第一句
#define RECITE_2nd    1       //背诵第二句
#define RECITE_All    2       //全文背诵
    bool    m_bReadReciting;
};

#endif // MAINFRAME_HPP