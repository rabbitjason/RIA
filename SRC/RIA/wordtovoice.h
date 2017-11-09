//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDTOVOICE_HPP
#define WORDTOVOICE_HPP

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordToVoice : public WindowImplBase
{
public:

    CWordToVoice(MainFrame* pParent);
    ~CWordToVoice();

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

    //virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

protected:	

    CWordToVoice(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //³õÊ¼»¯¿Ø¼þ
    void InitControls(TNotifyUI& msg);
    void ExportMp3();
    //_RecordsetPtr m_pSetPtr;
    MainFrame*  m_pParent;
    bool        m_bExported;
    CString     m_strPathFile;
};

#endif // MAINFRAME_HPP