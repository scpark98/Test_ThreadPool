
// Test_ThreadPoolDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_ThreadPool.h"
#include "Test_ThreadPoolDlg.h"
#include "afxdialogex.h"

#include "../../Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestThreadPoolDlg 대화 상자



CTestThreadPoolDlg::CTestThreadPoolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_THREADPOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestThreadPoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CTestThreadPoolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestThreadPoolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestThreadPoolDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTestThreadPoolDlg 메시지 처리기

BOOL CTestThreadPoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	init_list();

	CString fullpath = _T("c:\\abc/def\\123.txt");
	CString folder = get_part(fullpath, 2);

	fullpath = _T("/abc/def/12345");
	folder = get_part(fullpath, 3);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestThreadPoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestThreadPoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestThreadPoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CTestThreadPoolDlg::test_func1(int id, int start, int end)
{
	TRACE(_T("id = %d, start...\n"), id);

	for (int i = start; i < end; i++)
	{
		m_list.set_text(id, 1, i2S(i-start));
		TRACE(_T("id=%d, i = %d\n"), id, i);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	TRACE(_T("thread %d terminated.\n"), id);

	return 0;
}

int CTestThreadPoolDlg::test_func2(int id, int start)
{
	TRACE(_T("id = %d, start...\n"), id);

	for (int i = start + 100 - 1; i >= start; i--)
	{
		m_list.set_text(id, 1, i2S(i - start));
		TRACE(_T("id=%d, i = %d\n"), id, i);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	TRACE(_T("thread %d terminated.\n"), id);

	return id;
}

#include <functional>
void CTestThreadPoolDlg::OnBnClickedOk()
{
	SetWindowText(_T("total thread = ") + i2S(m_request_id + 1));

	//CString file_path = _T("agent/linkmemine_windows_agent_setup.exe");	//37 MB
	CString file_path = _T("lmmviewer/lmmviewer_old(solution).zip");	//210 MB
	//CRequestUrlParams* params = (CRequestUrlParams*)new CRequestUrlParams(m_server_ip, m_server_port, _T(""), _T("GET"));
	CRequestUrlParams params(m_server_ip, m_server_port, _T(""), _T("GET"));
	params.sub_url.Format(_T("/download/%s"), file_path);
	params.local_file_path.Format(_T("d:\\temp\\%s_%d"), file_path, m_request_id);
	params.use_thread = true;
	params.request_id = m_request_id;

	m_pool.EnqueueJob([this, params]() { request_url((CRequestUrlParams*)&params); });

	m_request_id++;

	return;

	//int id = m_thread_id;
	//SetWindowText(_T("total thread = ") + i2S(id));
	/*
	m_pool.QueueJob([id]() {std::function<int(int, int)> test_func; });
		//{
		//	for (int i = 0; i < 100; i++)
		//	{
		//		TRACE(_T("id=%d, i = %d\n"), id, i);
		//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//	}
		//});

	m_pool.Start();

	*/

	//만약 시간이 걸리는 작업일 경우는 id를 vector에 저장해놓고 동일한 크기의 vector에 thread종료 flag를 넣어
	//해당 flag가 true이면 바로 thread함수에서 리턴하도록 해야 한다.
	for (int id = 0; id < 10; id++)
	{
		m_list.insert_item(id, i2S(id), false, false);

		//아래 2줄처럼 리턴값도 받아서 활용하려 했으나 어디선가 lock되는 듯함.
		//auto result = m_pool.EnqueueJob([id, this]() -> int { return test_func1(id, id * 100, id * 100 + 100); });
		//TRACE(_T("result = %d\n"), result.get());

		//서로 다른 함수 타입도 모두 job으로 등록하여 thread로 실행할 수 있다.
		if (id % 2)
			m_pool.EnqueueJob([id, this]() { test_func1(id, id * 100, id * 100 + 100); });
		else
			m_pool.EnqueueJob([id, this]() { test_func2(id, id * 100); });
		////Wait(10);	//wait를 안주면 에러발생. ThreadPool 클래스의 EnqueueJob()에서  Sleep(10);을 주니 정상 동작함.
	}

	/*
	//std::vector<std::future<int>> futures;
	//futures.emplace_back(
	m_pool.EnqueueJob([id, this]()
		{

			//test_func(id, 0, 100); 

			//for (int i = 0; i < 100; i++)
			//{
			//	TRACE(_T("id=%d, i = %d\n"), id, i);
			//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
			//}
		}
	);
	*/
	//for (auto& f : futures)
	//{
	//	printf("result : %d\n", f.get());
	//}

}


void CTestThreadPoolDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CTestThreadPoolDlg::init_list()
{
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_list.set_headings(_T("id,50;진행률,100"));
	m_list.set_column_data_type(1, CVtListCtrlEx::column_data_type_progress);
	m_list.show_progress_text();
	m_list.progress_text_color(GRAY(64));
	m_list.set_use_own_imagelist(false);
	m_list.set_header_height(20);
	m_list.set_line_height(20);
	//m_list.set_color_theme(CVtListCtrlEx::color_theme_navy_blue);
}
