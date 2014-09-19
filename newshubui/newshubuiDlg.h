// newshubuiDlg.h : header file
//

#pragma once

#include "news_delegate.h"

#include "client_queue_thread_loop.h"
#include "socket.h"

#include <map>

// CNewsHubDlg dialog
class CNewsHubDlg : public CDialog, NewsHub::NewsDelegate, NewsHub::DeliveryDelegate
{
// Construction
public:
	CNewsHubDlg(CWnd* pParent = NULL);	// standard constructor
  ~CNewsHubDlg();

  virtual bool Message(const NewsHub::Socket & socket, const unsigned int messageId, const std::string & message);
  virtual void MessageDelivered(NewsHub::ClientQueueThreadLoop* queue, const NewsHub::Socket & socket, const unsigned int messageId, const std::string & message);

  // Dialog Data
	enum { IDD = IDD_NEWSHUBUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
  afx_msg void OnOK();
  afx_msg void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
  void onClose();

public:
  afx_msg void OnBnClickedServerStart();
  afx_msg void OnBnClickedServerFinish();
  afx_msg void OnBnClickedClientStart();
  afx_msg void OnBnClickedClientFinish();
  afx_msg void OnBnClickedSend();
};
