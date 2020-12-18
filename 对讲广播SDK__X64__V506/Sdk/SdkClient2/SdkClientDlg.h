// SdkClientDlg.h : header file
//

#if !defined(AFX_SDKCLIENTDLG_H__DF841196_9EDC_46FD_A70F_49CBD37DDB1D__INCLUDED_)
#define AFX_SDKCLIENTDLG_H__DF841196_9EDC_46FD_A70F_49CBD37DDB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include/cts_sdk.h"

typedef struct
{
	enSdkDevState eState;
	BYTE AlmInState;  // 报警输入通道状态指示码，每位对应一路报警: 0(无报警), 1(有报警)。
	BYTE AlmOutState; // 报警输出通道状态指示码，每位对应一路报警: 0(无报警), 1(有报警)。
}TDevState;

/////////////////////////////////////////////////////////////////////////////
// CSdkClientDlg dialog

class CSdkClientDlg : public CDialog
{
// Construction
public:
	CSdkClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSdkClientDlg)
	enum { IDD = IDD_SDKCLIENT_DIALOG };
	CSliderCtrl	m_AmpSlider;
	CEdit	m_edtTermName;
	CSpinButtonCtrl	m_DbValSpin;
	CComboBox	m_cbxTermList;
	CStatic	m_stcTerm;
	CEdit	m_edt485PipeData;
	CStatic	m_stcTips;
	CListBox	m_InfoList;
	CString	m_str485PipeData;
	CString	m_strSend485Data;
	int		m_nCapLevel;
	int		m_nAmpLevel;
	int		m_nCapTalkLevel;
	BOOL	m_bDbAlarm;
	CString	m_strTermName;
	CString	m_strUserData;
	CString	m_strWriteDate;
	UINT	m_nWaitTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSdkClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	TSdkEventTermRegister m_TermList[MAX_TERM_COUNT];
	TDevState m_TermState[MAX_TERM_COUNT];
	int m_nTermCnt;
	int m_nTalkTermIdx;
	BOOL m_bIsBroadingVoice;

	static int CALLBACK OnTzlSdkCallback(enSdkCbType eCbType, LPVOID pParam, DWORD dwSize, int usr_data);
	void UpdateTermListUI();
	CString GetExeDir();
	void LoadTermList();
	void StoreTermList();

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSdkClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOpenTalk();
	afx_msg void OnCloseTalk();
	afx_msg void OnBtnSend485();
	afx_msg void OnBtnStartBroadVoice();
	afx_msg void OnBtnSetupBroadGroup();
	afx_msg void OnBtnStopBroadVoice();
	afx_msg void OnBtnStartBroadMp3();
	afx_msg void OnBtnBroadMp3File();
	afx_msg void OnBtnStopBroadMp3();
	afx_msg void OnGetLevel();
	afx_msg void OnSetLevel();
	afx_msg void OnBtnDismissTalk();
	afx_msg void OnGetDbAlm();
	afx_msg void OnSetDbAlm();
	afx_msg void OnBtnReqIFrm();
	afx_msg void OnBtnOpenAlmOut1();
	afx_msg void OnBtnOpenAlmOut2();
	afx_msg void OnBtnClear();
	afx_msg void OnSelchangeCbxTermList();
	afx_msg void OnBtnChangeName();
	afx_msg void OnBtnOpenTalkSub();
	afx_msg void OnBtnCloseTalkSub();
	afx_msg void OnBtnReqIFrmSub();
	afx_msg void OnBtnGetUserData();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnGet();
	afx_msg void OnBtnOpenMon();
	afx_msg void OnBtnCloseMon();
	afx_msg void OnBtnPauseMp3Broad();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnChangeWaitTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDKCLIENTDLG_H__DF841196_9EDC_46FD_A70F_49CBD37DDB1D__INCLUDED_)
