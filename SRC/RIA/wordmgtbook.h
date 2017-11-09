//
// main_frame.hp
// ~~~~~~~~~~~~~~
//
// Beware of bugs.
//

#ifndef WORDBOOKMANAGE_HPP
#define WORDBOOKMANAGE_HPP

class MainFrame;
class CDlgInputName;
//////////////////////////////////////////////////////////////////////////
//
class CWordMgtBook : public WindowImplBase
{
public:

    CWordMgtBook(MainFrame* pParent);
    ~CWordMgtBook();

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
    void Close(UINT nRet = IDOK);

protected:	

    friend class CDlgInputName;
    CWordMgtBook(){}
    void Notify(TNotifyUI& msg);
    void OnPrepare(TNotifyUI& msg);
    void OnExit(TNotifyUI& msg);
    void OnTimer(TNotifyUI& msg);

    //////////////////////////////////////////////////////////////////////////
    //初始化控件
    static LPCTSTR g_strAttr;

    void InitControls(TNotifyUI& msg);
    MainFrame*  m_pParent;
    void InitBookList(UINT nGradeIndex, LPCTSTR lpstrBook = NULL);
    void InitUnitList(UINT nGradeId, UINT nBookId, LPCTSTR lpstrUnit = NULL);
    template<class Type>
    void ItemUp(CListUI* pList, int nItem, Type& array)
    {
        if (nItem <= 0)
        {
            return;
        }

        swap(array[nItem], array[nItem-1]);

        int nAfter = nItem - 1;
        CListLabelElementUI* pItem = new CListLabelElementUI;
        CListLabelElementUI* pOld = (CListLabelElementUI*)pList->GetItemAt(nItem);
        pItem->ApplyAttributeList(g_strAttr);
        pItem->SetText(pOld->GetText().GetData());
        pItem->SetUserData(pOld->GetUserData().GetData());
        pList->RemoveAt(nItem);
        pList->AddAt(pItem, nAfter);
        pList->SelectItem(nAfter);
        pList->SetPos(pList->GetPos());
        pList->EnsureVisible(nAfter);
        m_bModified = true;
    }
    template<class Type>
    void ItemDown(CListUI* pList, int nItem, Type& array)
    {
        if (nItem >= pList->GetCount() - 1)
        {
            return;
        }

        swap(array[nItem], array[nItem+1]);

        int nAfter = nItem + 1;
        CListLabelElementUI* pItem = new CListLabelElementUI;
        CListLabelElementUI* pOld = (CListLabelElementUI*)pList->GetItemAt(nItem);
        pItem->ApplyAttributeList(g_strAttr);
        pItem->SetText(pOld->GetText().GetData());
        pItem->SetUserData(pOld->GetUserData().GetData());
        pList->RemoveAt(nItem);
        pList->AddAt(pItem, nAfter);
        pList->SelectItem(nAfter);
        pList->SetPos(pList->GetPos());
        pList->EnsureVisible(nAfter);
        m_bModified = true;
    }

    //void myUpdateItem();
    //nType:0, grade; 1, book; 2, unit
    void myEditItem(int nType, int nItem, CEditUI* pEdit);

    void myAddModifyItem(int nType, int nItem);
    BOOL myCheckName(LPCTSTR strName);
    BOOL mySaveAll();
    //检查年级、教材、课文是否为空
    BOOL myCheckAll();

    //////////////////////////////////////////////////////////////////////////
    //data
private:
    typedef struct _tagDbDat
    {
        _tagDbDat() : bNew(false), bModified(false) {}
        CString strId;
        CString strName;
        CString strSeq;
        bool    bNew;
        bool    bModified;
    }tagDbDat;

    struct _tagUNIT : public tagDbDat
    {

    };
    typedef _tagDbDat tagUNIT;
    typedef vector<tagUNIT>    ArrayUnit;

    struct _tagBOOK : public tagDbDat
    {
        _tagBOOK() : nMaxUnitId(0){}
        CString strOrder;
        ArrayUnit   arUnit;
        ArrayUnit   arUnitRemoved;
        UINT    nMaxUnitId;
    };
    typedef _tagBOOK tagBOOK;
    typedef vector<tagBOOK>    ArrayBOOK;

    struct _tagGrade : public tagDbDat
    {
        _tagGrade() : nMaxBookId(0){}
        ArrayBOOK   arBooks;
        ArrayBOOK   arBooksRemoved;
        UINT        nMaxBookId;
    };
    typedef _tagGrade tagGrade;
    typedef vector<tagGrade>    ArrayGrade;
    UINT    m_nMaxGradeId;

    ArrayGrade  m_arGrade;
    //记录在此的，需要从数据库删除
    ArrayGrade  m_arGradeRemoved;

    CEditUI* m_pCurEdit;
    int      m_nCurType;
    int      m_nCurItem;
    tagGrade* m_pCurGradeDat;
    tagBOOK*  m_pCurBookDat;
    tagUNIT*  m_pCurUnitDat;

    tagDbDat*   m_pCurDbDat;

    bool    m_bModified;
};

#endif // MAINFRAME_HPP