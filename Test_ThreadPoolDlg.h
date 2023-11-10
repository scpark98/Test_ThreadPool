
// Test_ThreadPoolDlg.h: 헤더 파일
//

#pragma once

#include "../../Common/thread/ThreadPool/ThreadPool.h"
#include "../../Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"

// CTestThreadPoolDlg 대화 상자
class CTestThreadPoolDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestThreadPoolDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	ThreadPool	m_pool;
	int m_request_id = 0;
	int test_func1(int id, int start, int end);
	int test_func2(int id, int start);

	CString			m_server_ip = _T("sl.linkmemine.com");
	int				m_server_port = 443;
	CString			m_login_id = _T("user9");


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_THREADPOOL_DIALOG };
#endif

protected:
	void		init_list();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CVtListCtrlEx m_list;
};
