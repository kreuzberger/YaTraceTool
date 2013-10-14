#ifndef TOOLS_TRACETOOL_TCCLIENTCOCKET_H
#define TOOLS_TRACETOOL_TCCLIENTCOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QtCore/QDataStream>

#include <stdlib.h>

class TCClientSocket : public QTcpSocket
{
  Q_OBJECT
public:
  TCClientSocket( int sock, QObject *parent=0 );
  virtual ~TCClientSocket();

  int getConnectPort( void );
  qint32 getBlockSize() const ;

signals:
  void readyRead(TCClientSocket* s);
  void sendText(const QString&);

private slots:
  void socketConnected();
  void socketConnectionClosed();
  void socketError(QAbstractSocket::SocketError);
  void socketClosed();
  void readyRead();

private:
    int line;
    int mConnectPort;
    qint32 miBlockSize;
};

inline int TCClientSocket::getConnectPort( void )
{
  return mConnectPort;
}

inline qint32 TCClientSocket::getBlockSize() const
{
  return miBlockSize;
}

#endif
