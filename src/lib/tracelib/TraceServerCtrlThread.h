#ifndef TRACETOOL_TRACELIB_TSCTRLTHREAD_H
#define TRACETOOL_TRACELIB_TSCTRLTHREAD_H

#include "NTrace.h"
#include <qstringlist.h>
#include <qdatetime.h>
class QTcpSocket;

#include <QThread>
#include <QMutex>
class TraceServerCtrlThread: public QThread
{
  Q_OBJECT
public:
  TraceServerCtrlThread();
  virtual ~TraceServerCtrlThread();
  void run();
  virtual void init();
  void appendMessage(const QString& name, const unsigned int&, const QString& str);
  void sendFilter();

public slots:
  void doProcess();
  void quit();
  void aboutToQuit();
  void readyRead();

protected slots:
  void connected();
  void disconnected();

protected:
  void timerEvent(QTimerEvent*);

protected:
  void deleteSocket(void);
  bool checkConnection( );
  void disconnect();

private:
  bool writeToSocket();
  bool pollSocket();
  void readFromSocket();

  bool mbTerminate;
  static bool mbConnected;
  static QTcpSocket* mpSocket;
  QMutex mCriticalSection;


  QStringList mMessages;
  bool mbSendFilter;
  qint32 miBlockSize;
  int miTimerID;

};

inline void
TraceServerCtrlThread::appendMessage(const QString& name, const unsigned int& pid,const QString& str)
{
  //mDataOutStream << Trace::TraceFmt_Message;
  //mDataOutStream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss,zzz ");
  //mDataOutStream << str << QString("\n");
  //printf("%s\n",qPrintable(str));
  mMessages.append(QString("%1(%2) %3: %4").arg(name).arg(pid).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss,zzz ")).arg(str));
//  messageReceived.wakeAll();
}

#endif
