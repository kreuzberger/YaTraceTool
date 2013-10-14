#ifndef TCCHECKLISTITEM_H
#define TCCHECKLISTITEM_H

#include <Qt3Support/Q3ListView>
#include "lib/tracelib/NTrace.h"
#include <QtCore/QObject>

class TCCheckListItem: public Q3CheckListItem
{
public:
  TCCheckListItem(Q3ListView* parent, const QString& text);
  TCCheckListItem(TCCheckListItem* parent, const QString& text);
  virtual ~TCCheckListItem();
  virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
  void setScope(const QString& );
  void setPID( const QString& );
  bool isScope(void ) const;
  const QString& getScope( void ) const;
  const QString& getPID( void ) const;
  void setLogLevel( const Trace::TraceID& value);
  Trace::TraceID getLogLevel( void ) const;
  void setName( const QString& );
  const QString& getName( void ) const;

protected:

  virtual void activate();


  QString mScope;
  Trace::TraceID mLogLevel;
  QString mPID;
  QString mName;
};

inline void TCCheckListItem::setScope(const QString& scope)
{
  mScope = scope;
}

inline void TCCheckListItem::setPID(const QString& pid)
{
  mPID = pid;
}

inline bool TCCheckListItem::isScope(void ) const
{
  return (0 == childCount());
}

inline const QString& TCCheckListItem::getScope( void ) const
{
  return mScope;
}


inline Trace::TraceID TCCheckListItem::getLogLevel( void ) const
{
  return mLogLevel;
}

inline void TCCheckListItem::setName( const QString& name)
{
  mName = name;
}


inline const QString& TCCheckListItem::getName( void ) const
{
  return mName;
}

inline const QString& TCCheckListItem::getPID( void ) const
{
  return mPID;
}


#endif

