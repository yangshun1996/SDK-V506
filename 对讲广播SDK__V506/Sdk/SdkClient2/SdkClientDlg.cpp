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
#define AMSG(x)          ((x) ? "��" : "��")

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
		strText.Format("SDK ��ʼ���ɹ���");
	else
		strText.Format("SDK ��ʼ��ʧ�ܣ��������:%d", nResult);
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
		return "������";
	case TSDK_DEV_STATE_ONLINE:
		return "����";
	case TSDK_DEV_STATE_PLAYING:
		return "��������";
	case TSDK_DEV_STATE_TALKING:
		return "ͨ����";
	case TSDK_DEV_STATE_TALK_PLAY:
		return "ͨ���в���������";
	}

	return "δ֪״̬";
}

static LPCSTR GetTermType(enSdkDevType eTermType)
{
	switch(eTermType)
	{
	case TSDK_DEV_TYPE_TALK:
		return "[�Խ��㲥�ն�]";
	case TSDK_DEV_TYPE_BROAD:
		return "[�㲥�ն�]";
	case TSDK_DEV_TYPE_MP3:
		return "[����ʰ����]";
	}

	return "[δ֪�����ն�]";
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
			strText.Format("===> %s ����ע�᣺�ն�ID:%X , MAC��ַ:%02X-%02X-%02X-%02X-%02X-%02X , IP��ַ:%s , ����:%s",
				GetTermType(pEventTermRegister->eTermType), pEventTermRegister->dwTermID, p[0],p[1],p[2],p[3],p[4],p[5], pEventTermRegister->TermIp, pEventTermRegister->TermName);
			g_pMainWnd->m_InfoList.AddString(strText);

			for(int i=0; i<g_pMainWnd->m_nTermCnt; i++)
			{
				if(g_pMainWnd->m_TermList[i].dwTermID == pEventTermRegister->dwTermID)
					break;
			}

			if(i < g_pMainWnd->m_nTermCnt)
			{
				// ����
				g_pMainWnd->m_TermList[i] = *pEventTermRegister;

				g_pMainWnd->StoreTermList();
				g_pMainWnd->UpdateTermListUI();
			}
			else
			{
				// ע��
				if(g_pMainWnd->m_nTermCnt >= MAX_TERM_COUNT)
					return -1; // ���� 0 ����ע��ɹ����������ע��ʧ�� !!!

				g_pMainWnd->m_TermList[i] = *pEventTermRegister;
				g_pMainWnd->m_TermState[i].eState = TSDK_DEV_STATE_OFFLINE;
				g_pMainWnd->m_nTermCnt++;

				g_pMainWnd->StoreTermList();
				g_pMainWnd->UpdateTermListUI();
			}
			
			return CERR_SUCCESS; // ���� 0 ����ע��ɹ����������ע��ʧ�� !!!
		}

		case CB_Event_TermConnect:
		{
			ASSERT(dwSize == sizeof(TSdkEventTermConnect));
			TSdkEventTermConnect * pEventTermConnect = (TSdkEventTermConnect*)pParam;
			LPBYTE p = pEventTermConnect->TermMac;
			CString strText;
			strText.Format(">>> %s �������ӣ��ն�ID:%X , %s��Ƶ , MAC��ַ:%02X-%02X-%02X-%02X-%02X-%02X , IP��ַ:%s , ����:%s",
				GetTermType(pEventTermConnect->eTermType), pEventTermConnect->dwTermID, (pEventTermConnect->bHasVideo ? "��" : "��"),
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
				// ��ע���
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

				return CERR_SUCCESS; // ���� 0 �����������ӣ������������ !!!
			}
			else
			{
				// δע��
				return -1; // ���� 0 �����������ӣ������������ !!!
			}
		}

		case CB_Event_TermCnnLost:
		{
			ASSERT(dwSize == sizeof(TSdkEventTermCnnLost));
			TSdkEventTermCnnLost * pEventTermCnnLost = (TSdkEventTermCnnLost*)pParam;
			CString strText;
			strText.Format("xxx �ն˶Ͽ����ӣ��ն�ID:%X", pEventTermCnnLost->dwTermID);
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
				strText.Format("���ն���Ƶ�ɹ����ն�ID:%X ", pAswOpenTermAudVid->dwTermID);
			else
				strText.Format("���ն���Ƶʧ�ܣ��ն�ID:%X ���������:%d", pAswOpenTermAudVid->dwTermID, pAswOpenTermAudVid->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_CloseTermAudVid:
		{
			ASSERT(dwSize == sizeof(TSdkAswCloseTermAudVid));
			TSdkAswCloseTermAudVid * pAswCloseTermAudVid = (TSdkAswCloseTermAudVid*)pParam;
			CString strText;
			if(pAswCloseTermAudVid->nResult == CERR_SUCCESS)
				strText.Format("�ر��ն���Ƶ�ɹ����ն�ID:%X ", pAswCloseTermAudVid->dwTermID);
			else
				strText.Format("�ر��ն���Ƶʧ�ܣ��ն�ID:%X ���������:%d", pAswCloseTermAudVid->dwTermID, pAswCloseTermAudVid->nResult);
			
			break;
		}

		case CB_Asw_OpenTermTalk:
		{
			ASSERT(dwSize == sizeof(TSdkAswOpenTermTalk));
			TSdkAswOpenTermTalk * pAswOpenTermTalk = (TSdkAswOpenTermTalk*)pParam;
			CString strText;
			if(pAswOpenTermTalk->nResult == CERR_SUCCESS)
				strText.Format("���ն˶Խ��ɹ����ն�ID:%X ", pAswOpenTermTalk->dwTermID);
			else
				strText.Format("���ն˶Խ�ʧ�ܣ��ն�ID:%X ���������:%d", pAswOpenTermTalk->dwTermID, pAswOpenTermTalk->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Asw_CloseTermTalk:
		{
			ASSERT(dwSize == sizeof(TSdkAswCloseTermTalk));
			TSdkAswCloseTermTalk * pAswCloseTermTalk = (TSdkAswCloseTermTalk*)pParam;
			CString strText;
			if(pAswCloseTermTalk->nResult == CERR_SUCCESS)
				strText.Format("�ر��ն˶Խ��ɹ����ն�ID:%X ", pAswCloseTermTalk->dwTermID);
			else
				strText.Format("�ر��ն˶Խ�ʧ�ܣ��ն�ID:%X ���������:%d", pAswCloseTermTalk->dwTermID, pAswCloseTermTalk->nResult);
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
			strText.Format("��ǰ:%d", pAswGetAudLevel->nAmpValue);
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
				strText.Format("�����ն������ɹ����ն�ID:%X ", pAswSetAudLevel->dwTermID);

				if(pAswSetAudLevel->bIsEx)
				{
					g_pMainWnd->m_nAmpLevel = pAswSetAudLevel->eAmpLevel;
					((CComboBox*)g_pMainWnd->GetDlgItem(IDC_CMB_AMP))->SetCurSel(g_pMainWnd->m_nAmpLevel);
				}
				else
				{
					g_pMainWnd->m_AmpSlider.SetPos(pAswSetAudLevel->nAmpValue);
					CString strText;
					strText.Format("��ǰ:%d", pAswSetAudLevel->nAmpValue);
					g_pMainWnd->GetDlgItem(IDC_STATIC_CUR)->SetWindowText(strText);
				}
			}
			else
			{
				strText.Format("�����ն�����ʧ�ܣ��ն�ID:%X ���������:%d", pAswSetAudLevel->dwTermID, pAswSetAudLevel->nResult);
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
				g_pMainWnd->m_stcTips.SetWindowText("������ʾ��");
				strText.Format("������гɹ����ն�ID:%X ", pAswDismissTerm->dwTermID);
			}
			else
			{
				strText.Format("�������ʧ�ܣ��ն�ID:%X ���������:%d", pAswDismissTerm->dwTermID, pAswDismissTerm->nResult);
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
				strText.Format("�޸��ն����óɹ����ն�ID:%X ", pAswSetDbAlmCfg->dwTermID);
			else
				strText.Format("�޸��ն�����ʧ�ܣ��ն�ID:%X ���������:%d", pAswSetDbAlmCfg->dwTermID, pAswSetDbAlmCfg->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Post_TermSos:
		{
			ASSERT(dwSize == sizeof(TSdkPostTermSos));
			TSdkPostTermSos * pPostTermSos = (TSdkPostTermSos*)pParam;
			CString strText;
			if(pPostTermSos->nSosType == 1)
				strText.Format("�ն˽������� !!!!!! \n�ն�ID:%X", pPostTermSos->dwTermID);
			else
				strText.Format("�ն���ѯ���� !!!!!! \n�ն�ID:%X", pPostTermSos->dwTermID);
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
			strText.Format("�ն�ID:%X �ն�״̬:%s , IO���� == ����1:%s , ����2:%s , ���1:%s , ���2:%s", pPostTermState->dwTermID, GetStateText(pPostTermState->eTermState),
				AMSG(pPostTermState->AlmInState & 0x1), AMSG((pPostTermState->AlmInState >> 1) & 0x1),
				AMSG(pPostTermState->AlmOutState & 0x1), AMSG((pPostTermState->AlmOutState >> 1) & 0x1));
			g_pMainWnd->m_InfoList.AddString(strText);

			int nSel = g_pMainWnd->m_cbxTermList.GetCurSel();
			if(nSel >= 0 && g_pMainWnd->m_TermList[nSel].dwTermID == pPostTermState->dwTermID)
			{
				if(pPostTermState->AlmOutState & 0x1)
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("�رձ������1");
				else
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("�򿪱������1");

				if((pPostTermState->AlmOutState >> 1) & 0x1)
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("�رձ������2");
				else
					g_pMainWnd->GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("�򿪱������2");
			}

			break;
		}

		case CB_Post_485PipeData:
		{
			ASSERT(dwSize == sizeof(TSdkPost485PipeData));
			TSdkPost485PipeData * pPost485PipeData = (TSdkPost485PipeData*)pParam;
			CString strText;
			strText.Format("===> �յ�485���ݣ��ն�ID:%X , ����:%d , ����:%s",
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
				strText.Format("����%d : MP3�ļ����ţ�������������", pPostMp3PlayFinish->nBroadNum);
			else
				strText.Format("����%d : MP3�ļ����ţ����ų����������:%d", pPostMp3PlayFinish->nBroadNum, pPostMp3PlayFinish->nResult);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Post_AlmNotify:
		{
			ASSERT(dwSize == sizeof(TSdkPostAlmNotify));
			TSdkPostAlmNotify * pPostAlmNotify = (TSdkPostAlmNotify*)pParam;
			CString strText;
			if((pPostAlmNotify->eAlmType & SDK_ALMTYPE_DB_ALARM) == SDK_ALMTYPE_DB_ALARM)
				strText.Format("�ն��������� !!!    �ն�ID:%X", pPostAlmNotify->dwTermID);
			else
				strText.Format("�ն˱�������: 0x%X  !!!    �ն�ID:%X", pPostAlmNotify->eAlmType);
			g_pMainWnd->m_InfoList.AddString(strText);
			break;
		}

		case CB_Data_TermAudio:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermAudio));
			TSdkDataTermAudio * pDataTermAudio = (TSdkDataTermAudio*)pParam;
//			TRACE("== �ն���Ƶ ID:%X ����С %d �ֽ�\n", pDataTermAudio->dwTermID,pDataTermAudio->nDataSize);
			TSDK_Player_AudVidAdd(1, TSDK_AUDIO, pDataTermAudio->pAudData, pDataTermAudio->nDataSize);
			break;
		}

		case CB_Data_TermVideo:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermVideo));
			TSdkDataTermVideo * pDataTermVideo = (TSdkDataTermVideo*)pParam;
			if(pDataTermVideo->bIsKeyFrm)
				TRACE("== �ն�I֡��Ƶ ID:%X ����С %d �ֽ�\n", pDataTermVideo->dwTermID,pDataTermVideo->nFrmSize);
//			else
//				TRACE("== �ն�P֡��Ƶ ID:%X ����С %d �ֽ�\n", pDataTermVideo->dwTermID,pDataTermVideo->nFrmSize);

			TSDK_Player_AudVidAdd(1, TSDK_VIDEO, pDataTermVideo->pVidFrm, pDataTermVideo->nFrmSize);
			break;
		}

		case CB_Data_PcMicAudio:
		{
			ASSERT(dwSize == sizeof(TSdkDataPcMicAudio));
			TSdkDataPcMicAudio * pDataPcAudio = (TSdkDataPcMicAudio*)pParam;
//			TRACE("== ������˷���Ƶ����С %d �ֽ�\n", pDataPcAudio->nDataSize);

			if(g_pMainWnd->m_nTalkTermIdx != -1)
			{
				TSDK_Send_TalkAudio(g_pMainWnd->m_TermList[g_pMainWnd->m_nTalkTermIdx].dwTermID,
					pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);
			}

			if(g_pMainWnd->m_bIsBroadingVoice)
			{
				TSDK_BroadPcmVoice(g_MyGroupNum, pDataPcAudio->pPcmData, pDataPcAudio->nDataSize);

				/****************************
				// ����ж������
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
//			TRACE("== ��·��Ƶ ID:%X ����С %d �ֽ�\n", pDataBypassAudio->dwTermID,pDataBypassAudio->nDataSize);
			TSDK_Player_PcmAdd(1, pDataBypassAudio->pPcmData, pDataBypassAudio->nDataSize);
			break;
		}

		case CB_Data_TermMp3L:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermMp3L));
			TSdkDataTermMp3L * pDataTermMp3L = (TSdkDataTermMp3L*)pParam;
//			TRACE("== MP3��Ƶ ID:%X ����С %d �ֽ�\n", pDataTermMp3L->dwTermID,pDataTermMp3L->nDataSize);
			if(g_pMp3File1)
				fwrite(pDataTermMp3L->pMp3Data, 1, pDataTermMp3L->nDataSize, g_pMp3File1);
			break;
		}

		case CB_Data_TermMp3R:
		{
			ASSERT(dwSize == sizeof(TSdkDataTermMp3R));
			TSdkDataTermMp3R * pDataTermMp3R = (TSdkDataTermMp3R*)pParam;
//			TRACE("== MP3��Ƶ ID:%X ����С %d �ֽ�\n", pDataTermMp3R->dwTermID,pDataTermMp3R->nDataSize);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("������æ��", "��ʾ", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("��������ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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

		// ���ն��������˽��жԽ�ʱ������TSDK_Req_OpenTermAudVid�Զ�������Ƶ����ģʽ����ȡ�Խ�˫������Ƶ
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		// ע�⣺����������ն˶Խ�ǰ��Ӧ���ȼ���ն��Ƿ��ڶԽ�״̬(������ռ��)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// �����ռ�þͲ�Ӧ��ִ��������� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("������ʾ��");
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
			strText.Format("�ر�����ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
			strText.Format("����ر��ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////


		nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("����ر��ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_strSend485Data.GetLength() < 1)
	{
		MessageBox("���������ݣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Send_485PipeData(m_TermList[nSel].dwTermID, (LPBYTE)(LPCSTR)m_strSend485Data, m_strSend485Data.GetLength());
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("����485����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::UpdateTermListUI()
{
	int nCurSel = m_cbxTermList.GetCurSel();
	m_cbxTermList.ResetContent();

	CString strText;
	for(int i=0; i<m_nTermCnt; i++)
	{
		strText.Format("ID:[%X]  ����:[%s]  IP:[%s]  ״̬:[%s]", m_TermList[i].dwTermID, m_TermList[i].TermName,
			m_TermList[i].TermIp, m_TermState[i].eState==TSDK_DEV_STATE_OFFLINE ? "������":"����");

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
	// �����Դ���60(MAX_BROAD_GROUP)���㲥���飬
	// ������ʾ: �򵥵İ����зֻ�����Ϊһ���㲥���飬�����Ϊ��6
	// SDKʹ���ߣ����Ը����Լ���Ҫ���Ѹ����ֻ����õ����60�������У������Ϊ1��60 ��
	// ÿ���㲥�������֧��100(MAX_BROAD_TERM)���ն��豸
	// ���Ҫȡ���㲥�����е�ĳЩ�ֻ���ֻ��Ҫ��������TermList[MAX_BROAD_TERM]��
	// Ȼ�����TSDK_SetupGroup()���Ϳ���ȡ���ˡ�

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
		strText.Format("����%d�ŷ���ʧ�ܣ��������:%d", g_MyGroupNum, nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnStartBroadVoice() 
{
	int nResult = TSDK_StartBroadVoice(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�����㲥����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}

	// �����ɼ����Զ˵���˷�
	if(m_bIsBroadingVoice == FALSE)
	{
		TSDK_Player_PcMicOpen();
		m_bIsBroadingVoice = TRUE;
	}
}

void CSdkClientDlg::OnBtnStopBroadVoice() 
{
	// �رղɼ����Զ˵���˷�
	if(m_bIsBroadingVoice)
	{
		m_bIsBroadingVoice = FALSE;
		TSDK_Player_PcMicClose();
	}

	int nResult = TSDK_StopBroadVoice(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�رչ㲥����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnStartBroadMp3() 
{
	int nResult = TSDK_StartBroadMp3(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�����㲥MP3ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnBtnBroadMp3File() 
{
	CString strFilter;
	strFilter.Format("mp3�ļ� (*.mp3)|*.mp3||");
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
		strText.Format("�㲥MP3�ļ�ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}

	g_bPauseMp3Broad = FALSE;
	GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("��ͣMP3�㲥");
}

void CSdkClientDlg::OnBtnStopBroadMp3() 
{
	int nResult = TSDK_StopBroadMp3(g_MyGroupNum);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�رչ㲥MP3ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}
}

void CSdkClientDlg::OnGetLevel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_GetAudLevel(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("��ȡ��������ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnSetLevel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_nCapLevel == -1 || m_nAmpLevel == -1 || m_nCapTalkLevel == -1)
	{
		MessageBox("��ѡ����������", "��ʾ", MB_ICONWARNING);
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
		strText.Format("������������ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnDismissTalk() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_DismissTerm(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("����������ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnGetDbAlm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_GetDbAlmCfg(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("��ȡ������������ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnSetDbAlm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	TSdkDbAlmCfg Cfg;
	Cfg.bDbAlmEnable = m_bDbAlarm;
	Cfg.nDbAlmVal = m_DbValSpin.GetPos();

	int nResult = TSDK_Req_SetDbAlmCfg(m_TermList[nSel].dwTermID, &Cfg);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("����������������ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnReqIFrm() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_IFrame(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("����I֡ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnOpenAlmOut1() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetAlmOut(m_TermList[nSel].dwTermID, 0x1, (m_TermState[nSel].AlmOutState & 0x1) ? 0x0 : 0x1);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("���󱨾����1ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnOpenAlmOut2() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetAlmOut(m_TermList[nSel].dwTermID, 0x2, (m_TermState[nSel].AlmOutState & 0x2) ? 0x0 : 0x2);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("���󱨾����2ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
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
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("�رձ������1");
		else
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_1)->SetWindowText("�򿪱������1");

		if((m_TermState[nSel].AlmOutState >> 1) & 0x1)
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("�رձ������2");
		else
			GetDlgItem(IDC_BTN_OPEN_ALM_OUT_2)->SetWindowText("�򿪱������2");

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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetTermName(m_TermList[nSel].dwTermID, m_strTermName);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�����޸��ն�����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("������æ��", "��ʾ", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("��������ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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

		// ���ն��������˽��жԽ�ʱ������TSDK_Req_OpenTermAudVid_Sub�Զ�������Ƶ����ģʽ����ȡ�Խ�˫������Ƶ
		int nResult = TSDK_Req_OpenTermAudVid_Sub(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		// ע�⣺����������ն˶Խ�ǰ��Ӧ���ȼ���ն��Ƿ��ڶԽ�״̬(������ռ��)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// �����ռ�þͲ�Ӧ��ִ��������� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("������ʾ��");
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
			strText.Format("�ر�����ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
			strText.Format("����ر��ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////


		nResult = TSDK_Req_CloseTermAudVid_Sub(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("����ر��ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_IFrame_Sub(m_TermList[nSel].dwTermID);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("����I֡ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
}

void CSdkClientDlg::OnBtnGetUserData() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	TSdkTermUserData Data;
	int nResult = TSDK_GetTermUserData(m_TermList[nSel].dwTermID, &Data);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("��ȡ�ͻ�����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	TSdkGroupTermList gts;
	memset(&gts, 0, sizeof(TSdkGroupTermList));
	gts.TermList[0] = m_TermList[nSel].dwTermID;
	int nResult = TSDK_GroupAddTerm(g_MyGroupNum, &gts);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("����ն�[%s]���㲥����%dʧ�ܣ��������:%d", m_TermList[nSel].TermName, g_MyGroupNum, nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("�����ɹ����ն�[%s]����ӵ��㲥����%d", m_TermList[nSel].TermName, g_MyGroupNum);
		MessageBox(strText, "��ʾ", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnDel() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	TSdkGroupTermList gts;
	memset(&gts, 0, sizeof(TSdkGroupTermList));
	gts.TermList[0] = m_TermList[nSel].dwTermID;
	int nResult = TSDK_GroupDelTerm(g_MyGroupNum, &gts);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("�ӹ㲥����%dɾ���ն�[%s]ʧ�ܣ��������:%d", g_MyGroupNum, m_TermList[nSel].TermName, nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("�����ɹ����ն�[%s]�Ѵӹ㲥����%d��ɾ��", m_TermList[nSel].TermName, g_MyGroupNum);
		MessageBox(strText, "��ʾ", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnGet() 
{
	UpdateData(TRUE);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_GetTermGroupNum(m_TermList[nSel].dwTermID);
	if(nResult < 0)
	{
		CString strText;
		strText.Format("��ȡ�ն˹㲥�����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
	else
	{
		CString strText;
		strText.Format("�ն� [%s] �㲥����� : %d", m_TermList[nSel].TermName, nResult);
		MessageBox(strText, "��ʾ", MB_ICONINFORMATION);
	}
}

void CSdkClientDlg::OnBtnOpenMon() 
{
	UpdateData(TRUE);

	CString strText;

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_nTalkTermIdx != -1)
	{
		MessageBox("������æ��", "��ʾ", MB_ICONWARNING);
		return;
	}

	m_nTalkTermIdx = nSel;

	if(m_TermList[nSel].eTermType == TSDK_DEV_TYPE_MP3)
	{
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("��������ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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

		// ���ն��������˽��жԽ�ʱ������TSDK_Req_OpenTermAudVid�Զ�������Ƶ����ģʽ����ȡ�Խ�˫������Ƶ
		int nResult = TSDK_Req_OpenTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
		// ע�⣺����������ն˶Խ�ǰ��Ӧ���ȼ���ն��Ƿ��ڶԽ�״̬(������ռ��)(m_TermState[x].eState==TSDK_DEV_STATE_TALKING) ,
		// �����ռ�þͲ�Ӧ��ִ��������� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nResult = TSDK_Req_OpenTermTalk(m_TermList[m_nTalkTermIdx].dwTermID);

		if(nResult != CERR_SUCCESS)
		{
			strText.Format("�����ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}

		TSDK_Player_PcMicOpen();

		m_stcTips.SetWindowText("������ʾ��");
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
			strText.Format("�ر�����ʰ����MP3��ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
			strText.Format("����ر��ն˶Խ�ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
		}

*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		int nResult = TSDK_Req_CloseTermAudVid(m_TermList[m_nTalkTermIdx].dwTermID);
		if(nResult != CERR_SUCCESS)
		{
			strText.Format("����ر��ն�����Ƶʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
		strText.Format("��ͣMP3�㲥ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
		return;
	}

	g_bPauseMp3Broad = !g_bPauseMp3Broad;

	if(g_bPauseMp3Broad)
		GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("����MP3�㲥");
	else
		GetDlgItem(IDC_BTN_PAUSE_MP3_BROAD)->SetWindowText("��ͣMP3�㲥");
}

void CSdkClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	UpdateData(TRUE);

	int nCurPos = m_AmpSlider.GetPos();

	CString strText;
	strText.Format("��ǰ:%d", nCurPos);
	GetDlgItem(IDC_STATIC_CUR)->SetWindowText(strText);

	int nSel = m_cbxTermList.GetCurSel();
	if(nSel < 0)
	{
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
	}
	else if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
	}

	else if(m_nCapLevel == -1 || m_nAmpLevel == -1 || m_nCapTalkLevel == -1)
	{
		MessageBox("��ѡ����������", "��ʾ", MB_ICONWARNING);
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
				strText.Format("������������ʧ�ܣ�������Ϣ: ����δͬ�������Ȼ�ȡ�ٽ�������");
			else if(nResult == CERR_INVALID_BIOS_VERSION)
				strText.Format("������������ʧ�ܣ�������Ϣ: �ֻ�BIOS�汾����ȷ����������");
			else
				strText.Format("������������ʧ�ܣ��������:%d", nResult);
			MessageBox(strText, "����", MB_ICONERROR);
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
		MessageBox("��ѡ���ն��豸��", "��ʾ", MB_ICONWARNING);
		return;
	}

	if(m_TermState[nSel].eState == TSDK_DEV_STATE_OFFLINE)
	{
		MessageBox("�豸�����ߣ�", "��ʾ", MB_ICONWARNING);
		return;
	}

	int nResult = TSDK_Req_SetWaitTime(m_TermList[nSel].dwTermID, m_nWaitTime);
	if(nResult != CERR_SUCCESS)
	{
		CString strText;
		strText.Format("�����޸��ն�����ʧ�ܣ��������:%d", nResult);
		MessageBox(strText, "����", MB_ICONERROR);
	}
	else
	{
		MessageBox("�޸ĳɹ�", "��ʾ", MB_ICONINFORMATION);
	}
}
