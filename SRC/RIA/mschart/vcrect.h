#if !defined(AFX_VCRECT_H__11C4D72F_AE83_4A5E_A035_51FF1E2C9E94__INCLUDED_)
#define AFX_VCRECT_H__11C4D72F_AE83_4A5E_A035_51FF1E2C9E94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcCoor;

/////////////////////////////////////////////////////////////////////////////
// CVcRect wrapper class

class CVcRect : public COleDispatchDriver
{
public:
	CVcRect() {}		// Calls COleDispatchDriver default constructor
	CVcRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcRect(const CVcRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CVcCoor GetMin();
	CVcCoor GetMax();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCRECT_H__11C4D72F_AE83_4A5E_A035_51FF1E2C9E94__INCLUDED_)
