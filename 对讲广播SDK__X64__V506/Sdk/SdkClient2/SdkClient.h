// SdkClient.h : main header file for the SDKCLIENT application
//

#if !defined(AFX_SDKCLIENT_H__1D6CB7EC_01DA_4FF8_829C_775CDBF982E7__INCLUDED_)
#define AFX_SDKCLIENT_H__1D6CB7EC_01DA_4FF8_829C_775CDBF982E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSdkClientApp:
// See SdkClient.cpp for the implementation of this class
//

class CSdkClientApp : public CWinApp
{
public:
	CSdkClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSdkClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSdkClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDKCLIENT_H__1D6CB7EC_01DA_4FF8_829C_775CDBF982E7__INCLUDED_)
