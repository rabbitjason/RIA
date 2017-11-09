//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDMANAGEWORD_HPP
#define WORDMANAGEWORD_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordMgtword : public WindowImplBase
{
public:

    CWordMgtword(MainFrame* pParent);
    ~CWordMgtword();
    CString m_strGId;
    CString m_strBId;
    CString m_strUId;

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
    //virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LPCTSTR GetResourceID() const;

protected:	

    CWordMgtword(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //初始化控件
    void InitControls(TNotifyUI& msg);
    MainFrame*  m_pParent;

    void InitWordList(LPCTSTR strFilter=NULL);
    void Add2SelWord(UINT nId, LPCTSTR strWord, bool bNew = false, int nPos=-1);
    void Add2AllWord(UINT nId, LPCTSTR strWord, LPCTSTR strTrans);
    BOOL mySave();
    void myUp(int nItem);
    void myDown(int nItem);

    struct _tagWord
    {
        _tagWord(UINT _nId=0) : nId(_nId), bNew(false){}
        _tagWord(const _tagWord& rv)
        {
            if (this != &rv)
            {
                nId = rv.nId;
                bNew = rv.bNew;
                strWord = rv.strWord;
            }
        }
        _tagWord& operator=(const _tagWord& rv)
        {
            if (this != &rv)
            {
                nId = rv.nId;
                bNew = rv.bNew;
                strWord = rv.strWord;
            }
            return *this;
        }
        bool operator==(const _tagWord& rv)
        {
            return nId == rv.nId;
        }
        UINT nId;
        bool bNew;
        //for debug
        CString strWord;
        //bool bModified;
    };
    typedef _tagWord tagWord;
    typedef vector<tagWord> ArrayWords;
    ArrayWords  m_arSelWord;
    //记录在此的，需要从数据库删除
    ArrayWords  m_arWordRemoved;
    ArrayWords  m_arAllWord;

    static LPCTSTR m_strAttr;
};

#endif // MAINFRAME_HPP