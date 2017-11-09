//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef LISTEN_RESULT_HPP
#define LISTEN_RESULT_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CListenResult : public WindowImplBase
{
public:

    CListenResult(MainFrame* pParent);
    ~CListenResult();
    CString m_strQuestion;
    int m_nNumber;
    int m_nTrueNumber;
    //_RecordsetPtr   m_pOldQuestionPtr;
    //MyDictionary    m_ListenDic;
    CString m_strReviewQuestion;

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

    CListenResult(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    MainFrame*  m_pParent;
    SplitResultVector m_sptQuestion;

};

#endif // MAINFRAME_HPP