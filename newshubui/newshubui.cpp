// newshubui.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "newshubui.h"
#include "newshubuiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewsHubApp

BEGIN_MESSAGE_MAP(CNewsHubApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNewsHubApp construction

CNewsHubApp::CNewsHubApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNewsHubApp object

CNewsHubApp theApp;


// CNewsHubApp initialization

BOOL CNewsHubApp::InitInstance()
{
	CWinApp::InitInstance();

  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 0), &wsaData);

	CNewsHubDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
