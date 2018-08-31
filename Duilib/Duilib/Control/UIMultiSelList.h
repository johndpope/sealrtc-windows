#ifndef __UI_MULTISEL_LIST_H__
#define __UI_MULTISEL_LIST_H__

namespace DuiLib{
	

	class IMultiSelListUI
	{
	public:
		virtual bool SelectMultiItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual bool SelectShift(int iIndex) = 0;
		virtual void SetSingleSelect(bool bSingleSel) = 0;
		virtual bool GetSingleSelect() const = 0;
		virtual bool UnSelectItem(int iIndex) = 0;
		virtual void SelectAllItems() = 0;
		virtual void UnSelectAllItems() = 0;
		virtual int GetSelectItemCount() const = 0;
		virtual int GetNextSelItem(int nItem) const = 0;
	};


class UILIB_API CMultiSelListUI : public CListUI,public IMultiSelListUI
{
public:
	CMultiSelListUI();
	~CMultiSelListUI();

public:
	// CListUI
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	bool SetItemIndex(CControlUI* pControl, int iIndex);
	bool AddAt(CControlUI* pControl, int iIndex);
	bool Remove(CControlUI* pControl);
	bool RemoveAt(int iIndex);
	void DoEvent(TEventUI& event);
	int GetCurSel() const;
	bool SelectItem(int iIndex, bool bTakeFocus);
	void EnsureVisible(int iIndex);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

public:
	// IMultiSelListUI
	bool SelectMultiItem(int iIndex, bool bTakeFocus = false);
	bool SelectShift(int iIndex);
	void SetSingleSelect(bool bSingleSel);
	bool GetSingleSelect() const;	
	bool UnSelectItem(int iIndex);

	void SelectAllItems();
	void UnSelectAllItems();
	int GetSelectItemCount() const;
	int GetNextSelItem(int nItem) const;

public:
	void SetMultipleItem(bool bMultipleable);
	bool GetMultipleItem() const;

protected:
	int GetMinSelItemIndex();
	int GetMaxSelItemIndex();
protected:
	bool m_bSingleSel; // single or multiple 
	CStdPtrArray m_aSelItems; // Items in multiple mode
	int m_indexLastSel;
};

////////////////////////////////////////////////////////////////////////////
class UILIB_API CMultiListContainerElementUI : public CListContainerElementUI
{
public:
	CMultiListContainerElementUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetOwner(CControlUI* pOwner);
	void DoEvent(TEventUI& event);
public:
	IMultiSelListUI *m_pMultiOwner;
};

}// namespace DuiLib

#endif // __UI_MULTISEL_LIST_H__
