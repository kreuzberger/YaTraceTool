#ifndef TRACEDEMO_H
#define TRACEDEMO_H

#include <QtGui/QDialog>
//Added by qt3to4:
#include <QTimerEvent>
#include <lib/tracelib/Trace.h>


class MyDlg:public QDialog
{
Q_OBJECT

TRACECALLBACK_CLASS(MyDlg)

public:
  MyDlg();
  virtual ~MyDlg();
  void dump(); // a callback
protected:
  void timerEvent(QTimerEvent*);
private:
  void timerfunc( void );
  int miCntId0;
  int miCntId1;
  int miTimerID1;
  int miTimerID2;

  };


#endif

