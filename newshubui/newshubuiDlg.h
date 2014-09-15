// newshubuiDlg.h : header file
//

#pragma once

#include "news_delegate.h"
#include "socket.h"

#include <map>

// CNewsHubDlg dialog
class CNewsHubDlg : public CDialog, NewsHub::NewsDelegate
{
// Construction
public:
	CNewsHubDlg(CWnd* pParent = NULL);	// standard constructor
  ~CNewsHubDlg();

  virtual void Message(const std::string & message);

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
  std::map<CString, NewsHub::Socket*> clientSockets;

  void onClose();
  CString getSocketId() const;

public:
  afx_msg void OnBnClickedServerStart();
  afx_msg void OnBnClickedFinish();
  afx_msg void OnBnClickedSend();
};
