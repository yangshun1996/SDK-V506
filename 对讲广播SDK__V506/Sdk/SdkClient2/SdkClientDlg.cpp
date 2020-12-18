// SdkClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SdkClient.h"
#include "SdkClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INI_FILE        "SdkClient.ini"
#define AMSG(x)          ((x) ? "有" : "无")

CSdkClientDlg * g_pMainWnd = NULL;
static int g_MyGroupNum = 6;
FILE * g_pMp3File1 = NULL;
FILE * g_pMp3File2 = NULL;
BOOL g_bPauseMp3Broad = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdkClientDlg dialog

CSdkClientDlg::CSdkClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSdkClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSdkClientDlg)
	m_str485PipeData = _T("");
	m_strSend485Data = _T("");
	m_nCapLevel = -1;
	m_nAmpLevel = -1;
	m_nCapTalkLevel = -1;
	m_bDbAlarm = FALSE;
	m_strTermName = _T("");
	m_strUserData = _T("");
	m_strWriteDate = _T("");
	m_nWaitTime = 60;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pMainWnd = this;

	memset(m_TermList, 0, sizeof(m_TermList));
	memset(m_TermState, 0, sizeof(m_TermState));
	m_nTermCnt = 0;
	m_nTalkTermIdx = -1;
	m_bIsBroadingVoice = FALSE;
}

void CSdkClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSdkClientDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_AmpSlider);
	DDX_Control(pDX, IDC_EDIT_TERM_NAME, m_edtTermName);
	DDX_Control(pDX, IDC_SPIN_DB_VAL, m_DbValSpin);
	DDX_Control(pDX, IDC_CBX_TERM_LIST, m_cbxTermList);
	DDX_Control(pDX, IDC_STATIC_TERM, m_stcTerm);
	DDX_Control(pDX, IDC_EDT_485_PIPE_DATA, m_edt485PipeData);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_stcTips);
	DDX_Control(pDX, IDC_LIST_INFO, m_InfoList);
	DDX_Text(pDX, IDC_EDT_485_PIPE_DATA, m_str485PipeData);
	DDX_Text(pDX, IDC_EDT_SEND_485_DATA, m_strSend485Data);
	DDX_CBIndex(pDX, IDC_CMB_CAP, m_nCapLevel);
	DDX_CBIndex(pDX, IDC_CMB_AMP, m_nAmpLevel);
	DDX_CBIndex(pDX, IDC_CMB_CAP_TALK, m_nCapTalkLevel);
	DDX_Check(pDX, IDC_CHK_DB_ALARM, m_bDbAlarm);
	DDX_Text(pDX, IDC_EDIT_TERM_NAME, m_strTermName);
	DDX_Text(pDX, IDC_EDT_USER_DATA, m_strUserData);
	DDX_Text(pDX, IDC_EDT_WRITE_DATE, m_strWriteDate);
	DDX_Text(pDX, IDC_EDIT_WAIT_TIME, m_nWaitTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSdkClientDlg, CDialog)
	//{{AFX_MSG_MAP(CSdkClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_OPEN_TALK, OnOpenTalk)
	ON_BN_CLICKED(IDC_BTN_CLOSE_TALK, OnCloseTalk)
	ON_BN_CLICKED(IDC_BTN_SEND_485, OnBtnSend485)
	ON_BN_CLICKED(IDC_BTN_START_BROAD_VOICE, OnBtnStartBroadVoice)
	ON_BN_CLICKED(IDC_BTN_SETUP_BROAD_GROUP, OnBtnSetupBroadGroup)
	ON_BN_CLICKED(IDC_BTN_STOP_BROAD_VOICE, OnBtnStopBroadVoice)
	ON_BN_CLICKED(IDC_BTN_START_BROAD_MP3, OnBtnStartBroadMp3)
	ON_BN_CLICKED(IDC_BTN_BROAD_MP3_FILE, OnBtnBroadMp3File)
	ON_BN_CLICKED(IDC_BTN_STOP_BROAD_MP3, OnBtnStopBroadMp3)
	ON_BN_CLICKED(IDC_GET_LEVEL, OnGetLevel)
	ON_BN_CLICKED(IDC_SET_LEVEL, OnSetLevel)
	ON_BN_CLICKED(IDC_BTN_DISMISS_TALK, OnBtnDismissTalk)
	ON_BN_CLICKED(IDC_GET_DB_ALM, OnGetDbAlm)
	ON_BN_CLICKED(IDC_SET_DB_ALM, OnSetDbAlm)
	ON_BN_CLICKED(IDC_BTN_REQ_I_FRM, OnBtnReqIFrm)
	ON_BN_CLICKED(IDC_BTN_OPEN_ALM_OUT_1, OnBtnOpenAlmOut1)
	ON_BN_CLICKED(IDC_BTN_OPEN_ALM_OUT_2, OnBtnOpenAlmOut2)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_CBN_SELCHANGE(IDC_CBX_TERM_LIST, OnSelchangeCbxTermList)
	ON_BN_CLICKED(IDC_BTN_CHANGE_NAME, OnBtnChangeName)
	ON_BN_CLICKED(IDC_BTN_OPEN_TALK_SUB, OnBtnOpenTalkSub)
	ON_BN_CLICKED(IDC_BTN_CLOSE_TALK_SUB, OnBtnCloseTalkSub)
	ON_BN_CLICKED(IDC_BTN_REQ_I_FRM_SUB, OnBtnReqIFrmSub)
	ON_BN_CLICKED(IDC_BTN_GET_USER_DATA, OnBtnGetUserData)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_OPEN_MON, OnBtnOpenMon)
	ON_BN_CLICKED(IDC_BTN_CLOSE_MON, OnBtnCloseMon)
	ON_BN_CLICKED(IDC_BTN_PAUSE_MP3_BROAD, OnBtnPauseMp3Broad)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_CHANGE_WAIT_TIME, OnBtnChangeWaitTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdkClientDlg message handlers

BOOL CSdkClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	int nResult = TSDK_Init(OnTzlSdkCallback, TRUE, 0x12345678);
	CString strText;
	if(nResult == CERR_SUCCESS)
		strText.Format("SDK 初始化成功！");
	else
		strText.Format("SDK 初始化失败！错误代码:%d", nResult);
	m_InfoList.AddString(strText);

	LoadTermList();
	UpdateTermListUI();

	m_DbValSpin.SetRange(10, 99);
	m_DbValSpin.SetPos(70);

	m_AmpSlider.SetRange(0, 100);
	m_AmpSlider.SetPageSize(5);

	m_edtTermName.LimitText(32);

	if(m_nTermCnt > 0)
	{
		m_strTermName = m_TermList[0].TermName;
		UpdateData(FALSE);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSdkClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSdkClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSdkClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSdkClientDlg::OnOK() 
{

}

void CSdkClientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSdkClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	TSDK_DeInit();
}

CString CSdkClientDlg::GetExeDir()
{
	CString strExeDir = "";

	char aExeName[MAX_PATH];
	char aDriver[MAX_PATH];
	char aDir[MAX_PATH];

	DWORD dwRet = ::GetModuleFileName (AfxGetApp()->m_hInstance, aExeName, MAX_PATH);
	if(dwRet == 0)
		return strExeDir;

	::_splitpath (aExeName, aDriver, aDir, NULL, NULL);

	strExeDir = aDriver;
	strExeDir += aDir;

	return strExeDir;
}

void CSdkClientDlg::LoadTermList()
{
	CString strIniFile;
	CString strSection, strKey;

	strIniFile.Format("%s%s", GetExeDir(), INI_FILE);
	strSection = "TermRegister";

	strKey = "TermCnt";
	m_nTermCnt = GetPrivateProfileInt(strSection, strKey, 0, strIniFile);

	for(int i=0; i<m_nTermCnt; i++)
	{
		strKey.Format("t%04d_TermID", i);
		m_TermList[i].dwTermID = GetPrivateProfileInt(strSection, strKey, 0, strIniFile);

		strKey.Format("t%04d_TermMac1", i);
		int nMac1 = GetPrivateProfileInt(strSection, strKey, 0, strIniFile);
		strKey.Format("t%04d_TermMac2", i);
		int nMac2 = GetPrivateProfileInt(strSection, strKey, 0, strIniFile);
		memcpy(m_TermList[i].TermMac, &nMac1, 3);
		memcpy(m_TermList[i].TermMac+3, &nMac2, 3);

		strKey.Format("t%04d_TermIp", i);
		GetPrivateProfileString(strSection, strKey, "", m_TermList[i].TermIp, 16, strIniFile);

		strKey.Format("t%04d_TermName", i);
		GetPrivateProfileString(strSection, strKey, "", m_TermList[i].TermName, 32, strIniFile);
	}
}

void CSdkClientDlg::StoreTermList()
{
	CString strIniFile;
	CString strSection, strKey, strTmp;

	strIniFile.Format("%s%s", GetExeDir(), INI_FILE);
	strSection = "TermRegister";
	
	for(int i=0; i<m_nTermCnt; i++)
	{
		strKey.Format("t%04d_TermID", i);
		strTmp.Format("0x%X", m_TermList[i].dwTermID);
		WritePrivateProfileString(strSection, strKey, strTmp, strIniFile);

		LPBYTE p = m_TermList[i].TermMac;
		int nMac1 = p[0] | (p[1] << 8) | (p[2] << 16);
		int nMac2 = p[3] | (p[4] << 8) | (p[5] << 16);
		strKey.Format("t%04d_TermMac1", i);
		strTmp.Format("0x%06X", nMac1);
		WritePrivateProfileString(strSection, strKey, strTmp, strIniFile);
		strKey.Format("t%04d_TermMac2", i);
		strTmp.Format("0x%06X", nMac2);
		WritePrivateProfileString(strSection, strKey, strTmp, strIniFile);

		strKey.Format("t%04d_TermIp", i);
		WritePrivateProfileString(strSection, strKey, m_TermList[i].TermIp, strIniFile);

		strKey.Format("t%04d_TermName", i);
		WritePrivateProfileString(strSection, strKey, m_TermList[i].TermName, strIniFile);
	}
	
	strKey = "TermCnt";
	strTmp.Format("%d", m_nTermCnt);
	WritePrivateProfileString(strSection, strKey, strTmp, strIniFile);
}

void CSdkClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

static LPCSTR GetStateText(enSdkDevState eState)
{
	switch(eState)
	{
	case TSDK_DEV_STATE_OFFLINE:
		return "不在线";
	case TSDK_DEV_STATE_ONLINE:
		return "在线";
	case TSDK_DEV_STATE_PLAYING:
		return "播放音乐";
	case TSDK_DEV_STATE_TALKING:
		return "通话中";
	case TSDK_DEV_STATE_TALK_PLAY:
		return "通话中并播放音乐";
	}

	return "未知状态";
}

static LPCSTR GetTermType(enSdkDevType eTermType)
{
	switch(eTermType)
	{
	case TSDK_DEV_TYPE_TALK:
		return "[对讲广播终端]";
	case TSDK_DEV_TYPE_BROAD:
		return "[广播终端]";
	case TSDK_DEV_TYPE_MP3:
		return "[网络拾音器]";
	}

	return "[未知类型终端]";
}

int CALLBACK CSdkClientDlg::OnTzlSdkCallback(enSdkCbType eCbType, LPVOID pParam, DWORD dwSize, int usr_data)
{
	switch(eCbType)
	{
		case CB_Event_TermRegister:
		{
			ASSERT(dwSize == sizeof(TSdkEventTermRegister));
			TSdkEventTermRegister * pEventTermRegister = (TSdkEventTermRegister*)pParam;
			LPBYTE p = pEventTermRegister->TermMac;
			CString strText;
			strText.Format("===> %s 请求注册：终端ID:%X , MAC地址:%02X-%02X-%02X-%02X-%02X-%02X , IP地址:%s , 名称:%s",
				GetTermType(pEventTermRegister->eTermType), pEventTermRegister->dwTermID, p[0],p[1],p[2],p[3],p[4],p[5], pEventTermRegister->TermIp, pEventTermRegister->TermName);
			g_pMainWnd->m_InfoList.AddString(strText);

			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if(g_pMainWnd->m_TermList[i].dwTermID == pEventTermRegister->dwTermID)
					break;
			}

			if(i < g_pMainWnd->m_nTermCnt)
			{
				// 更新
				g_pMainWnd->m_TermList[i] = *pEventTermRegister;

				g_pMainWnd->StoreTermList();
				g_pMainWnd->UpdateTermListUI();
			}
			else
			{
				// 注册
				if(g_pMainWnd->m_nTermCnt >= MAX_TERM_COUNT)
					return -1; // 返回 0 代表注册成功，否则代表注册失败 !!!

				g_pMainWnd->m_TermList[i] = *pEventTermRegister;
				g_pMainWnd->m_TermState[i].eState = TSDK_DEV_STATE_OFFLINE;
				g_pMainWnd->m_nTermCnt++;

				g_pMainWnd->StoreTermList();
				g_pMainWnd->UpdateTermListUI();
			}
			
			return CERR_SUCCESS; // 返回 0 代表注册成功，否则代表注册失败 !!!
		}

		case CB_Event_TermConnect:
		{
			ASSERT(dwSize == sizeof(TSdkEventTermConnect));
			TSdkEventTermConnect * pEventTermConnect = (TSdkEventTermConnect*)pParam;
			LPBYTE p = pEventTermConnect->TermMac;
			CString strText;
			strText.Format(">>> %s 请求连接：终端ID:%X , %s视频 , MAC地址:%02X-%02X-%02X-%02X-%02X-%02X , IP地址:%s , 名称:%s",
				GetTermType(pEventTermConnect->eTermType), pEventTermConnect->dwTermID, (pEventTermConnect->bHasVideo ? "有" : "无"),
				p[0],p[1],p[2],p[3],p[4],p[5], pEventTermConnect->TermIp, pEventTermConnect->TermName);
			g_pMainWnd->m_InfoList.AddString(strText);

			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if( (g_pMainWnd->m_TermList[i].dwTermID == pEventTermConnect->dwTermID) &&
					memcmp(g_pMainWnd->m_TermList[i].TermMac, pEventTermConnect->TermMac, 6) == 0)
				{
					break;
				}
			}

			if(i < g_pMainWnd->m_nTermCnt)
			{
				// 已注册过
				BOOL bChangeIp = strcmp(g_pMainWnd->m_TermList[i].TermIp, pEventTermConnect->TermIp);
				if(bChangeIp)
					strcpy(g_pMainWnd->m_TermList[i].TermIp, pEventTermConnect->TermIp);

				BOOL bChangeName = strcmp(g_pMainWnd->m_TermList[i].TermName, pEventTermConnect->TermName);
				if(bChangeName)
					strcpy(g_pMainWnd->m_TermList[i].TermName, pEventTermConnect->TermName);

				g_pMainWnd->m_TermList[i].eTermType = pEventTermConnect->eTermType;
				g_pMainWnd->m_TermState[i].eState = TSDK_DEV_STATE_ONLINE;

				if(bChangeIp || bChangeName)
					g_pMainWnd->StoreTermList();

				g_pMainWnd->UpdateTermListUI();

				int nSel = g_pMainWnd->m_cbxTermList.GetCurSel();
				if(nSel == i)
				{
					g_pMainWnd->m_edtTermName.SetWindowText(g_pMainWnd->m_TermList[i].TermName);
					g_pMainWnd->m_strTermName = g_pMainWnd->m_TermList[i].TermName;
				}

				return CERR_SUCCESS; // 返回 0 代表允许连接，否则代表不允许 !!!
			}
			else
			{
				// 未注册
				return -1; // 返回 0 代表允许连接，否则代表不允许 !!!
			}
		}

		case CB_Event_TermCnnLost:
		{
			ASSERT(dwSize == sizeof(TSdkEventTermCnnLost));
			TSdkEventTermCnnLost * pEventTermCnnLost = (TSdkEventTermCnnLost*)pParam;
			CString strText;
			strText.Format("xxx 终端断开连接：终端ID:%X", pEventTermCnnLost->dwTermID);
			g_pMainWnd->m_InfoList.AddString(strText);

			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if(g_pMainWnd->m_TermList[i].dwTermID == pEventTermCnnLost->dwTermID)
				{
					g_pMainWnd->m_TermState[i].eState = TSDK_DEV_STATE_OFFLINE;
					break;
				}
			}

			g_pMainWnd->UpdateTermListUI();

			break;
		}
		
		case CB_Asw_OpenTermAudVid:
		{
			ASSERT(dwSize == sizeof(TSdkAswOpenTermAudVid));
			TSdkAswOpenTermAudVid * pAswOpenTermAudVid = (TSdkAswOpenTermAudVid*)pParam;
			CString strText;
			if(pAswOpenTermAudVid->nResult == CERR_SUCCESS)
				strText.Format("打开终端音频成功！终端ID:%X ", pAswOpenTermAudVid->dwTermID);
			else
				strText.Format("打开终端音频失败！终端ID:%X ，错误代码:%d", pAswOpenTermAudVid->dwTermID, pAswOpenTermAudVid->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_CloseTermAudVid:
		{
			ASSERT(dwSize == sizeof(TSdkAswCloseTermAudVid));
			TSdkAswCloseTermAudVid * pAswCloseTermAudVid = (TSdkAswCloseTermAudVid*)pParam;
			CString strText;
			if(pAswCloseTermAudVid->nResult == CERR_SUCCESS)
				strText.Format("关闭终端音频成功！终端ID:%X ", pAswCloseTermAudVid->dwTermID);
			else
				strText.Format("关闭终端音频失败！终端ID:%X ，错误代码:%d", pAswCloseTermAudVid->dwTermID, pAswCloseTermAudVid->nResult);
			
			break;
		}

		case CB_Asw_OpenTermTalk:
		{
			ASSERT(dwSize == sizeof(TSdkAswOpenTermTalk));
			TSdkAswOpenTermTalk * pAswOpenTermTalk = (TSdkAswOpenTermTalk*)pParam;
			CString strText;
			if(pAswOpenTermTalk->nResult == CERR_SUCCESS)
				strText.Format("打开终端对讲成功！终端ID:%X ", pAswOpenTermTalk->dwTermID);
			else
				strText.Format("打开终端对讲失败！终端ID:%X ，错误代码:%d", pAswOpenTermTalk->dwTermID, pAswOpenTermTalk->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_CloseTermTalk:
		{
			ASSERT(dwSize == sizeof(TSdkAswCloseTermTalk));
			TSdkAswCloseTermTalk * pAswCloseTermTalk = (TSdkAswCloseTermTalk*)pParam;
			CString strText;
			if(pAswCloseTermTalk->nResult == CERR_SUCCESS)
				strText.Format("关闭终端对讲成功！终端ID:%X ", pAswCloseTermTalk->dwTermID);
			else
				strText.Format("关闭终端对讲失败！终端ID:%X ，错误代码:%d", pAswCloseTermTalk->dwTermID, pAswCloseTermTalk->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_GetAudLevel:
		{
			ASSERT(dwSize == sizeof(TSdkAswGetAudLevel));
			TSdkAswGetAudLevel * pAswGetAudLevel = (TSdkAswGetAudLevel*)pParam;
			g_pMainWnd->m_nCapLevel = pAswGetAudLevel->Level.eCapLevel;
			g_pMainWnd->m_nAmpLevel = pAswGetAudLevel->Level.eAmpLevel;
			g_pMainWnd->m_nCapTalkLevel = pAswGetAudLevel->Level.eCapTalkLevel;
			((CComboBox*)g_pMainWnd->GetDlgItem(IDC_CMB_CAP))->SetCurSel(g_pMainWnd->m_nCapLevel);
			((CComboBox*)g_pMainWnd->GetDlgItem(IDC_CMB_AMP))->SetCurSel(g_pMainWnd->m_nAmpLevel);
			((CComboBox*)g_pMainWnd->GetDlgItem(IDC_CMB_CAP_TALK))->SetCurSel(g_pMainWnd->m_nCapTalkLevel);
			g_pMainWnd->m_AmpSlider.SetPos(pAswGetAudLevel->nAmpValue);
			CString strText;
			strText.Format("当前:%d", pAswGetAudLevel->nAmpValue);
			g_pMainWnd->GetDlgItem(IDC_STATIC_CUR)->SetWindowText(strText);
			break;
		}

		case CB_Asw_SetAudLevel:
		{
			ASSERT(dwSize == sizeof(TSdkAswSetAudLevel));
			TSdkAswSetAudLevel * pAswSetAudLevel = (TSdkAswSetAudLevel*)pParam;
			CString strText;
			if(pAswSetAudLevel->nResult == CERR_SUCCESS)
			{
				strText.Format("设置终端音量成功！终端ID:%X ", pAswSetAudLevel->dwTermID);

				if(pAswSetAudLevel->bIsEx)
				{
					g_pMainWnd->m_nAmpLevel = pAswSetAudLevel->eAmpLevel;
					((CComboBox*)g_pMainWnd->GetDlgItem(IDC_CMB_AMP))->SetCurSel(g_pMainWnd->m_nAmpLevel);
				}
				else
				{
					g_pMainWnd->m_AmpSlider.SetPos(pAswSetAudLevel->nAmpValue);
					CString strText;
					strText.Format("当前:%d", pAswSetAudLevel->nAmpValue);
					g_pMainWnd->GetDlgItem(IDC_STATIC_CUR)->SetWindowText(strText);
				}
			}
			else
			{
				strText.Format("设置终端音量失败！终端ID:%X ，错误代码:%d", pAswSetAudLevel->dwTermID, pAswSetAudLevel->nResult);
			}
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_DismissTerm:
		{
			ASSERT(dwSize == sizeof(TSdkAswDismissTerm));
			TSdkAswDismissTerm * pAswDismissTerm = (TSdkAswDismissTerm*)pParam;
			CString strText;
			if(pAswDismissTerm->nResult == CERR_SUCCESS)
			{
				g_pMainWnd->m_stcTips.SetWindowText("呼叫提示：");
				strText.Format("解除呼叫成功！终端ID:%X ", pAswDismissTerm->dwTermID);
			}
			else
			{
				strText.Format("解除呼叫失败！终端ID:%X ，错误代码:%d", pAswDismissTerm->dwTermID, pAswDismissTerm->nResult);
			}
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_GetDbAlmCfg:
		{
			ASSERT(dwSize == sizeof(TSdkAswGetDbAlmCfg));
			TSdkAswGetDbAlmCfg * pAswGetDbAlmCfg = (TSdkAswGetDbAlmCfg*)pParam;
			g_pMainWnd->m_bDbAlarm = pAswGetDbAlmCfg->Cfg.bDbAlmEnable;
			((CButton*)g_pMainWnd->GetDlgItem(IDC_CHK_DB_ALARM))->SetCheck(pAswGetDbAlmCfg->Cfg.bDbAlmEnable);
			g_pMainWnd->m_DbValSpin.SetPos(pAswGetDbAlmCfg->Cfg.nDbAlmVal);
			break;
		}

		case CB_Asw_SetDbAlmCfg:
		{
			ASSERT(dwSize == sizeof(TSdkAswSetDbAlmCfg));
			TSdkAswSetDbAlmCfg * pAswSetDbAlmCfg = (TSdkAswSetDbAlmCfg*)pParam;
			CString strText;
			if(pAswSetDbAlmCfg->nResult == CERR_SUCCESS)
				strText.Format("修改终端配置成功！终端ID:%X ", pAswSetDbAlmCfg->dwTermID);
			else
				strText.Format("修改终端配置失败！终端ID:%X ，错误代码:%d", pAswSetDbAlmCfg->dwTermID, pAswSetDbAlmCfg->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Post_TermSos:
		{
			ASSERT(dwSize == sizeof(TSdkPostTermSos));
			TSdkPostTermSos * pPostTermSos = (TSdkPostTermSos*)pParam;
			CString strText;
			if(pPostTermSos->nSosType == 1)
				strText.Format("终端紧急呼叫 !!!!!! \n终端ID:%X", pPostTermSos->dwTermID);
			else
				strText.Format("终端咨询呼叫 !!!!!! \n终端ID:%X", pPostTermSos->dwTermID);
			g_pMainWnd->m_InfoList.AddString(strText);
			g_pMainWnd->m_stcTips.SetWindowText(strText);

			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if(g_pMainWnd->m_TermList[i].dwTermID == pPostTermSos->dwTermID)
				{
					g_pMainWnd->m_cbxTermList.SetCurSel(i);
					break;
				}
			}			

			break;
		}

		case CB_Post_TermState:
		{
			ASSERT(dwSize == sizeof(TSdkPostTermState));
			TSdkPostTermState * pPostTermState = (TSdkPostTermState*)pParam;
			
			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if(g_pMainWnd->m_TermList[i].dwTermID == pPostTermState->dwTermID)
				{
					g_pMainWnd->m_TermState[i].eState = pPostTermState->eTermState;
					g_pMainWnd->m_TermState[i].AlmInState = pPostTermState->AlmInState;
					g_pMainWnd->m_TermState[i].AlmOutState = pPostTermState->AlmOutState;
					break;
				}
			}

			CString strText;
			strText.Format("终端ID:%X 终端状态:%s , IO报警 == 输入1:%s , 输入2:%s , 输出1:%s , 输出2:%s", pPostTermState->dwTermID, GetStateText(pPostTermState->eTermState),
				AMSG(pPostTermState->AlmInState & 0x1), AMSG((pPostTermState->AlmInState >> 1) & 0x1),
				AMSG(pPostTermState->AlmOutState & 0x1), AMSG((pPostTermState->AlmOutState >> 1) & 0x1));
			g_pMainWnd->m_InfoList.AddString(strText);

			int nSel = g_pMainWnd->m_cbxTermList.GetCurSel();
			if(nSel >= 0 && g_pMainWnd->m_TermList[nSel].dwTermID == pPostTermState->dwTermID)
			{
				if(pPostTermState->AlmOutState & 0x1)
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("关闭报警输出1");
				else
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("打开报警输出1");

				if((pPostTermState->AlmOutState >> 1) & 0x1)
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("关闭报警输出2");
				else
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("打开报警输出2");
			}

			break;
		}

		case CB_Post_485PipeData:
		{
			ASSERT(dwSize == sizeof(TSdkPost485PipeData));
			TSdkPost485PipeData * pPost485PipeData = (TSdkPost485PipeData*)pParam;
			CString strText;
			strText.Format("===> 收到485数据：终端ID:%X , 长度:%d , 内容:%s",
				pPost485PipeData->dwTermID, pPost485PipeData->nFrmLen, pPost485PipeData->FrmData);
			g_pMainWnd->m_InfoList.AddString(strText);

			g_pMainWnd->m_str485PipeData += pPost485PipeData->FrmData;
			g_pMainWnd->m_str485PipeData += "\r\n";
			g_pMainWnd->m_edt485PipeData.SetWindowText(g_pMainWnd->m_str485PipeData);
			break;
		}

		case CB_Post_Mp3PlayFinish:
		{
			ASSERT(dwSize == sizeof(TSdkPostMp3PlayFinish));
			TSdkPostMp3PlayFinish * pPostMp3PlayFinish = (TSdkPostMp3PlayFinish*)pParam;
			CString strText;
			if(pPostMp3PlayFinish->nResult == CERR_SUCCESS)
				strText.Format("分组%d : MP3文件播放：正常结束。。", pPostMp3PlayFinish->nBroadNum);
			else
				strText.Format("分组%d : MP3文件播放：播放出错，错误代码:%d", pPostMp3PlayFinish->nBroadNum, pPostMp3PlayFinish->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Post_AlmNotify:
		{
			ASSERT(dwSize == sizeof(TSdkPostAlmNotify));
			TSdkPostAlmNotify * pPostAlmNotify = (TSdkPostAlmNotify*)pParam;
			CString strText;
			if((pPostAlmNotify->eAlmType & SDK_ALMTYPE_DB_ALARM) == SDK_ALMTYPE_DB_ALARM)
				strText.Format("终端喧哗报警 !!!    终端ID:%X", pPostAlmNotify->dwTermID);
			else
				strText.Format("终端报警类型: 0x%X  !!!    终端ID:%X", pPostAlmNotify->eAlmType);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Data_TermAudio:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermAudio));
			TSdkDataTermAudio * pDataTermAudio = (TSdkDataTermAudio*)pParam;
//			TRACE("== 终端音频 ID:%X ：大小 %d 字节\n", pDataTermAudio->dwTermID,pDataTermAudio->nDataSize);
			TSDK_Player_AudVidAdd(1, TSDK_AUDIO, pDataTermAudio->pAudData, pDataTermAudio->nDataSize);
			break;
		}

		case CB_Data_TermVideo:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermVideo));
			TSdkDataTermVideo * pDataTermVideo = (TSdkDataTermVideo*)pParam;
			if(pDataTermVideo->bIsKeyFrm)
				TRACE("== 终端I帧视频 ID:%X ：大小 %d 字节\n", pDataTermVideo->dwTermID,pDataTermVideo->nFrmSize);
//			else
//				TRACE("== 终端P帧视频 ID:%X ：大小 %d 字节\n", pDataTermVideo->dwTermID,pDataTermVideo->nFrmSize);

			TSDK_Player_AudVidAdd(1, TSDK_VIDEO, pDataTermVideo->pVidFrm, pDataTermVideo->nFrmSize);
			break;
		}

		case CB_Data_PcMicAudio:
		{
			ASSERT(dwSize == sizeof(TSdkDataPcMicAudio));
			TSdkDataPcMicAudio * pDataPcAudio = (TSdkDataPcMicAudio*)pParam;
//			TRACE("== 电脑麦克风音频：大小 %d 字节\n", pDataPcAudio->nDataSize);

			if(g_pMainWnd->m_nTalkTermIdx != -1)
			{
				TSDK_Send_TalkAudio(g_pMainWnd->m_TermList[g_pMainWnd->m_nTalkTermIdx].dwTermID,
					pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);
			}

			if(g_pMainWnd->m_bIsBroadingVoice)
			{
				TSDK_BroadPcmVoice(g_MyGroupNum, pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);

				/****************************
				// 如果有多个分组
				TSDK_BroadPcmVoice(xx, pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);
				TSDK_BroadPcmVoice(xxx, pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);
				.......................
				.......................
				*****************************/
			}
			break;
		}

		case CB_Data_BypassAudio:
		{
			ASSERT(dwSize == sizeof(TSdkDataBypassAudio));
			TSdkDataBypassAudio * pDataBypassAudio = (TSdkDataBypassAudio*)pParam;
//			TRACE("== 旁路音频 ID:%X ：大小 %d 字节\n", pDataBypassAudio->dwTermID,pDataBypassAudio->nDataSize);
			TSDK_Player_PcmAdd(1, pDataBypassAudio->pPcmData, pDataBypassAudio->nDataSize);
			break;
		}

		case CB_Data_TermMp3L:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermMp3L));
			TSdkDataTermMp3L * pDataTermMp3L = (TSdkDataTermMp3L*)pParam;
//			TRACE("== MP3音频 ID:%X ：大小 %d 字节\n", pDataTermMp3L->dwTermID,pDataTermMp3L->nDataSize);
			if(g_pMp3File1)
				fwrite(pDataTermMp3L->pMp3Data, 1, pDataTermMp3L->nDataSize, g_pMp3File1);
			break;
		}

		case CB_Data_TermMp3R:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermMp3R));
			TSdkDataTermMp3R * pDataTermMp3R = (TSdkDataTermMp3R*)pParam;
//			TRACE("== MP3音频 ID:%X ：大小 %d 字节\n", pDataTermMp3R->dwTermID,pDataTermMp3R->nDataSize);
			if(g_pMp3File2)
				fwrite(pDataTermMp3R->pMp3Data, 1, pDataTermMp3R->nDataSize, g_pMp3File2);
			break;
		}

		default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	return CERR_SUCCESS;
}

void CSdkClientDlg::OnOpenTalk() 
{
	UpdateData(TRUE);

	CString strText;

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("操作繁忙！", "提示", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);

		strText.Format("%s%s", GetExeDir(), "1.mp3");
		g_pMp3File1 = fopen(strText, "wb");
		strText.Format("%s%s", GetExeDir(), "2.mp3");
		g_pMp3File2 = fopen(strText, "wb");
	}
	else
	{
		TSDK_Player_AudVidOpen(1, m_stcTerm.m_hWnd);
		TSDK_Player_PcmOpen(1);

		// 当终端与其他人进行对讲时，调用TSDK_Req_OpenTermAudVid自动开启音频旁听模式：获取对讲双方的音频
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		// 注意：正常情况打开终端对讲前，应该先检查终端是否处于对讲状态(被别人占用)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// 如果被占用就不应该执行下面操作 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("呼叫提示：");
	}
}

void CSdkClientDlg::OnCloseTalk() 
{
	UpdateData(TRUE);

	CString strText;

	if(m_nTalkTermIdx == -1)
		return;

	if(m_TermList[m_nTalkTermIdx].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("关闭网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);

		if(g_pMp3File1)
		{
			fclose(g_pMp3File1);
			g_pMp3File1 = NULL;
		}
		if(g_pMp3File2)
		{
			fclose(g_pMp3File2);
			g_pMp3File2 = NULL;
		}
	}
	else
	{
		TSDK_Player_PcMicClose();

		int nResult = TSDK_Req_CloseTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS && nResult != CERR_INVALID_DEV_STATE)
		{
			strText.Format("请求关闭终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////


		nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求关闭终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_AudVidClose(1);
		TSDK_Player_PcmClose(1);
	}

	m_nTalkTermIdx = -1;

	m_stcTerm.Invalidate(TRUE);
}

void CSdkClientDlg::OnBtnSend485() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_strSend485Data.GetLength() < 1)
	{
		MessageBox("请输入数据！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Send_485PipeData(m_TermList[nSel].dwTermID, (LPBYTE)(LPCSTR)m_strSend485Data, m_strSend485Data.GetLength());
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("发送485数据失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::UpdateTermListUI()
{
	int nCurSel = m_cbxTermList.GetCurSel();
	m_cbxTermList.ResetContent();

	CString strText;
	for(int i=0; i<m_nTermCnt; i++)
	{
		strText.Format("ID:[%X]  名称:[%s]  IP:[%s]  状态:[%s]", m_TermList[i].dwTermID, m_TermList[i].TermName,
			m_TermList[i].TermIp, m_TermState[i].eState==TSDK_DEV_STATE_OFFLINE ? "不在线":"在线");

		m_cbxTermList.AddString(strText);
	}

	if(m_nTermCnt > 0)
	{
		if(nCurSel >= 0 && nCurSel < m_nTermCnt)
			m_cbxTermList.SetCurSel(nCurSel);
		else
			m_cbxTermList.SetCurSel(0);
	}
}

void CSdkClientDlg::OnBtnSetupBroadGroup() 
{
	// 最多可以创建60(MAX_BROAD_GROUP)个广播分组，
	// 这里演示: 简单的把所有分机配置为一个广播分组，分组号为：6
	// SDK使用者，可以根据自己需要，把各个分机配置到最多60个分组中，分组号为1～60 。
	// 每个广播分组最多支持100(MAX_BROAD_TERM)个终端设备
	// 如果要取消广播分组中的某些分机，只需要重新设置TermList[MAX_BROAD_TERM]，
	// 然后调用TSDK_SetupGroup()，就可以取消了。

	TSdkGroupTermList GroupTermList_xxx;
	memset(&GroupTermList_xxx, 0, sizeof(GroupTermList_xxx));
	for(int tidx=0; tidx<m_nTermCnt; tidx++)
	{
		GroupTermList_xxx.TermList[tidx] = m_TermList[tidx].dwTermID;
	}
	
	int nResult = TSDK_SetupGroup(g_MyGroupNum, &GroupTermList_xxx);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("创建%d号分区失败！错误代码:%d", g_MyGroupNum, nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnStartBroadVoice() 
{
	int nResult = TSDK_StartBroadVoice(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("开启广播喊话失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}

	// 开启采集电脑端的麦克风
	if(m_bIsBroadingVoice == FALSE)
	{
		TSDK_Player_PcMicOpen();
		m_bIsBroadingVoice = TRUE;
	}
}

void CSdkClientDlg::OnBtnStopBroadVoice() 
{
	// 关闭采集电脑端的麦克风
	if(m_bIsBroadingVoice)
	{
		m_bIsBroadingVoice = FALSE;
		TSDK_Player_PcMicClose();
	}

	int nResult = TSDK_StopBroadVoice(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("关闭广播喊话失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnStartBroadMp3() 
{
	int nResult = TSDK_StartBroadMp3(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("开启广播MP3失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnBroadMp3File() 
{
	CString strFilter;
	strFilter.Format("mp3文件 (*.mp3)|*.mp3||");
	CFileDialog dlg( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT| OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES, strFilter);
	CString strMapDir;
	strMapDir.Format("%s", GetExeDir());
	dlg.m_ofn.lpstrInitialDir = strMapDir;
	if( dlg.DoModal() != IDOK )
		return;

	CString strText;
	CString strMp3FilePath = dlg.GetPathName();
	
	int nResult = TSDK_BroadMp3File(g_MyGroupNum, strMp3FilePath);
	if(nResult != CERR_SUCCESS)
	{
		strText.Format("广播MP3文件失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}

	g_bPauseMp3Broad = FALSE;
	GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("暂停MP3广播");
}

void CSdkClientDlg::OnBtnStopBroadMp3() 
{
	int nResult = TSDK_StopBroadMp3(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("关闭广播MP3失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnGetLevel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_GetAudLevel(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("获取音量级别失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnSetLevel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_nCapLevel == -1 || m_nAmpLevel == -1 || m_nCapTalkLevel == -1)
	{
		MessageBox("请选择音量级别！", "提示", MB_ICONWARNING);
		return;
	}

	TSdkAudioLevel Level;
	Level.eCapLevel = (enSdkAudCapLevel)m_nCapLevel;
	Level.eAmpLevel = (enSdkAudAmpLevel)m_nAmpLevel;
	Level.eCapTalkLevel = (enSdkAudCapTalkLevel)m_nCapTalkLevel;

	int nResult = TSDK_Req_SetAudLevel(m_TermList[nSel].dwTermID, &Level);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("设置音量级别失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnDismissTalk() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_DismissTerm(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求解除呼叫失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnGetDbAlm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_GetDbAlmCfg(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("获取喧哗报警配置失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnSetDbAlm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	TSdkDbAlmCfg Cfg;
	Cfg.bDbAlmEnable = m_bDbAlarm;
	Cfg.nDbAlmVal = m_DbValSpin.GetPos();

	int nResult = TSDK_Req_SetDbAlmCfg(m_TermList[nSel].dwTermID, &Cfg);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("设置喧哗报警配置失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnReqIFrm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_IFrame(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求I帧失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnOpenAlmOut1() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetAlmOut(m_TermList[nSel].dwTermID, 0x1, (m_TermState[nSel].AlmOutState & 0x1) ? 0x0 : 0x1);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求报警输出1失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnOpenAlmOut2() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetAlmOut(m_TermList[nSel].dwTermID, 0x2, (m_TermState[nSel].AlmOutState & 0x2) ? 0x0 : 0x2);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求报警输出2失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnClear() 
{
	m_InfoList.ResetContent();
}

void CSdkClientDlg::OnSelchangeCbxTermList() 
{
	int nSel = m_cbxTermList.GetCurSel();
	if(nSel >= 0)
	{
		if(m_TermState[nSel].AlmOutState & 0x1)
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("关闭报警输出1");
		else
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("打开报警输出1");

		if((m_TermState[nSel].AlmOutState >> 1) & 0x1)
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("关闭报警输出2");
		else
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("打开报警输出2");

		m_strTermName = m_TermList[nSel].TermName;

		UpdateData(FALSE);
	}
}

void CSdkClientDlg::OnBtnChangeName() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetTermName(m_TermList[nSel].dwTermID, m_strTermName);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求修改终端名称失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}

	strcpy(m_TermList[nSel].TermName, m_strTermName);
	StoreTermList();
	UpdateTermListUI();
}

void CSdkClientDlg::OnBtnOpenTalkSub() 
{
	UpdateData(TRUE);

	CString strText;

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("操作繁忙！", "提示", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);

		strText.Format("%s%s", GetExeDir(), "1.mp3");
		g_pMp3File1 = fopen(strText, "wb");
		strText.Format("%s%s", GetExeDir(), "2.mp3");
		g_pMp3File2 = fopen(strText, "wb");
	}
	else
	{
		TSDK_Player_AudVidOpen(1, m_stcTerm.m_hWnd);
		TSDK_Player_PcmOpen(1);

		// 当终端与其他人进行对讲时，调用TSDK_Req_OpenTermAudVid_Sub自动开启音频旁听模式：获取对讲双方的音频
		int nResult = TSDK_Req_OpenTermAudVid_Sub(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		// 注意：正常情况打开终端对讲前，应该先检查终端是否处于对讲状态(被别人占用)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// 如果被占用就不应该执行下面操作 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("呼叫提示：");
	}
}

void CSdkClientDlg::OnBtnCloseTalkSub() 
{
	UpdateData(TRUE);

	CString strText;

	if(m_nTalkTermIdx == -1)
		return;

	if(m_TermList[m_nTalkTermIdx].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("关闭网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);

		if(g_pMp3File1)
		{
			fclose(g_pMp3File1);
			g_pMp3File1 = NULL;
		}
		if(g_pMp3File2)
		{
			fclose(g_pMp3File2);
			g_pMp3File2 = NULL;
		}
	}
	else
	{
		TSDK_Player_PcMicClose();

		int nResult = TSDK_Req_CloseTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS && nResult != CERR_INVALID_DEV_STATE)
		{
			strText.Format("请求关闭终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////


		nResult = TSDK_Req_CloseTermAudVid_Sub(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求关闭终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_AudVidClose(1);
		TSDK_Player_PcmClose(1);
	}

	m_nTalkTermIdx = -1;

	m_stcTerm.Invalidate(TRUE);
}

void CSdkClientDlg::OnBtnReqIFrmSub() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_IFrame_Sub(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求I帧失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnGetUserData() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	TSdkTermUserData Data;
	int nResult = TSDK_GetTermUserData(m_TermList[nSel].dwTermID, &Data);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("获取客户数据失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}

	m_strUserData = Data.UserData;
	m_strWriteDate = Data.WriteDate;

	UpdateData(FALSE);
}

void CSdkClientDlg::OnBtnAdd() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	TSdkGroupTermList gts;
	memset(&gts, 0, sizeof(TSdkGroupTermList));
	gts.TermList[0] = m_TermList[nSel].dwTermID;
	int nResult = TSDK_GroupAddTerm(g_MyGroupNum, &gts);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("添加终端[%s]到广播分组%d失败！错误代码:%d", m_TermList[nSel].TermName, g_MyGroupNum, nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("操作成功！终端[%s]已添加到广播分组%d", m_TermList[nSel].TermName, g_MyGroupNum);
		MessageBox(strText, "提示", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnDel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	TSdkGroupTermList gts;
	memset(&gts, 0, sizeof(TSdkGroupTermList));
	gts.TermList[0] = m_TermList[nSel].dwTermID;
	int nResult = TSDK_GroupDelTerm(g_MyGroupNum, &gts);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("从广播分组%d删除终端[%s]失败！错误代码:%d", g_MyGroupNum, m_TermList[nSel].TermName, nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("操作成功！终端[%s]已从广播分组%d中删除", m_TermList[nSel].TermName, g_MyGroupNum);
		MessageBox(strText, "提示", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnGet() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_GetTermGroupNum(m_TermList[nSel].dwTermID);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("获取终端广播分组号失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("终端 [%s] 广播分组号 : %d", m_TermList[nSel].TermName, nResult);
		MessageBox(strText, "提示", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnOpenMon() 
{
	UpdateData(TRUE);

	CString strText;

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("操作繁忙！", "提示", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, TRUE);

		strText.Format("%s%s", GetExeDir(), "1.mp3");
		g_pMp3File1 = fopen(strText, "wb");
		strText.Format("%s%s", GetExeDir(), "2.mp3");
		g_pMp3File2 = fopen(strText, "wb");
	}
	else
	{
		TSDK_Player_AudVidOpen(1, m_stcTerm.m_hWnd);
		TSDK_Player_PcmOpen(1);

		// 当终端与其他人进行对讲时，调用TSDK_Req_OpenTermAudVid自动开启音频旁听模式：获取对讲双方的音频
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
		// 注意：正常情况打开终端对讲前，应该先检查终端是否处于对讲状态(被别人占用)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// 如果被占用就不应该执行下面操作 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("呼叫提示：");
*/
	}
}

void CSdkClientDlg::OnBtnCloseMon() 
{
	UpdateData(TRUE);

	CString strText;

	if(m_nTalkTermIdx == -1)
		return;

	if(m_TermList[m_nTalkTermIdx].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("关闭网络拾音器MP3流失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Req_PreviewMp3L(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);
		TSDK_Req_PreviewMp3R(m_TermList[m_nTalkTermIdx].dwTermID, FALSE);

		if(g_pMp3File1)
		{
			fclose(g_pMp3File1);
			g_pMp3File1 = NULL;
		}
		if(g_pMp3File2)
		{
			fclose(g_pMp3File2);
			g_pMp3File2 = NULL;
		}
	}
	else
	{
/*		TSDK_Player_PcMicClose();

		int nResult = TSDK_Req_CloseTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS && nResult != CERR_INVALID_DEV_STATE)
		{
			strText.Format("请求关闭终端对讲失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		int nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("请求关闭终端音视频失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}

		TSDK_Player_AudVidClose(1);
		TSDK_Player_PcmClose(1);
	}

	m_nTalkTermIdx = -1;

	m_stcTerm.Invalidate(TRUE);
}

void CSdkClientDlg::OnBtnPauseMp3Broad() 
{
	CString strText;
	
	int nResult = TSDK_PauseMp3Broad(g_MyGroupNum, !g_bPauseMp3Broad);
	if(nResult != CERR_SUCCESS)
	{
		strText.Format("暂停MP3广播失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
		return;
	}

	g_bPauseMp3Broad = !g_bPauseMp3Broad;

	if(g_bPauseMp3Broad)
		GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("继续MP3广播");
	else
		GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("暂停MP3广播");
}

void CSdkClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	UpdateData(TRUE);

	int nCurPos = m_AmpSlider.GetPos();

	CString strText;
	strText.Format("当前:%d", nCurPos);
	GetDlgItem(IDC_STATIC_CUR)->SetWindowText(strText);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
	}
	else if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
	}

	else if(m_nCapLevel == -1 || m_nAmpLevel == -1 || m_nCapTalkLevel == -1)
	{
		MessageBox("请选择音量级别！", "提示", MB_ICONWARNING);
	}
	else
	{
		TSdkAudioLevelEx Level;
		Level.eCapLevel = (enSdkAudCapLevel)m_nCapLevel;
		Level.nAmpValue = nCurPos;
		Level.eCapTalkLevel = (enSdkAudCapTalkLevel)m_nCapTalkLevel;

		int nResult = TSDK_Req_SetAudLevelEx(m_TermList[nSel].dwTermID, &Level);
		if(nResult != CERR_SUCCESS)
		{
			CString strText;
			if(nResult == CERR_DATA_NOT_SYNC)
				strText.Format("设置音量级别失败！错误信息: 数据未同步，请先获取再进行设置");
			else if(nResult == CERR_INVALID_BIOS_VERSION)
				strText.Format("设置音量级别失败！错误信息: 分机BIOS版本不正确，请先升级");
			else
				strText.Format("设置音量级别失败！错误代码:%d", nResult);
			MessageBox(strText, "错误", MB_ICONERROR);
		}
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSdkClientDlg::OnBtnChangeWaitTime() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("请选择终端设备！", "提示", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("设备不在线！", "提示", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetWaitTime(m_TermList[nSel].dwTermID, m_nWaitTime);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("请求修改终端名称失败！错误代码:%d", nResult);
		MessageBox(strText, "错误", MB_ICONERROR);
	}
	else
	{
		MessageBox("修改成功", "提示", MB_ICONINFORMATION);
	}
}
