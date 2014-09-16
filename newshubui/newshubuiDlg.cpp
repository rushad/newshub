// newshubuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "newshubui.h"
#include "newshubuiDlg.h"

#include "serverLoop.h"

#include "client_queue_thread_loop.h"
#include "server_thread_loop.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_socket.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNewsHubDlg dialog

CNewsHubDlg::CNewsHubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsHubDlg::IDD, pParent),
    messageId(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNewsHubDlg::~CNewsHubDlg()
{
}

void CNewsHubDlg::Message(const NewsHub::Socket & socket, const unsigned int messageId, const std::string & message)
{
  CListBox* pReceivedMessages = (CListBox*)GetDlgItem(IDC_RECEIVED_MESSAGES);
  CString str;
  std::string from, to;
  int fromPort, toPort;
  NewsHub::TcpSocket* tcpSocket = (NewsHub::TcpSocket*)&socket;
  tcpSocket->SockAddr(to, toPort);
  tcpSocket->PeerAddr(from, fromPort);
  str.Format(_T("(From: %s:%d to: %s:%d msgId:%d): %s"), 
    CString(CA2CT(from.c_str())), fromPort, CString(CA2CT(to.c_str())), toPort, messageId, CString(CA2CT(message.c_str())));
  int row = pReceivedMessages->AddString(str);
  pReceivedMessages->SetCurSel(row);
}

void CNewsHubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewsHubDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_SERVER_START, &CNewsHubDlg::OnBnClickedServerStart)
  ON_BN_CLICKED(IDC_FINISH, &CNewsHubDlg::OnBnClickedFinish)
  ON_BN_CLICKED(IDC_SEND, &CNewsHubDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CNewsHubDlg message handlers

BOOL CNewsHubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

  CButton* pServerTcp = (CButton*)GetDlgItem(IDC_SERVER_TCP);
  pServerTcp->SetCheck(true);

  CEdit* pServerPort = (CEdit*)GetDlgItem(IDC_SERVER_PORT);
  pServerPort->SetWindowText(CString("12345"));

  CButton* pClientTcp = (CButton*)GetDlgItem(IDC_CLIENT_TCP);
  pClientTcp->SetCheck(true);

  CEdit* pHost = (CEdit*)GetDlgItem(IDC_HOST);
  pHost->SetWindowText(CString("localhost"));

  CEdit* pPort = (CEdit*)GetDlgItem(IDC_PORT);
  pPort->SetWindowText(CString("12345"));

  return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNewsHubDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

void CNewsHubDlg::OnOK()
{
  onClose();
  CDialog::OnOK();
}

void CNewsHubDlg::OnCancel()
{
  onClose();
  CDialog::OnCancel();
}

void CNewsHubDlg::onClose()
{
  CListBox* pRunningServers = (CListBox*)GetDlgItem(IDC_RUNNING_SERVERS);
  for (int i = 0; i < pRunningServers->GetCount(); ++i)
  {
    ServerLoop* serverLoop = (ServerLoop*)pRunningServers->GetItemDataPtr(i);
    delete serverLoop;
  }

/*  std::map<CString, NewsHub::Socket*>::iterator itSocket;
  for (itSocket = clientSockets.begin(); itSocket != clientSockets.end(); itSocket++)
    delete itSocket->second;*/
}

CString CNewsHubDlg::getDestinationId() const
{
  CEdit* pHost = (CEdit*)GetDlgItem(IDC_HOST);
  CEdit* pPort = (CEdit*)GetDlgItem(IDC_PORT);

  CString host, port;
  pHost->GetWindowText(host);
  pPort->GetWindowText(port);

  return CString("TCP:") + host + CString(":") + port;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNewsHubDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNewsHubDlg::OnBnClickedServerStart()
{
  CEdit* pPort = (CEdit*)GetDlgItem(IDC_SERVER_PORT);
  CString strPort;
  pPort->GetWindowText(strPort);

  ServerLoop* serverLoop;
  try
  {
    NewsHub::TcpServer* server = new NewsHub::TcpServer(atoi(CT2CA(strPort)));
    serverLoop = new ServerLoop(server, new NewsHub::ServerThreadLoop(*server, *this));
  }
  catch (std::exception & e)
  {
    MessageBox(CString(e.what()), _T("Exception"), MB_OK);
    return;
  }

  CString serverDescription = CString("TCP server listening on port ") + strPort; 

  CListBox* pRunningServers = (CListBox*)GetDlgItem(IDC_RUNNING_SERVERS);
  int row = pRunningServers->AddString(serverDescription);
  pRunningServers->SetItemDataPtr(row, serverLoop);
}

void CNewsHubDlg::OnBnClickedFinish()
{
  CListBox* pRunningServers = (CListBox*)GetDlgItem(IDC_RUNNING_SERVERS);
  
  int row = pRunningServers->GetCurSel();
  if (row != LB_ERR)
  {
    ServerLoop* serverLoop = (ServerLoop*)pRunningServers->GetItemDataPtr(row);
    delete serverLoop;
    pRunningServers->DeleteString(row);
  }
}

void CNewsHubDlg::OnBnClickedSend()
{
  CString destinationId = getDestinationId();
  NewsHub::ClientQueueThreadLoop *queue;

  std::map<CString, NewsHub::ClientQueueThreadLoop*>::iterator itQueue = clientQueues.find(destinationId);
  if (itQueue == clientQueues.end())
  {
    CEdit* pHost = (CEdit*)GetDlgItem(IDC_HOST);
    CEdit* pPort = (CEdit*)GetDlgItem(IDC_PORT);

    CString host, port;
    pHost->GetWindowText(host);
    pPort->GetWindowText(port);

    try
    {
      NewsHub::TcpClient* client = new NewsHub::TcpClient(std::string(CT2CA(host)), atoi(CT2CA(port)));
      queue = new NewsHub::ClientQueueThreadLoop(*client);
    }
    catch (std::exception & e)
    {
      MessageBox(CString(e.what()), _T("Exception"), MB_OK);
      return;
    }

    itQueue = clientQueues.insert(std::pair<CString, NewsHub::ClientQueueThreadLoop*>(destinationId, queue)).first;
  }
  else
  {
    queue = itQueue->second;
  }

  CEdit* pMessage = (CEdit*)GetDlgItem(IDC_MESSAGE);
  CString message;
  pMessage->GetWindowText(message);
  std::cout << "> " << CT2CA(message) << std::endl;
  queue->AddMessage(++messageId, std::string(CT2CA(message)));
}
