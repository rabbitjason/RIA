//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef LISTEN_VOICE_SETUP_HPP
#define LISTEN_VOICE_SETUP_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CListenVoiceSetup : public WindowImplBase
{
public:

    CListenVoiceSetup(MainFrame* pParent);
    ~CListenVoiceSetup();

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

    CListenVoiceSetup(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    MainFrame*  m_pParent;

};

#endif // MAINFRAME_HPP