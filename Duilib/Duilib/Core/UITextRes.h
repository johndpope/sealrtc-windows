#ifndef __UITEXTRES_H__
#define __UITEXTRES_H__

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
class IFind
{
public:
	virtual CDuiString Find(CDuiString key) = 0;
};

class UILIB_API CTextRes
{
public:
	CTextRes();
	~CTextRes();

public:
	static void SetIFind(IFind* find);

	static void AddTextItem(CDuiString key, CDuiString value);
	static void Clear();

	static CDuiString FindTextValue(CDuiString key);

private:
	static IFind* s_find;

};
} // namespace DuiLib

#endif // __UITEXTRES_H__