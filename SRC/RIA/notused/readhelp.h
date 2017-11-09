//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef READHELPWND_HPP
#define READHELPWND_HPP

#define xxFindControl(T, name) static_cast<T*>(m_pPaintManager_->FindControl(name))

class MainFrame;
//////////////////////////////////////////////////////////////////////////
//
class CWordHelp : public CControlUI
{
public:

    CWordHelp();
    ~CWordHelp();
    void SetMainFrame(MainFrame* pFrame);
    //返回FALSE如果没有建议词
    BOOL UpdateList(CString& strKeyWord);
    //cx, cy单词位置，nWidth, nHeight控件的宽、高
    void Relayout(int cx, int cy, int nWidth, int nHeight);
    int GetHelpCount() const;
    CString GetHelpString() const;
    void SelectItem(int nIndx);
public:

    //LPCTSTR GetWindowClassName() const;	
    //virtual void OnFinalMessage(HWND hWnd);
    //virtual void InitWindow();
    //virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    //virtual UILIB_RESOURCETYPE GetResourceType() const;
    //virtual CDuiString GetSkinFile();
    //virtual CDuiString GetSkinFolder();
    //virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    //virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    //virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //virtual LPCTSTR GetResourceID() const;

    void HelpNotify(TNotifyUI& msg);
protected:	
    //void Notify(TNotifyUI& msg);
    //void OnPrepare(TNotifyUI& msg);
    //void OnExit(TNotifyUI& msg);
    //void OnTimer(TNotifyUI& msg);
    
    //////////////////////////////////////////////////////////////////////////
    void _UpdateList(_RecordsetPtr& pRecdPtr);
    MainFrame*  m_pFrame;
    CString m_strKeyWord;
    CPaintManagerUI* m_pPaintManager_;
};

#endif // MAINFRAME_HPP