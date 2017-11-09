#if !defined(AFX_VCFONT_H__785EEE8D_354D_4400_B4DE_DF2A0C8DB752__INCLUDED_)
#define AFX_VCFONT_H__785EEE8D_354D_4400_B4DE_DF2A0C8DB752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcColor;

/////////////////////////////////////////////////////////////////////////////
// CVcFont wrapper class

class CVcFont : public COleDispatchDriver
{
public:
	CVcFont() {}		// Calls COleDispatchDriver default constructor
	CVcFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcFont(const CVcFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	float GetSize();
	void SetSize(float newValue);
	short GetStyle();
	void SetStyle(short nNewValue);
	short GetEffect();
	void SetEffect(short nNewValue);
	CVcColor GetVtColor();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCFONT_H__785EEE8D_354D_4400_B4DE_DF2A0C8DB752__INCLUDED_)
