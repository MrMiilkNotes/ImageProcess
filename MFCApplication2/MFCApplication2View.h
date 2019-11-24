
// MFCApplication2View.h: CMFCApplication2View 类的接口

#pragma once
#include<memory>
#include"MyDIB.h"
#include"BMP8.h"
#include"Histogram.h"
#include"tools.h"

class CMFCApplication2View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication2View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2View)

// 特性
public:
	CMFCApplication2Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	std::shared_ptr<BMPdib> bmp_sptr;
};

#ifndef _DEBUG  // MFCApplication2View.cpp 中的调试版本
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

