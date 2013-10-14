#include "TCCBListItem.h"
#include <q3popupmenu.h>
#include "TraceClientView.h"


TCCBListItem::TCCBListItem(Q3ListView* parent, const QString& text)
:Q3ListViewItem(parent, text)
, mCallback()
, mCallbackArgs()
{
}

TCCBListItem::TCCBListItem(TCCBListItem* parent, const QString& text)
:Q3ListViewItem(parent, text)
, mCallback()
, mCallbackArgs()
{
}

TCCBListItem::~TCCBListItem()
{
}
