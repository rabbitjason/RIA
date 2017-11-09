#include "stdafx.h"
#include "dbfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDbFile::CDbFile()
: m_pConnection(NULL)
{
	;
}

CDbFile::~CDbFile()
{
	Close();
}

BOOL CDbFile::Open(const CString& strFileName)
{
	BOOL bRet = FALSE;
	_RecordsetPtr m_pRecordset = NULL;
	HRESULT Hr = S_OK;

	do 
	{
		CFileFind finder;
		TCHAR szPath[MAX_PATH];
		HRESULT Hr = ::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, szPath);
		if (S_OK != Hr)
		{
			TRACE(_T("SHGetFolderPath failed with 0x%08x\n"), Hr);
			MyAfxMessageBox(_T("%s"), _T("获取Local Settings Application Data文件夹失败！"));
			break;
		}
		//TRACE(_T("Local Settings Application Data:%s\n"), szPath);
		CString strTitle;
		strTitle.LoadString(AFX_IDS_APP_TITLE);
		::PathAppend(szPath, _T("nyedu"));
		::PathAppend(szPath, strTitle);
		strTitle.Format(_T("%s\\%s"), szPath, g_strStudyDBaseFile);
		BOOL bWorking = finder.FindFile(strTitle);
		if (!bWorking)
		{
			TCHAR strDB[1024] = {_T('\0'),}, strDest[1024] = {_T('\0'),};
			StringCchPrintf(strDB, _countof(strDB), _T("%s%s\0"), CPaintManagerUI::GetInstancePath().GetData(), g_strStudyDBaseFile);
			StringCchPrintf(strDest, _countof(strDest), _T("%s\0"), strTitle);
			TRACE(_T("Copy %s to %s\n"), strDB, strDest);
			SHFILEOPSTRUCT shfo = {0};
			shfo.hwnd = NULL;
			shfo.wFunc = FO_COPY;
			shfo.pFrom = strDB;
			shfo.pTo = strDest;
			shfo.fFlags = FOF_NOCONFIRMMKDIR;
			int x = ::SHFileOperation(&shfo);
			if (0 != x)
			{
				//TCHAR err[1024];
				//::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, dwErr, 0, err, 1024, 0);
				MyAfxMessageBox(_T("初始化用户数据库失败：0x%x"), x);
				break;
			}
		}
		finder.Close();

		//for safer
		if (strFileName.IsEmpty())
		{
			break;
		}

		//Provider=SQLOLEDB
		//driver={SQL Server};Server=XP-201203132217\\SQLSERVER2005;Database=Student;UID=sa;PWD=yuzhongchun","","",adModeUnknown
		CString strOpen, strStudy;
		//strOpen.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=4knlnl1vmlpp22cna"), strFileName);
		strOpen.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=asdjhfy468asddsdm"), strFileName);
		strStudy.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=asdjhfy468asddsdm"), strTitle);

		try
		{
			//or, "ADODB.Connection"
			CLSID cs_connect = __uuidof(Connection);
			Hr = m_pConnection.CreateInstance(cs_connect);
			if (FAILED(Hr))
			{
				MyAfxMessageBox(_T("连接数据库失败:%s"),
					MyFormatMessage(Hr));
				break;
			}
			m_pConnection->put_Mode(adModeReadWrite);
			m_pConnection->Open((_bstr_t)strOpen, _T(""), _T(""), adConnectUnspecified);

			Hr = m_pConnectionLog.CreateInstance(__uuidof(Connection));
			m_pConnectionLog->put_Mode(adModeReadWrite);
			m_pConnectionLog->Open((_bstr_t)strStudy, _T(""), _T(""), adConnectUnspecified);
		}
		catch(_com_error e)
		{
			MyAfxMessageBox(_T("连接数据库失败!\r\n%s,%s"),
				(LPCTSTR)e.Description(), e.ErrorMessage());
			break;
		}
#if 0
		try
		{
			//m_pRecordset.CreateInstance(_T("ADODB.Recordset"));
			Hr = m_pRecordset.CreateInstance(__uuidof(_Recordset));

			m_AllTables.clear();
			m_pRecordset = m_pConnection->OpenSchema(adSchemaTables);
			while(!m_pRecordset->GetadoEOF())
			{
				_bstr_t table_name = m_pRecordset->Fields->GetItem(_T("TABLE_NAME"))->Value;

				//TRACE(_T("table name:%s\n"), (LPCTSTR)table_name);
				m_AllTables.push_back((LPCTSTR)table_name);
				m_pRecordset->MoveNext();
			}
			CloseRecord(m_pRecordset);

#if 0
			strOpen = _T("Select top 10 * from 听力习题 where 年级=2 and 类型=3 and Id in (Select 习题 from 听力小题) Order by Rnd(-(Id+1234))");
			//strOpen = _T("Select top 10 * from 听力习题");
			TRACE(_T("sql:%s\n"), (LPCTSTR)strOpen);
			m_pRecordset = m_pConnection->Execute((_bstr_t)strOpen, &RecordsAffected, adCmdText);
			MyCStringArray strlocal;
			FieldsPtr fields = m_pRecordset->GetFields();
			long x = fields->GetCount(), y;
			for (y = 0; y < x; y++)
			{
				FieldPtr fd = fields->GetItem(y);
				strlocal.push_back(LPCTSTR(fd->GetName()));
				TRACE(_T("name:%s, value:%s, type:%d\n"), (LPCTSTR)fd->GetName(), CString(fd->GetValue()), fd->GetType());
			}

			_variant_t val;
			MyCStringArray result;
			MyCStringArray::const_iterator pCollect;
			MyDbCStringArray all_re;
			while(!m_pRecordset->GetadoEOF())
			{
				result.clear();
				for (pCollect = strlocal.begin(); pCollect != strlocal.end(); ++pCollect)
				{
					val = m_pRecordset->GetCollect((LPCTSTR)*pCollect);
					result.push_back((val.vt == VT_NULL) ? CString(_T("")) : CString(val));
				}

				all_re.push_back(result);
				m_pRecordset->MoveNext();
			}
			CloseRecord(m_pRecordset);//DB_E_ERRORSINCOMMAND

			MyDbCStringArray::const_iterator pp;
			int idx = 0;
			for (pp = all_re.begin(); pp != all_re.end(); ++pp)
			{
				TRACE(_T("row %d:"), idx++);
				for (pCollect = pp->begin(); pCollect != pp->end(); ++pCollect)
				{
					TRACE(_T("%s,"), (LPCTSTR)*pCollect);
				}
				TRACE(_T("\n\n"));
			}
#endif
		}
		catch(_com_error &e)
		{
			AfxMessageBox((LPCTSTR)e.Description());
			break;
		}
#endif
		//try
		//{
		//    if(!m_pRecordset->BOF)
		//        m_pRecordset->MoveFirst();
		//    else
		//    {
		//        //MyAfxMessageBox(_T("表内数据为空"));
		//        break;
		//    }
		//}
		//catch(_com_error &e)
		//{
		//    MyAfxMessageBox(e.Description());
		//}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

void CDbFile::Close()
{
	CloseConnection();
}

CString CDbFile::GetSetup(LPCTSTR strParam)
{
	CString strOpen;
	CString strValue = _T("0");
	_RecordsetPtr m_pRecordset = NULL;

	do 
	{
		try
		{
			CString strTmp = strParam;
			strTmp.Replace(_T("'"), _T("''"));
			strOpen.Format(_T("Select 值 from 配置 where 参数='%s'"),
				strTmp);

			strValue = GetSingleField(strOpen);

			//m_pRecordset.CreateInstance(__uuidof(_Recordset));

			//m_pRecordset = m_pConnection->Execute((_bstr_t)strOpen, &RecordsAffected, adCmdText);

			//if(!m_pRecordset->GetBOF())
			//    m_pRecordset->MoveFirst();
			//else
			//{
			//    TRACE(_T("配置 %s 值为空\n"),
			//        strParam);
			//    break;
			//}

			//strValue = GetCollect(m_pRecordset, _T("值"));
			//_variant_t val;
			//val = m_pRecordset->GetCollect(_T("值"));
			//if (val.vt != VT_NULL)
			//{
			//    strValue = (LPCTSTR)(_bstr_t)val;
			//}
		}
		catch(_com_error &e)
		{
			MyAfxMessageBox(_T("读取配置 %s 失败：%s。"),
				strParam, (LPCTSTR)e.Description());
			break;
		}
	} while (FALSE);

	CloseRecord(m_pRecordset);
	return strValue;
}

BOOL CDbFile::SetSetup(LPCTSTR strParam, LPCTSTR strValue)
{
	BOOL bRet = FALSE;
	CString strOpen;
	//_CommandPtr     pCmdChange;

	do 
	{
		try
		{
			CString strTmp = strParam;
			strTmp.Replace(_T("'"), _T("''"));
			CString strTmp2 = strValue;
			strTmp2.Replace(_T("'"), _T("''"));
			strOpen.Format(_T("Update 配置 set 值='%s' where 参数='%s'"),
				strTmp2, strTmp);

			//TRACE(_T("%s\n"), strOpen);
			bRet = ExecuteSql(strOpen);
			//m_pConnection->Execute((_bstr_t)strOpen, &RecordsAffected, adCmdText | adExecuteNoRecords);

			//pCmdChange.CreateInstance(__uuidof(Command));
			//pCmdChange->ActiveConnection = m_pConnection;
			//pCmdChange->CommandText = _bstr_t((LPCTSTR)strOpen);
			//pCmdChange->Execute(NULL, NULL, adCmdText);
		}
		catch(_com_error &e)
		{
			bRet = FALSE;
			MyAfxMessageBox(_T("保存配置 %s：%s 失败：%s。"),
				strParam, strValue, (LPCTSTR)e.Description());
			break;
		}

		//bRet = TRUE;

	} while (FALSE);

	return bRet;
}

_RecordsetPtr CDbFile::GetTablesBySql(LPCTSTR pstrSql, bool bStudyLog/*=false*/)
{
	BOOL bRet = FALSE;
	_RecordsetPtr pRecordset = NULL;
	_ConnectionPtr& pConnection = bStudyLog ? m_pConnectionLog : m_pConnection;

	do 
	{
		try
		{
			//pRecordset.CreateInstance(__uuidof(_Recordset));

			pRecordset = pConnection->Execute((_bstr_t)pstrSql, &RecordsAffected, adCmdText);

			if(!pRecordset->GetBOF())
				pRecordset->MoveFirst();
			else
			{
				TRACE(_T("表内数据为空, %s\n"), pstrSql);
				break;
			}
		}
		catch(_com_error &e)
		{
			MyAfxMessageBox(_T("数据库访问GetTablesBySql失败:%s。"),
				(LPCTSTR)e.Description());
			break;
		}
		catch(...)
		{
			AfxMessageBox(_T("数据库访问GetTablesBySql失败。"));
			break;
		}

		bRet = TRUE;

	} while (FALSE);

	if (!bRet)
	{
		CloseRecord(pRecordset);
		pRecordset = NULL;
	}
	return pRecordset;
}

CString CDbFile::GetSingleField(LPCTSTR lpStrSql, bool bStudyLog/*=false*/)
{
	CString strValue = _T("");
	_RecordsetPtr m_pRecordset = NULL;
	_ConnectionPtr& pConnection = bStudyLog ? m_pConnectionLog : m_pConnection;

	do 
	{
		try
		{
			//m_pRecordset.CreateInstance(__uuidof(_Recordset));

			m_pRecordset = pConnection->Execute((_bstr_t)lpStrSql, &RecordsAffected, adCmdText);
			//m_pRecordset->Open((_bstr_t)lpStrSql,
			//    m_pConnection.GetInterfacePtr(),
			//    adOpenStatic,  //用静态游标
			//    adLockOptimistic,
			//    adCmdText);

			if(!m_pRecordset->GetBOF())
				m_pRecordset->MoveFirst();
			else
			{
				TRACE(_T("查询 %s 为空\n"),
					lpStrSql);
				break;
			}

			strValue = GetCollect(m_pRecordset, (long)0);
			//            _variant_t val;
			//#if 0
			//            FieldsPtr fields = m_pRecordset->GetFields();
			//            TRACE("count:%d\n", fields->GetCount());
			//#endif
			//            val = m_pRecordset->GetCollect((_variant_t((long)0)));
			//            if (val.vt != VT_NULL)
			//            {
			//                strValue = (LPCTSTR)(_bstr_t)val;
			//            }
			//TRACE(_T("value:%s\n"), strValue);
		}
		catch(_com_error &e)
		{
			MyAfxMessageBox(_T("数据库访问GetSingleField失败：%s, %s, %s。"),
				lpStrSql, (LPCTSTR)e.Description(), e.ErrorMessage());
			break;
		}
		catch(...)
		{
			AfxMessageBox(_T("数据库访问GetSingleField失败。"));
			break;
		}
	} while (FALSE);

	CloseRecord(m_pRecordset);
	return strValue;
}

BOOL CDbFile::ExecuteSql(LPCTSTR pstrSql, bool bStudyLog/*=false*/)
{
	BOOL bRet = FALSE;
	_ConnectionPtr& pConnection = bStudyLog ? m_pConnectionLog : m_pConnection;

	do 
	{
		try
		{
			pConnection->Execute((_bstr_t)pstrSql, &RecordsAffected, adCmdText);
		}
		catch(_com_error &e)
		{
			//CString strTmp;
			//strTmp.Format(_T("%d:%s, sql:%s"), __LINE__, (LPCTSTR)e.Description(), pstrSql);
			//MyAfxMessageBox(strTmp);
			MyAfxMessageBox(_T("数据库访问ExecuteSql失败：%s。"),
				(LPCTSTR)e.Description());
			break;
		}
		catch(...)
		{
			AfxMessageBox(_T("数据库访问ExecuteSql失败。"));
			break;
		}

		bRet = TRUE;

	} while (FALSE);

	return bRet;
}

#if 0
struct TableField
{
	TableField()
		: strFieldName(_T(""))
		, nFieldType(0)
		, nFieldLength(0)
	{
	}

	CString strFieldName; // 字段名称
	int nFieldType; // 字段类型
	int nFieldLength; //字段大小
};

/*
@brief 获取表的所有字段
@param[in] pConnPtr 数据库连接
@param[in] strTableName 表名称
@param[out] vTableFields 字段列表
@return 获取是否成功
*/
bool GetTableField(_ConnectionPtr& pConnPtr, const CString& strTableName, vector<TableField>& vTableFields)
{
	CString strSql = _T("");
	strSql.Format(_T("select * from %s"), strTableName);

	_RecordsetPtr pRecordsetOrig;
	pRecordsetOrig.CreateInstance(__uuidof(Recordset));
	HRESULT = pRecordsetOrig->Open(_bstr_t(strSql), _variant_t(pConnPtr.GetInterfacePtr()),
		adOpenDynamic, adLockOptimistic, adCmdText);

	// 得到所有字段
	FieldsPtr fields = pRecordsetOrig->GetFields();
	for (long i = 0; i < fields->Count; i++)
	{
		FieldPtr field = fields->Item[i];

		TableField curField;
		curField.strFieldName = field->Name;//字段名
		curField.nFieldType = field->Type;//字段类型
		curField.nFieldLength = field->NumericScale;

		vTableFields.push_back(curField);
	}

	pRecordsetOrig->Close();

	return true;
}
#endif