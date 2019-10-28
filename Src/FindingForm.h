//FindingForm.h
#ifndef _FINDINGFORM_H
#define _FINDINGFORM_H
#include "resource.h"
#include "MessageForm.h"

class TopicBox;
class FindingForm :public CDialog {
public:
	enum { IDD = IDD_FINDINGFORM };
public:
	FindingForm(CWnd *parent = NULL);
	BOOL OnInitDialog();
	int GetFirstIndex()const;
	int GetLastIndex()const;
	TopicBox** GetIndexes()const;
	int GetIndex()const;
	void PostNcDestroy();
private:
	MessageForm *messageForm;
	TCHAR(*findTopic);
	TopicBox* (*indexes);
	int firstIndex;
	int lastIndex;
	int index;
	int count;
protected:
	afx_msg void OnFindButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()//¸ÅÅ©·Î
};

inline int FindingForm::GetFirstIndex()const {
	return this->firstIndex;
}
inline int FindingForm::GetLastIndex()const {
	return this->lastIndex;
}
inline TopicBox** FindingForm::GetIndexes()const {
	return this->indexes;
}
inline int FindingForm::GetIndex()const {
	return this->index;
}
#endif //_FINDINGFORM_H