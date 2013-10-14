#ifndef TCCBLISTITEM_H
#define TCCBLISTITEM_H

#include <q3listview.h>
#include "lib/tracelib/NTrace.h"
#include <qobject.h>

class TCCBListItem: public Q3ListViewItem
{
public:
  TCCBListItem(Q3ListView* parent, const QString& text);
  TCCBListItem(TCCBListItem* parent, const QString& text);
  virtual ~TCCBListItem();
 // virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
  void setCallback(const QString&, const QString&, const QStringList& );
  bool isCallback(void ) const;
  const QStringList& getCallbackArg( void ) const;
  const QString& getCallback( void ) const;
  const QString& getPID( void ) const;

protected:
  QString mCallback;
  QStringList mCallbackArgs;
  QString mPID;
};

inline void TCCBListItem::setCallback(const QString& pid, const QString& name, const QStringList& arg)
{
  mCallback = name;
  mCallbackArgs = arg;
  mPID = pid;
}

inline bool TCCBListItem::isCallback(void ) const
{
  return (0 == childCount());
}

inline const QStringList& TCCBListItem::getCallbackArg(  ) const
{
  return mCallbackArgs;
}

inline const QString& TCCBListItem::getCallback(  ) const
{
  return mCallback;
}

inline const QString& TCCBListItem::getPID(  ) const
{
  return mPID;
}

#endif

