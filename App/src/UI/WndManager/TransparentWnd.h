#pragma once
#include "StdAfx.h"

namespace DuiLib 
{
class CTransparentWnd: public CWindowWnd
{
public:
	CTransparentWnd();
	virtual ~CTransparentWnd(void);

	void SetBKImage(LPCTSTR fileName);
	void Show();
	void Hide();
	void MoveRect();
	void DrawRect();
	void SetMargin(RECT rc) { m_rcMargin = rc; }
	void Set9GridMargin(RECT rc){ m_rc9GridMargin = rc;}
protected:
	virtual LPCTSTR GetWindowClassName() const;

	virtual UINT GetClassStyle() const;

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	CUString m_BKImage;
	RECT     m_rcMargin;
	RECT		m_rc9GridMargin;
	RECT     m_rect;
	bool       m_isUpdate;
};

}

