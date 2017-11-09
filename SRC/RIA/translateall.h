//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef TRANSLATEALL_HPP
#define TRANSLATEALL_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CTranslateAll : public WindowImplBase
{
public:

    CTranslateAll(MainFrame* pParent);
    ~CTranslateAll();
    //х╚нд
    CString m_strContent;

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

    CTranslateAll(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);
    
    //////////////////////////////////////////////////////////////////////////
    HRESULT SpeakIt(LPCTSTR strCtrl);

    MainFrame*  m_pParent;

};

#endif // MAINFRAME_HPP