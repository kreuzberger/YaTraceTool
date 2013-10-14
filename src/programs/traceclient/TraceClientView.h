/****************************************************************************
** $Id: TraceClientView.h,v 1.1 2011-06-14 08:43:24 kreuzberger Exp $
**
** Copyright (C) 1992-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef TRACECLIENTVIEW_H
#define TRACECLIENTVIEW_H

#include <qwidget.h>
//Added by qt3to4:
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>

class Q3ListView;
class QLabel;
class QPushButton;
class TraceClient;

#include "TraceClientPara.h"
#include "TraceClientCBView.h"
#include "TCCheckListItem.h"
#include "lib/tracelib/NTrace.h"
#include <Qt3Support/Q3ListView>
#include <Qt3Support/Q3TextEdit>


class Q3ListViewItem;
class Q3StyleSheetItem;

class TraceClientView : public QWidget
{
    Q_OBJECT

public:
    TraceClientView( QWidget *parent = 0, const char *name = 0 );
    virtual ~TraceClientView();

 public slots:
    void connectToHost()
    {
      rcvText(QString("connect to %1:%2").arg(moPara.moHost).arg(moPara.muiPort));
      emit connectToHost(moPara.moHost, moPara.muiPort);
      moList.clear();
      mpListView->clear();
    }

    void disconnect()
    {
      emit closeConnection();
      moList.clear();
      mpListView->clear();
    }

    void setPara(const TraceClientPara& );

    void rcvText(const QString& text)
    {
      mpLogView->append( text );

    }

    void rcvText(const Trace::TraceMessage& text)
    {
      for(Trace::TraceMessage::const_iterator it=text.begin();it != text.end();++it)
      {
        QString str = (*it);
        str.replace("&lt;br&gt;","\n");
        str.replace("<br>","\n");
        str.replace("<error>","<font color=red>");
        str.replace("</error>","</font>");
        str.replace("<warn>","<font color=blue>");
        str.replace("</warn>","</font>");
        str.replace("<scope>","<font color=black>");
        str.replace("</scope>","</font>");
        str.replace("<log>","<font color=black>");
        str.replace("</log>","</font>");
        mpLogView->append( str );
      }
    }

    void updateFilter(const Trace::TraceFilterMap&);
    void updateCallback( const Trace::TraceCallbackMap&);
    void clicked(Q3ListViewItem*, const QPoint&, int);
    void showCallback();

signals:
    void connectToHost(const QString&, Trace::TracePort);
    void closeConnection();
    void setFilter( const Trace::TraceFilterMap&);
    void setCallback( const Trace::TraceCallbackMap&);

private:
    Q3VBoxLayout *mpBase;
    Q3HBoxLayout *mpLay;
    Q3HBoxLayout *mpLayBtn;
    Q3HBoxLayout *mpVbox1;
    Q3VBoxLayout *mpVbox2;
    Q3ListView *mpListView;
    Q3TextEdit* mpLogView;
    QLabel *mpLabel;
    QPushButton* mpBtnConnect;
    QPushButton* mpBtnDisconnect;
    QPushButton* mpBtnQuit;
    QPushButton* mpBtnClear;
    QPushButton* mpBtnCallback;
    TraceClientPara moPara;
    QMap<QString,TCCheckListItem*> moList;
    TraceClientCBView mCallbackView;
};

#endif
