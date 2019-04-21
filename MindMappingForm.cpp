//MindMappingForm.cpp
#include"MindMappingForm.h"
#include"InsertingImageForm.h"
#include"FindingForm.h"
#include"MindMap.h"
#include"Frame.h"
#include"TopicBox.h"
#include"DrawVisitor.h"
#include"ConnectingLine.h"
#include"InformationForm.h"
#include<iostream>
#include<atlimage.h>
#include<afxdlgs.h>
#include<Windows.h>
#include<imm.h>

BEGIN_MESSAGE_MAP(MindMappingForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	//파일
	ON_BN_CLICKED(1, &MindMappingForm::OnNewButtonClicked)
	ON_BN_CLICKED(2, &MindMappingForm::OnLoadButtonClicked)
	ON_BN_CLICKED(3, &MindMappingForm::OnSaveButtonClicked)
	ON_BN_CLICKED(4, &MindMappingForm::OnSaveAsButtonClicked)
	ON_BN_CLICKED(5, &MindMappingForm::OnPrintButtonClicked)
	ON_BN_CLICKED(6, &MindMappingForm::OnClose)
	//편집
	ON_BN_CLICKED(7, &MindMappingForm::OnCutButtonClicked)
	ON_BN_CLICKED(8, &MindMappingForm::OnCopyButtonClicked)
	ON_BN_CLICKED(9, &MindMappingForm::OnPasteButtonClicked)
	ON_BN_CLICKED(10, &MindMappingForm::OnUnDoButtonClicked)
	ON_BN_CLICKED(11, &MindMappingForm::OnReDoButtonClicked)
	ON_BN_CLICKED(12, &MindMappingForm::OnEmphasizeButtonClicked)
	ON_BN_CLICKED(13, &MindMappingForm::OnInsertImageButtonClicked)

	//도구
	ON_BN_CLICKED(14, &MindMappingForm::OnFindButtonClicked)
	ON_BN_CLICKED(15, &MindMappingForm::OnZoomInButtonClicked)
	ON_BN_CLICKED(16, &MindMappingForm::OnZoomOutButtonClicked)

	//정렬
	ON_BN_CLICKED(17, &MindMappingForm::LeftRadialSort)
	ON_BN_CLICKED(18, &MindMappingForm::RightRadialSort)
	ON_BN_CLICKED(19, &MindMappingForm::RadialSort)

	// 도움말
	ON_BN_CLICKED(20, &MindMappingForm::OnHelpButtonClicked)
	ON_BN_CLICKED(21, &MindMappingForm::OnInformationButtonClicked)







	//ON_CBN_SELCHANGE(109, OnComboBoxButtonClicked)







END_MESSAGE_MAP()

MindMappingForm::MindMappingForm(CWnd *parent) :CFrameWnd()
{
	this->mindMap = NULL;
	this->current = 0;
	this->copyFrame = 0;
	this->mousePtDown = 0;
	this->mousePtUp = 0;
	this->mouseDown = false;
	this->mouseMove = false;
	this->mouseUp = false;
	this->moveIcon = false;
	this->mouseDouble = false;
	this->r = 1;
	this->findingForm = 0;
	this->caret = new Caret;
	this->text = 0;
}

// 윈도우가 생성 되었을 때
int MindMappingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	RECT rect;
	GetClientRect(&rect);

	// 1. 수직스크롤을 만든다
	RECT vertRect;
	vertRect = rect;
	vertRect.left = vertRect.right - 20;
	vertRect.bottom = vertRect.bottom - 20;
	vscrollBar.Create(WS_CHILD | WS_BORDER | SBS_VERT, vertRect, this, 4001);
	vscrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
	vscrollBar.SetScrollRange(0, 100);
	vscrollBar.SetScrollPos(0);

	// 2. 수평스크롤을 만든다
	RECT horzRect;
	horzRect = rect;
	horzRect.right = horzRect.right - 20;
	horzRect.top = horzRect.bottom - 20;
	hscrollBar.Create(WS_CHILD | WS_BORDER | SBS_HORZ, horzRect, this, 4002);
	hscrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
	hscrollBar.SetScrollRange(0, 100);
	hscrollBar.SetScrollPos(0);

	CMenu menuMain;
	menuMain.CreateMenu();
	CMenu menuPopup;
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING, 1, _T("새로 만들기"));
	menuPopup.AppendMenuW(MF_STRING, 2, _T("열기"));
	menuPopup.AppendMenuW(MF_STRING, 3, _T("저장"));
	menuPopup.AppendMenuW(MF_STRING, 4, _T("다른 이름으로 저장"));
	menuPopup.AppendMenuW(MF_STRING, 5, _T("인쇄"));
	menuPopup.AppendMenuW(MF_STRING, 6, _T("끝내기"));
	menuMain.AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("파일"));
	SetMenu(&menuMain);
	menuMain.Detach();

	// 편집
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 7, _T("잘라내기"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 8, _T("복사하기"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 9, _T("붙혀넣기"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 10, _T("실행취소"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 11, _T("다시실행"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 12, _T("강조하기"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 13, _T("이미지삽입"));


	CMenu *pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("편집"));

	//도구
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 14, _T("찾기"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 15, _T("확대"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 16, _T("축소"));

	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("도구"));

	//정렬
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 17, _T("왼쪽 방사형"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 18, _T("오른쪽 방사형"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 19, _T("방사형"));

	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("정렬"));


	//도움말
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 20, _T("도움말"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 21, _T("정보"));
	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("기타"));

	//// 3. 확대 버튼을 만든다
	////zoomInButton.Create(_T(" 확대 "), BS_DEFPUSHBUTTON, CRect(100, 100, 200, 200), this, 101);
	//// 4. 축소 버튼을 만든다
	//zoomOutButton.Create(_T(" 축소 "), BS_DEFPUSHBUTTON, CRect(200, 100, 300, 200), this, 102);
	//// 5. 잘라내기 버튼을 만든다
	//cutButton.Create(_T(" 잘라내기 "), BS_DEFPUSHBUTTON, CRect(0, 100, 100, 200), this, 103);
	//// 6. 복사하기 버튼을 만든다
	//copyButton.Create(_T(" 복사하기 "), BS_DEFPUSHBUTTON, CRect(0, 200, 100, 300), this, 104);
	//// 7. 붙여넣기 버튼을 만든다
	//pasteButton.Create(_T(" 붙여넣기 "), BS_DEFPUSHBUTTON, CRect(0, 300, 100, 400), this, 105);
	//// 8. 이미지 삽입 버튼을 만든다
	//imageButton.Create(_T("이미지삽입"), BS_DEFPUSHBUTTON, CRect(100,200,200,300), this, 100);
	//// 9. 찾기 버튼을 만든다
	//findButton.Create(_T("찾다"), BS_DEFPUSHBUTTON, CRect(0, 0, 100, 100), this, 106);
	//// 10. 실행취소 버튼을 만든다
	//unDoButton.Create(_T("실행취소"), BS_DEFPUSHBUTTON, CRect(100, 0, 200, 100), this, 107);
	//// 11. 다시실행 버튼을 만든다
	//reDoButton.Create(_T("다시실행"), BS_DEFPUSHBUTTON, CRect(200, 0, 300, 100), this, 108);
	//// 12. 진행방향 콤보박스를 만든다
	//directionType.Create(CBS_DROPDOWNLIST ,CRect(300,0,500,100),this,109);
	//// 13. 진행방향을 준비한다
	//directionType.AddString(_T("왼쪽방사형정렬"));
	//directionType.AddString(_T("오른쪽방사형정렬"));
	//directionType.AddString(_T("방사형정렬"));

	//// 14. 저장 버튼을 만든다.
	//loadButton.Create(_T("저장"), BS_DEFPUSHBUTTON, CRect(0, 400, 100, 500), this, 110);
	//// 15. 불러오기 버튼을 만든다.
	//saveButton.Create(_T("불러오기"), BS_DEFPUSHBUTTON, CRect(100, 400, 200, 500), this, 111);
	//// 16. 새로만들기 버튼을 만든다.
	//newButton.Create(_T("새로만들기"), BS_DEFPUSHBUTTON, CRect(200, 400, 300, 500), this, 112);
	//// 17. 삭제하기 버튼을 만든다.
	//deleteButton.Create(_T("삭제"), BS_DEFPUSHBUTTON, CRect(200, 200, 300, 300), this, 113);
	//// 18. 강조하기 버튼을 만든다.
	//emphasizeButton.Create(_T("강조하기"), BS_DEFPUSHBUTTON, CRect(100, 300, 200, 400), this, 114);
	//// * 인쇄하기 버튼을 만든다.
	//printButton.Create(_T("인쇄하기"), BS_DEFPUSHBUTTON, CRect(200, 300, 300, 400), this, 115);
	//// * 다른이름으로저장 버튼을 만든다.
	//saveAsButton.Create(_T("다른이름으로 저장"), BS_DEFPUSHBUTTON, CRect(0, 500, 100, 600), this, 116);
	//// * 도움말 버튼을 만든다
	//helpButton.Create(_T("도움말"), BS_DEFPUSHBUTTON,CRect(100,500,200,600),this,117);
	//// * 정보 버튼을 만든다
	//informationButton.Create(_T("정보"), BS_DEFPUSHBUTTON, CRect(200, 500, 300, 600), this, 118);
	// 19. 마인드맵을 만든다
	this->mindMap = new MindMap;
	// 20. 중심 테두리를 만든다
	Frame *centerFrame = this->mindMap->InsertCenterFrame(rect.right / 2 - 65, rect.bottom / 2 - 65, 130, 130);
	// 21. 토픽 상자를 만든다
	centerFrame->Add(new TopicBox(rect.right / 2 - 45, rect.bottom / 2 - 20, 90, 40, _T("중심토픽")));

	return 0;
}

BOOL MindMappingForm::PreTranslateMessage(MSG *pMsg) {

	//if (pMsg->message == WM_KEYDOWN) {
	//	//1. 프레임이 선택된 상태에서 Delete 키를 눌렀을 경우 
	//	if (pMsg->message == 256 && this->current != 0) {
	//		// 1.1. 선택한 테두리를 읽는다
	//		if (this->current != this->mindMap->GetCenterFrame()) {
	//			Frame *highRankFrame = mindMap->HighRankFrame(current);
	//			// 1.2. 마지막 작업을 적는다.
	//			task.UnDoWrite(mindMap);
	//			// 1.3 선택한 테두리를 지운다.
	//			highRankFrame->Remove(current);
	//			this->current = highRankFrame;
	//		}
	//	}
	//	//2. Ctrl + X 키를 눌렀을 경우
	//	else if (pMsg->message == 0x58 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 2.1 자를 토픽을 정한다
	//		if (this->current != 0) {
	//			// 2.2 정한 토픽과 하위 토픽들을 옮겨 적는다
	//			this->copyFrame = new Frame(*this->current);
	//			Frame *highRankFrame = this->mindMap->HighRankFrame(this->current);
	//			// 2.3 마지막 작업을 적는다
	//			task.UnDoWrite(mindMap);
	//			// 2.4 정한 토픽과 하위 토픽들을 지운다.
	//			highRankFrame->Remove(this->current);
	//			this->current = 0;
	//		}
	//	}
	//	//3. Ctrl + C 키를 눌렀을 경우
	//	else if (pMsg->message == 0x43 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 3.1 복사할 토픽을 정한다
	//		if (this->current != 0) {
	//			// 3.2 정한 토픽과 하위토픽들을 옮겨 적는다.
	//			this->copyFrame = new Frame(*this->current);
	//		}
	//	}
	//	//4. Ctrl + V 키를 눌렀을 경우
	//	else if (pMsg->message == 0x56 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 4.1 붙여넣을 토픽을 정한다.
	//		if (this->current != 0) {
	//			// 4.2 옮길 위치를 정한다. (방사형으로 배치하도록 작업 필요)
	//			// 4.3 옮겨 놓은 토픽들을 정한 위치에 옮겨 적는다
	//			// 4.4 옮긴 토픽 테두리와 정한 테두리 사이에 관계선을 그린다 (기존 관계선 지우는 작업 필요)
	//			this->current->Add(this->copyFrame);
	//		}
	//	}
	//	// 5. Ctrl+Z 키를 눌렀을 경우
	//	else if ((pMsg->message == 0x5A && GetKeyState(VK_CONTROL) & 0x80000000)) {
	//		//5.1. 불러오기 전의 마지막 작업을 저장한다.
	//		//5.2. 가장 최근 작업을 불러온다.
	//		if (task.GetUnDoLength() > 0) {
	//			task.ReDoWrite(this->mindMap);
	//			MindMap *unDoMindMap = task.UnDoRead();
	//			*this->mindMap = *unDoMindMap;
	//			delete unDoMindMap;
	//		}
	//	}
	//	// 6. Ctrl+Y 키를 눌렀을 경우
	//	else if ((pMsg->message == 0x59 && GetKeyState(VK_CONTROL) & 0x80000000)) {

	//		//6.2.1. 불러오기전의 마지막 작업을 불러온다. 
	//		if (task.GetReDoLength() > 0) {
	//			task.UnDoWrite(this->mindMap);
	//			MindMap *reDoMindMap = task.ReDoRead();
	//			*this->mindMap = *reDoMindMap;
	//			delete reDoMindMap;
	//		}
	//	}
	//	else if (pMsg->message == 0x46 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		if (findingForm != 0) {
	//			findingForm->SetFocus();
	//		}
	//		else {
	//			findingForm = new FindingForm;
	//			findingForm->Create(IDD_FINDINGFORM, this);
	//			findingForm->ShowWindow(SW_SHOW);
	//		}
	//	}
	//	else if (pMsg->message == VK_LEFT) {
	//		if (this->current != 0 && mouseDouble == true) {
	//			CSize size;
	//			CPoint point;
	//			CString str;
	//			CClientDC dc(this);
	//			TopicBox *topicBox = 0;
	//			TCHAR(*topic) = 0;
	//			TCHAR(*temp) = 0;
	//			TCHAR(*temp2) = 0;
	//			TCHAR(*temp3) = 0;
	//			int currentIndex;
	//			int x;
	//			int y;
	//			int width;
	//			int height;
	//			int i = 0;
	//			int j = 0;
	//			int k = 0;
	//			int l = 0;
	//			int m = 0;
	//			int o = 0;
	//			int previousIndex;//문자열 배열에서 전전 '\r'의 위치
	//			int rCount = 0;//\r의 개수
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//프레임에서 토픽박스를 찾는다.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					//토픽에서 현재 위치를 읽는다.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					Array<int> tempIndex(currentIndex + 1);//'\r'의 위치들을 담은 정수배열
	//														   //토픽에 아무것도없는 경우
	//					if (currentIndex == 0) {

	//					}
	//					//토픽이 채워진 경우
	//					else {
	//						temp = new TCHAR[currentIndex + 1];
	//						temp[0] = topic[currentIndex - 1];
	//						temp[1] = L'\0';
	//						//현재 위치의 왼쪽이 문자인 경우
	//						if (temp[0] != L'\r') {
	//							str.Format(_T("%s"), temp);
	//							size = dc.GetTextExtent(str);
	//							caret->Move(caret->GetX() - size.cx, caret->GetY());
	//						}
	//						//현재 위치의 왼쪽이 enter('\r')인 경우
	//						else {
	//							//enter('\r')의 개수를 센다.
	//							while (j <= topicBox->GetLength()) {
	//								if (topic[j] == L'\r') {
	//									rCount++;
	//									tempIndex[l] = j;
	//									l++;
	//									//현재 위치 왼쪽의 '\r'을 찾은 경우
	//									if (j == currentIndex - 1) {
	//										//현재 위치 왼쪽의 '\r'이 첫 번째인 경우
	//										if (rCount == 1) {
	//											//현재 캐럿 윗줄이 공백인 경우
	//											if (currentIndex - 1 == 0) {
	//												str.Format(_T("%s"), temp);
	//												size = dc.GetTextExtent(str);
	//												caret->Move(caret->GetX(), caret->GetY() - size.cy);
	//											}
	//											//현재 캐럿 윗줄이 문자인 경우
	//											else {
	//												temp2 = new TCHAR[currentIndex + 1];
	//												while (k < currentIndex - 1) {
	//													temp2[k] = topic[k];
	//													k++;
	//												}
	//												temp2[k] = L'\0';
	//												str.Format(_T("%s"), temp2);
	//												size = dc.GetTextExtent(str);
	//												caret->Move(caret->GetX() + size.cx, caret->GetY() - size.cy);
	//											}
	//										}
	//										//현재 위치 왼쪽의 '\r'이 첫 번째가 아닌 경우
	//										else {
	//											temp2 = new TCHAR[currentIndex + 1];
	//											temp2[0] = topic[currentIndex - 2];
	//											temp2[1] = L'\0';
	//											//현재 캐럿 윗줄이 공백인 경우
	//											if (temp2[0] == L'\r') {
	//												str.Format(_T("%s"), temp2);
	//												size = dc.GetTextExtent(str);
	//												caret->Move(caret->GetX(), caret->GetY() - size.cy);
	//											}
	//											//현재 캐럿 윗줄이 문자인 경우
	//											else {
	//												while (m < tempIndex.GetLength()) {
	//													if (tempIndex[m] == currentIndex - 1) {
	//														previousIndex = tempIndex[m - 1] + 1;
	//														while (previousIndex < currentIndex - 1) {
	//															temp3 = new TCHAR[currentIndex + 1];
	//															temp3[o] = topic[previousIndex];
	//															o++;
	//															previousIndex++;
	//														}
	//														temp3[o] = L'\0';
	//														str.Format(_T("%s"), temp3);
	//														size = dc.GetTextExtent(str);
	//														caret->Move(caret->GetX() + size.cx, caret->GetY() - size.cy);
	//													}
	//													m++;
	//												}
	//											}
	//										}
	//									}
	//								}
	//								j++;
	//							}

	//						}
	//						topicBox->Modify(x, y, width, height, topic, currentIndex - 1);

	//					}

	//				}
	//				i++;
	//			}

	//		}
	//	}
	//	else if (pMsg->message == VK_RIGHT) {
	//		if (this->current != 0 && mouseDouble == true) {
	//			CSize size;
	//			CPoint point;
	//			CString str;
	//			CClientDC dc(this);
	//			TopicBox *topicBox = 0;
	//			TCHAR(*topic) = 0;
	//			TCHAR(*temp) = 0;
	//			int currentIndex;
	//			int x;
	//			int y;
	//			int width;
	//			int height;
	//			int i = 0;
	//			int j = 0;
	//			int rCount = 0;//\r의 개수
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//프레임에서 토픽박스를 찾는다.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					//토픽에서 현재 위치를 읽는다.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					Array<int> tempIndex(currentIndex + 1);//'\r'의 위치들을 담은 정수배열
	//														   //현재 위치가 '\0'인 경우 
	//					if (topic[currentIndex] == _T('\0')) {

	//					}
	//					//현재 위치가 '\r'인 경우
	//					else if (topic[currentIndex] == _T('\r')) {
	//						while (topic[j] != _T('\0') && j != currentIndex + 1) {
	//							if (topic[j] == _T('\r')) {
	//								rCount++;
	//							}
	//							j++;
	//						}
	//						str.Format(_T("%s"), topic);
	//						size = dc.GetTextExtent(str);
	//						caret->Move(x, y + size.cy*rCount);
	//						topicBox->Modify(x, y, width, height, topic, currentIndex + 1);
	//					}
	//					//현재 위치가 문자인 경우
	//					else {

	//						temp = new TCHAR[currentIndex + 2];
	//						temp[0] = topic[currentIndex];
	//						temp[1] = _T('\0');
	//						str.Format(_T("%s"), temp);
	//						size = dc.GetTextExtent(str);
	//						caret->Move(caret->GetX() + size.cx, caret->GetY());
	//						topicBox->Modify(x, y, width, height, topic, currentIndex + 1);
	//					}
	//				}
	//				i++;
	//			}
	//		}
	//	}
	//	else if (pMsg->message == VK_UP) {
	//		if (this->current != 0 && mouseDouble == true) {
	//			CSize firstSize;//토픽에서 현재 위치(currentIndex)와 왼쪽 방향으로 가장 가까운 엔터 사이의 길이
	//			CSize secondSize;//현재위치 왼쪽 방향으로 두번째 엔터와 첫번째 엔터 사이의 길이
	//			CSize thirdSize;
	//			CPoint point;
	//			CString str;
	//			CString cStr;
	//			CClientDC dc(this);
	//			TopicBox *topicBox = 0;
	//			TCHAR(*topic) = 0;
	//			TCHAR(*temp) = 0;
	//			TCHAR(*temp2) = 0;
	//			int currentIndex;
	//			int x;
	//			int y;
	//			int width;
	//			int height;
	//			int i = 0;
	//			int j = 0;//firsSize길이를 재기위한 반복제어 변수
	//			int k = 0;//첨자 제어 변수
	//			int l = 0;//secondSize길이를 재기위한 반복제어 변수
	//			int m;
	//			int rCount = 0;//\r의 개수
	//			int firstR;//현재 위치로부터 가장 가까운 \r의 위치
	//			int secondR;//현재 위치로부터 두번째로 가까운 \r의 위치
	//			int tempIndex;//현재 위치를 초기값으로 갖는 반복제어 변수
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//프레임에서 토픽박스를 찾는다.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					CString ccstr;
	//					ccstr.Format(_T("%s"), topic);
	//					CSize ccsize = dc.GetTextExtent(ccstr);
	//					//토픽에서 현재 위치를 읽는다.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					tempIndex = currentIndex - 1;
	//					//현재 위치로부터 왼쪽 방향에 있는 \r 2개를 찾는다.
	//					while (tempIndex >= 0 && rCount < 2) {
	//						if (topic[tempIndex] == _T('\r')) {
	//							if (rCount == 0) {
	//								firstR = tempIndex;
	//							}
	//							else {
	//								secondR = tempIndex;
	//							}
	//							rCount++;
	//						}
	//						tempIndex--;
	//					}
	//					secondSize.cx = 0;
	//					secondSize.cy = 0;
	//					temp = new TCHAR[currentIndex];
	//					if (rCount == 0) {

	//					}
	//					else {
	//						j = firstR;;
	//						j++;
	//						while (j < currentIndex) {
	//							temp[k] = topic[j];
	//							k++;
	//							j++;
	//						}
	//						temp[k] = _T('\0');
	//						str.Format(_T("%s"), temp);
	//						firstSize = dc.GetTextExtent(str);
	//						thirdSize.cx = 0;
	//						thirdSize.cy = 0;
	//						//엔터가 두개 일때
	//						if (rCount == 2) {
	//							//첫 번째 엔터에서 현재 위치까지의 길이를 잰다.
	//							l = secondR;
	//							l++;
	//							//두 번째 엔터와 첫 번째 엔터 사이의 길이를 재고 비교한다.
	//							while (secondSize.cx < firstSize.cx && l != firstR) {
	//								m = 0;
	//								if (l < firstR) {
	//									temp2 = new TCHAR[currentIndex];
	//									temp2[m] = topic[l];
	//									temp2[m + 1] = _T('\0');
	//									cStr.Format(_T("%s"), temp2);
	//									thirdSize = dc.GetTextExtent(cStr);
	//									l++;
	//								}
	//								if (temp2 != 0) {
	//									delete[] temp2;
	//									temp2 = 0;
	//								}
	//								secondSize.cx = secondSize.cx + thirdSize.cx;
	//							}
	//							caret->Move(x + secondSize.cx, caret->GetY() - ccsize.cy);
	//							topicBox->Modify(x, y, width, height, topic, l);
	//						}
	//						//엔터가 한 개 일때
	//						if (rCount == 1) {
	//							//두번째 길이를 재고 첫번째 길이와 비교한다.
	//							while (secondSize.cx < firstSize.cx && l != firstR) {
	//								m = 0;
	//								if (l < firstR) {
	//									temp2 = new TCHAR[currentIndex];
	//									temp2[m] = topic[l];
	//									temp2[m + 1] = _T('\0');
	//									cStr.Format(_T("%s"), temp2);
	//									thirdSize = dc.GetTextExtent(cStr);
	//									l++;
	//								}
	//								if (temp2 != 0) {
	//									delete[] temp2;
	//									temp2 = 0;
	//								}
	//								secondSize.cx = secondSize.cx + thirdSize.cx;
	//							}
	//							caret->Move(x + secondSize.cx, caret->GetY() - ccsize.cy);
	//							topicBox->Modify(x, y, width, height, topic, l);
	//						}
	//					}
	//				}
	//				i++;
	//			}
	//		}

	//	}
	//	else if (pMsg->message == VK_DOWN) {
	//		if (this->current != 0 && mouseDouble == true) {
	//			CSize leftSize;//토픽에서 현재 위치(currentIndex)와 왼쪽 방향으로 가장 가까운 엔터 사이의 길이
	//			CSize rightSize;//현재위치에서 오른쪽 엔터 이후의 길이
	//			CSize thirdSize;
	//			CPoint point;
	//			CString str;
	//			CString cStr;
	//			CClientDC dc(this);
	//			TopicBox *topicBox = 0;
	//			TCHAR(*topic) = 0;
	//			TCHAR(*temp) = 0;
	//			TCHAR(*temp2) = 0;
	//			int currentIndex;
	//			int x;
	//			int y;
	//			int width;
	//			int height;
	//			int i = 0;
	//			int j = 0;//firsSize길이를 재기위한 반복제어 변수
	//			int k = 0;//첨자 제어 변수
	//			int l = 0;//secondSize길이를 재기위한 반복제어 변수
	//			int m;
	//			int rCount = 0;//현재 위치 오른쪽 \r의 개수
	//			int lCount = 0;//현재 위치 왼쪽 엔터의 개수
	//			int leftR;//현재 위치에서 왼쪽에 있는 \r의 위치
	//			int rightR;//현재 위치에서 오른쪽에 있는 \r의 위치
	//			int tempIndex;//현재 위치를 초기값으로 갖는 반복제어 변수
	//			int tempIndex2;
	//			bool R = false;
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//프레임에서 토픽박스를 찾는다.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					CString ccstr;
	//					ccstr.Format(_T("%s"), topic);
	//					CSize ccsize = dc.GetTextExtent(ccstr);
	//					//토픽에서 현재 위치를 읽는다.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					tempIndex = currentIndex;
	//					tempIndex2 = currentIndex - 1;
	//					rightSize.cx = 0;
	//					rightSize.cy = 0;
	//					thirdSize.cx = 0;
	//					thirdSize.cy = 0;
	//					//현재 위치 오른쪽에 엔터가 있는지 확인

	//					while (topic[tempIndex] != _T('\0') && rCount != 1) {
	//						if (topic[tempIndex] == _T('\r')) {
	//							rightR = tempIndex;
	//							rCount++;
	//						}
	//						tempIndex++;
	//					}
	//					//현재 위치 오른쪽에 엔터가 있는 경우
	//					if (rCount == 1) {
	//						//현재 위치 왼쪽에 엔터가 있는지 확인한다.
	//						while (tempIndex2 >= 0 && lCount != 1) {
	//							if (topic[tempIndex2] == _T('\r')) {
	//								leftR = tempIndex2;
	//								lCount++;
	//							}
	//							tempIndex2--;
	//						}
	//						//현재 위치 왼쪽에 엔터가 있는 경우
	//						if (lCount == 1) {
	//							j = leftR;
	//							j++;
	//							temp = new TCHAR[topicBox->GetLength()];
	//							while (j < currentIndex) {
	//								temp[k] = topic[j];
	//								k++;
	//								j++;
	//							}
	//							temp[k] = _T('\0');
	//							str.Format(_T("%s"), temp);
	//							leftSize = dc.GetTextExtent(str);

	//						}
	//						//현재 위치 바로 왼쪽이 엔터인 경우 
	//						else if (topic[currentIndex - 1] == _T('\r')) {

	//						}
	//						//현재 위치 왼쪽에 엔터가 없는 경우
	//						else {
	//							temp = new TCHAR[topicBox->GetLength()];
	//							while (j < currentIndex) {
	//								temp[k] = topic[j];
	//								k++;
	//								j++;
	//							}
	//							temp[k] = _T('\0');
	//							str.Format(_T("%s"), temp);
	//							leftSize = dc.GetTextExtent(str);

	//						}
	//						//현재 위치 왼쪽 길이와 오른쪽 엔터 이후의 길이를 비교한다.
	//						l = rightR;
	//						l++;
	//						while (rightSize.cx < leftSize.cx && topic[l] != _T('\0') && topic[l] != _T('\r')) {
	//							m = 0;
	//							if (topic[l] != _T('\0')) {
	//								temp2 = new TCHAR[topicBox->GetLength()];
	//								temp2[m] = topic[l];
	//								temp2[m + 1] = _T('\0');
	//								cStr.Format(_T("%s"), temp2);
	//								thirdSize = dc.GetTextExtent(cStr);
	//								l++;
	//							}
	//							if (temp2 != 0) {
	//								delete[] temp2;
	//								temp2 = 0;
	//							}
	//							rightSize.cx = rightSize.cx + thirdSize.cx;
	//						}
	//						caret->Move(x + rightSize.cx, caret->GetY() + ccsize.cy);
	//						topicBox->Modify(x, y, width, height, topic, l);
	//					}
	//					//현재 위치 오른쪽에 엔터가 없는 경우
	//					else {

	//					}

	//				}
	//				i++;
	//			}
	//		}
	//	}
	//	RedrawWindow();
	//	return true;
	//}
	//
	return __super::PreTranslateMessage(pMsg);
}

// 찾기 버튼을 클릭했을 때
void MindMappingForm::OnFindButtonClicked() {
	if (findingForm != 0) {
		findingForm->SetFocus();
	}
	else {
		findingForm = new FindingForm;
		findingForm->Create(IDD_FINDINGFORM, this);
		findingForm->ShowWindow(SW_SHOW);
	}
}

// 포커스를 생성했을 때
void MindMappingForm::OnSetFocus(CWnd *pOldWnd) {
	CWnd::OnSetFocus(pOldWnd);
	CreateSolidCaret(1, 20);
}
// 더블 버퍼링
void MindMappingForm::DrawImage() {
	// 1. 수직 스크롤을 그린다
	vscrollBar.ShowScrollBar();
	// 2. 수평 스크롤을 그린다
	hscrollBar.ShowScrollBar();
	// 3. 확대버튼을 그린다
	//zoomInButton.ShowWindow(SW_SHOW);
	//// 4. 축소버튼을 그린다
	//zoomOutButton.ShowWindow(SW_SHOW);
	//// 5. 잘라내기 버튼을 그린다
	//cutButton.ShowWindow(SW_SHOW);
	//// 6. 복사하기 버튼을 그린다.
	//copyButton.ShowWindow(SW_SHOW);
	//// 7. 붙여넣기 버튼을 그린다.
	//pasteButton.ShowWindow(SW_SHOW);
	//findButton.ShowWindow(SW_SHOW);
	//// 8. 이미지 버튼을 그린다.
	//imageButton.ShowWindow(SW_SHOW);
	//// 9. 실행취소 버튼을 그린다.
	//unDoButton.ShowWindow(SW_SHOW);
	////10. 다시실행 버튼을 그린다.
	//reDoButton.ShowWindow(SW_SHOW);
	//// 11. 진행방향 콤보박스를 그린다
	//directionType.ShowWindow(SW_SHOW);
	//// 12. 저장하기 버튼을 그린다.
	//saveButton.ShowWindow(SW_SHOW);
	//// 13. 불러오기 버튼을 그린다.
	//loadButton.ShowWindow(SW_SHOW);
	//// 14. 새로만들기 버튼을 그린다.
	//newButton.ShowWindow(SW_SHOW);
	//// 15. 삭제하기 버튼을 그린다.
	//deleteButton.ShowWindow(SW_SHOW);
	//// 16. 강조하기 버튼을 그린다.
	//emphasizeButton.ShowWindow(SW_SHOW);
	//// * 인쇄하기 버튼을 그린다.
	//printButton.ShowWindow(SW_SHOW);
	//// * 다른이름으로저장 버튼을 그린다.
	//saveAsButton.ShowWindow(SW_SHOW);
	//// * 도움말 버튼을 그린다.
	//helpButton.ShowWindow(SW_SHOW);
	//// * 정보 버튼을 그린다.
	//informationButton.ShowWindow(SW_SHOW);
	// 17. 중심테두리를 출력한다.
	// 18. 토픽 상자를 출력한다.
	// 19. 토픽 상자에 입력한 글자를 출력한다
	// 20. 정한 위치에 이미지를 출력한다.
	// 21. 선택한 테두리의 선택선을 그린다.
	// 22. 연결선을 그린다.

	Frame *centerFrame = mindMap->GetCenterFrame();
	POINT point;
	if (mouseDouble == true) {
		point.x = caret->GetX();
		point.y = caret->GetY();
		SetCaretPos(point);
		ShowCaret();
	}
	DrawVisitor drawVisitor;
	centerFrame->Accept(&drawVisitor);
}
// 윈도우가 갱신 됐을 때
void MindMappingForm::OnPaint() {
	CPaintDC dc(this);
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	CBitmap *oldBitmap;
	CBitmap bmp;
	memDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	oldBitmap = (CBitmap*)memDC.SelectObject(&bmp);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
	DrawImage();
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();
}

LRESULT MindMappingForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	int x;
	int y;
	int width;
	int height;
	int length = 0;
	int currentIndex;
	CSize size;
	CString cstr;
	CDC *pDC = this->GetDC();
	TCHAR(*topic) = 0;
	TCHAR(*temp);
	TCHAR(*temp2);
	TCHAR(*temp3);
	//TCHAR (*temp) = new TCHAR[length + 1];
	//TCHAR (*temp2) = new TCHAR[length + 1];
	//TCHAR(*temp3) = new TCHAR[2];
	char(*charTopic) = 0;;
	int i = 0;
	int j;
	TopicBox *topicBox = 0;
	// 1. 선택된 프레임의 토픽 박스의 토픽을 읽는다
	while (i < current->GetLength()) {
		Shape *shape = current->GetChild(i);
		if (dynamic_cast<TopicBox*>(shape)) {
			topicBox = (TopicBox*)shape;
			x = topicBox->GetX();
			y = topicBox->GetY();
			width = topicBox->GetWidth();
			height = topicBox->GetHeight();
			topic = topicBox->GetTopic();
			length = topicBox->GetLength();
			currentIndex = topicBox->GetCurrentIndex();
		}
		i++;
	}

	if (lParam & GCS_RESULTSTR) {

		i = 0;
		j = 0;
		temp = new TCHAR[length + 1];
		while (i < currentIndex) {
			temp[j] = topic[i];
			i++;
			j++;
		}
		temp[j] = wParam;
		j++;
		while (topic[i] != '\0') {
			temp[j] = topic[i];
			j++;
			i++;
		}
		temp[j] = '\0';
		i = 0;
		j = 0;
		int newTopicBoxWidth = width;
		int newFrameWidth = current->GetWidth();
		temp2 = new TCHAR[length + 1];
		while (temp[i] != '\0') {
			while (temp[i] != '\r' && temp[i] != '\0') {
				temp2[j] = temp[i];
				i++;
				j++;
			}
			temp2[j] = '\0';
			cstr.Format(_T("%s"), temp2);
			// 3. 합친 토픽의 크기를 잰다
			size = pDC->GetTextExtent(cstr);
			if (size.cx > newTopicBoxWidth) {
				newTopicBoxWidth = size.cx;
				if (newFrameWidth - newTopicBoxWidth < 40) {
					newFrameWidth = newFrameWidth + 10;
				}
			}
			if (temp[i] != '\0') {
				i++;
			}
			j = 0;
		}
		cstr.Format(_T("%c"), wParam);
		size = pDC->GetTextExtent(cstr);
		this->caret->Move(this->caret->GetX() + size.cx, this->caret->GetY());
		topicBox->Modify(x, y, newTopicBoxWidth, height, temp, currentIndex + 1);
		current->Resize(current->GetX(), current->GetY(), newFrameWidth, current->GetHeight());
		delete[]text;
		text = 0;
	}
	else if (lParam & GCS_COMPSTR) {  // 조합중이면;.
		i = 0;
		j = 0;
		delete[]text;
		text = new TCHAR[length + 1];
		while (i < currentIndex) {
			text[j] = topic[i];
			i++;
			j++;
		}
		if (wParam != 27) {

			text[j] = wParam;
			j++;
		}
		while (topic[i] != '\0') {
			text[j] = topic[i];
			j++;
			i++;
		}
		text[j] = '\0';

	}
	RedrawWindow();
	return 0;
}

// 문자 키를 누를 때
void MindMappingForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->current != 0) {
		int x;
		int y;
		int width;
		int height;
		int currentIndex;
		int count;
		TCHAR(*topic) = 0;
		CDC *pDC = this->GetDC();
		CSize size;
		int caretX = this->caret->GetX();
		int caretY = this->caret->GetY();

		//ctrl 키를 누를 때는 글씨가 써지지 않도록 함
		if (GetKeyState(VK_CONTROL) & 0x80000000) {
		}
		else {
			int i = 0;
			int j = 0;
			TopicBox *topicBox = 0;
			// 1. 선택된 프레임의 토픽 박스의 토픽을 읽는다
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					topic = topicBox->GetTopic();
					currentIndex = topicBox->GetCurrentIndex();
				}
				i++;
			}
			CString str;
			CString cstr;
			int length = topicBox->GetLength();
			TCHAR(*temp) = new TCHAR[length + 1];
			TCHAR(*temp2) = new TCHAR[length + 1];
			// 2. 새로 입력 받은 문자와 기존토픽을 합친다
			if (nChar == VK_BACK) {
				if (currentIndex == 0) {

				}
				else if (currentIndex>0) {
					if (topic[currentIndex - 1] == '\r') {
						i = 0;
						j = 0;
						while (i < currentIndex - 1) {
							temp[j] = topic[i];
							i++;
							j++;
						}
						i++;
						while (topic[i] != '\0') {
							temp[j] = topic[i];
							j++;
							i++;
						}
						temp[j] = '\0';
						i = 0;
						j = 0;
						int newTopicBoxWidth;
						int newFrameWidth = current->GetWidth();
						count = 0;
						while (temp[i] != '\0') {
							while (temp[i] != '\r' && temp[i] != '\0') {
								temp2[j] = temp[i];
								i++;
								j++;
							}
							temp2[j] = '\0';
							cstr.Format(_T("%s"), temp2);

							// 3. 합친 토픽의 크기를 잰다
							size = pDC->GetTextExtent(cstr);
							if (count == 0) {
								newTopicBoxWidth = size.cx;
							}
							if (size.cx > newTopicBoxWidth) {
								newTopicBoxWidth = size.cx;
							}
							if (temp[i] != '\0') {
								i++;
							}
							j = 0;
							count++;
						}
						char(*temp2);
						temp2 = new char[2];
						temp2[0] = 'a';
						temp2[1] = '\0';
						str.Format(_T("%s"), temp2);
						size = pDC->GetTextExtent(str);
						//i = 0;
						//while (i < length - 2) {
						//	temp[i] = topic[i];
						//	i++;
						//}
						//temp[i] = '\0';
						if (currentIndex > 1 && topic[currentIndex - 2] == '\r') {
							this->caret->Move(x, this->caret->GetY() - size.cy);
						}
						else {
							size = pDC->GetTextExtent(cstr);
							this->caret->Move(x + size.cx, this->caret->GetY() - size.cy);
						}
						topicBox->Modify(x, y, newTopicBoxWidth, height - size.cy, temp, currentIndex - 1);
						current->Resize(current->GetX(), current->GetY(), newFrameWidth, current->GetHeight() - size.cy);
					}
					else {
						i = 0;
						j = 0;
						while (i < currentIndex - 1) {
							temp[j] = topic[i];
							i++;
							j++;
						}
						i++;
						while (topic[i] != '\0') {
							temp[j] = topic[i];
							j++;
							i++;
						}
						temp[j] = '\0';
						i = 0;
						j = 0;
						int newTopicBoxWidth;
						int newFrameWidth = current->GetWidth();
						count = 0;
						while (temp[i] != '\0') {
							while (temp[i] != '\r' && temp[i] != '\0') {
								temp2[j] = temp[i];
								i++;
								j++;
							}
							temp2[j] = '\0';
							cstr.Format(_T("%s"), temp2);

							// 3. 합친 토픽의 크기를 잰다
							size = pDC->GetTextExtent(cstr);
							if (count == 0) {
								newTopicBoxWidth = size.cx;
							}
							if (size.cx > newTopicBoxWidth) {
								newTopicBoxWidth = size.cx;
							}
							if (temp[i] != '\0') {
								i++;
							}
							j = 0;
							count++;
						}
						i = 0;
						temp2[0] = topic[currentIndex - 1];
						temp2[1] = '\0';
						cstr.Format(_T("%s"), temp2);
						size = pDC->GetTextExtent(cstr);
						if (newTopicBoxWidth + 2 < width) {
							width = newTopicBoxWidth + 2;
							newFrameWidth = newFrameWidth - size.cx;
						}
						this->caret->Move(this->caret->GetX() - size.cx, this->caret->GetY());
						topicBox->Modify(x, y, width, height, temp, currentIndex - 1);
						current->Resize(current->GetX(), current->GetY(), newFrameWidth, current->GetHeight());

					}
				}
			}
			else if (nChar == VK_RETURN) {
				str.Format(_T("%s"), _T("a"));
				size = pDC->GetTextExtent(str);
				i = 0;
				j = 0;
				while (i < currentIndex) {
					temp[j] = topic[i];
					i++;
					j++;
				}
				temp[j] = '\r';
				j++;
				while (topic[i] != '\0') {
					temp[j] = topic[i];
					j++;
					i++;
				}
				temp[j] = '\0';
				this->caret->Move(x, this->caret->GetY() + size.cy);
				topicBox->Modify(x, y, width, height + size.cy, temp, currentIndex + 1);
				current->Resize(current->GetX(), current->GetY(), current->GetWidth(), current->GetHeight() + size.cy);
			}
			else {
				i = 0;
				j = 0;
				while (i < currentIndex) {
					temp[j] = topic[i];
					i++;
					j++;
				}
				temp[j] = nChar;
				j++;
				while (topic[i] != '\0') {
					temp[j] = topic[i];
					j++;
					i++;
				}
				temp[j] = '\0';
				i = 0;
				int j = 0;
				int newTopicBoxWidth = width;
				int newFrameWidth = current->GetWidth();
				while (temp[i] != '\0') {
					while (temp[i] != '\r' && temp[i] != '\0') {
						temp2[j] = temp[i];
						i++;
						j++;
					}
					temp2[j] = '\0';
					if (temp[i] == '\0') {
						cstr.Format(_T("%s%c"), temp2, nChar);
					}
					else {
						cstr.Format(_T("%s"), temp2);
					}
					// 3. 합친 토픽의 크기를 잰다
					size = pDC->GetTextExtent(cstr);
					if (size.cx > newTopicBoxWidth) {
						newTopicBoxWidth = size.cx;
						if (newFrameWidth - newTopicBoxWidth < 40) {
							newFrameWidth = newFrameWidth + 10;
						}
					}
					if (temp[i] != '\0') {
						i++;
					}
					j = 0;
				}
				cstr.Format(_T("%c"), nChar);
				size = pDC->GetTextExtent(cstr);
				this->caret->Move(this->caret->GetX() + size.cx, this->caret->GetY());
				topicBox->Modify(x, y, newTopicBoxWidth, height, temp, currentIndex + 1);
				current->Resize(current->GetX(), current->GetY(), newFrameWidth, current->GetHeight());

			}
		}
		this->ReleaseDC(pDC);
		RedrawWindow();
	}
}
// 키보드를 누를 때
void MindMappingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//1. 프레임이 선택된 상태에서 Delete 키를 눌렀을 경우 
	if (nChar == VK_DELETE && this->current != 0) {
		// 1.1. 선택한 테두리를 읽는다
		if (this->current != this->mindMap->GetCenterFrame()) {
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			// 1.2. 마지막 작업을 적는다.
			task.UnDoWrite(mindMap);
			// 1.3 선택한 테두리를 지운다.
			highRankFrame->Remove(current);
			this->current = highRankFrame;
		}
	}
	//2. Ctrl + X 키를 눌렀을 경우
	else if (nChar == 0x58 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 2.1 자를 토픽을 정한다
		if (this->current != 0) {
			// 2.2 정한 토픽과 하위 토픽들을 옮겨 적는다
			this->copyFrame = new Frame(*this->current);
			Frame *highRankFrame = this->mindMap->HighRankFrame(this->current);
			// 2.3 마지막 작업을 적는다
			task.UnDoWrite(mindMap);
			// 2.4 정한 토픽과 하위 토픽들을 지운다.
			highRankFrame->Remove(this->current);
			this->current = 0;
		}
	}
	//3. Ctrl + C 키를 눌렀을 경우
	else if (nChar == 0x43 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 3.1 복사할 토픽을 정한다
		if (this->current != 0) {
			// 3.2 정한 토픽과 하위토픽들을 옮겨 적는다.
			this->copyFrame = new Frame(*this->current);
		}
	}
	//4. Ctrl + V 키를 눌렀을 경우
	else if (nChar == 0x56 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 4.1 붙여넣을 토픽을 정한다.
		if (this->current != 0) {
			// 4.2 옮길 위치를 정한다. (방사형으로 배치하도록 작업 필요)
			// 4.3 옮겨 놓은 토픽들을 정한 위치에 옮겨 적는다
			// 4.4 옮긴 토픽 테두리와 정한 테두리 사이에 관계선을 그린다 (기존 관계선 지우는 작업 필요)
			this->current->Add(this->copyFrame);
		}
	}
	// 5. Ctrl+Z 키를 눌렀을 경우
	else if ((nChar == 0x5A && GetKeyState(VK_CONTROL) & 0x80000000)) {
		//5.1. 불러오기 전의 마지막 작업을 저장한다.
		//5.2. 가장 최근 작업을 불러온다.
		if (task.GetUnDoLength() > 0) {
			task.ReDoWrite(this->mindMap);
			MindMap *unDoMindMap = task.UnDoRead();
			*this->mindMap = *unDoMindMap;
			delete unDoMindMap;
		}
	}
	// 6. Ctrl+Y 키를 눌렀을 경우
	else if ((nChar == 0x59 && GetKeyState(VK_CONTROL) & 0x80000000)) {

		//6.2.1. 불러오기전의 마지막 작업을 불러온다. 
		if (task.GetReDoLength() > 0) {
			task.UnDoWrite(this->mindMap);
			MindMap *reDoMindMap = task.ReDoRead();
			*this->mindMap = *reDoMindMap;
			delete reDoMindMap;
		}
	}
	else if (nChar == 0x46 && GetKeyState(VK_CONTROL) & 0x80000000) {
		if (findingForm != 0) {
			findingForm->SetFocus();
		}
		else {
			findingForm = new FindingForm;
			findingForm->Create(IDD_FINDINGFORM, this);
			findingForm->ShowWindow(SW_SHOW);
		}
	}
	else if (nChar == VK_LEFT) {
		if (this->current != 0 && mouseDouble == true) {
			CSize size;
			CPoint point;
			CString str;
			CClientDC dc(this);
			TopicBox *topicBox = 0;
			TCHAR(*topic) = 0;
			TCHAR(*temp) = 0;
			TCHAR(*temp2) = 0;
			TCHAR(*temp3) = 0;
			int currentIndex;
			int x;
			int y;
			int width;
			int height;
			int i = 0;
			int j = 0;
			int k = 0;
			int l = 0;
			int m = 0;
			int o = 0;
			int previousIndex;//문자열 배열에서 전전 '\r'의 위치
			int rCount = 0;//\r의 개수
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//프레임에서 토픽박스를 찾는다.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					//토픽에서 현재 위치를 읽는다.
					currentIndex = topicBox->GetCurrentIndex();
					Array<int> tempIndex(currentIndex + 1);//'\r'의 위치들을 담은 정수배열
														   //토픽에 아무것도없는 경우
					if (currentIndex == 0) {

					}
					//토픽이 채워진 경우
					else {
						temp = new TCHAR[currentIndex + 1];
						temp[0] = topic[currentIndex - 1];
						temp[1] = L'\0';
						//현재 위치의 왼쪽이 문자인 경우
						if (temp[0] != L'\r') {
							str.Format(_T("%s"), temp);
							size = dc.GetTextExtent(str);
							caret->Move(caret->GetX() - size.cx, caret->GetY());
						}
						//현재 위치의 왼쪽이 enter('\r')인 경우
						else {
							//enter('\r')의 개수를 센다.
							while (j <= topicBox->GetLength()) {
								if (topic[j] == L'\r') {
									rCount++;
									tempIndex[l] = j;
									l++;
									//현재 위치 왼쪽의 '\r'을 찾은 경우
									if (j == currentIndex - 1) {
										//현재 위치 왼쪽의 '\r'이 첫 번째인 경우
										if (rCount == 1) {
											//현재 캐럿 윗줄이 공백인 경우
											if (currentIndex - 1 == 0) {
												str.Format(_T("%s"), temp);
												size = dc.GetTextExtent(str);
												caret->Move(caret->GetX(), caret->GetY() - size.cy);
											}
											//현재 캐럿 윗줄이 문자인 경우
											else {
												temp2 = new TCHAR[currentIndex + 1];
												while (k < currentIndex - 1) {
													temp2[k] = topic[k];
													k++;
												}
												temp2[k] = L'\0';
												str.Format(_T("%s"), temp2);
												size = dc.GetTextExtent(str);
												caret->Move(caret->GetX() + size.cx, caret->GetY() - size.cy);
											}
										}
										//현재 위치 왼쪽의 '\r'이 첫 번째가 아닌 경우
										else {
											temp2 = new TCHAR[currentIndex + 1];
											temp2[0] = topic[currentIndex - 2];
											temp2[1] = L'\0';
											//현재 캐럿 윗줄이 공백인 경우
											if (temp2[0] == L'\r') {
												str.Format(_T("%s"), temp2);
												size = dc.GetTextExtent(str);
												caret->Move(caret->GetX(), caret->GetY() - size.cy);
											}
											//현재 캐럿 윗줄이 문자인 경우
											else {
												while (m < tempIndex.GetLength()) {
													if (tempIndex[m] == currentIndex - 1) {
														previousIndex = tempIndex[m - 1] + 1;
														while (previousIndex < currentIndex - 1) {
															temp3 = new TCHAR[currentIndex + 1];
															temp3[o] = topic[previousIndex];
															o++;
															previousIndex++;
														}
														temp3[o] = L'\0';
														str.Format(_T("%s"), temp3);
														size = dc.GetTextExtent(str);
														caret->Move(caret->GetX() + size.cx, caret->GetY() - size.cy);
													}
													m++;
												}
											}
										}
									}
								}
								j++;
							}

						}
						topicBox->Modify(x, y, width, height, topic, currentIndex - 1);

					}

				}
				i++;
			}

		}
	}
	else if (nChar == VK_RIGHT) {
		if (this->current != 0 && mouseDouble == true) {
			CSize size;
			CPoint point;
			CString str;
			CClientDC dc(this);
			TopicBox *topicBox = 0;
			TCHAR(*topic) = 0;
			TCHAR(*temp) = 0;
			int currentIndex;
			int x;
			int y;
			int width;
			int height;
			int i = 0;
			int j = 0;
			int rCount = 0;//\r의 개수
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//프레임에서 토픽박스를 찾는다.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					//토픽에서 현재 위치를 읽는다.
					currentIndex = topicBox->GetCurrentIndex();
					Array<int> tempIndex(currentIndex + 1);//'\r'의 위치들을 담은 정수배열
														   //현재 위치가 '\0'인 경우 
					if (topic[currentIndex] == _T('\0')) {

					}
					//현재 위치가 '\r'인 경우
					else if (topic[currentIndex] == _T('\r')) {
						while (topic[j] != _T('\0') && j != currentIndex + 1) {
							if (topic[j] == _T('\r')) {
								rCount++;
							}
							j++;
						}
						str.Format(_T("%s"), topic);
						size = dc.GetTextExtent(str);
						caret->Move(x, y + size.cy*rCount);
						topicBox->Modify(x, y, width, height, topic, currentIndex + 1);
					}
					//현재 위치가 문자인 경우
					else {

						temp = new TCHAR[currentIndex + 2];
						temp[0] = topic[currentIndex];
						temp[1] = _T('\0');
						str.Format(_T("%s"), temp);
						size = dc.GetTextExtent(str);
						caret->Move(caret->GetX() + size.cx, caret->GetY());
						topicBox->Modify(x, y, width, height, topic, currentIndex + 1);
					}
				}
				i++;
			}
		}
	}
	else if (nChar == VK_UP) {
		if (this->current != 0 && mouseDouble == true) {
			CSize firstSize;//토픽에서 현재 위치(currentIndex)와 왼쪽 방향으로 가장 가까운 엔터 사이의 길이
			CSize secondSize;//현재위치 왼쪽 방향으로 두번째 엔터와 첫번째 엔터 사이의 길이
			CSize thirdSize;
			CPoint point;
			CString str;
			CString cStr;
			CClientDC dc(this);
			TopicBox *topicBox = 0;
			TCHAR(*topic) = 0;
			TCHAR(*temp) = 0;
			TCHAR(*temp2) = 0;
			int currentIndex;
			int x;
			int y;
			int width;
			int height;
			int i = 0;
			int j = 0;//firsSize길이를 재기위한 반복제어 변수
			int k = 0;//첨자 제어 변수
			int l = 0;//secondSize길이를 재기위한 반복제어 변수
			int m;
			int rCount = 0;//\r의 개수
			int firstR;//현재 위치로부터 가장 가까운 \r의 위치
			int secondR;//현재 위치로부터 두번째로 가까운 \r의 위치
			int tempIndex;//현재 위치를 초기값으로 갖는 반복제어 변수
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//프레임에서 토픽박스를 찾는다.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					CString ccstr;
					ccstr.Format(_T("%s"), topic);
					CSize ccsize = dc.GetTextExtent(ccstr);
					//토픽에서 현재 위치를 읽는다.
					currentIndex = topicBox->GetCurrentIndex();
					tempIndex = currentIndex - 1;
					//현재 위치로부터 왼쪽 방향에 있는 \r 2개를 찾는다.
					while (tempIndex >= 0 && rCount < 2) {
						if (topic[tempIndex] == _T('\r')) {
							if (rCount == 0) {
								firstR = tempIndex;
							}
							else {
								secondR = tempIndex;
							}
							rCount++;
						}
						tempIndex--;
					}
					secondSize.cx = 0;
					secondSize.cy = 0;
					temp = new TCHAR[currentIndex];
					if (rCount == 0) {

					}
					else {
						j = firstR;;
						j++;
						while (j < currentIndex) {
							temp[k] = topic[j];
							k++;
							j++;
						}
						temp[k] = _T('\0');
						str.Format(_T("%s"), temp);
						firstSize = dc.GetTextExtent(str);
						thirdSize.cx = 0;
						thirdSize.cy = 0;
						//엔터가 두개 일때
						if (rCount == 2) {
							//첫 번째 엔터에서 현재 위치까지의 길이를 잰다.
							l = secondR;
							l++;
							//두 번째 엔터와 첫 번째 엔터 사이의 길이를 재고 비교한다.
							while (secondSize.cx < firstSize.cx && l != firstR) {
								m = 0;
								if (l < firstR) {
									temp2 = new TCHAR[currentIndex];
									temp2[m] = topic[l];
									temp2[m + 1] = _T('\0');
									cStr.Format(_T("%s"), temp2);
									thirdSize = dc.GetTextExtent(cStr);
									l++;
								}
								if (temp2 != 0) {
									delete[] temp2;
									temp2 = 0;
								}
								secondSize.cx = secondSize.cx + thirdSize.cx;
							}
							caret->Move(x + secondSize.cx, caret->GetY() - ccsize.cy);
							topicBox->Modify(x, y, width, height, topic, l);
						}
						//엔터가 한 개 일때
						if (rCount == 1) {
							//두번째 길이를 재고 첫번째 길이와 비교한다.
							while (secondSize.cx < firstSize.cx && l != firstR) {
								m = 0;
								if (l < firstR) {
									temp2 = new TCHAR[currentIndex];
									temp2[m] = topic[l];
									temp2[m + 1] = _T('\0');
									cStr.Format(_T("%s"), temp2);
									thirdSize = dc.GetTextExtent(cStr);
									l++;
								}
								if (temp2 != 0) {
									delete[] temp2;
									temp2 = 0;
								}
								secondSize.cx = secondSize.cx + thirdSize.cx;
							}
							caret->Move(x + secondSize.cx, caret->GetY() - ccsize.cy);
							topicBox->Modify(x, y, width, height, topic, l);
						}
					}
				}
				i++;
			}
		}

	}
	else if (nChar == VK_DOWN) {
		if (this->current != 0 && mouseDouble == true) {
			CSize leftSize;//토픽에서 현재 위치(currentIndex)와 왼쪽 방향으로 가장 가까운 엔터 사이의 길이
			CSize rightSize;//현재위치에서 오른쪽 엔터 이후의 길이
			CSize thirdSize;
			CPoint point;
			CString str;
			CString cStr;
			CClientDC dc(this);
			TopicBox *topicBox = 0;
			TCHAR(*topic) = 0;
			TCHAR(*temp) = 0;
			TCHAR(*temp2) = 0;
			int currentIndex;
			int x;
			int y;
			int width;
			int height;
			int i = 0;
			int j = 0;//firsSize길이를 재기위한 반복제어 변수
			int k = 0;//첨자 제어 변수
			int l = 0;//secondSize길이를 재기위한 반복제어 변수
			int m;
			int rCount = 0;//현재 위치 오른쪽 \r의 개수
			int lCount = 0;//현재 위치 왼쪽 엔터의 개수
			int leftR;//현재 위치에서 왼쪽에 있는 \r의 위치
			int rightR;//현재 위치에서 오른쪽에 있는 \r의 위치
			int tempIndex;//현재 위치를 초기값으로 갖는 반복제어 변수
			int tempIndex2;
			bool R = false;
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//프레임에서 토픽박스를 찾는다.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					CString ccstr;
					ccstr.Format(_T("%s"), topic);
					CSize ccsize = dc.GetTextExtent(ccstr);
					//토픽에서 현재 위치를 읽는다.
					currentIndex = topicBox->GetCurrentIndex();
					tempIndex = currentIndex;
					tempIndex2 = currentIndex - 1;
					rightSize.cx = 0;
					rightSize.cy = 0;
					thirdSize.cx = 0;
					thirdSize.cy = 0;
					//현재 위치 오른쪽에 엔터가 있는지 확인

					while (topic[tempIndex] != _T('\0') && rCount != 1) {
						if (topic[tempIndex] == _T('\r')) {
							rightR = tempIndex;
							rCount++;
						}
						tempIndex++;
					}
					//현재 위치 오른쪽에 엔터가 있는 경우
					if (rCount == 1) {
						//현재 위치 왼쪽에 엔터가 있는지 확인한다.
						while (tempIndex2 >= 0 && lCount != 1) {
							if (topic[tempIndex2] == _T('\r')) {
								leftR = tempIndex2;
								lCount++;
							}
							tempIndex2--;
						}
						//현재 위치 왼쪽에 엔터가 있는 경우
						if (lCount == 1) {
							j = leftR;
							j++;
							temp = new TCHAR[topicBox->GetLength()];
							while (j < currentIndex) {
								temp[k] = topic[j];
								k++;
								j++;
							}
							temp[k] = _T('\0');
							str.Format(_T("%s"), temp);
							leftSize = dc.GetTextExtent(str);

						}
						//현재 위치 바로 왼쪽이 엔터인 경우 
						else if (topic[currentIndex - 1] == _T('\r')) {

						}
						//현재 위치 왼쪽에 엔터가 없는 경우
						else {
							temp = new TCHAR[topicBox->GetLength()];
							while (j < currentIndex) {
								temp[k] = topic[j];
								k++;
								j++;
							}
							temp[k] = _T('\0');
							str.Format(_T("%s"), temp);
							leftSize = dc.GetTextExtent(str);

						}
						//현재 위치 왼쪽 길이와 오른쪽 엔터 이후의 길이를 비교한다.
						l = rightR;
						l++;
						while (rightSize.cx < leftSize.cx && topic[l] != _T('\0') && topic[l] != _T('\r')) {
							m = 0;
							if (topic[l] != _T('\0')) {
								temp2 = new TCHAR[topicBox->GetLength()];
								temp2[m] = topic[l];
								temp2[m + 1] = _T('\0');
								cStr.Format(_T("%s"), temp2);
								thirdSize = dc.GetTextExtent(cStr);
								l++;
							}
							if (temp2 != 0) {
								delete[] temp2;
								temp2 = 0;
							}
							rightSize.cx = rightSize.cx + thirdSize.cx;
						}
						caret->Move(x + rightSize.cx, caret->GetY() + ccsize.cy);
						topicBox->Modify(x, y, width, height, topic, l);
					}
					//현재 위치 오른쪽에 엔터가 없는 경우
					else {

					}

				}
				i++;
			}
		}
	}
	RedrawWindow();
}

void MindMappingForm::OnKillFocus(CWnd* pNewWnd) {
	CWnd::OnKillFocus(pNewWnd);
	HideCaret();
	DestroyCaret();
}

// 이미지 삽입 버튼을 클릭했을 때
void MindMappingForm::OnInsertImageButtonClicked() {
	InsertingImageForm insertingImageForm;
	// 1. 이미지 삽입 윈도우를 생성한다
	if (insertingImageForm.DoModal() == IDOK) {
		insertingImageForm.OnOpenButtonClicked();
		RedrawWindow();
	}
}
// 마우스 왼쪽을 눌렀을 때
void MindMappingForm::OnLButtonDown(UINT nFlags, CPoint point) {
	// 1. 누른 위치를 읽는다
	this->mousePtDown = point;
	this->current = 0;
	this->moveIcon = false;
	this->mouseDown = false;


	Frame *frame = IsFrame(point);

	// 2. 누른 위치가 테두리 안인지 확인한다.
	if (frame != 0) {
		// 2.1 안이면
		// 2.1.1 누른 테두리를 표시한다
		this->current = frame;
		this->mouseDown = true;
		// 2.1.2 이동 아이콘을 활성화 시킨다.

		RECT rect;
		GetClientRect(&rect);

		//CRect moveRect = CRect(current->GetX() + current->GetWidth() / 2 - 7, current->GetY(), current->GetX() + current->GetWidth() / 2 - 7 + 14, current->GetY() + 14);

		CRect moveRect = CRect(((current->GetX() + current->GetWidth() / 2 - 7) - scroll.GetX() - rect.right / 2)*r + rect.right / 2,
			(current->GetY() - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2, (current->GetX() + current->GetWidth() / 2 - 7 + 14)*r + rect.right / 2,
			((current->GetY() + 14) - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2);

		// 2.1.2.1 이동 아이콘을 누른지 확인한다
		if (moveRect.PtInRect(point)) {
			// 2.1.2.1.1 참이면 이동 아이콘을 눌렀음을 적는다
			this->moveIcon = true;
		}
	}
	RedrawWindow();
}


// 마우스를 움직일때
void MindMappingForm::OnMouseMove(UINT nFlags, CPoint point) {
	this->mouseMove = false;
	// * 마우스 움직인 위치를 적는다.
	this->mousePtMove = point;
	// 1. 마우스 왼쪽버튼을 눌렀는지 확인한다.
	if (mouseDown && (nFlags & MK_LBUTTON) == MK_LBUTTON) {
		//1.1 참이면
		CRect currentRect = CRect(current->GetX() - scroll.GetX(), current->GetY() - scroll.GetY(), current->GetX() + current->GetWidth() - scroll.GetX(),
			current->GetY() + current->GetHeight() - scroll.GetY());
		// 1.1.1 선택된 테두리의 위치를 읽는다
		// 1.1.2 왼쪽 마우스를 움직인 위치가 선택된 위치를 벗어났는지 확인한다.
		if (currentRect.PtInRect(point) == false)
			//1.1.2.1 참이면 움직였다를 적는다.
		{
			this->mouseMove = true;
		}
	}
	CDC *pDC;
	pDC = this->GetDC();
	CString strPos;
	strPos.Format(_T("마우스 좌표 X : %04d  Y : %04d"), point.x, point.y);
	pDC->TextOut(500, 600, strPos);
	this->ReleaseDC(pDC);
}

// 왼쪽 마우스를 뗐을때
void MindMappingForm::OnLButtonUp(UINT nFlags, CPoint point) {
	// 1. 마우스를 뗀 위치를 적는다.
	mousePtUp = point;
	// 2. 마우스를 눌렀으며, 마우스를 움직였는지, 이동아이콘을 눌리지 않았는지 확인한다
	if (mouseDown == TRUE && mouseMove == TRUE && moveIcon == FALSE) {
		task.UnDoWrite(this->mindMap);
		//2.1 참이면 뗀 위치에 프레임,토픽박스를 만든다
		RECT rect;
		GetClientRect(&rect);
		//int index = current->Add(new Frame(point.x - 65 + scroll.GetX(), point.y - 65 + scroll.GetY(), 130, 130));
		int index = current->Add(new Frame((point.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - 65,
			(point.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - 65, 130, 130));

		Frame *childFrame = (Frame *)current->GetChild(index);
		childFrame->Add(new TopicBox((point.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - 45, (point.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - 20
			, 90, 40, _T("하위토픽")));
		mouseDown = false;
		mouseMove = false;
	}
	// 3. 이동아이콘을 눌렀는지 확인한다.
	if (moveIcon == TRUE) {
		// 3.1 참이면 뗀 위치가 프레임인지 확인한다.
		//3.1.1 참이면
		Frame *frame = IsFrame(point);
		if (frame != 0 && frame != current) {
			task.UnDoWrite(this->mindMap);
			int i = 0;
			int length = current->GetLength();
			bool connectingLine = false;
			while (i < length && connectingLine == false) {
				Shape *shape = this->current->GetChild(i);
				if (dynamic_cast<ConnectingLine*>(shape)) {
					this->current->Remove(i);
					connectingLine = true;
				}
				i++;
			}
			// 3.1.1.1 상위 토픽을 바꾼다.
			Frame *temp = new Frame(*this->current);
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			highRankFrame->Remove(current);
			frame->Add(temp);

		}
		// 3.1.2 거짓이면 프레임 위치를 이동한다.
		else {
			task.UnDoWrite(this->mindMap);
			OnMoveIconClicked();
			moveIcon = false;
		}
	}

	RedrawWindow();
}

// 이동버튼을 눌렀을 때
void MindMappingForm::OnMoveIconClicked() {

	RECT rect;
	GetClientRect(&rect);
	//1. 이동한 거리를 구한다.
	int moveX = (mousePtUp.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - ((mousePtDown.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2);
	int moveY = (mousePtUp.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - ((mousePtDown.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2);


	// 2. 프레임의 위치를 이동한 거리만큼 이동한다.
	this->current->Resize(current->GetX() + moveX, current->GetY() + moveY, current->GetWidth(), current->GetHeight());


	int i = 0;
	int length = current->GetLength();
	while (i < length) {
		Shape *shape = current->GetChild(i);

		int x = shape->GetX();
		int y = shape->GetY();
		int width = shape->GetWidth();
		int height = shape->GetHeight();

		if (dynamic_cast<TopicBox*>(shape)) {
			// 3. 토픽박스의 위치를 이동한 거리만큼 이동한다.
			((TopicBox*)shape)->Resize(x + moveX, y + moveY, width, height);
		}
		if (dynamic_cast<ConnectingLine*>(shape)) {
			int childX = ((ConnectingLine*)shape)->GetChildX();
			int childY = ((ConnectingLine*)shape)->GetChildY();
			// 4. 상위 프레임과 움직인 프레임의 연결선의 위치를 이동한 거리만큼 이동한다.
			((ConnectingLine*)shape)->Resize(x, y, childX + moveX, childY + moveY);
		}
		if (dynamic_cast<Frame*>(shape)) {
			int j = 0;
			while (j < ((Frame*)shape)->GetLength()) {
				Shape *shape2 = ((Frame*)shape)->GetChild(j);
				if (dynamic_cast<ConnectingLine*>(shape2)) {
					int childX = ((ConnectingLine*)shape2)->GetChildX();
					int childY = ((ConnectingLine*)shape2)->GetChildY();
					// 5. 하위 프레임과 움직인 프레임의 연결선을 이동한 거리만큼 이동한다.
					((ConnectingLine*)shape2)->Resize(current->GetX() + current->GetWidth() / 2, current->GetY() + current->GetHeight() / 2, childX, childY);
				}
				j++;
			}

		}
		i++;
	}
	RedrawWindow();
}

// 수평 스크롤을 눌렀을 때
void MindMappingForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	UINT nCurPos = pScrollBar->GetScrollPos();
	RECT rect;
	GetClientRect(&rect);

	int x = this->scroll.GetX();
	int y = this->scroll.GetY();

	if (pScrollBar->GetDlgCtrlID() == 4002)
	{
		switch (nSBCode) {
			// 1. 왼쪽 버튼을 눌렀을 때
		case SB_LINELEFT:
			nCurPos -= 1;
			x -= rect.right / 100 * 1;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.left;
			}
			//1.1 스크롤 바를 좌로 이동한다
			pScrollBar->SetScrollPos(nCurPos);
			//1.2 화면을 좌로 이동한다
			this->scroll.Move(x, y);
			break;

			// 2. 오른쪽 버튼을 눌렀을 때
		case SB_LINERIGHT:
			nCurPos++;
			x += rect.right / 100 * 1;
			if (nCurPos > SCROLL_MAX) {
				nCurPos = SCROLL_MAX;
				x = rect.right;
			}
			//2.2 스크롤 바를 우로 이동한다
			pScrollBar->SetScrollPos(nCurPos);
			//2.3 화면을 우로 이동한다
			this->scroll.Move(x, y);
			break;

			//3. 좌측 공간을 눌렀을 때
		case SB_PAGELEFT:
			nCurPos -= 5;
			x -= rect.right / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.left;
			}
			// 3.1 스크롤 바를 좌로 이동한다
			pScrollBar->SetScrollPos(nCurPos);
			// 3.2 화면을 좌로 이동한다
			this->scroll.Move(x, y);
			break;

			//4. 우측 공간을 눌렀을 때
		case SB_PAGERIGHT:
			nCurPos += 5;
			x += rect.right / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.right;
			}
			// 4.1 스크롤 바를 우로 이동한다.
			pScrollBar->SetScrollPos(nCurPos);
			// 4.2 화면을 우로 이동한다.
			this->scroll.Move(x, y);
			break;

			//5. 스크롤 바의 위치가 변했을 때
		case SB_THUMBPOSITION:
			// 5.1 변한 위치로 스크롤 바를 이동시킨다.
			pScrollBar->SetScrollPos(nPos);
			// 5.2 변한 위치만큼 화면을 이동시킨다.
			if (nPos < nCurPos) {
				this->scroll.Move(x - rect.right / 100 * (nCurPos - nPos), y);
			}
			else {
				this->scroll.Move(x + rect.right / 100 * (nPos - nCurPos), y);
			}
			break;

			//6. 스크롤 바가 이동할 때
		case SB_THUMBTRACK:
			// 6.1 이동한 위치로 스크롤 바를 이동시킨다
			pScrollBar->SetScrollPos(nPos);
			// 6.2 이동한 위치만큼 화면을 이동시킨다
			if (nPos < nCurPos) {
				this->scroll.Move(x - rect.right / 100 * (nCurPos - nPos), y);
			}
			else {
				this->scroll.Move(x + rect.right / 100 * (nPos - nCurPos), y);
			}
			break;

		default:
			break;
		}
	}
	RedrawWindow();

}

// 수직 스크롤을 눌렀을 때
void MindMappingForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	if (pScrollBar->GetDlgCtrlID() == 4001) {
		RECT rect;
		GetClientRect(&rect);
		UINT nCurPos = pScrollBar->GetScrollPos();
		int x = scroll.GetX();
		int y = scroll.GetY();

		switch (nSBCode)
		{
			// 1. 위 버튼을 눌렀을 때
		case SB_LINEUP:
			nCurPos -= 1;
			y -= rect.bottom / 100 * 1;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				y = rect.top;
			}
			//1.1 스크롤 바를 올린다
			pScrollBar->SetScrollPos(nCurPos);
			//1.2 화면을 위로 올린다
			this->scroll.Move(x, y);
			break;

			// 2. 아래 버튼을 눌렀을 때
		case SB_LINEDOWN:
			nCurPos += 1;
			y += rect.bottom / 100 * 1;
			if (nCurPos > SCROLL_MAX) {
				nCurPos = SCROLL_MAX;
				y = rect.bottom;
			}
			//2.2 스크롤 바를 내린다
			pScrollBar->SetScrollPos(nCurPos);
			//2.3 화면을 아래로 내린다
			this->scroll.Move(x, y);
			break;

			// 3. 상단 공간을 눌렀을 때
		case SB_PAGEUP:
			nCurPos -= 5;
			y -= rect.bottom / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				y = rect.top;
			}
			// 3.1 스크롤 바를 올린다
			pScrollBar->SetScrollPos(nCurPos);
			// 3.2 화면을 위로 올린다
			this->scroll.Move(x, y);
			break;

			//4. 하단 공간을 눌렀을 때
		case SB_PAGEDOWN:
			nCurPos += 5;
			y += rect.bottom / 100 * 5;
			if (nCurPos > SCROLL_MAX) {
				nCurPos = SCROLL_MAX;
				y = rect.bottom;
			}
			pScrollBar->SetScrollPos(nCurPos);
			this->scroll.Move(x, y);
			break;

			//5. 스크롤 바의 위치가 변했을 때
		case SB_THUMBPOSITION:
			// 5.1 변한 위치로 스크롤 바를 이동시킨다.
			pScrollBar->SetScrollPos(nPos);
			// 5.2 변한 위치만큼 화면을 이동시킨다.
			if (nPos < nCurPos) {
				this->scroll.Move(x, y - rect.bottom / 100 * (nCurPos - nPos));
			}
			else {
				this->scroll.Move(x, y + rect.bottom / 100 * (nPos - nCurPos));
			}
			break;

			//6. 스크롤 바가 이동할 때
		case SB_THUMBTRACK:
			// 6.1 이동한 위치로 스크롤 바를 이동시킨다
			pScrollBar->SetScrollPos(nPos);
			// 6.2 이동한 위치만큼 화면을 이동시킨다
			if (nPos < nCurPos) {
				this->scroll.Move(x, y - rect.bottom / 100 * (nCurPos - nPos));
			}
			else {
				this->scroll.Move(x, y + rect.bottom / 100 * (nPos - nCurPos));
			}
			break;

		default:
			break;
		}
	}
	RedrawWindow();
}

// 윈도우 사이즈가 변화되었을 때
void MindMappingForm::OnSize(UINT nType, int cx, int cy) {
	// 스크롤 바 크기 변화 시 조정 필요
}

//스크롤 휠을 누를 때
BOOL MindMappingForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	int ret = 0;
	RECT rect;
	GetClientRect(&rect);
	int x = this->scroll.GetX();
	int y = this->scroll.GetY();

	// 1. 휠을 아래로 내릴 때
	if (zDelta < 0) {
		// 1.1 ctrl 키를 눌렀는지 확인한다
		if (nFlags == MK_CONTROL) {
			// 1.1.1 참이면
			// 1.1.1.1 스크롤 바를 늘린다
			// 1.1.1.2 축소한 비율만큼 테두리, 토픽박스, 이미지, 관계선을 그린다.
			this->r = this->r * 9 / 10;
			if (this->r <= 3 / 10) {
				this->r = 3 / 10;
			}
			ret = -1;
		}
		else {
			// 1.1.2 거짓이면
			int pos = this->vscrollBar.GetScrollPos();
			int curPos = pos + 3;
			// 1.1.2.1 스크롤 바를 내린다
			// 1.1.2.2 작업영역을 아래로 내린다
			if (curPos >= 100) {
				curPos = 100;
				this->vscrollBar.SetScrollPos(curPos);
				this->scroll.Move(x, y + rect.bottom / 100 * (curPos - pos));
			}
			else {
				this->vscrollBar.SetScrollPos(curPos);
				this->scroll.Move(x, y + rect.bottom / 100 * 3);
			}
			ret = -1;
		}
	}
	// 2. 휠을 위로 올릴 때
	else if (zDelta > 0) {
		// 2.1 ctrl 키를 눌렀는지 확인한다
		if (nFlags == MK_CONTROL) {
			// 2.1.1 참이면
			// 2.1.1.1 스크롤 바를 줄인다
			// 2.1.1.2 확대한 비율만큼 테두리, 토픽박스, 이미지, 관계선을 그린다.
			this->r = this->r * 11 / 10;
			if (this->r >= 3) {
				this->r = 3;
			}
			ret = -1;
		}
		else {
			// 2.1.2 거짓이면
			// 2.1.2.1 스크롤 바를 올린다
			// 2.1.2.2 작업영역을 위로 올린다.
			int pos = this->vscrollBar.GetScrollPos();
			int curPos = pos - 3;
			if (curPos <= 0) {
				curPos = 0;
				this->vscrollBar.SetScrollPos(curPos);
				this->scroll.Move(x, y - rect.bottom / 100 * (pos - curPos));
			}
			else {
				this->vscrollBar.SetScrollPos(pos - 3);
				this->scroll.Move(x, y - rect.bottom / 100 * 3);
			}
			ret = -1;
		}
	}
	RedrawWindow();
	return ret;
}
// 화면에 입력한 위치가 프레임 범위 내에 해당하는지 알려주는 메소드
Frame* MindMappingForm::IsFrame(CPoint point) {
	Frame *frame = 0;
	Iterator<Array<Frame*>> *mindMapIterator = mindMap->CreateIterator();
	for (mindMapIterator->First(); !mindMapIterator->IsDone(); mindMapIterator->Next()) {
		Array<Frame*> arrayFrame = mindMapIterator->Current();
		int i = 0;
		while (i < arrayFrame.GetLength() && frame == 0) {
			Frame *currentFrame = arrayFrame.GetAt(i);
			int x = currentFrame->GetX();
			int y = currentFrame->GetY();
			int width = currentFrame->GetWidth();
			int height = currentFrame->GetHeight();
			RECT rect;
			GetClientRect(&rect);
			CRect frameRect = CRect((x - scroll.GetX() - rect.right / 2)*r + rect.right / 2, (y - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2,
				(x + width - scroll.GetX() - rect.right / 2)*r + rect.right / 2, (y + height - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2);
			if (frameRect.PtInRect(point)) {
				frame = currentFrame;
			}
			i++;
		}
	}
	return frame;
}

// 확대 버튼을 클릭했을 때
void MindMappingForm::OnZoomInButtonClicked() {
	// 1. 작업영역의 중심위치를 읽는다
	this->r = this->r * 11 / 10;
	if (this->r >= 3) {
		this->r = 3;
	}
	// 2. 확대한 비율만큼 테두리, 토픽박스, 이미지, 관계선을 그린다.
	RedrawWindow();
}

// 축소 버튼을 클릭했을 때
void MindMappingForm::OnZoomOutButtonClicked() {
	// 1. 작업영역의 중심위치를 읽는다
	this->r = this->r * 9 / 10;
	if (this->r <= 3 / 10) {
		this->r = 3 / 10;
	}
	// 2. 축소한 비율만큼 테두리, 토픽박스, 이미지, 관계선을 그린다.
	RedrawWindow();
}

// 잘라내기 버튼을 클릭했을 때
void MindMappingForm::OnCutButtonClicked() {
	// 1. 자를 토픽을 정한다.
	if (this->current != 0 && this->current != this->mindMap->GetCenterFrame()) {
		// 2. 정한 토픽과 하위 토픽들을 옮겨 적는다.
		int i = 0;
		int j = 0;
		int length = current->GetLength();
		while (i < length) {
			Shape *shape = this->current->GetChild(j);
			if (!dynamic_cast<ConnectingLine*>(shape)) {
				j++;
			}
			else {
				this->current->Remove(shape);
			}
			i++;
		}
		this->copyFrame = new Frame(*this->current);
		// 3. 정한 토픽과 하위 토픽들을 지운다.
		Frame *parentFrame = this->mindMap->HighRankFrame(this->current);
		parentFrame->Remove(this->current);
		RedrawWindow();
	}
}

// 복사하기 버튼을 클릭했을 때
void MindMappingForm::OnCopyButtonClicked() {
	// 1. 복사할 토픽을 정한다.
	if (this->current != 0 && this->current != this->mindMap->GetCenterFrame()) {
		// 2. 정한 토픽과 하위 토픽들을 옮겨 적는다.
		this->copyFrame = new Frame(*this->current);
	}
}

// 붙여넣기 버튼을 클릭했을 때
void MindMappingForm::OnPasteButtonClicked() {
	// 1. 붙여넣을 토픽을 정한다.
	if (this->current != 0 && this->copyFrame != 0) {
		// 2. 옮길 위치를 정한다 <방사형 형태로 필요>
		// 3. 옮겨 놓은 토픽들을 정한 위치에 옮겨 적는다.
		//this->copyFrame->LeftRadialSort();
		(this->current)->Add(this->copyFrame);
		// 4. 옮길 토픽 테두리와 정한 토픽 테두리 사이에 관계선을 그린다.
		RedrawWindow();
	}
}

// 실행취소 버튼을 클릭했을 때
void MindMappingForm::OnUnDoButtonClicked() {
	if (task.GetUnDoLength() > 0) {
		task.ReDoWrite(this->mindMap);
		MindMap *unDoMindMap = task.UnDoRead();
		*this->mindMap = *unDoMindMap;
		delete unDoMindMap;
	}
	RedrawWindow();
}

// 다시실행 버튼을 클릭했을 때
void MindMappingForm::OnReDoButtonClicked() {
	if (task.GetReDoLength() > 0) {
		task.UnDoWrite(this->mindMap);
		MindMap *reDoMindMap = task.ReDoRead();
		*this->mindMap = *reDoMindMap;
		delete reDoMindMap;
	}
	RedrawWindow();
}

// 자동정렬 콤보 박스 항목을 클릭했을 때
void MindMappingForm::OnComboBoxButtonClicked() {
	if (this->current != 0) {
		int index = directionType.GetCurSel();
		if (index == 0) {
			current->LeftRadialSort();
		}
		else if (index == 1) {
			current->RightRadialSort();
		}
		else if (index == 2) {
			current->RadialSort();
		}
	}
	RedrawWindow();

}

// 저장 버튼을 눌렀을 때
void MindMappingForm::OnSaveButtonClicked() {
	this->mindMap->Save();
	RedrawWindow();
}

// 불러오기 버튼을 눌렀을때
void MindMappingForm::OnLoadButtonClicked() {
	CFileDialog load(TRUE, NULL, NULL, OFN_READONLY, _T("DAT File(*.dat)|*.dat"));
	if (load.DoModal() == IDOK) {
		//Frame *centerFrame = mindMap->GetCenterFrame();
		delete mindMap->GetCenterFrame();
		CString filePath = load.GetPathName();
		CT2CA pszConvertedAnsiString(filePath);
		this->mindMap->Load(std::string(pszConvertedAnsiString));
	}
	RedrawWindow();

}

// 다른이름으로 저장 버튼을 눌렀을 때
void MindMappingForm::OnSaveAsButtonClicked() {
	CFileDialog saveAs(FALSE, _T("dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("DAT File(*.dat)"), NULL);
	if (saveAs.DoModal() == IDOK) {
		CString filePath = saveAs.GetPathName();
		CT2CA pszConvertedAnsiString(filePath);
		this->mindMap->SaveAs(std::string(pszConvertedAnsiString));
	}
	RedrawWindow();
}


// 새로만들기 버튼을 눌렀을 때
void MindMappingForm::OnNewButtonClicked() {
	// 마지막 작업을 적는다
	task.UnDoWrite(mindMap);
	Frame *centerFrame = this->mindMap->GetCenterFrame();
	int i = 0;
	int j = 0;
	int length = centerFrame->GetLength();
	while (i < length) {
		Shape *shape = centerFrame->GetChild(j);
		if (dynamic_cast<TopicBox*>(shape)) {
			j++;
		}
		else {
			centerFrame->Remove(j);
		}
		i++;
	}
	RedrawWindow();
}
// 삭제버튼을 눌렀을 때
void MindMappingForm::OnDeleteButtonClicked() {
	//1. 프레임이 선택된 상태에서 삭제버튼을 눌렀을 경우 
	if (this->current != 0) {
		// 1.1. 선택한 테두리를 읽는다
		if (this->current != this->mindMap->GetCenterFrame()) {
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			// 1.2. 마지막 작업을 적는다.
			task.UnDoWrite(mindMap);
			// 1.3 선택한 테두리를 지운다.
			highRankFrame->Remove(current);
			this->current = highRankFrame;
		}
	}
	RedrawWindow();
}

// 강조하기 버튼을 눌렀을 때
void MindMappingForm::OnEmphasizeButtonClicked() {
	if (this->current != 0) {
		task.UnDoWrite(this->mindMap);
		this->current->Emphasize();
	}
	RedrawWindow();
}
// 왼쪽마우스를 더블클릭했을 때
void MindMappingForm::OnLButtonDblClk(UINT nFlags, CPoint point) {
	TopicBox *topicBox = 0;
	int x;
	int y;
	int j = 0;
	int k = 0;
	int l = 0;
	int rCount = 0;// \r의 개수
	TCHAR(*topic) = 0;
	//누른 위치가 테두리 안인지 확인한다.
	Frame *frame = IsFrame(point);
	if (frame != 0) {
		int i = 0;
		while (i < frame->GetLength()) {
			Shape *shape = frame->GetChild(i);
			//프레임에서 토픽박스를 찾는다.
			if (dynamic_cast<TopicBox*>(shape)) {
				mouseDouble = true;
				topicBox = (TopicBox*)shape;
				x = topicBox->GetX();
				y = topicBox->GetY();
				topic = topicBox->GetTopic();
				//'\r'(줄바꿈) 개수를 센다.
				while (j < topicBox->GetLength()) {
					if (topic[j] == L'\r') {
						rCount++;
					}
					j++;
				}
				CClientDC dc(this);
				//한 줄 토픽과 여러 줄 토픽을 구분해 캐럿을 이동시킨다.
				if (rCount != 0) {
					//토픽의 줄 수가 1개 이상인경우 (\r >1)
					j = 0;
					while (topic[j] != L'\0'&& k != rCount) {
						if (topic[j] == L'\r') {
							k++;
						}
						j++;
					}
					//가장 마지막 줄 문자열을 temp에 담는다.
					TCHAR(*temp) = 0;
					temp = new TCHAR[topicBox->GetLength()];
					while (topic[j] != L'\0') {
						temp[l] = topic[j];
						l++;
						j++;
					}
					temp[l] = L'\0';
					CString cstr;
					CSize tempSize;
					if (l>0) {
						cstr.Format(_T("%s"), temp);
						tempSize = dc.GetTextExtent(cstr);
						this->caret = new Caret(x + tempSize.cx, y + tempSize.cy*rCount);
					}
					else {
						CString str;
						str.Format(_T("%s"), topic);
						tempSize = dc.GetTextExtent(str);
						this->caret = new Caret(x, y + tempSize.cy*rCount);
					}
				}
				else {
					//토픽이 한 줄인 경우(\r=0)
					CString str;
					str.Format(_T("%s"), topic);
					CSize size = dc.GetTextExtent(str, str.GetLength());
					this->caret = new Caret(x + size.cx, y);
				}
			}
			i++;
		}
	}
	RedrawWindow();
}
void MindMappingForm::OnPrintButtonClicked() {
	int nWidth, nHeight;
	CClientDC dc(this);//this->pImgWnd
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	CRect rect;
	GetClientRect(rect);
	nWidth = rect.Width();
	nHeight = rect.Height();

	CBitmap BMP;
	BMP.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = MemDC.SelectObject(&BMP);
	MemDC.BitBlt(0, 0, nWidth, nHeight, &dc, 0, 0, SRCCOPY);


	HANDLE hDib;

	LPSTR pDib;
	LPBITMAPINFO lpBitInfo;
	HANDLE hlpBitInfo;
	//CBitmap BMP;

	//BMP.LoadBitmap(IDB_BITMAP1);

	hDib = GlobalAlloc(GHND, nWidth*nHeight * 3);
	pDib = (LPSTR)GlobalLock(hDib);
	hlpBitInfo = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFO));
	lpBitInfo = (LPBITMAPINFO)GlobalLock(hlpBitInfo);

	//BITMAPINFO
	lpBitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBitInfo->bmiHeader.biWidth = nWidth;
	lpBitInfo->bmiHeader.biHeight = nHeight;
	lpBitInfo->bmiHeader.biPlanes = 1;
	lpBitInfo->bmiHeader.biBitCount = 24;
	lpBitInfo->bmiHeader.biCompression = BI_RGB;
	lpBitInfo->bmiHeader.biSizeImage = nWidth * nHeight * 3;
	lpBitInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBitInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBitInfo->bmiHeader.biClrUsed = 0;
	lpBitInfo->bmiHeader.biClrImportant = 0;
	////BITMAPINFO

	HDC hdc = ::GetDC(this->m_hWnd);
	GetDIBits(hdc, (HBITMAP)BMP, 0, nHeight, pDib, lpBitInfo, DIB_RGB_COLORS);
	::ReleaseDC(this->m_hWnd, hdc);

	static DOCINFO docinfo = { sizeof(DOCINFO), _T("IMAGE"), NULL };

	CPrintDialog dlg(FALSE);
	if (dlg.DoModal() == IDCANCEL)
	{

		GlobalUnlock(hDib);
		GlobalFree(hDib);
		GlobalUnlock(hlpBitInfo);
		GlobalFree(hDib);

		DeleteObject(BMP.m_hObject);

		DeleteDC(MemDC.m_hDC);
		DeleteDC(dc.m_hDC);

		return;
	}
	HDC hpdc = dlg.GetPrinterDC();
	int cx, cy;

	cy = GetDeviceCaps(hpdc, VERTRES);
	cx = GetDeviceCaps(hpdc, HORZRES);

	if (StartDoc(hpdc, &docinfo))
	{
		if (StartPage(hpdc))
		{
			StretchDIBits(hpdc,
				0, 0, cx, cy, 0, 0, nWidth, nHeight, pDib, lpBitInfo, DIB_RGB_COLORS, SRCCOPY);
			EndPage(hpdc);
		}
		EndDoc(hpdc);
	}



	GlobalUnlock(hDib);
	GlobalFree(hDib);
	GlobalUnlock(hlpBitInfo);
	GlobalFree(hlpBitInfo);

	DeleteObject(BMP.m_hObject);
	DeleteDC(dc.m_hDC);
	DeleteDC(MemDC.m_hDC);
	::RestoreDC(hpdc, -1);
}
void MindMappingForm::OnHelpButtonClicked() {
	CString path = "빈트리 도움말.chm";
	::ShellExecute(::GetDesktopWindow(), _T("open"), path, 0, 0, SW_SHOWMAXIMIZED);
}

void MindMappingForm::OnInformationButtonClicked() {
	InformationForm informationForm;
	informationForm.DoModal();
}

void MindMappingForm::LeftRadialSort()
{
	current->LeftRadialSort();
	RedrawWindow();
}

void MindMappingForm::RightRadialSort()
{
	current->RightRadialSort();
	RedrawWindow();
}

void MindMappingForm::RadialSort()
{
	current->RadialSort();
	RedrawWindow();
}

// 닫기 버튼을 클릭했을 때
void MindMappingForm::OnClose() {
	// 마인드 맵을 지운다.
	delete this->mindMap;
	// 윈도우를 닫는다.
	CFrameWnd::OnClose();
}
