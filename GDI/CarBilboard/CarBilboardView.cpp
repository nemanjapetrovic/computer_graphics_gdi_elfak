
// CarBilboardView.cpp : implementation of the CCarBilboardView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CarBilboard.h"
#endif

#include "CarBilboardDoc.h"
#include "CarBilboardView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarBilboardView

IMPLEMENT_DYNCREATE(CCarBilboardView, CView)

BEGIN_MESSAGE_MAP(CCarBilboardView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCarBilboardView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CCarBilboardView construction/destruction

int move = 0;
DImage *sky;
HENHMETAFILE tocak,clio;
int clio_width = 645;
int clio_height = 260;
int tocak_width = 100;
int tocak_height = 100;

CCarBilboardView::CCarBilboardView()
{
	sky = new DImage();
	sky->Load((CString)"sky.jpg");
	
	tocak = GetEnhMetaFile((CString)"tocak.emf");
	clio = GetEnhMetaFile((CString)"clio.emf");
}

CCarBilboardView::~CCarBilboardView()
{
	delete sky;
	DeleteEnhMetaFile(tocak);
	DeleteEnhMetaFile(clio);
}

BOOL CCarBilboardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCarBilboardView drawing

void CCarBilboardView::OnDraw(CDC* pDC)
{
	CCarBilboardDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(&rect);
	
	CDC *memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	memDC->SelectObject(&bmp);
	
	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldTransform;
	memDC->GetWorldTransform(&oldTransform);
	
	//draw..	
	DrawAll(rect, memDC);
	//draw..
	
	memDC->SetWorldTransform(&oldTransform);
	memDC->SetGraphicsMode(oldMode);
	
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),memDC,0,0,SRCCOPY);
	
	memDC->DeleteDC();
	delete memDC;	
}

void CCarBilboardView::DrawAll(CRect rect, CDC *pDC)
{
	// crtanje pozadine
	sky->Draw(pDC, CRect(0,0,sky->Width(),sky->Height()), CRect(0, 0, rect.Width(), rect.Height()));
	
	// bilboard postolje
	int postolje_width = 200;
	int postolje_height = rect.Height() - (rect.Height() / 2 + clio_height / 2 + 40 + 40);
	
	int postoljeStartX = rect.Width() / 2  - postolje_width / 2;	
	int postoljeStartY = rect.Height() - postolje_height;
	int postoljeEndX = rect.Width() / 2 + postolje_width / 2;
	int postoljeEndY = rect.Height();

	CBrush blackBrush;
	blackBrush.CreateSolidBrush(RGB(0,0,0));	
	CBrush *oldBrush1 = pDC->SelectObject(&blackBrush);
	
	pDC->Rectangle(postoljeStartX,postoljeStartY,postoljeEndX,postoljeEndY);
	
	pDC->SelectObject(oldBrush1);
	blackBrush.DeleteObject();
	
	// sivi bilboard 
	int sivi_width = clio_width + 160;
	int sivi_height = clio_height + 160;
	
	int siviStartX = rect.Width() / 2 - sivi_width / 2;
	int siviStartY = rect.Height() / 2 - sivi_height / 2;
	int siviEndX = rect.Width() / 2 + sivi_width / 2;
	int siviEndY = rect.Height() / 2 + sivi_height / 2;
	
	// bilboard senka 1 - iza sivog dela
	CBrush blackBrush2;
	blackBrush2.CreateSolidBrush(RGB(0,0,0));
	CBrush *oldBrush2 = pDC->SelectObject(&blackBrush2);

	pDC->RoundRect(CRect(siviStartX+5,siviStartY+5,siviEndX+5,siviEndY+5),CPoint(10,10));
	
	pDC->SelectObject(oldBrush2);
	blackBrush2.DeleteObject();
	
	// bilboard sivi deo	
	CBrush sivaBrush;
	sivaBrush.CreateSolidBrush(RGB(68,68,68));
	CBrush *oldBrush3 = pDC->SelectObject(&sivaBrush);
	
	pDC->RoundRect(CRect(siviStartX, siviStartY, siviEndX, siviEndY), CPoint(10, 10));

	pDC->SelectObject(oldBrush3);
	sivaBrush.DeleteObject();
	
	// beli bilboard
	int beli_width = clio_width + 80;
	int beli_height = clio_height + 80;
	
	int beliStartX = rect.Width() / 2 - beli_width / 2;
	int beliStartY = rect.Height() / 2  - beli_height / 2;
	int beliEndX = rect.Width() / 2 + beli_width / 2;
	int beliEndY = rect.Height() / 2 + beli_height / 2;
	
	// bilboard senka 2 - iza belog dela
	CBrush blackBrush3;
	blackBrush3.CreateSolidBrush(RGB(0,0,0));
	CBrush *oldBrush4 = pDC->SelectObject(&blackBrush3);
	
	pDC->RoundRect(CRect(beliStartX+5,beliStartY+5,beliEndX+5,beliEndY+5),CPoint(10,10));
	
	pDC->SelectObject(oldBrush4);
	blackBrush3.DeleteObject();	
	
	// bilboard  beli deo
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(RGB(255,255,255));
	CBrush *oldBrush5 = pDC->SelectObject(&whiteBrush);
	
	pDC->RoundRect(CRect(beliStartX,beliStartY,beliEndX,beliEndY),CPoint(10,10));
	
	pDC->SelectObject(oldBrush5);
	whiteBrush.DeleteObject();
	
	// crtanje clio	
	int startClioX = rect.Width() / 2 - clio_width / 2 + move;
	int startClioY = rect.Height() / 2 - clio_height / 2;
	int endClioX = rect.Width() / 2 + clio_width / 2 + move;
	int endClioY = rect.Height() / 2 + clio_height / 2;	
	pDC->PlayMetaFile(clio,CRect(startClioX,startClioY,endClioX,endClioY));
	
	// Uglovi rotacije i transformacija
	float tmpAngle = move / (2 * 3.14);//ugao rotacije za predjeni put
	float angle = toRad(tmpAngle);
	XFORM tmpForm;
	pDC->GetWorldTransform(&tmpForm);
	
	// levi tocak	
	int levitStartX = rect.Width() / 2 - clio_width / 2 + 80 + move;
	int levitStartY = rect.Height() / 2 + 55;
	int levitEndX = levitStartX + tocak_width;
	int levitEndY = levitStartY + tocak_height;
	translateAndRotate(pDC, levitStartX + tocak_width / 2, levitStartY + tocak_height / 2, angle);
	pDC->PlayMetaFile(tocak,CRect(levitStartX,levitStartY,levitEndX,levitEndY));
	pDC->SetWorldTransform(&tmpForm);
	
	// ##### FIX #####
	// desni tocak
	int desnitStartX = rect.Width() / 2 + 175 + move;
	int desnitStartY = rect.Height() / 2 + 55;
	int desnitEndX = desnitStartX + tocak_width;
	int desnitEndY = desnitStartY + tocak_height;
	//translateAndRotate(pDC, desnitStartX + tocak_width / 2, desnitStartY + tocak_height / 2, angle);
	//Translate(pDC, desnitStartX + tocak_width / 2, desnitStartY + tocak_height / 2, false);
	Rotate(pDC, tmpAngle, false);
	Translate(pDC, desnitStartX + tocak_width / 2, desnitStartY + tocak_height / 2, true);
	pDC->PlayMetaFile(tocak, CRect(-tocak_width / 2, -tocak_height / 2, tocak_width / 2, tocak_height / 2));
	pDC->SetWorldTransform(&tmpForm);
}

void CCarBilboardView::translateAndRotate(CDC *pDC,float x, float y, float angle)
{
	XFORM form;
	form.eM11 = cos(angle);
	form.eM12 = sin(angle);
	form.eM21 = -sin(angle);
	form.eM22 = cos(angle);
	form.eDx = (float)(x - cos(angle)*x + sin(angle)*y);
	form.eDy = (float)(y - cos(angle)*y - sin(angle)*x);
	pDC->SetWorldTransform(&form);
}

void CCarBilboardView::Translate(CDC *pDC, float x, float y, bool right)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = x;
	form.eDy = y;
	if (right) {
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	}
	else
	{
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
	}
}

void CCarBilboardView::Rotate(CDC *pDC, float angle, bool right)
{
	XFORM form;
	form.eM11 = cos(toRad(angle));
	form.eM12 = sin(toRad(angle));
	form.eM21 = -sin(toRad(angle));
	form.eM22 = cos(toRad(angle));
	form.eDx = 0;
	form.eDy = 0;

	if (right)
		pDC->ModifyWorldTransform(&form, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}

float CCarBilboardView::toRad(float angle)
{
	return (angle*3.14)/180;
}

void CCarBilboardView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCarBilboardView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCarBilboardView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCarBilboardView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCarBilboardView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCarBilboardView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCarBilboardView diagnostics

#ifdef _DEBUG
void CCarBilboardView::AssertValid() const
{
	CView::AssertValid();
}

void CCarBilboardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCarBilboardDoc* CCarBilboardView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarBilboardDoc)));
	return (CCarBilboardDoc*)m_pDocument;
}
#endif //_DEBUG


// CCarBilboardView message handlers


void CCarBilboardView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_LEFT){
		move -= 5;
		Invalidate();
	}
	if(nChar == VK_RIGHT){
		move += 5;
		Invalidate();
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CCarBilboardView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
