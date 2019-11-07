
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMFCApplication1View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMFCApplication1View::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &CMFCApplication1View::OnFileSave)
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 绘图

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	unsigned char grey, r, g, b;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			r = *(m_pImage + w * i * 3 + j * 3);
			g = *(m_pImage + w * i * 3 + j * 3 + 1);
			b = *(m_pImage + w * i * 3 + j * 3 + 2);
			pDC->SetPixelV(10 + j, 10 + i, RGB(r, g, b));
		}
	}
}


// CMFCApplication1View 打印

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序


void CMFCApplication1View::OnFileOpen()
{
	CFileDialog dlg(TRUE, _T("raw"), _T("*.raw"));
	CFile file;
	if (dlg.DoModal() == IDOK) {
		VERIFY(file.Open(dlg.GetPathName(), CFile::modeRead));
	}
	DWORD ndwSizeRaw = (DWORD)file.GetLength();
	m_pImage = new unsigned char[ndwSizeRaw];
	file.Read(m_pImage, ndwSizeRaw);
	file.Close();
	// 彩色
	m_Nsize = (int)sqrt(double(ndwSizeRaw / 3));
	// 长方形
	w = 752;
	h = 600;
	TRACE("N=%d\n",m_Nsize);
	Invalidate();
}


void CMFCApplication1View::OnFileSaveAs()
{
	DWORD ndwSizeRaw = w * h * 3;
	WriteRawFile(m_pImage, ndwSizeRaw);
	return;
}

void CMFCApplication1View::OnFileSave()
{
	DWORD ndwSizeRaw = w * h * 3;
	WriteRawFile(m_pImage, ndwSizeRaw);
	return;
}

BOOL CMFCApplication1View::WriteRawFile(LPVOID lpvBits, DWORD ndwSizeRaw)
{
	CFileDialog dlg(FALSE, _T("raw"), _T("*.raw"));
	CFile RawFile;
	if (dlg.DoModal() == IDOK) {
		VERIFY(RawFile.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite));
	}
	try {
		RawFile.Write((LPVOID)lpvBits, ndwSizeRaw);
	}
	catch (CException * pe) {
		pe->Delete();
		AfxMessageBox(_T("write error"));
		RawFile.Close();
		return FALSE;
	}
	RawFile.Close();
	return TRUE;
}


