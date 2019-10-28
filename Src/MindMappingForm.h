//MindMappingForm.h
#pragma comment(lib,"imm32.lib")
#ifndef _MINDMAPPINGFORM_H
#define _MINDMAPPINGFORM_H
#include "Scroll.h"
#include "Task.h"
#include "FindingForm.h"
#include "resource.h"
#include "Caret.h"


#define SCROLL_MAX 100
#define SCROLL_MIN 0
using namespace std;

class MindMap;
class Frame;
class Scroll;
class Task;
class MindMappingForm :public CFrameWnd {
public:
	MindMappingForm(CWnd *parent = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnInsertImageButtonClicked();
	void OnZoomInButtonClicked();
	void OnZoomOutButtonClicked();
	void OnCutButtonClicked();
	void OnCopyButtonClicked();
	void OnPasteButtonClicked();
	void OnMoveIconClicked();
	void OnFindButtonClicked();
	void OnUnDoButtonClicked();
	void OnReDoButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnNewButtonClicked();
	void OnDeleteButtonClicked();
	void OnEmphasizeButtonClicked();
	void OnComboBoxButtonClicked();
	void OnPrintButtonClicked();
	void OnSaveAsButtonClicked();
	void OnHelpButtonClicked();
	void OnInformationButtonClicked();

	void LeftRadialSort();
	void RightRadialSort();
	void RadialSort();
	// 더블버퍼링 처리
	void DrawImage();
	TCHAR* GetText()const;
	CPoint GetMousePtDown()const;
	CPoint GetMousePtUp()const;
	boolean GetMouseDown()const;
	boolean GetMouseMove()const;
	Scroll GetScroll()const;
	Frame* IsFrame(CPoint point);
	Frame* GetCurrent()const;
	float GetR()const;
public:
	MindMap *mindMap;
	FindingForm *findingForm;
	CDC memDC;
private:
	Frame *current;
	Frame *copyFrame;
	CComboBox directionType;
	/*CButton imageButton;
	CButton zoomInButton;
	CButton zoomOutButton;
	CButton cutButton;
	CButton copyButton;
	CButton pasteButton;
	CButton findButton;
	CButton unDoButton;
	CButton reDoButton;
	CButton loadButton;
	CButton saveButton;
	CButton newButton;
	CButton deleteButton;
	CButton emphasizeButton;
	CButton printButton;
	CButton saveAsButton;
	CButton helpButton;
	CButton informationButton;*/
	CPoint mousePtDown;
	CPoint mousePtMove;
	CPoint mousePtUp;
	Caret *caret;
	Scroll scroll;
	Task task;
	CScrollBar vscrollBar;
	CScrollBar hscrollBar;
	boolean mouseDown;
	boolean mouseMove;
	boolean mouseUp;
	boolean moveIcon;
	boolean mouseDouble;
	int currentIndex;
	float r;
	TCHAR(*text);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP() // 매크로
};
inline CPoint MindMappingForm::GetMousePtDown()const {
	return this->mousePtDown;
}
inline CPoint MindMappingForm::GetMousePtUp()const {
	return this->mousePtUp;
}
inline boolean MindMappingForm::GetMouseDown()const {
	return this->mouseDown;
}
inline boolean MindMappingForm::GetMouseMove()const {
	return this->mouseMove;
}
inline Scroll MindMappingForm::GetScroll()const {
	return this->scroll;
}
inline Frame* MindMappingForm::GetCurrent()const {
	return this->current;
}
inline float MindMappingForm::GetR()const {
	return this->r;
}
inline TCHAR* MindMappingForm::GetText()const {
	return this->text;
}
#endif // _MINDMAPPINGFORM_H
