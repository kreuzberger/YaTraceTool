/****************************************************************************
** $Id: TraceClientCBView.h,v 1.1 2011-06-14 08:43:24 kreuzberger Exp $
**
** Copyright (C) 1992-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef TRACECLIENTCBVIEW_H
#define TRACECLIENTCBVIEW_H

#include <qwidget.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Qt3Support/Q3VBoxLayout>
#include <Qt3Support/Q3PtrList>
#include <Qt3Support/Q3HBoxLayout>
#include <QtGui/QLabel>

class Q3ListView;
class QLabel;
class QPushButton;
class TCCBListItem;

#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3ListView>
#include "lib/tracelib/NTrace.h"
#include <QtGui/QLineEdit>

class TraceClientCBView : public QWidget
{
    Q_OBJECT

public:
    TraceClientCBView( QWidget *parent = 0, const char *name = 0 );
    virtual ~TraceClientCBView();

 public slots:
    void updateCallback( const Trace::TraceCallbackMap&);
    void sendCallback( );
    void clicked(Q3ListViewItem*);

signals:
    void setCallback( const Trace::TraceCallbackMap&);

private:
    Q3VBoxLayout *mpBase;
    Q3HBoxLayout *mpLay;
    Q3HBoxLayout *mpLayBtn;
    Q3HBoxLayout *mpVbox1;
    Q3VBoxLayout *mpVbox2;
    Q3ListView *mpListView;
    QLabel *mpLabel;
    QMap<QString,TCCBListItem*> moList;
    QPushButton* mpBtnSend;
    Q3PtrList<QLineEdit> mArgsEdit;
    const unsigned int muiMaxParaCount;
    TCCBListItem* mpCurrent;
};

#endif
