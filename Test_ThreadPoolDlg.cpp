
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
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON_ADD_QUEUE, &CTestThreadPoolDlg::OnBnClickedButtonAddQueue)
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
	m_resize.Create(this);
	m_resize.Add(IDC_LIST, 0, 0, 100, 100);
	m_resize.Add(IDOK, 100, 100, 0, 0);
	m_resize.Add(IDCANCEL, 100, 100, 0, 0);

	init_list();

	m_pool.resize(100);

	RestoreWindowPosition(&theApp, this);

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

	for (int i = start; i <= end; i++)
	{
		m_list.set_text(id, 1, i2S(i-start));
		logWrite(_T("id=%02d, i = %d"), id, i);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	TRACE(_T("thread %d terminated.\n"), id);

	return 0;
}

int CTestThreadPoolDlg::test_func2(int id, int start)
{
	TRACE(_T("id = %d, start...\n"), id);

	for (int i = start + 100; i >= start; i--)
	{
		m_list.set_text(id, 1, i2S(i - start));
		logWrite(_T("id=02%d, i = %d"), id, i);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	TRACE(_T("thread %d terminated.\n"), id);

	return id;
}

void CTestThreadPoolDlg::test_func3()
{
	while (true)
	{

		//TRACE(_T("test_func3()\n"));
		//std::unique_lock<std::mutex> lock(m_mutex);
		//m_cond.wait(lock, [this]() -> bool
		//	{
		//		return m_int_queue.size();
		//	}
		//);

		//위와 같이 std::condition_variable로 wait를 걸고 조건이 true가 되면
		//실제 body를 처리하는 것이 정석이다.
		//아래와 같이 조건이 true이면 처리, false이면 sleep_for는 정석은 아니지만
		//CPU 사용률에 영향이 거의 안느껴진다.
		if (m_int_queue.size())
		{
			TRACE(_T("pop_back()\n"));
			m_mutex.lock();
			m_int_queue.pop_back();
			TRACE(_T("queue size = %d\n"), m_int_queue.size());
			m_mutex.unlock();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

#include <functional>
void CTestThreadPoolDlg::OnBnClickedOk()
{
	//SetWindowText(_T("total thread = ") + i2S(m_request_id + 1));
	/*
	//HttpRequest를 통한 웹 파일 다운로드는 최대 2개까지만 동시에 진행되는 제약이 있다.
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
	*/

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

	//m_pool.EnqueueJob([this]() { test_func3(); });
#if 1
	//만약 시간이 걸리는 작업일 경우는 id를 vector에 저장해놓고 동일한 크기의 vector에 thread종료 flag를 넣어
	//해당 flag가 true이면 바로 thread함수에서 리턴하도록 해야 한다.
	int loop = 100;
	long t0 = clock();
	//100개의 thread를 넣는데 1526ms
	for (int id = 0; id < m_pool.get_max_pool_size(); id++)
	{
		m_list.insert_item(id, i2S(id), false, false);

		//아래 2줄처럼 리턴값도 받아서 활용하려 했으나 어디선가 lock되는 듯함.
		//auto result = m_pool.EnqueueJob([id, this]() -> int { return test_func1(id, id * 100, id * 100 + 100); });
		//TRACE(_T("result = %d\n"), result.get());

		//서로 다른 함수 타입도 모두 job으로 등록하여 thread로 실행할 수 있다.
		if (id % 2)
			m_pool.EnqueueJob([id, loop, this]() { test_func1(id, id * loop, id * loop + loop); });
		else
			m_pool.EnqueueJob([id, loop, this]() { test_func2(id, id * loop); });
		////Wait(10);	//wait를 안주면 에러발생. ThreadPool 클래스의 EnqueueJob()에서  Sleep(10);을 주니 정상 동작함.
	}

	SetWindowText(i2S(clock() - t0));
	TRACE(_T("elapsed = %ld\n"), clock() - t0);
#endif
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
	int job_count = m_pool.get_job_count();
	TRACE(_T("job_count = %d\n"), job_count);
	if (job_count > 0)
	{
		m_pool.stop_all_threads();
		return;
	}

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
	m_list.set_header_height(24);
	m_list.set_line_height(20);
	//m_list.set_color_theme(CVtListCtrlEx::color_theme_navy_blue);
}


void CTestThreadPoolDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}


void CTestThreadPoolDlg::OnBnClickedButtonAddQueue()
{
	m_mutex.lock();
	m_int_queue.push_back(random19937(0, 100));
	TRACE(_T("queue size = %d\n"), m_int_queue.size());
	m_mutex.unlock();

	m_cond.notify_one();
}
