//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef TRANSLATEWORD_HPP
#define TRANSLATEWORD_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CTranslateWord : public WindowImplBase
{
public:

    CTranslateWord(MainFrame* pParent);
    ~CTranslateWord();
    //Òª·­ÒëµÄµ¥´Ê
    CString m_strWord;

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

    CTranslateWord(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);
    
    //////////////////////////////////////////////////////////////////////////
    HRESULT SpeakIt(LPCTSTR strCtrl);

    MainFrame*  m_pParent;

};

#endif // MAINFRAME_HPP