
// MFCApplication2View.cpp: CMFCApplication2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"
#include"MyDIB.h"
#include"Histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMFCApplication2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMFCApplication2View::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMFCApplication2View::OnFileSaveAs)
END_MESSAGE_MAP()

// CMFCApplication2View 构造/析构

CMFCApplication2View::CMFCApplication2View() noexcept
	:m_dib(nullptr)
{
	// TODO: 在此处添加构造代码
}

CMFCApplication2View::~CMFCApplication2View()
{
	if (m_dib != nullptr) {
		delete m_dib;
		m_dib = nullptr;
	}
	//if(m_pImage)
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication2View 绘图

void CMFCApplication2View::OnDraw(CDC* pDC)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//unsigned char grey, r, g, b;
	if (m_dib) {
		for (int i = 0; i < m_dib->height(); i++) {
			for (int j = 0; j < m_dib->width(); j++)
			{
				/*g= *(m_dib->data() + m_dib->width() * i * 3 + j * 3);
				r = *(m_dib->data() + m_dib->width() * i * 3 + j * 3 + 1);
				b = *(m_dib->data() + m_dib->width() * i * 3 + j * 3 + 2);*/
				pDC->SetPixelV(10 + j, 10 + i, (m_dib->_colorAt)(i, j));
			}
		}
	}
}


// CMFCApplication2View 打印

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication2View 诊断

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication2View 消息处理程序


void CMFCApplication2View::OnFileOpen()
{
	if (m_dib) {
		delete m_dib;
		m_dib = nullptr;
	}

	CFileDialog dlg(true, _T("bmp"), _T("*.bmp"));

	if (dlg.DoModal() == IDOK) {
		CString fileName = dlg.GetPathName();
		m_dib = new DIB1;
		m_dib->Read(fileName);
	}
	auto hist = histogram(*m_dib);
	//binarization(*m_dib, 128);
	equalization(*m_dib, hist);

	//CString fileName = CString("D:\\Projects\\DigitalImageProcess\\test_pictures\\bmp\\3\\girl.bmp");// 
	//auto m_dib_2 = new DIB1;
	//m_dib_2->Read(fileName);

	//auto hist_1 = accu_hist(histogram(*m_dib));
	//auto hist_2 = accu_hist(histogram(*m_dib_2));
	//normalized(*m_dib, hist_1, hist_2);

	Invalidate();
}


void CMFCApplication2View::OnFileSave()
{
	if (m_dib == nullptr) return;

	CFileDialog dlg(false);
	if (dlg.DoModal() == IDOK) {
		CString fileName = dlg.GetPathName();
		m_dib->Write(fileName);
	}

	Invalidate();
}


void CMFCApplication2View::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
}
