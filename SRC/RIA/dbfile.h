#ifndef __ICDFILE_H__
#define __ICDFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef vector<CString> MyCStringArray;
//typedef vector<MyCStringArray> MyDbCStringArray;
//
//inline
//CString FindVal(const MyDbCStringArray& strResult,
//                const CString& strKey)
//{
//    MyDbCStringArray::const_iterator p;
//    for (p = strResult.begin(); p != strResult.end(); ++p)
//    {
//        if (strKey == p->at(0))
//        {
//            return p->at(1);
//        }
//    }
//    return _T("");
//}

class CDbFile
{
public:
    CDbFile();
    ~CDbFile();
    //打开ICD文件
    BOOL Open(const CString& strFileName);
    //关闭icd文件，释放资源，不必显示调用，析构函数会自动调用
    void Close();
    CString GetSetup(LPCTSTR strParam);
    BOOL SetSetup(LPCTSTR strParam, LPCTSTR strValue);
    CString GetSingleField(LPCTSTR lpStrSql, bool bStudyLog=false);
    _RecordsetPtr GetTablesBySql(LPCTSTR pstrSql, bool bStudyLog=false);
    BOOL ExecuteSql(LPCTSTR pstrSql, bool bStudyLog=false);
    static void CloseRecord(_RecordsetPtr& ptr)
    {
        try
        {
            if (ptr != NULL)
            {
                if (ptr->State == adStateOpen)
                {
                    ptr->Close();
                }
                ptr.Release();
            }
        }
        catch(_com_error &e)
        {
            UNREFERENCED_PARAMETER(e);
            TRACE(_T("release or close dbase failed:%s\n"), e.ErrorMessage());
        }
    }
protected:
    void CloseConnection()
    {
        try
        {
            if (m_pConnection)
            {
                if (m_pConnection->State == adStateOpen)
                {
                    m_pConnection->Close();
                }
                m_pConnection.Release();
            }
            if (m_pConnectionLog)
            {
                if (m_pConnectionLog->State == adStateOpen)
                {
                    m_pConnectionLog->Close();
                }
                m_pConnectionLog.Release();
            }
        }
        catch(_com_error &e)
        {
            UNREFERENCED_PARAMETER(e);
            TRACE(_T("release or close dbase failed:%s\n"), e.ErrorMessage());
        }
    }
private:
    _ConnectionPtr m_pConnection;
    _ConnectionPtr m_pConnectionLog;
    _variant_t RecordsAffected;
    //_RecordsetPtr m_pRecordset;
    MyCStringArray  m_AllTables;
};

#endif