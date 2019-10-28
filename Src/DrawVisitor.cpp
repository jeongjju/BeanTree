//DrawVisitor.cpp
#include "stdafx.h"
#include "DrawVisitor.h"
#include "TopicBox.h"
#include "Image.h"
#include "Frame.h"
#include "ConnectingLine.h"
#include "MindMappingForm.h"
#include "Scroll.h"



void DrawVisitor::VisitTopicBox(TopicBox *e) {
	
	MindMappingForm *mindMappingForm;
	mindMappingForm = (MindMappingForm*)AfxGetApp()->GetMainWnd();
	Scroll scroll = mindMappingForm->GetScroll();
	int x = scroll.GetX();
	int y = scroll.GetY();
	RECT rect;
	mindMappingForm->GetClientRect(&rect);
	//토픽상자를 출력한다
	mindMappingForm->memDC.Rectangle((e->GetX() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		(e->GetY() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
		(e->GetX() + e->GetWidth() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		(e->GetY() + e->GetHeight() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);

	CString cstr;
	cstr.Format(_T("%s"), e->GetTopic());
	//토픽을 출력한다.
	TopicBox *topicBox=0;
	TCHAR(*topic);
	if (mindMappingForm->GetCurrent() != 0) {

		int i = 0;
		while (i < mindMappingForm->GetCurrent()->GetLength()) {
			Shape *shape = mindMappingForm->GetCurrent()->GetChild(i);
			if (dynamic_cast<TopicBox*>(shape)) {
				topicBox = (TopicBox*)shape;
				topic = topicBox->GetTopic();
			}
			i++;
		}
		if (e == topicBox) {

			if (mindMappingForm->GetText() != 0) {
				cstr.Format(_T("%s"), mindMappingForm->GetText());
				mindMappingForm->memDC.DrawText(cstr, CRect((e->GetX() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetY() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
					(e->GetWidth() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetHeight() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2),
					DT_LEFT | DT_NOCLIP);

			}
			else {
				mindMappingForm->memDC.DrawText(cstr, CRect((e->GetX() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetY() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
					(e->GetWidth() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetHeight() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2),
					DT_LEFT | DT_NOCLIP);
			}
		}
	}
	mindMappingForm->memDC.DrawText(cstr, CRect((e->GetX() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
			(e->GetY() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
			(e->GetWidth() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
			(e->GetHeight() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2),
			DT_LEFT | DT_NOCLIP);

	// 찾기
	FindingForm *findingForm = mindMappingForm->findingForm;
	if (findingForm != 0) {
		if (findingForm->GetFirstIndex() != 0) {
			TopicBox* (*indexes) = findingForm->GetIndexes();
			int index = findingForm->GetIndex();
			cstr.Format(_T("%d %d"), findingForm->GetFirstIndex(), findingForm->GetLastIndex());
			if (e == indexes[index]) {
				mindMappingForm->memDC.DrawText(cstr, CRect((e->GetX() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetY() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2 - 5,
					(e->GetWidth() - x - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
					(e->GetHeight() - y - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2 - 5),
					DT_LEFT | DT_NOCLIP);
			}
		}
	}
}

void DrawVisitor::VisitImage(Image *e) {
	MindMappingForm *mindMappingForm;
	mindMappingForm = (MindMappingForm*)AfxGetApp()->GetMainWnd();
	Scroll scroll = mindMappingForm->GetScroll();
	CImage image;
	CString imagePath;
	imagePath = CString(e->GetImagePath().c_str());
	image.Load(imagePath);
	CDC memDC;
	memDC.CreateCompatibleDC(&mindMappingForm->memDC);
	CBitmap bitMap;
	CBitmap *oldBitMap;
	bitMap.LoadBitmap(imagePath);
	oldBitMap = memDC.SelectObject(&bitMap);
	mindMappingForm->memDC.StretchBlt(e->GetX() - scroll.GetX(), e->GetY() - scroll.GetY(), e->GetWidth(), e->GetHeight(), &memDC, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
	memDC.SelectObject(oldBitMap);
	//이미지를 출력한다.
	image.Draw(mindMappingForm->memDC.m_hDC, e->GetX() - scroll.GetX(), e->GetY() - scroll.GetY(), e->GetWidth(), e->GetHeight());
}

void DrawVisitor::VisitFrame(Frame *e) {
	MindMappingForm *mindMappingForm;
	mindMappingForm = (MindMappingForm*)AfxGetApp()->GetMainWnd();
	Scroll scroll = mindMappingForm->GetScroll();
	int scrollX = scroll.GetX();
	int scrollY = scroll.GetY();
	int x = e->GetX();
	int y = e->GetY();
	int width = e->GetWidth();
	int height = e->GetHeight();
	RECT rect;
	mindMappingForm->GetClientRect(&rect);
	//테두리를 그린다.
	mindMappingForm->memDC.Ellipse((x - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		(y - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
		(x + width - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		(y + height - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);	
	CPoint cPoint = mindMappingForm->GetMousePtDown();
	Frame *current = mindMappingForm->GetCurrent();
	if (current!=0) {
		DWORD style[] = { 3,3 };
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(192, 0, 0);
		CPen newPen;
		newPen.CreatePen(PS_GEOMETRIC | PS_USERSTYLE | PS_ENDCAP_FLAT | PS_JOIN_MITER, 3, &lb, 2, style);
		CPen *pOldPen = mindMappingForm->memDC.SelectObject(&newPen);
		RECT rect;
		mindMappingForm->GetClientRect(&rect);
		//선택선을 그린다
		CBrush brush;
		brush.CreateStockObject(NULL_BRUSH);
		CBrush *pOldBrush = mindMappingForm->memDC.SelectObject(&brush);
		mindMappingForm->memDC.Ellipse((current->GetX() - scrollX - rect.right / 2)*mindMappingForm->GetR()+ rect.right / 2,
			(current->GetY() - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
			(current->GetX() + current->GetWidth() - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
			(current->GetY() + current->GetHeight() - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);
		mindMappingForm->memDC.SelectObject(pOldPen);
		mindMappingForm->memDC.SelectObject(pOldBrush);
		
		//이동박스를 그린다
		int moveIconLeft = mindMappingForm->GetCurrent()->GetX()+ mindMappingForm->GetCurrent()->GetWidth()/2-7;
		int moveIconTop = mindMappingForm->GetCurrent()->GetY();
		int moveIconRight = mindMappingForm->GetCurrent()->GetX()+mindMappingForm->GetCurrent()->GetWidth()/2-7+14;
		int moveIconBottom = mindMappingForm->GetCurrent()->GetY()+14;
		CRect moveRect = CRect(moveIconLeft, moveIconTop, moveIconRight, moveIconBottom);

		mindMappingForm->memDC.Rectangle((moveRect.left - scrollX - rect.right / 2)*mindMappingForm->GetR() +rect.right/2,
		(moveRect.top - scrollY - rect.bottom / 2)*mindMappingForm->GetR()+rect.bottom/2,
			(moveRect.right - scrollX - rect.right / 2)*mindMappingForm->GetR()+rect.right/2,
			(moveRect.bottom - scrollY - rect.bottom / 2)*mindMappingForm->GetR()+rect.bottom/2);
	}
	else {
	}
	if (e->GetEmphasize() == true) {
		// 빈 사각형으로 강조하기 표시
		DWORD style[] = { 3,3 };
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(051, 153, 255);
		CPen newPen;
		newPen.CreatePen(PS_GEOMETRIC | PS_USERSTYLE | PS_ENDCAP_FLAT | PS_JOIN_MITER, 3, &lb, 2, style);
		CPen *pOldPen = mindMappingForm->memDC.SelectObject(&newPen);
		CBrush brush;
		brush.CreateStockObject(NULL_BRUSH);
		CBrush *pOldBrush = mindMappingForm->memDC.SelectObject(&brush);
		//CRect rect((x - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		//	(y - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
		//	(x + width - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
		//	(y + height - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);
		mindMappingForm->memDC.Rectangle((x - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
			(y - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2,
			(x + width - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2,
			(y + height - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);
		mindMappingForm->memDC.SelectObject(pOldPen);
		mindMappingForm->memDC.SelectObject(pOldBrush);
	}
}

void DrawVisitor::VisitConnectingLine(ConnectingLine *e) {

	MindMappingForm *mindMappingForm;
	mindMappingForm = (MindMappingForm *)AfxGetApp()->GetMainWnd();
	Scroll scroll = mindMappingForm->GetScroll();
	int scrollX = scroll.GetX();
	int scrollY = scroll.GetY();
	int x = e->GetX();
	int y = e->GetY();
	int width = e->GetWidth();
	int height = e->GetHeight();
	int childX = e->GetChildX();
	int childY = e->GetChildY();

	RECT rect;
	mindMappingForm->GetClientRect(&rect);
	//연결선을 그린다
	mindMappingForm->memDC.MoveTo((x - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2, (y - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);
	mindMappingForm->memDC.LineTo((childX - scrollX - rect.right / 2)*mindMappingForm->GetR() + rect.right / 2, (childY - scrollY - rect.bottom / 2)*mindMappingForm->GetR() + rect.bottom / 2);
}