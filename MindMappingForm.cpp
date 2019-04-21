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
	//����
	ON_BN_CLICKED(1, &MindMappingForm::OnNewButtonClicked)
	ON_BN_CLICKED(2, &MindMappingForm::OnLoadButtonClicked)
	ON_BN_CLICKED(3, &MindMappingForm::OnSaveButtonClicked)
	ON_BN_CLICKED(4, &MindMappingForm::OnSaveAsButtonClicked)
	ON_BN_CLICKED(5, &MindMappingForm::OnPrintButtonClicked)
	ON_BN_CLICKED(6, &MindMappingForm::OnClose)
	//����
	ON_BN_CLICKED(7, &MindMappingForm::OnCutButtonClicked)
	ON_BN_CLICKED(8, &MindMappingForm::OnCopyButtonClicked)
	ON_BN_CLICKED(9, &MindMappingForm::OnPasteButtonClicked)
	ON_BN_CLICKED(10, &MindMappingForm::OnUnDoButtonClicked)
	ON_BN_CLICKED(11, &MindMappingForm::OnReDoButtonClicked)
	ON_BN_CLICKED(12, &MindMappingForm::OnEmphasizeButtonClicked)
	ON_BN_CLICKED(13, &MindMappingForm::OnInsertImageButtonClicked)

	//����
	ON_BN_CLICKED(14, &MindMappingForm::OnFindButtonClicked)
	ON_BN_CLICKED(15, &MindMappingForm::OnZoomInButtonClicked)
	ON_BN_CLICKED(16, &MindMappingForm::OnZoomOutButtonClicked)

	//����
	ON_BN_CLICKED(17, &MindMappingForm::LeftRadialSort)
	ON_BN_CLICKED(18, &MindMappingForm::RightRadialSort)
	ON_BN_CLICKED(19, &MindMappingForm::RadialSort)

	// ����
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

// �����찡 ���� �Ǿ��� ��
int MindMappingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	RECT rect;
	GetClientRect(&rect);

	// 1. ������ũ���� �����
	RECT vertRect;
	vertRect = rect;
	vertRect.left = vertRect.right - 20;
	vertRect.bottom = vertRect.bottom - 20;
	vscrollBar.Create(WS_CHILD | WS_BORDER | SBS_VERT, vertRect, this, 4001);
	vscrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
	vscrollBar.SetScrollRange(0, 100);
	vscrollBar.SetScrollPos(0);

	// 2. ����ũ���� �����
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
	menuPopup.AppendMenuW(MF_STRING, 1, _T("���� �����"));
	menuPopup.AppendMenuW(MF_STRING, 2, _T("����"));
	menuPopup.AppendMenuW(MF_STRING, 3, _T("����"));
	menuPopup.AppendMenuW(MF_STRING, 4, _T("�ٸ� �̸����� ����"));
	menuPopup.AppendMenuW(MF_STRING, 5, _T("�μ�"));
	menuPopup.AppendMenuW(MF_STRING, 6, _T("������"));
	menuMain.AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("����"));
	SetMenu(&menuMain);
	menuMain.Detach();

	// ����
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 7, _T("�߶󳻱�"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 8, _T("�����ϱ�"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 9, _T("�����ֱ�"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 10, _T("�������"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 11, _T("�ٽý���"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 12, _T("�����ϱ�"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 13, _T("�̹�������"));


	CMenu *pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("����"));

	//����
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 14, _T("ã��"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 15, _T("Ȯ��"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 16, _T("���"));

	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("����"));

	//����
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 17, _T("���� �����"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 18, _T("������ �����"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 19, _T("�����"));

	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("����"));


	//����
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 20, _T("����"));
	menuPopup.AppendMenuW(MF_STRING || MF_OWNERDRAW, 21, _T("����"));
	pMenu = GetMenu();
	pMenu->AppendMenuW(MF_POPUP, (UINT_PTR)menuPopup.Detach(), _T("��Ÿ"));

	//// 3. Ȯ�� ��ư�� �����
	////zoomInButton.Create(_T(" Ȯ�� "), BS_DEFPUSHBUTTON, CRect(100, 100, 200, 200), this, 101);
	//// 4. ��� ��ư�� �����
	//zoomOutButton.Create(_T(" ��� "), BS_DEFPUSHBUTTON, CRect(200, 100, 300, 200), this, 102);
	//// 5. �߶󳻱� ��ư�� �����
	//cutButton.Create(_T(" �߶󳻱� "), BS_DEFPUSHBUTTON, CRect(0, 100, 100, 200), this, 103);
	//// 6. �����ϱ� ��ư�� �����
	//copyButton.Create(_T(" �����ϱ� "), BS_DEFPUSHBUTTON, CRect(0, 200, 100, 300), this, 104);
	//// 7. �ٿ��ֱ� ��ư�� �����
	//pasteButton.Create(_T(" �ٿ��ֱ� "), BS_DEFPUSHBUTTON, CRect(0, 300, 100, 400), this, 105);
	//// 8. �̹��� ���� ��ư�� �����
	//imageButton.Create(_T("�̹�������"), BS_DEFPUSHBUTTON, CRect(100,200,200,300), this, 100);
	//// 9. ã�� ��ư�� �����
	//findButton.Create(_T("ã��"), BS_DEFPUSHBUTTON, CRect(0, 0, 100, 100), this, 106);
	//// 10. ������� ��ư�� �����
	//unDoButton.Create(_T("�������"), BS_DEFPUSHBUTTON, CRect(100, 0, 200, 100), this, 107);
	//// 11. �ٽý��� ��ư�� �����
	//reDoButton.Create(_T("�ٽý���"), BS_DEFPUSHBUTTON, CRect(200, 0, 300, 100), this, 108);
	//// 12. ������� �޺��ڽ��� �����
	//directionType.Create(CBS_DROPDOWNLIST ,CRect(300,0,500,100),this,109);
	//// 13. ��������� �غ��Ѵ�
	//directionType.AddString(_T("���ʹ��������"));
	//directionType.AddString(_T("�����ʹ��������"));
	//directionType.AddString(_T("���������"));

	//// 14. ���� ��ư�� �����.
	//loadButton.Create(_T("����"), BS_DEFPUSHBUTTON, CRect(0, 400, 100, 500), this, 110);
	//// 15. �ҷ����� ��ư�� �����.
	//saveButton.Create(_T("�ҷ�����"), BS_DEFPUSHBUTTON, CRect(100, 400, 200, 500), this, 111);
	//// 16. ���θ���� ��ư�� �����.
	//newButton.Create(_T("���θ����"), BS_DEFPUSHBUTTON, CRect(200, 400, 300, 500), this, 112);
	//// 17. �����ϱ� ��ư�� �����.
	//deleteButton.Create(_T("����"), BS_DEFPUSHBUTTON, CRect(200, 200, 300, 300), this, 113);
	//// 18. �����ϱ� ��ư�� �����.
	//emphasizeButton.Create(_T("�����ϱ�"), BS_DEFPUSHBUTTON, CRect(100, 300, 200, 400), this, 114);
	//// * �μ��ϱ� ��ư�� �����.
	//printButton.Create(_T("�μ��ϱ�"), BS_DEFPUSHBUTTON, CRect(200, 300, 300, 400), this, 115);
	//// * �ٸ��̸��������� ��ư�� �����.
	//saveAsButton.Create(_T("�ٸ��̸����� ����"), BS_DEFPUSHBUTTON, CRect(0, 500, 100, 600), this, 116);
	//// * ���� ��ư�� �����
	//helpButton.Create(_T("����"), BS_DEFPUSHBUTTON,CRect(100,500,200,600),this,117);
	//// * ���� ��ư�� �����
	//informationButton.Create(_T("����"), BS_DEFPUSHBUTTON, CRect(200, 500, 300, 600), this, 118);
	// 19. ���ε���� �����
	this->mindMap = new MindMap;
	// 20. �߽� �׵θ��� �����
	Frame *centerFrame = this->mindMap->InsertCenterFrame(rect.right / 2 - 65, rect.bottom / 2 - 65, 130, 130);
	// 21. ���� ���ڸ� �����
	centerFrame->Add(new TopicBox(rect.right / 2 - 45, rect.bottom / 2 - 20, 90, 40, _T("�߽�����")));

	return 0;
}

BOOL MindMappingForm::PreTranslateMessage(MSG *pMsg) {

	//if (pMsg->message == WM_KEYDOWN) {
	//	//1. �������� ���õ� ���¿��� Delete Ű�� ������ ��� 
	//	if (pMsg->message == 256 && this->current != 0) {
	//		// 1.1. ������ �׵θ��� �д´�
	//		if (this->current != this->mindMap->GetCenterFrame()) {
	//			Frame *highRankFrame = mindMap->HighRankFrame(current);
	//			// 1.2. ������ �۾��� ���´�.
	//			task.UnDoWrite(mindMap);
	//			// 1.3 ������ �׵θ��� �����.
	//			highRankFrame->Remove(current);
	//			this->current = highRankFrame;
	//		}
	//	}
	//	//2. Ctrl + X Ű�� ������ ���
	//	else if (pMsg->message == 0x58 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 2.1 �ڸ� ������ ���Ѵ�
	//		if (this->current != 0) {
	//			// 2.2 ���� ���Ȱ� ���� ���ȵ��� �Ű� ���´�
	//			this->copyFrame = new Frame(*this->current);
	//			Frame *highRankFrame = this->mindMap->HighRankFrame(this->current);
	//			// 2.3 ������ �۾��� ���´�
	//			task.UnDoWrite(mindMap);
	//			// 2.4 ���� ���Ȱ� ���� ���ȵ��� �����.
	//			highRankFrame->Remove(this->current);
	//			this->current = 0;
	//		}
	//	}
	//	//3. Ctrl + C Ű�� ������ ���
	//	else if (pMsg->message == 0x43 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 3.1 ������ ������ ���Ѵ�
	//		if (this->current != 0) {
	//			// 3.2 ���� ���Ȱ� �������ȵ��� �Ű� ���´�.
	//			this->copyFrame = new Frame(*this->current);
	//		}
	//	}
	//	//4. Ctrl + V Ű�� ������ ���
	//	else if (pMsg->message == 0x56 && GetKeyState(VK_CONTROL) & 0x80000000) {
	//		// 4.1 �ٿ����� ������ ���Ѵ�.
	//		if (this->current != 0) {
	//			// 4.2 �ű� ��ġ�� ���Ѵ�. (��������� ��ġ�ϵ��� �۾� �ʿ�)
	//			// 4.3 �Ű� ���� ���ȵ��� ���� ��ġ�� �Ű� ���´�
	//			// 4.4 �ű� ���� �׵θ��� ���� �׵θ� ���̿� ���輱�� �׸��� (���� ���輱 ����� �۾� �ʿ�)
	//			this->current->Add(this->copyFrame);
	//		}
	//	}
	//	// 5. Ctrl+Z Ű�� ������ ���
	//	else if ((pMsg->message == 0x5A && GetKeyState(VK_CONTROL) & 0x80000000)) {
	//		//5.1. �ҷ����� ���� ������ �۾��� �����Ѵ�.
	//		//5.2. ���� �ֱ� �۾��� �ҷ��´�.
	//		if (task.GetUnDoLength() > 0) {
	//			task.ReDoWrite(this->mindMap);
	//			MindMap *unDoMindMap = task.UnDoRead();
	//			*this->mindMap = *unDoMindMap;
	//			delete unDoMindMap;
	//		}
	//	}
	//	// 6. Ctrl+Y Ű�� ������ ���
	//	else if ((pMsg->message == 0x59 && GetKeyState(VK_CONTROL) & 0x80000000)) {

	//		//6.2.1. �ҷ��������� ������ �۾��� �ҷ��´�. 
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
	//			int previousIndex;//���ڿ� �迭���� ���� '\r'�� ��ġ
	//			int rCount = 0;//\r�� ����
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//�����ӿ��� ���ȹڽ��� ã�´�.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					//���ȿ��� ���� ��ġ�� �д´�.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					Array<int> tempIndex(currentIndex + 1);//'\r'�� ��ġ���� ���� �����迭
	//														   //���ȿ� �ƹ��͵����� ���
	//					if (currentIndex == 0) {

	//					}
	//					//������ ä���� ���
	//					else {
	//						temp = new TCHAR[currentIndex + 1];
	//						temp[0] = topic[currentIndex - 1];
	//						temp[1] = L'\0';
	//						//���� ��ġ�� ������ ������ ���
	//						if (temp[0] != L'\r') {
	//							str.Format(_T("%s"), temp);
	//							size = dc.GetTextExtent(str);
	//							caret->Move(caret->GetX() - size.cx, caret->GetY());
	//						}
	//						//���� ��ġ�� ������ enter('\r')�� ���
	//						else {
	//							//enter('\r')�� ������ ����.
	//							while (j <= topicBox->GetLength()) {
	//								if (topic[j] == L'\r') {
	//									rCount++;
	//									tempIndex[l] = j;
	//									l++;
	//									//���� ��ġ ������ '\r'�� ã�� ���
	//									if (j == currentIndex - 1) {
	//										//���� ��ġ ������ '\r'�� ù ��°�� ���
	//										if (rCount == 1) {
	//											//���� ĳ�� ������ ������ ���
	//											if (currentIndex - 1 == 0) {
	//												str.Format(_T("%s"), temp);
	//												size = dc.GetTextExtent(str);
	//												caret->Move(caret->GetX(), caret->GetY() - size.cy);
	//											}
	//											//���� ĳ�� ������ ������ ���
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
	//										//���� ��ġ ������ '\r'�� ù ��°�� �ƴ� ���
	//										else {
	//											temp2 = new TCHAR[currentIndex + 1];
	//											temp2[0] = topic[currentIndex - 2];
	//											temp2[1] = L'\0';
	//											//���� ĳ�� ������ ������ ���
	//											if (temp2[0] == L'\r') {
	//												str.Format(_T("%s"), temp2);
	//												size = dc.GetTextExtent(str);
	//												caret->Move(caret->GetX(), caret->GetY() - size.cy);
	//											}
	//											//���� ĳ�� ������ ������ ���
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
	//			int rCount = 0;//\r�� ����
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//�����ӿ��� ���ȹڽ��� ã�´�.
	//				if (dynamic_cast<TopicBox*>(shape)) {
	//					topicBox = (TopicBox*)shape;
	//					topic = topicBox->GetTopic();
	//					x = topicBox->GetX();
	//					y = topicBox->GetY();
	//					width = topicBox->GetWidth();
	//					height = topicBox->GetHeight();
	//					//���ȿ��� ���� ��ġ�� �д´�.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					Array<int> tempIndex(currentIndex + 1);//'\r'�� ��ġ���� ���� �����迭
	//														   //���� ��ġ�� '\0'�� ��� 
	//					if (topic[currentIndex] == _T('\0')) {

	//					}
	//					//���� ��ġ�� '\r'�� ���
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
	//					//���� ��ġ�� ������ ���
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
	//			CSize firstSize;//���ȿ��� ���� ��ġ(currentIndex)�� ���� �������� ���� ����� ���� ������ ����
	//			CSize secondSize;//������ġ ���� �������� �ι�° ���Ϳ� ù��° ���� ������ ����
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
	//			int j = 0;//firsSize���̸� ������� �ݺ����� ����
	//			int k = 0;//÷�� ���� ����
	//			int l = 0;//secondSize���̸� ������� �ݺ����� ����
	//			int m;
	//			int rCount = 0;//\r�� ����
	//			int firstR;//���� ��ġ�κ��� ���� ����� \r�� ��ġ
	//			int secondR;//���� ��ġ�κ��� �ι�°�� ����� \r�� ��ġ
	//			int tempIndex;//���� ��ġ�� �ʱⰪ���� ���� �ݺ����� ����
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//�����ӿ��� ���ȹڽ��� ã�´�.
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
	//					//���ȿ��� ���� ��ġ�� �д´�.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					tempIndex = currentIndex - 1;
	//					//���� ��ġ�κ��� ���� ���⿡ �ִ� \r 2���� ã�´�.
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
	//						//���Ͱ� �ΰ� �϶�
	//						if (rCount == 2) {
	//							//ù ��° ���Ϳ��� ���� ��ġ������ ���̸� ���.
	//							l = secondR;
	//							l++;
	//							//�� ��° ���Ϳ� ù ��° ���� ������ ���̸� ��� ���Ѵ�.
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
	//						//���Ͱ� �� �� �϶�
	//						if (rCount == 1) {
	//							//�ι�° ���̸� ��� ù��° ���̿� ���Ѵ�.
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
	//			CSize leftSize;//���ȿ��� ���� ��ġ(currentIndex)�� ���� �������� ���� ����� ���� ������ ����
	//			CSize rightSize;//������ġ���� ������ ���� ������ ����
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
	//			int j = 0;//firsSize���̸� ������� �ݺ����� ����
	//			int k = 0;//÷�� ���� ����
	//			int l = 0;//secondSize���̸� ������� �ݺ����� ����
	//			int m;
	//			int rCount = 0;//���� ��ġ ������ \r�� ����
	//			int lCount = 0;//���� ��ġ ���� ������ ����
	//			int leftR;//���� ��ġ���� ���ʿ� �ִ� \r�� ��ġ
	//			int rightR;//���� ��ġ���� �����ʿ� �ִ� \r�� ��ġ
	//			int tempIndex;//���� ��ġ�� �ʱⰪ���� ���� �ݺ����� ����
	//			int tempIndex2;
	//			bool R = false;
	//			while (i < current->GetLength()) {
	//				Shape *shape = current->GetChild(i);
	//				//�����ӿ��� ���ȹڽ��� ã�´�.
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
	//					//���ȿ��� ���� ��ġ�� �д´�.
	//					currentIndex = topicBox->GetCurrentIndex();
	//					tempIndex = currentIndex;
	//					tempIndex2 = currentIndex - 1;
	//					rightSize.cx = 0;
	//					rightSize.cy = 0;
	//					thirdSize.cx = 0;
	//					thirdSize.cy = 0;
	//					//���� ��ġ �����ʿ� ���Ͱ� �ִ��� Ȯ��

	//					while (topic[tempIndex] != _T('\0') && rCount != 1) {
	//						if (topic[tempIndex] == _T('\r')) {
	//							rightR = tempIndex;
	//							rCount++;
	//						}
	//						tempIndex++;
	//					}
	//					//���� ��ġ �����ʿ� ���Ͱ� �ִ� ���
	//					if (rCount == 1) {
	//						//���� ��ġ ���ʿ� ���Ͱ� �ִ��� Ȯ���Ѵ�.
	//						while (tempIndex2 >= 0 && lCount != 1) {
	//							if (topic[tempIndex2] == _T('\r')) {
	//								leftR = tempIndex2;
	//								lCount++;
	//							}
	//							tempIndex2--;
	//						}
	//						//���� ��ġ ���ʿ� ���Ͱ� �ִ� ���
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
	//						//���� ��ġ �ٷ� ������ ������ ��� 
	//						else if (topic[currentIndex - 1] == _T('\r')) {

	//						}
	//						//���� ��ġ ���ʿ� ���Ͱ� ���� ���
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
	//						//���� ��ġ ���� ���̿� ������ ���� ������ ���̸� ���Ѵ�.
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
	//					//���� ��ġ �����ʿ� ���Ͱ� ���� ���
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

// ã�� ��ư�� Ŭ������ ��
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

// ��Ŀ���� �������� ��
void MindMappingForm::OnSetFocus(CWnd *pOldWnd) {
	CWnd::OnSetFocus(pOldWnd);
	CreateSolidCaret(1, 20);
}
// ���� ���۸�
void MindMappingForm::DrawImage() {
	// 1. ���� ��ũ���� �׸���
	vscrollBar.ShowScrollBar();
	// 2. ���� ��ũ���� �׸���
	hscrollBar.ShowScrollBar();
	// 3. Ȯ���ư�� �׸���
	//zoomInButton.ShowWindow(SW_SHOW);
	//// 4. ��ҹ�ư�� �׸���
	//zoomOutButton.ShowWindow(SW_SHOW);
	//// 5. �߶󳻱� ��ư�� �׸���
	//cutButton.ShowWindow(SW_SHOW);
	//// 6. �����ϱ� ��ư�� �׸���.
	//copyButton.ShowWindow(SW_SHOW);
	//// 7. �ٿ��ֱ� ��ư�� �׸���.
	//pasteButton.ShowWindow(SW_SHOW);
	//findButton.ShowWindow(SW_SHOW);
	//// 8. �̹��� ��ư�� �׸���.
	//imageButton.ShowWindow(SW_SHOW);
	//// 9. ������� ��ư�� �׸���.
	//unDoButton.ShowWindow(SW_SHOW);
	////10. �ٽý��� ��ư�� �׸���.
	//reDoButton.ShowWindow(SW_SHOW);
	//// 11. ������� �޺��ڽ��� �׸���
	//directionType.ShowWindow(SW_SHOW);
	//// 12. �����ϱ� ��ư�� �׸���.
	//saveButton.ShowWindow(SW_SHOW);
	//// 13. �ҷ����� ��ư�� �׸���.
	//loadButton.ShowWindow(SW_SHOW);
	//// 14. ���θ���� ��ư�� �׸���.
	//newButton.ShowWindow(SW_SHOW);
	//// 15. �����ϱ� ��ư�� �׸���.
	//deleteButton.ShowWindow(SW_SHOW);
	//// 16. �����ϱ� ��ư�� �׸���.
	//emphasizeButton.ShowWindow(SW_SHOW);
	//// * �μ��ϱ� ��ư�� �׸���.
	//printButton.ShowWindow(SW_SHOW);
	//// * �ٸ��̸��������� ��ư�� �׸���.
	//saveAsButton.ShowWindow(SW_SHOW);
	//// * ���� ��ư�� �׸���.
	//helpButton.ShowWindow(SW_SHOW);
	//// * ���� ��ư�� �׸���.
	//informationButton.ShowWindow(SW_SHOW);
	// 17. �߽��׵θ��� ����Ѵ�.
	// 18. ���� ���ڸ� ����Ѵ�.
	// 19. ���� ���ڿ� �Է��� ���ڸ� ����Ѵ�
	// 20. ���� ��ġ�� �̹����� ����Ѵ�.
	// 21. ������ �׵θ��� ���ü��� �׸���.
	// 22. ���ἱ�� �׸���.

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
// �����찡 ���� ���� ��
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
	// 1. ���õ� �������� ���� �ڽ��� ������ �д´�
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
			// 3. ��ģ ������ ũ�⸦ ���
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
	else if (lParam & GCS_COMPSTR) {  // �������̸�;.
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

// ���� Ű�� ���� ��
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

		//ctrl Ű�� ���� ���� �۾��� ������ �ʵ��� ��
		if (GetKeyState(VK_CONTROL) & 0x80000000) {
		}
		else {
			int i = 0;
			int j = 0;
			TopicBox *topicBox = 0;
			// 1. ���õ� �������� ���� �ڽ��� ������ �д´�
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
			// 2. ���� �Է� ���� ���ڿ� ���������� ��ģ��
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

							// 3. ��ģ ������ ũ�⸦ ���
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

							// 3. ��ģ ������ ũ�⸦ ���
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
					// 3. ��ģ ������ ũ�⸦ ���
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
// Ű���带 ���� ��
void MindMappingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//1. �������� ���õ� ���¿��� Delete Ű�� ������ ��� 
	if (nChar == VK_DELETE && this->current != 0) {
		// 1.1. ������ �׵θ��� �д´�
		if (this->current != this->mindMap->GetCenterFrame()) {
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			// 1.2. ������ �۾��� ���´�.
			task.UnDoWrite(mindMap);
			// 1.3 ������ �׵θ��� �����.
			highRankFrame->Remove(current);
			this->current = highRankFrame;
		}
	}
	//2. Ctrl + X Ű�� ������ ���
	else if (nChar == 0x58 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 2.1 �ڸ� ������ ���Ѵ�
		if (this->current != 0) {
			// 2.2 ���� ���Ȱ� ���� ���ȵ��� �Ű� ���´�
			this->copyFrame = new Frame(*this->current);
			Frame *highRankFrame = this->mindMap->HighRankFrame(this->current);
			// 2.3 ������ �۾��� ���´�
			task.UnDoWrite(mindMap);
			// 2.4 ���� ���Ȱ� ���� ���ȵ��� �����.
			highRankFrame->Remove(this->current);
			this->current = 0;
		}
	}
	//3. Ctrl + C Ű�� ������ ���
	else if (nChar == 0x43 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 3.1 ������ ������ ���Ѵ�
		if (this->current != 0) {
			// 3.2 ���� ���Ȱ� �������ȵ��� �Ű� ���´�.
			this->copyFrame = new Frame(*this->current);
		}
	}
	//4. Ctrl + V Ű�� ������ ���
	else if (nChar == 0x56 && GetKeyState(VK_CONTROL) & 0x80000000) {
		// 4.1 �ٿ����� ������ ���Ѵ�.
		if (this->current != 0) {
			// 4.2 �ű� ��ġ�� ���Ѵ�. (��������� ��ġ�ϵ��� �۾� �ʿ�)
			// 4.3 �Ű� ���� ���ȵ��� ���� ��ġ�� �Ű� ���´�
			// 4.4 �ű� ���� �׵θ��� ���� �׵θ� ���̿� ���輱�� �׸��� (���� ���輱 ����� �۾� �ʿ�)
			this->current->Add(this->copyFrame);
		}
	}
	// 5. Ctrl+Z Ű�� ������ ���
	else if ((nChar == 0x5A && GetKeyState(VK_CONTROL) & 0x80000000)) {
		//5.1. �ҷ����� ���� ������ �۾��� �����Ѵ�.
		//5.2. ���� �ֱ� �۾��� �ҷ��´�.
		if (task.GetUnDoLength() > 0) {
			task.ReDoWrite(this->mindMap);
			MindMap *unDoMindMap = task.UnDoRead();
			*this->mindMap = *unDoMindMap;
			delete unDoMindMap;
		}
	}
	// 6. Ctrl+Y Ű�� ������ ���
	else if ((nChar == 0x59 && GetKeyState(VK_CONTROL) & 0x80000000)) {

		//6.2.1. �ҷ��������� ������ �۾��� �ҷ��´�. 
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
			int previousIndex;//���ڿ� �迭���� ���� '\r'�� ��ġ
			int rCount = 0;//\r�� ����
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//�����ӿ��� ���ȹڽ��� ã�´�.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					//���ȿ��� ���� ��ġ�� �д´�.
					currentIndex = topicBox->GetCurrentIndex();
					Array<int> tempIndex(currentIndex + 1);//'\r'�� ��ġ���� ���� �����迭
														   //���ȿ� �ƹ��͵����� ���
					if (currentIndex == 0) {

					}
					//������ ä���� ���
					else {
						temp = new TCHAR[currentIndex + 1];
						temp[0] = topic[currentIndex - 1];
						temp[1] = L'\0';
						//���� ��ġ�� ������ ������ ���
						if (temp[0] != L'\r') {
							str.Format(_T("%s"), temp);
							size = dc.GetTextExtent(str);
							caret->Move(caret->GetX() - size.cx, caret->GetY());
						}
						//���� ��ġ�� ������ enter('\r')�� ���
						else {
							//enter('\r')�� ������ ����.
							while (j <= topicBox->GetLength()) {
								if (topic[j] == L'\r') {
									rCount++;
									tempIndex[l] = j;
									l++;
									//���� ��ġ ������ '\r'�� ã�� ���
									if (j == currentIndex - 1) {
										//���� ��ġ ������ '\r'�� ù ��°�� ���
										if (rCount == 1) {
											//���� ĳ�� ������ ������ ���
											if (currentIndex - 1 == 0) {
												str.Format(_T("%s"), temp);
												size = dc.GetTextExtent(str);
												caret->Move(caret->GetX(), caret->GetY() - size.cy);
											}
											//���� ĳ�� ������ ������ ���
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
										//���� ��ġ ������ '\r'�� ù ��°�� �ƴ� ���
										else {
											temp2 = new TCHAR[currentIndex + 1];
											temp2[0] = topic[currentIndex - 2];
											temp2[1] = L'\0';
											//���� ĳ�� ������ ������ ���
											if (temp2[0] == L'\r') {
												str.Format(_T("%s"), temp2);
												size = dc.GetTextExtent(str);
												caret->Move(caret->GetX(), caret->GetY() - size.cy);
											}
											//���� ĳ�� ������ ������ ���
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
			int rCount = 0;//\r�� ����
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//�����ӿ��� ���ȹڽ��� ã�´�.
				if (dynamic_cast<TopicBox*>(shape)) {
					topicBox = (TopicBox*)shape;
					topic = topicBox->GetTopic();
					x = topicBox->GetX();
					y = topicBox->GetY();
					width = topicBox->GetWidth();
					height = topicBox->GetHeight();
					//���ȿ��� ���� ��ġ�� �д´�.
					currentIndex = topicBox->GetCurrentIndex();
					Array<int> tempIndex(currentIndex + 1);//'\r'�� ��ġ���� ���� �����迭
														   //���� ��ġ�� '\0'�� ��� 
					if (topic[currentIndex] == _T('\0')) {

					}
					//���� ��ġ�� '\r'�� ���
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
					//���� ��ġ�� ������ ���
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
			CSize firstSize;//���ȿ��� ���� ��ġ(currentIndex)�� ���� �������� ���� ����� ���� ������ ����
			CSize secondSize;//������ġ ���� �������� �ι�° ���Ϳ� ù��° ���� ������ ����
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
			int j = 0;//firsSize���̸� ������� �ݺ����� ����
			int k = 0;//÷�� ���� ����
			int l = 0;//secondSize���̸� ������� �ݺ����� ����
			int m;
			int rCount = 0;//\r�� ����
			int firstR;//���� ��ġ�κ��� ���� ����� \r�� ��ġ
			int secondR;//���� ��ġ�κ��� �ι�°�� ����� \r�� ��ġ
			int tempIndex;//���� ��ġ�� �ʱⰪ���� ���� �ݺ����� ����
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//�����ӿ��� ���ȹڽ��� ã�´�.
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
					//���ȿ��� ���� ��ġ�� �д´�.
					currentIndex = topicBox->GetCurrentIndex();
					tempIndex = currentIndex - 1;
					//���� ��ġ�κ��� ���� ���⿡ �ִ� \r 2���� ã�´�.
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
						//���Ͱ� �ΰ� �϶�
						if (rCount == 2) {
							//ù ��° ���Ϳ��� ���� ��ġ������ ���̸� ���.
							l = secondR;
							l++;
							//�� ��° ���Ϳ� ù ��° ���� ������ ���̸� ��� ���Ѵ�.
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
						//���Ͱ� �� �� �϶�
						if (rCount == 1) {
							//�ι�° ���̸� ��� ù��° ���̿� ���Ѵ�.
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
			CSize leftSize;//���ȿ��� ���� ��ġ(currentIndex)�� ���� �������� ���� ����� ���� ������ ����
			CSize rightSize;//������ġ���� ������ ���� ������ ����
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
			int j = 0;//firsSize���̸� ������� �ݺ����� ����
			int k = 0;//÷�� ���� ����
			int l = 0;//secondSize���̸� ������� �ݺ����� ����
			int m;
			int rCount = 0;//���� ��ġ ������ \r�� ����
			int lCount = 0;//���� ��ġ ���� ������ ����
			int leftR;//���� ��ġ���� ���ʿ� �ִ� \r�� ��ġ
			int rightR;//���� ��ġ���� �����ʿ� �ִ� \r�� ��ġ
			int tempIndex;//���� ��ġ�� �ʱⰪ���� ���� �ݺ����� ����
			int tempIndex2;
			bool R = false;
			while (i < current->GetLength()) {
				Shape *shape = current->GetChild(i);
				//�����ӿ��� ���ȹڽ��� ã�´�.
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
					//���ȿ��� ���� ��ġ�� �д´�.
					currentIndex = topicBox->GetCurrentIndex();
					tempIndex = currentIndex;
					tempIndex2 = currentIndex - 1;
					rightSize.cx = 0;
					rightSize.cy = 0;
					thirdSize.cx = 0;
					thirdSize.cy = 0;
					//���� ��ġ �����ʿ� ���Ͱ� �ִ��� Ȯ��

					while (topic[tempIndex] != _T('\0') && rCount != 1) {
						if (topic[tempIndex] == _T('\r')) {
							rightR = tempIndex;
							rCount++;
						}
						tempIndex++;
					}
					//���� ��ġ �����ʿ� ���Ͱ� �ִ� ���
					if (rCount == 1) {
						//���� ��ġ ���ʿ� ���Ͱ� �ִ��� Ȯ���Ѵ�.
						while (tempIndex2 >= 0 && lCount != 1) {
							if (topic[tempIndex2] == _T('\r')) {
								leftR = tempIndex2;
								lCount++;
							}
							tempIndex2--;
						}
						//���� ��ġ ���ʿ� ���Ͱ� �ִ� ���
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
						//���� ��ġ �ٷ� ������ ������ ��� 
						else if (topic[currentIndex - 1] == _T('\r')) {

						}
						//���� ��ġ ���ʿ� ���Ͱ� ���� ���
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
						//���� ��ġ ���� ���̿� ������ ���� ������ ���̸� ���Ѵ�.
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
					//���� ��ġ �����ʿ� ���Ͱ� ���� ���
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

// �̹��� ���� ��ư�� Ŭ������ ��
void MindMappingForm::OnInsertImageButtonClicked() {
	InsertingImageForm insertingImageForm;
	// 1. �̹��� ���� �����츦 �����Ѵ�
	if (insertingImageForm.DoModal() == IDOK) {
		insertingImageForm.OnOpenButtonClicked();
		RedrawWindow();
	}
}
// ���콺 ������ ������ ��
void MindMappingForm::OnLButtonDown(UINT nFlags, CPoint point) {
	// 1. ���� ��ġ�� �д´�
	this->mousePtDown = point;
	this->current = 0;
	this->moveIcon = false;
	this->mouseDown = false;


	Frame *frame = IsFrame(point);

	// 2. ���� ��ġ�� �׵θ� ������ Ȯ���Ѵ�.
	if (frame != 0) {
		// 2.1 ���̸�
		// 2.1.1 ���� �׵θ��� ǥ���Ѵ�
		this->current = frame;
		this->mouseDown = true;
		// 2.1.2 �̵� �������� Ȱ��ȭ ��Ų��.

		RECT rect;
		GetClientRect(&rect);

		//CRect moveRect = CRect(current->GetX() + current->GetWidth() / 2 - 7, current->GetY(), current->GetX() + current->GetWidth() / 2 - 7 + 14, current->GetY() + 14);

		CRect moveRect = CRect(((current->GetX() + current->GetWidth() / 2 - 7) - scroll.GetX() - rect.right / 2)*r + rect.right / 2,
			(current->GetY() - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2, (current->GetX() + current->GetWidth() / 2 - 7 + 14)*r + rect.right / 2,
			((current->GetY() + 14) - scroll.GetY() - rect.bottom / 2)*r + rect.bottom / 2);

		// 2.1.2.1 �̵� �������� ������ Ȯ���Ѵ�
		if (moveRect.PtInRect(point)) {
			// 2.1.2.1.1 ���̸� �̵� �������� �������� ���´�
			this->moveIcon = true;
		}
	}
	RedrawWindow();
}


// ���콺�� �����϶�
void MindMappingForm::OnMouseMove(UINT nFlags, CPoint point) {
	this->mouseMove = false;
	// * ���콺 ������ ��ġ�� ���´�.
	this->mousePtMove = point;
	// 1. ���콺 ���ʹ�ư�� �������� Ȯ���Ѵ�.
	if (mouseDown && (nFlags & MK_LBUTTON) == MK_LBUTTON) {
		//1.1 ���̸�
		CRect currentRect = CRect(current->GetX() - scroll.GetX(), current->GetY() - scroll.GetY(), current->GetX() + current->GetWidth() - scroll.GetX(),
			current->GetY() + current->GetHeight() - scroll.GetY());
		// 1.1.1 ���õ� �׵θ��� ��ġ�� �д´�
		// 1.1.2 ���� ���콺�� ������ ��ġ�� ���õ� ��ġ�� ������� Ȯ���Ѵ�.
		if (currentRect.PtInRect(point) == false)
			//1.1.2.1 ���̸� �������ٸ� ���´�.
		{
			this->mouseMove = true;
		}
	}
	CDC *pDC;
	pDC = this->GetDC();
	CString strPos;
	strPos.Format(_T("���콺 ��ǥ X : %04d  Y : %04d"), point.x, point.y);
	pDC->TextOut(500, 600, strPos);
	this->ReleaseDC(pDC);
}

// ���� ���콺�� ������
void MindMappingForm::OnLButtonUp(UINT nFlags, CPoint point) {
	// 1. ���콺�� �� ��ġ�� ���´�.
	mousePtUp = point;
	// 2. ���콺�� ��������, ���콺�� ����������, �̵��������� ������ �ʾҴ��� Ȯ���Ѵ�
	if (mouseDown == TRUE && mouseMove == TRUE && moveIcon == FALSE) {
		task.UnDoWrite(this->mindMap);
		//2.1 ���̸� �� ��ġ�� ������,���ȹڽ��� �����
		RECT rect;
		GetClientRect(&rect);
		//int index = current->Add(new Frame(point.x - 65 + scroll.GetX(), point.y - 65 + scroll.GetY(), 130, 130));
		int index = current->Add(new Frame((point.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - 65,
			(point.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - 65, 130, 130));

		Frame *childFrame = (Frame *)current->GetChild(index);
		childFrame->Add(new TopicBox((point.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - 45, (point.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - 20
			, 90, 40, _T("��������")));
		mouseDown = false;
		mouseMove = false;
	}
	// 3. �̵��������� �������� Ȯ���Ѵ�.
	if (moveIcon == TRUE) {
		// 3.1 ���̸� �� ��ġ�� ���������� Ȯ���Ѵ�.
		//3.1.1 ���̸�
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
			// 3.1.1.1 ���� ������ �ٲ۴�.
			Frame *temp = new Frame(*this->current);
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			highRankFrame->Remove(current);
			frame->Add(temp);

		}
		// 3.1.2 �����̸� ������ ��ġ�� �̵��Ѵ�.
		else {
			task.UnDoWrite(this->mindMap);
			OnMoveIconClicked();
			moveIcon = false;
		}
	}

	RedrawWindow();
}

// �̵���ư�� ������ ��
void MindMappingForm::OnMoveIconClicked() {

	RECT rect;
	GetClientRect(&rect);
	//1. �̵��� �Ÿ��� ���Ѵ�.
	int moveX = (mousePtUp.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2 - ((mousePtDown.x + scroll.GetX() - rect.right / 2) / r + rect.right / 2);
	int moveY = (mousePtUp.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2 - ((mousePtDown.y + scroll.GetY() - rect.bottom / 2) / r + rect.bottom / 2);


	// 2. �������� ��ġ�� �̵��� �Ÿ���ŭ �̵��Ѵ�.
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
			// 3. ���ȹڽ��� ��ġ�� �̵��� �Ÿ���ŭ �̵��Ѵ�.
			((TopicBox*)shape)->Resize(x + moveX, y + moveY, width, height);
		}
		if (dynamic_cast<ConnectingLine*>(shape)) {
			int childX = ((ConnectingLine*)shape)->GetChildX();
			int childY = ((ConnectingLine*)shape)->GetChildY();
			// 4. ���� �����Ӱ� ������ �������� ���ἱ�� ��ġ�� �̵��� �Ÿ���ŭ �̵��Ѵ�.
			((ConnectingLine*)shape)->Resize(x, y, childX + moveX, childY + moveY);
		}
		if (dynamic_cast<Frame*>(shape)) {
			int j = 0;
			while (j < ((Frame*)shape)->GetLength()) {
				Shape *shape2 = ((Frame*)shape)->GetChild(j);
				if (dynamic_cast<ConnectingLine*>(shape2)) {
					int childX = ((ConnectingLine*)shape2)->GetChildX();
					int childY = ((ConnectingLine*)shape2)->GetChildY();
					// 5. ���� �����Ӱ� ������ �������� ���ἱ�� �̵��� �Ÿ���ŭ �̵��Ѵ�.
					((ConnectingLine*)shape2)->Resize(current->GetX() + current->GetWidth() / 2, current->GetY() + current->GetHeight() / 2, childX, childY);
				}
				j++;
			}

		}
		i++;
	}
	RedrawWindow();
}

// ���� ��ũ���� ������ ��
void MindMappingForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	UINT nCurPos = pScrollBar->GetScrollPos();
	RECT rect;
	GetClientRect(&rect);

	int x = this->scroll.GetX();
	int y = this->scroll.GetY();

	if (pScrollBar->GetDlgCtrlID() == 4002)
	{
		switch (nSBCode) {
			// 1. ���� ��ư�� ������ ��
		case SB_LINELEFT:
			nCurPos -= 1;
			x -= rect.right / 100 * 1;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.left;
			}
			//1.1 ��ũ�� �ٸ� �·� �̵��Ѵ�
			pScrollBar->SetScrollPos(nCurPos);
			//1.2 ȭ���� �·� �̵��Ѵ�
			this->scroll.Move(x, y);
			break;

			// 2. ������ ��ư�� ������ ��
		case SB_LINERIGHT:
			nCurPos++;
			x += rect.right / 100 * 1;
			if (nCurPos > SCROLL_MAX) {
				nCurPos = SCROLL_MAX;
				x = rect.right;
			}
			//2.2 ��ũ�� �ٸ� ��� �̵��Ѵ�
			pScrollBar->SetScrollPos(nCurPos);
			//2.3 ȭ���� ��� �̵��Ѵ�
			this->scroll.Move(x, y);
			break;

			//3. ���� ������ ������ ��
		case SB_PAGELEFT:
			nCurPos -= 5;
			x -= rect.right / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.left;
			}
			// 3.1 ��ũ�� �ٸ� �·� �̵��Ѵ�
			pScrollBar->SetScrollPos(nCurPos);
			// 3.2 ȭ���� �·� �̵��Ѵ�
			this->scroll.Move(x, y);
			break;

			//4. ���� ������ ������ ��
		case SB_PAGERIGHT:
			nCurPos += 5;
			x += rect.right / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				x = rect.right;
			}
			// 4.1 ��ũ�� �ٸ� ��� �̵��Ѵ�.
			pScrollBar->SetScrollPos(nCurPos);
			// 4.2 ȭ���� ��� �̵��Ѵ�.
			this->scroll.Move(x, y);
			break;

			//5. ��ũ�� ���� ��ġ�� ������ ��
		case SB_THUMBPOSITION:
			// 5.1 ���� ��ġ�� ��ũ�� �ٸ� �̵���Ų��.
			pScrollBar->SetScrollPos(nPos);
			// 5.2 ���� ��ġ��ŭ ȭ���� �̵���Ų��.
			if (nPos < nCurPos) {
				this->scroll.Move(x - rect.right / 100 * (nCurPos - nPos), y);
			}
			else {
				this->scroll.Move(x + rect.right / 100 * (nPos - nCurPos), y);
			}
			break;

			//6. ��ũ�� �ٰ� �̵��� ��
		case SB_THUMBTRACK:
			// 6.1 �̵��� ��ġ�� ��ũ�� �ٸ� �̵���Ų��
			pScrollBar->SetScrollPos(nPos);
			// 6.2 �̵��� ��ġ��ŭ ȭ���� �̵���Ų��
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

// ���� ��ũ���� ������ ��
void MindMappingForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	if (pScrollBar->GetDlgCtrlID() == 4001) {
		RECT rect;
		GetClientRect(&rect);
		UINT nCurPos = pScrollBar->GetScrollPos();
		int x = scroll.GetX();
		int y = scroll.GetY();

		switch (nSBCode)
		{
			// 1. �� ��ư�� ������ ��
		case SB_LINEUP:
			nCurPos -= 1;
			y -= rect.bottom / 100 * 1;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				y = rect.top;
			}
			//1.1 ��ũ�� �ٸ� �ø���
			pScrollBar->SetScrollPos(nCurPos);
			//1.2 ȭ���� ���� �ø���
			this->scroll.Move(x, y);
			break;

			// 2. �Ʒ� ��ư�� ������ ��
		case SB_LINEDOWN:
			nCurPos += 1;
			y += rect.bottom / 100 * 1;
			if (nCurPos > SCROLL_MAX) {
				nCurPos = SCROLL_MAX;
				y = rect.bottom;
			}
			//2.2 ��ũ�� �ٸ� ������
			pScrollBar->SetScrollPos(nCurPos);
			//2.3 ȭ���� �Ʒ��� ������
			this->scroll.Move(x, y);
			break;

			// 3. ��� ������ ������ ��
		case SB_PAGEUP:
			nCurPos -= 5;
			y -= rect.bottom / 100 * 5;
			if (nCurPos < SCROLL_MIN) {
				nCurPos = SCROLL_MIN;
				y = rect.top;
			}
			// 3.1 ��ũ�� �ٸ� �ø���
			pScrollBar->SetScrollPos(nCurPos);
			// 3.2 ȭ���� ���� �ø���
			this->scroll.Move(x, y);
			break;

			//4. �ϴ� ������ ������ ��
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

			//5. ��ũ�� ���� ��ġ�� ������ ��
		case SB_THUMBPOSITION:
			// 5.1 ���� ��ġ�� ��ũ�� �ٸ� �̵���Ų��.
			pScrollBar->SetScrollPos(nPos);
			// 5.2 ���� ��ġ��ŭ ȭ���� �̵���Ų��.
			if (nPos < nCurPos) {
				this->scroll.Move(x, y - rect.bottom / 100 * (nCurPos - nPos));
			}
			else {
				this->scroll.Move(x, y + rect.bottom / 100 * (nPos - nCurPos));
			}
			break;

			//6. ��ũ�� �ٰ� �̵��� ��
		case SB_THUMBTRACK:
			// 6.1 �̵��� ��ġ�� ��ũ�� �ٸ� �̵���Ų��
			pScrollBar->SetScrollPos(nPos);
			// 6.2 �̵��� ��ġ��ŭ ȭ���� �̵���Ų��
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

// ������ ����� ��ȭ�Ǿ��� ��
void MindMappingForm::OnSize(UINT nType, int cx, int cy) {
	// ��ũ�� �� ũ�� ��ȭ �� ���� �ʿ�
}

//��ũ�� ���� ���� ��
BOOL MindMappingForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	int ret = 0;
	RECT rect;
	GetClientRect(&rect);
	int x = this->scroll.GetX();
	int y = this->scroll.GetY();

	// 1. ���� �Ʒ��� ���� ��
	if (zDelta < 0) {
		// 1.1 ctrl Ű�� �������� Ȯ���Ѵ�
		if (nFlags == MK_CONTROL) {
			// 1.1.1 ���̸�
			// 1.1.1.1 ��ũ�� �ٸ� �ø���
			// 1.1.1.2 ����� ������ŭ �׵θ�, ���ȹڽ�, �̹���, ���輱�� �׸���.
			this->r = this->r * 9 / 10;
			if (this->r <= 3 / 10) {
				this->r = 3 / 10;
			}
			ret = -1;
		}
		else {
			// 1.1.2 �����̸�
			int pos = this->vscrollBar.GetScrollPos();
			int curPos = pos + 3;
			// 1.1.2.1 ��ũ�� �ٸ� ������
			// 1.1.2.2 �۾������� �Ʒ��� ������
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
	// 2. ���� ���� �ø� ��
	else if (zDelta > 0) {
		// 2.1 ctrl Ű�� �������� Ȯ���Ѵ�
		if (nFlags == MK_CONTROL) {
			// 2.1.1 ���̸�
			// 2.1.1.1 ��ũ�� �ٸ� ���δ�
			// 2.1.1.2 Ȯ���� ������ŭ �׵θ�, ���ȹڽ�, �̹���, ���輱�� �׸���.
			this->r = this->r * 11 / 10;
			if (this->r >= 3) {
				this->r = 3;
			}
			ret = -1;
		}
		else {
			// 2.1.2 �����̸�
			// 2.1.2.1 ��ũ�� �ٸ� �ø���
			// 2.1.2.2 �۾������� ���� �ø���.
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
// ȭ�鿡 �Է��� ��ġ�� ������ ���� ���� �ش��ϴ��� �˷��ִ� �޼ҵ�
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

// Ȯ�� ��ư�� Ŭ������ ��
void MindMappingForm::OnZoomInButtonClicked() {
	// 1. �۾������� �߽���ġ�� �д´�
	this->r = this->r * 11 / 10;
	if (this->r >= 3) {
		this->r = 3;
	}
	// 2. Ȯ���� ������ŭ �׵θ�, ���ȹڽ�, �̹���, ���輱�� �׸���.
	RedrawWindow();
}

// ��� ��ư�� Ŭ������ ��
void MindMappingForm::OnZoomOutButtonClicked() {
	// 1. �۾������� �߽���ġ�� �д´�
	this->r = this->r * 9 / 10;
	if (this->r <= 3 / 10) {
		this->r = 3 / 10;
	}
	// 2. ����� ������ŭ �׵θ�, ���ȹڽ�, �̹���, ���輱�� �׸���.
	RedrawWindow();
}

// �߶󳻱� ��ư�� Ŭ������ ��
void MindMappingForm::OnCutButtonClicked() {
	// 1. �ڸ� ������ ���Ѵ�.
	if (this->current != 0 && this->current != this->mindMap->GetCenterFrame()) {
		// 2. ���� ���Ȱ� ���� ���ȵ��� �Ű� ���´�.
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
		// 3. ���� ���Ȱ� ���� ���ȵ��� �����.
		Frame *parentFrame = this->mindMap->HighRankFrame(this->current);
		parentFrame->Remove(this->current);
		RedrawWindow();
	}
}

// �����ϱ� ��ư�� Ŭ������ ��
void MindMappingForm::OnCopyButtonClicked() {
	// 1. ������ ������ ���Ѵ�.
	if (this->current != 0 && this->current != this->mindMap->GetCenterFrame()) {
		// 2. ���� ���Ȱ� ���� ���ȵ��� �Ű� ���´�.
		this->copyFrame = new Frame(*this->current);
	}
}

// �ٿ��ֱ� ��ư�� Ŭ������ ��
void MindMappingForm::OnPasteButtonClicked() {
	// 1. �ٿ����� ������ ���Ѵ�.
	if (this->current != 0 && this->copyFrame != 0) {
		// 2. �ű� ��ġ�� ���Ѵ� <����� ���·� �ʿ�>
		// 3. �Ű� ���� ���ȵ��� ���� ��ġ�� �Ű� ���´�.
		//this->copyFrame->LeftRadialSort();
		(this->current)->Add(this->copyFrame);
		// 4. �ű� ���� �׵θ��� ���� ���� �׵θ� ���̿� ���輱�� �׸���.
		RedrawWindow();
	}
}

// ������� ��ư�� Ŭ������ ��
void MindMappingForm::OnUnDoButtonClicked() {
	if (task.GetUnDoLength() > 0) {
		task.ReDoWrite(this->mindMap);
		MindMap *unDoMindMap = task.UnDoRead();
		*this->mindMap = *unDoMindMap;
		delete unDoMindMap;
	}
	RedrawWindow();
}

// �ٽý��� ��ư�� Ŭ������ ��
void MindMappingForm::OnReDoButtonClicked() {
	if (task.GetReDoLength() > 0) {
		task.UnDoWrite(this->mindMap);
		MindMap *reDoMindMap = task.ReDoRead();
		*this->mindMap = *reDoMindMap;
		delete reDoMindMap;
	}
	RedrawWindow();
}

// �ڵ����� �޺� �ڽ� �׸��� Ŭ������ ��
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

// ���� ��ư�� ������ ��
void MindMappingForm::OnSaveButtonClicked() {
	this->mindMap->Save();
	RedrawWindow();
}

// �ҷ����� ��ư�� ��������
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

// �ٸ��̸����� ���� ��ư�� ������ ��
void MindMappingForm::OnSaveAsButtonClicked() {
	CFileDialog saveAs(FALSE, _T("dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("DAT File(*.dat)"), NULL);
	if (saveAs.DoModal() == IDOK) {
		CString filePath = saveAs.GetPathName();
		CT2CA pszConvertedAnsiString(filePath);
		this->mindMap->SaveAs(std::string(pszConvertedAnsiString));
	}
	RedrawWindow();
}


// ���θ���� ��ư�� ������ ��
void MindMappingForm::OnNewButtonClicked() {
	// ������ �۾��� ���´�
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
// ������ư�� ������ ��
void MindMappingForm::OnDeleteButtonClicked() {
	//1. �������� ���õ� ���¿��� ������ư�� ������ ��� 
	if (this->current != 0) {
		// 1.1. ������ �׵θ��� �д´�
		if (this->current != this->mindMap->GetCenterFrame()) {
			Frame *highRankFrame = mindMap->HighRankFrame(current);
			// 1.2. ������ �۾��� ���´�.
			task.UnDoWrite(mindMap);
			// 1.3 ������ �׵θ��� �����.
			highRankFrame->Remove(current);
			this->current = highRankFrame;
		}
	}
	RedrawWindow();
}

// �����ϱ� ��ư�� ������ ��
void MindMappingForm::OnEmphasizeButtonClicked() {
	if (this->current != 0) {
		task.UnDoWrite(this->mindMap);
		this->current->Emphasize();
	}
	RedrawWindow();
}
// ���ʸ��콺�� ����Ŭ������ ��
void MindMappingForm::OnLButtonDblClk(UINT nFlags, CPoint point) {
	TopicBox *topicBox = 0;
	int x;
	int y;
	int j = 0;
	int k = 0;
	int l = 0;
	int rCount = 0;// \r�� ����
	TCHAR(*topic) = 0;
	//���� ��ġ�� �׵θ� ������ Ȯ���Ѵ�.
	Frame *frame = IsFrame(point);
	if (frame != 0) {
		int i = 0;
		while (i < frame->GetLength()) {
			Shape *shape = frame->GetChild(i);
			//�����ӿ��� ���ȹڽ��� ã�´�.
			if (dynamic_cast<TopicBox*>(shape)) {
				mouseDouble = true;
				topicBox = (TopicBox*)shape;
				x = topicBox->GetX();
				y = topicBox->GetY();
				topic = topicBox->GetTopic();
				//'\r'(�ٹٲ�) ������ ����.
				while (j < topicBox->GetLength()) {
					if (topic[j] == L'\r') {
						rCount++;
					}
					j++;
				}
				CClientDC dc(this);
				//�� �� ���Ȱ� ���� �� ������ ������ ĳ���� �̵���Ų��.
				if (rCount != 0) {
					//������ �� ���� 1�� �̻��ΰ�� (\r >1)
					j = 0;
					while (topic[j] != L'\0'&& k != rCount) {
						if (topic[j] == L'\r') {
							k++;
						}
						j++;
					}
					//���� ������ �� ���ڿ��� temp�� ��´�.
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
					//������ �� ���� ���(\r=0)
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
	CString path = "��Ʈ�� ����.chm";
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

// �ݱ� ��ư�� Ŭ������ ��
void MindMappingForm::OnClose() {
	// ���ε� ���� �����.
	delete this->mindMap;
	// �����츦 �ݴ´�.
	CFrameWnd::OnClose();
}
