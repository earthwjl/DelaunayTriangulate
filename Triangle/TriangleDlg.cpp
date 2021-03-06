
// TriangleDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Triangle.h"
#include "TriangleDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTriangleDlg 对话框



CTriangleDlg::CTriangleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRIANGLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	rand_num = 0;
}

void CTriangleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, groupbox);
}

BEGIN_MESSAGE_MAP(CTriangleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BTN_RANDPOINTS, &CTriangleDlg::OnBnClickedBtnRandpoints)
ON_BN_CLICKED(IDC_BTN_TRIANGLE, &CTriangleDlg::OnBnClickedBtnTriangle)
ON_EN_UPDATE(IDC_EDIT1, &CTriangleDlg::OnEnUpdateEdit1)
//ON_BN_CLICKED(IDC_BUTTON1, &CTriangleDlg::OnBnClickedButton1)
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


// CTriangleDlg 消息处理程序

BOOL CTriangleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTriangleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTriangleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect windowrect;
		GetWindowRect(windowrect);
		dc.MoveTo(windowrect.Width() - 180, 0);
		dc.LineTo(windowrect.Width() - 180, windowrect.Height());

		CBrush  brush; brush.CreateSolidBrush(RGB(255, 0, 0));
		CBrush* old_brush = dc.SelectObject(&brush);
		int radius = 3;
		for (size_t i = 0; i < points.size(); ++i)
		{
			CRect tmp(points[i].x - radius, points[i].y - radius, points[i].x + radius, points[i].y + radius);
			dc.Ellipse(tmp);
		}
		dc.SelectObject(old_brush);

		for (auto iter = triangles.begin(); iter != triangles.end(); ++iter)
		{
			iter->Draw(&dc);
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTriangleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool sortCPoint(const CPoint& p1, const CPoint& p2) { return p1.x < p2.x; }
void CTriangleDlg::OnBnClickedBtnRandpoints()
{
	//复位
	triangles.clear();
	points.clear();
	minX = INT_MAX;
	maxX = INT_MIN;
	maxY = INT_MIN;
	minY = INT_MAX;

	rand_num = GetDlgItemInt(IDC_EDIT1);
	if (rand_num < 3)rand_num = 3;
	srand(clock());
	CRect rect;
	GetClientRect(&rect);

	for (size_t i = 0; i < rand_num; ++i)
	{
		CPoint p(rand() % (rect.Width() - 180), rand() % (rect.Height()));
		if (p.x > maxX)maxX = p.x;
		if (p.x < minX)minX = p.x;
		if (p.y > maxY)maxY = p.y;
		if (p.y < minY)minY = p.y;
		points.push_back(p);
	}
	sort(points.begin(), points.end(), sortCPoint);
	Invalidate();
}


void CTriangleDlg::OnBnClickedBtnTriangle()
{
	time_t start = clock();
	Bowyer_Lawson();
	time_t end = clock();
	CString msg;
	msg.Format(_T("%d ms"), end - start);
	MessageBox(msg);
	Invalidate();
}

void CTriangleDlg::Bowyer_Lawson()
{
	if (points.size() < 3)return;
  	const auto dx = maxX - minX;
	const auto dy = maxY - minY;
	const auto dmax = max(dx, dy);
	const auto midx = (maxX + minX) / 2;
	const auto midy = (maxY + minY) / 2;

	triangles.clear();
	const auto p0 = CPoint(midx - 20 * dmax, midy - dmax);
	const auto p1 = CPoint(midx, midy + 20 * dmax);
	const auto p2 = CPoint(midx + 20 * dmax, midy - dmax);


	triangles.push_back(Triangle(p0, p1, p2));
	for (auto const& pt : points)
	{
		std::list<Triangle>tmp;
		std::list<Edge>edges;
		for (auto const& tri : triangles)
		{
			if (tri.CircleContain(pt))
			{
				edges.push_back(Edge(tri.left, tri.mid));
				edges.push_back(Edge(tri.mid, tri.right));
				edges.push_back(Edge(tri.left, tri.right));
			}
			else
				tmp.push_back(tri);
		}

		//移除重复边
		for (auto it1 = edges.begin(); it1 != edges.end();)
		{
			bool hasRepeat = false;
			for (auto it2 = edges.begin(); it2 != edges.end();)
			{
				if (it1 == it2)
				{
					it2++;
					continue;
				}
				else if (*it1 == *it2)
				{
					hasRepeat = true;
					edges.erase(it1);
					edges.erase(it2);
					break;
				}
				else
					it2++;
			}
			if (hasRepeat)it1 = edges.begin();
			else it1++;
		}

		for (const Edge& e : edges)
		{
			tmp.push_back(Triangle(pt, e));
		}
		triangles = tmp;
	}

	for (auto it = triangles.begin(); it != triangles.end();)
	{
		if (it->left == p0 || it->left == p1 || it->left == p2)
			it = triangles.erase(it);
		else if (it->mid == p0 || it->mid == p1 || it->mid == p2)
			it = triangles.erase(it);
		else if (it->right == p0 || it->right == p1 || it->right == p2)
			it = triangles.erase(it);
		else it++;
	}
}


void CTriangleDlg::OnEnUpdateEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	//OnBnClickedBtnRandpoints();
}


//void CTriangleDlg::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	FILE f;
//	FILE* fp = &f;
//	FILE** fpp = &fp;
//
//	if (!fopen_s(fpp, "data.txt", "w"))
//	{
//		rand_num = 3;
//		for (size_t i = 0; i < 1000; i++)
//		{
//			OnBnClickedBtnRandpoints();
//			Bowyer_Lawson();
//			if (triangles.size() != 1)
//			{
//				fprintf(*fpp, "%d,%d\t%d,%d\t%d,%d  k1 = %lf,  k2 = %lf\n", points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y,(double)(points[1].y - points[0].y)/(points[1].x - points[0].x),
//							(double)(points[2].y - points[1].y)/(points[2].x - points[1].x));
//			}
//		}
//		fclose(*fpp);
//	}
//
//}


void CTriangleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	if (rect.Width() - point.x > 180)
	{
		points.push_back(point);
		maxX = max(maxX, point.x);
		minX = min(minX, point.x);
		maxY = max(maxY, point.y);
		minY = min(minY, point.y);
		CString text; text.Format(_T("%d"), points.size());
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(text);
		//Bowyer_Lawson();
		Invalidate();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTriangleDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Bowyer_Lawson();
	Invalidate();
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CTriangleDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	points.clear();
	triangles.clear();
	Invalidate();
	CDialogEx::OnMButtonUp(nFlags, point);
}
