#if !defined(AFX_VCSTATLINE_H__FA3F8021_6DD3_4AB2_9994_780634D8728C__INCLUDED_)
#define AFX_VCSTATLINE_H__FA3F8021_6DD3_4AB2_9994_780634D8728C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcColor;

/////////////////////////////////////////////////////////////////////////////
// CVcStatLine wrapper class

class CVcStatLine : public COleDispatchDriver
{
public:
	CVcStatLine() {}		// Calls COleDispatchDriver default constructor
	CVcStatLine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcStatLine(const CVcStatLine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetFlag();
	void SetFlag(short nNewValue);
	CVcColor GetVtColor();
	float GetWidth();
	void SetWidth(float newValue);
	long GetStyle(long type);
	void SetStyle(long type, long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCSTATLINE_H__FA3F8021_6DD3_4AB2_9994_780634D8728C__INCLUDED_)
