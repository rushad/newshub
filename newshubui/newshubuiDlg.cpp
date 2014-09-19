// newshubuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "newshubui.h"
#include "newshubuiDlg.h"

#include "utils.h"

#include "client_queue_thread_loop.h"
#include "server_thread_loop.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_socket.h"
#include "udp_client.h"
#include "udp_server.h"
#include "udp_socket.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNewsHubDlg dialog

CNewsHubDlg::CNewsHubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsHubDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNewsHubDlg::~CNewsHubDlg()
{
}

bool CNewsHubDlg::Message(const NewsHub::Socket & socket, const unsigned int messageId, const std::string & message)
{
  CListBox* pReceivedMessages = (CListBox*)GetDlgItem(IDC_RECEIVED_MESSAGES);
  CString str;

  if (socket.Type() == "TCP")
  {
    std::string from, to;
    int fromPort, toPort;
    NewsHub::TcpSocket* tcpSocket = (NewsHub::TcpSocket*)&socket;
    tcpSocket->SockAddr(to, toPort);
    tcpSocket->PeerAddr(from, fromPort);
    str.Format(_T("(From: %s:%d to: %s:%d msgId:%d): %s"), 
      CString(from.c_str()), fromPort, CString(to.c_str()), toPort, messageId, CString(message.c_str()));
  }
  else
  {
    str.Format(_T("%d: %s"), messageId, CString(message.c_str()));
  }

  int row = pReceivedMessages->AddString(str);
  pReceivedMessages->SetCurSel(row);

  CButton* pSendDeliveryConfirmation = (CButton*)GetDlgItem(IDC_SEND_DELIVERY_CONFIRMATION);
  return pSendDeliveryConfirmation->GetCheck() == BST_CHECKED;
}

void CNewsHubDlg::MessageDelivered(NewsHub::ClientQueueThreadLoop* queue, const NewsHub::Socket & socket, const unsigned int messageId, const std::string & message)
{
  CListBox* pSentMessages = (CListBox*)GetDlgItem(IDC_SENT_MESSAGES);
  for (int i = 0; i < pSentMessages->GetCount(); ++i)
  {
    MessageInfo* messageInfo = (MessageInfo*)pSentMessages->GetItemDataPtr(i);
    if ((messageInfo->queue == queue) && (messageInfo->messageId == messageId))
    {
      CString str;
      pSentMessages->GetText(i, str);
      pSentMessages->DeleteString(i);
      pSentMessages->InsertString(i, str + _T(": Delivered"));
      pSentMessages->SetItemDataPtr(i, messageInfo);
      pSentMessages->SetCurSel(i);
      break;
    }
  }
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
  ON_BN_CLICKED(IDC_SERVER_FINISH, &CNewsHubDlg::OnBnClickedServerFinish)
  ON_BN_CLICKED(IDC_CLIENT_START, &CNewsHubDlg::OnBnClickedClientStart)
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

  CEdit* pTimeout = (CEdit*)GetDlgItem(IDC_TIMEOUT);
  pTimeout->SetWindowText(CString("1"));

  CButton* pSendDeliveryConfirmation = (CButton*)GetDlgItem(IDC_SEND_DELIVERY_CONFIRMATION);
  pSendDeliveryConfirmation->SetCheck(BST_CHECKED);

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

  CListBox* pRunningClients = (CListBox*)GetDlgItem(IDC_RUNNING_CLIENTS);
  for (int i = 0; i < pRunningClients->GetCount(); ++i)
  {
    ClientLoop* clientLoop = (ClientLoop*)pRunningClients->GetItemDataPtr(i);
    delete clientLoop;
  }

  CListBox* pSentMessages = (CListBox*)GetDlgItem(IDC_SENT_MESSAGES);
  for (int i = 0; i < pSentMessages->GetCount(); ++i)
  {
    MessageInfo* messageInfo = (MessageInfo*)pSentMessages->GetItemDataPtr(i);
    delete messageInfo;
  }
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
  int port = atoi(CT2CA(strPort));

  CButton* pServerTcp = (CButton*)GetDlgItem(IDC_SERVER_TCP);
  bool bTcp = (pServerTcp->GetCheck() == BST_CHECKED);

  ServerLoop* serverLoop;
  try
  {
    NewsHub::Server* server;
    if (bTcp)
      server = new NewsHub::TcpServer(port);
    else
      server = new NewsHub::UdpServer(port);
    serverLoop = new ServerLoop(server, new NewsHub::ServerThreadLoop(*server, *this, bTcp));
  }
  catch (std::exception & e)
  {
    MessageBox(CString(e.what()), _T("Exception"), MB_OK);
    return;
  }

  CString serverDescription = CString(bTcp?"TCP":"UDP") + CString(" server is listening on port ") + strPort; 

  CListBox* pRunningServers = (CListBox*)GetDlgItem(IDC_RUNNING_SERVERS);
  int row = pRunningServers->AddString(serverDescription);
  pRunningServers->SetItemDataPtr(row, serverLoop);
}

void CNewsHubDlg::OnBnClickedServerFinish()
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

void CNewsHubDlg::OnBnClickedClientStart()
{
  CEdit* pPort = (CEdit*)GetDlgItem(IDC_PORT);
  CEdit* pHost = (CEdit*)GetDlgItem(IDC_HOST);
  CEdit* pTimeout = (CEdit*)GetDlgItem(IDC_TIMEOUT);

  CString host, strPort, strTimeout;
  pHost->GetWindowText(host);
  pPort->GetWindowText(strPort);
  pTimeout->GetWindowText(strTimeout);
  int port = atoi(CT2CA(strPort));
  int timeout = atoi(CT2CA(strTimeout));

  CButton* pClientTcp = (CButton*)GetDlgItem(IDC_CLIENT_TCP);
  bool bTcp = (pClientTcp->GetCheck() == BST_CHECKED);

  ClientLoop* clientLoop;
  try
  {
    NewsHub::Client* client;
    if (bTcp)
      client = new NewsHub::TcpClient(std::string(CT2CA(host)), port);
    else
      client = new NewsHub::UdpClient(std::string(CT2CA(host)), port);

    clientLoop = new ClientLoop(client, new NewsHub::ClientQueueThreadLoop(*client, *this, timeout * 1000));
  }
  catch (std::exception & e)
  {
    MessageBox(CString(e.what()), _T("Exception"), MB_OK);
    return;
  }

  CString clientDescription = CString(bTcp?"TCP":"UDP") + CString(" client to host ") + host + CString(" on port ") + strPort; 

  CListBox* pRunningClients = (CListBox*)GetDlgItem(IDC_RUNNING_CLIENTS);
  int row = pRunningClients->AddString(clientDescription);
  pRunningClients->SetItemDataPtr(row, clientLoop);
}

void CNewsHubDlg::OnBnClickedSend()
{
  CListBox* pRunningClients = (CListBox*)GetDlgItem(IDC_RUNNING_CLIENTS);

  int row = pRunningClients->GetCurSel();
  if (row == LB_ERR)
  {
    MessageBox(_T("Please select one of running clients to send message through"), _T("No client selected"), MB_OK);
    return;
  }

  CEdit* pMessage = (CEdit*)GetDlgItem(IDC_MESSAGE);
  CString message;
  pMessage->GetWindowText(message);

  ClientLoop* clientLoop = (ClientLoop*)pRunningClients->GetItemDataPtr(row);
  unsigned int messageId = clientLoop->nextMessageId();
  clientLoop->Queue()->AddMessage(messageId, std::string(CT2CA(message)));

  CListBox* pSentMessages = (CListBox*)GetDlgItem(IDC_SENT_MESSAGES);
  NewsHub::TcpClient* tcpClient = (NewsHub::TcpClient*)clientLoop->Client();
  CString str;
  str.Format(_T("(To: %s:%d msgId:%d): %s"), 
    CString(tcpClient->Host().c_str()), tcpClient->Port(), messageId, message);

  row = pSentMessages->AddString(str);

  MessageInfo* messageInfo = new MessageInfo;
  messageInfo->queue = clientLoop->Queue();
  messageInfo->messageId = messageId;
  pSentMessages->SetItemDataPtr(row, messageInfo);

  pSentMessages->SetCurSel(row);
}
