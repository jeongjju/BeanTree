//FindingForm.cpp
#include "FindingForm.h"
#include"MessageForm.h"
#include"MindMappingForm.h"
#include"MindMap.h"
#include"Frame.h"
#include"Image.h"
#include"TopicBox.h"
#include"MindMapIterator.h"
#include<iostream>
#include<afxwin.h>
#include<afxdlgs.h>
#include<atlimage.h>

BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_FIND_BUTTON, OnFindButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd *parent) : CDialog(FindingForm::IDD, parent)
{
	CDialog();
	findTopic = 0;
	indexes = 0;
	index = 0;
	count = 0;
	firstIndex = 0;
	lastIndex = 0;
}

BOOL FindingForm::OnInitDialog() {
	CDialog::OnInitDialog();
	return FALSE;
}

void FindingForm::OnFindButtonClicked() {
	CWinApp *cWinApp = AfxGetApp();
	CWnd *cWnd = cWinApp->GetMainWnd();
	MindMappingForm *mindMappingForm = (MindMappingForm *)cWnd;
	CString str;
	TCHAR(*temp);
	GetDlgItem(IDC_EDITBOX)->GetWindowText(str);
	TCHAR(*topic) = (TCHAR*)(LPCTSTR)str;
	if (topic != 0 && topic != findTopic) {
		indexes = 0;
		indexes = new TopicBox*[1000];
		index = 0;
		count = 0;
		TCHAR(*charTemp) = 0;
		int i = 0;
		int topicCount = 0;
		while (topic[i] != '\0') {
			i++;
			topicCount++;
		}
		Iterator<Array<Frame*>> *mindMapIterator = mindMappingForm->mindMap->CreateIterator();
		for (mindMapIterator->First(); !mindMapIterator->IsDone(); mindMapIterator->Next()) {
			Array<Frame*> arrayFrame = mindMapIterator->Current();
			int i = 0;
			while (i < arrayFrame.GetLength()) {
				Frame *frame = arrayFrame.GetAt(i);
				int j = 0;
				while (j < frame->GetLength()) {
					if (dynamic_cast<TopicBox*>(frame->GetChild(j))) {
						temp = ((TopicBox*)frame->GetChild(j))->GetTopic();
						int length = ((TopicBox*)frame->GetChild(j))->GetLength();
						if (length > topicCount) {
							int k = 0;
							int n = 0;
							while (k < length - topicCount) {
								int l = 0;
								charTemp = 0;
								charTemp = new TCHAR[length];
								while (topic[l] != '\0') {
									charTemp[l] = temp[n];
									n++;
									l++;
								}
								charTemp[l] = '\0';
								if (*charTemp == *topic) {
									indexes[count] = ((TopicBox*)frame->GetChild(j));
									if (count == 0) {
										firstIndex = n - topicCount;
										lastIndex = n - 1;
									}
									count++;
								}
								k++;
								n = k;
							}
						}
					}
					j++;
				}
				i++;
			}
		}
	}
	else if (topic != 0 && topic == findTopic) {
		count = 0;
		TCHAR(*charTemp) = 0;
		int i = 0;
		int topicCount = 0;
		while (topic[i] != '\0') {
			i++;
			topicCount++;
		}
		Iterator<Array<Frame*>> *mindMapIterator = mindMappingForm->mindMap->CreateIterator();
		for (mindMapIterator->First(); !mindMapIterator->IsDone(); mindMapIterator->Next()) {
			Array<Frame*> arrayFrame = mindMapIterator->Current();
			int i = 0;
			while (i < arrayFrame.GetLength()) {
				Frame *frame = arrayFrame.GetAt(i);
				int j = 0;
				while (j < frame->GetLength()) {
					if (dynamic_cast<TopicBox*>(frame->GetChild(j))) {
						temp = ((TopicBox*)frame->GetChild(j))->GetTopic();
						int length = ((TopicBox*)frame->GetChild(j))->GetLength();
						if (length > topicCount) {
							int k = 0;
							int n = 0;
							while (k < length - topicCount) {
								int l = 0;
								charTemp = 0;
								charTemp = new TCHAR[length];
								while (topic[l] != '\0') {
									charTemp[l] = temp[n];
									n++;
									l++;
								}
								charTemp[l] = '\0';
								if (*charTemp == *topic) {
									if (index + 1 == count) {
										firstIndex = n - topicCount;
										lastIndex = n - 1;
									}
									count++;
								}
								k++;
								n = k;
							}
						}
					}
					j++;
				}
				i++;
			}
		}
		index++;
	}
	findTopic = topic;
	if (topic != 0 && count != 0 && index < count) {
		//indexes[index]의 토픽을 firstIndex부터lastIndex까지 블럭표시
	}
	else {
		//없다는 메시지출력
		messageForm = new MessageForm;
		messageForm->Create(IDD_MESSAGEFORM, this);
		messageForm->ShowWindow(SW_SHOW);
	}
}

void FindingForm::PostNcDestroy() {
	((MindMappingForm*)GetActiveWindow())->findingForm = NULL;
	delete this;
}

void FindingForm::OnClose() {
	DestroyWindow();
}