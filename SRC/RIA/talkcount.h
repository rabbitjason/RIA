//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef TALKCOUNT_HPP
#define TALKCOUNT_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CTalkCount : public WindowImplBase
{
public:

    CTalkCount(MainFrame* pParent);
    ~CTalkCount();
    CString m_strWord;
    CString m_strSymbol;

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

    CTalkCount(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);
    
    //////////////////////////////////////////////////////////////////////////
    void InitCtrl(bool bSymbol);

    MainFrame*  m_pParent;
    SplitResultVector m_sptWord, m_sptSymbol;

};

#endif // MAINFRAME_HPP