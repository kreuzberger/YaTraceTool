#include "TCCheckListItem.h"
#include <q3popupmenu.h>
#include "TraceClientView.h"

//static const char * warn_xpm[] = {
//"16 16 3 1",
//" 	c None",
//".	c #0000F0",
//"+	c #000000",
//"...............+",
//"..............+ ",
//".............+  ",
//"............+   ",
//"...........+    ",
//"..........+     ",
//".........+      ",
//"........+       ",
//".......+        ",
//"......+         ",
//".....+          ",
//"....+           ",
//"...+            ",
//"..+             ",
//".+              ",
//"+               "};

///* XPM */
//static const char * err_xpm[] = {
//"16 16 3 1",
//" 	c None",
//".	c #000000",
//"+	c #FF1919",
//"               .",
//"              .+",
//"             .++",
//"            .+++",
//"           .++++",
//"          .+++++",
//"         .++++++",
//"        .+++++++",
//"       .++++++++",
//"      .+++++++++",
//"     .++++++++++",
//"    .+++++++++++",
//"   .++++++++++++",
//"  .+++++++++++++",
//" .++++++++++++++",
//".+++++++++++++++"};


///* XPM */
//static const char * warnerr_xpm[] = {
//"16 16 4 1",
//" 	c None",
//".	c #0000F0",
//"+	c #000000",
//"@	c #FF1919",
//"...............+",
//"..............+@",
//".............+@@",
//"............+@@@",
//"...........+@@@@",
//"..........+@@@@@",
//".........+@@@@@@",
//"........+@@@@@@@",
//".......+@@@@@@@@",
//"......+@@@@@@@@@",
//".....+@@@@@@@@@@",
//"....+@@@@@@@@@@@",
//"...+@@@@@@@@@@@@",
//"..+@@@@@@@@@@@@@",
//".+@@@@@@@@@@@@@@",
//"+@@@@@@@@@@@@@@@"};


TCCheckListItem::TCCheckListItem(Q3ListView* parent, const QString& text)
:Q3CheckListItem(parent, text, Q3CheckListItem::CheckBoxController)
, mScope()
, mLogLevel(Trace::TraceID_Invalid)
, mPID()
, mName()
{
}

TCCheckListItem::TCCheckListItem(TCCheckListItem* parent, const QString& text)
:Q3CheckListItem(parent, text, Q3CheckListItem::CheckBoxController)
, mScope()
, mLogLevel(Trace::TraceID_Invalid)
, mPID()
{
}

TCCheckListItem::~TCCheckListItem()
{
}

void
TCCheckListItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
  QColorGroup _cg( cg );
  // if error an warn are on both
  if ( (Trace::TraceID_Error & mLogLevel) && (Trace::TraceID_Warn & mLogLevel) && !(Trace::TraceID_Log & mLogLevel ))
  {
    _cg.setColor( QColorGroup::Text, Qt::darkRed );
  }
  else if ( (Trace::TraceID_Error & mLogLevel) && (Trace::TraceID_Warn & mLogLevel) && (Trace::TraceID_Log & mLogLevel ))
  {
    _cg.setColor( QColorGroup::Text, Qt::black );
  }
  // only error
  else if ( ( Trace::TraceID_Error & mLogLevel ) && !(Trace::TraceID_Log & mLogLevel ) )
  {
    _cg.setColor( QColorGroup::Text, Qt::red );
  }
  else if ( ( Trace::TraceID_Error & mLogLevel ) && (Trace::TraceID_Log & mLogLevel ) )
  {
    _cg.setColor( QColorGroup::Text, Qt::red );
  }
  // only warning
  else if ( ( Trace::TraceID_Warn & mLogLevel ) && !(Trace::TraceID_Log & mLogLevel ) )
  {
    _cg.setColor( QColorGroup::Text, Qt::blue );
  }
  else if ( ( Trace::TraceID_Warn & mLogLevel ) && (Trace::TraceID_Log & mLogLevel ) )
  {
    _cg.setColor( QColorGroup::Text, Qt::blue );
  }
  else if (!(Trace::TraceID_Invalid == mLogLevel) )
  {
    // if log / enterexit set  
    _cg.setColor( QColorGroup::Text, Qt::black);
  }
  else
  {
    _cg.setColor( QColorGroup::Text, Qt::black);
  }

  Q3CheckListItem::paintCell(p,_cg,column, width, align);
}


void TCCheckListItem::activate()
{
  // do nothing in here, the clicked and checkbox handling is handled in TraceClientView
}


void TCCheckListItem::setLogLevel(const Trace::TraceID& value )
{
  mLogLevel = value;
  if( Trace::TraceID_Invalid != mLogLevel)
  {
    setState(Q3CheckListItem::On);
  }
  else
  {
    setState(Q3CheckListItem::Off);
  }

}
