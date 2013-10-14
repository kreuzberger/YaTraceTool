/****************************************************************************
** $Id: TraceClientCBView.cpp,v 1.1 2011-06-14 08:43:24 kreuzberger Exp $
**
** Copyright (C) 1992-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "TraceClientCBView.h"
#include "TCCBListItem.h"

#include <q3listview.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3valuelist.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <q3textedit.h>
#include <qpushbutton.h>
#include <q3stylesheet.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>


/*
 * Constructor
 *
 * Create all child widgets of the CheckList Widget
 */

TraceClientCBView::TraceClientCBView( QWidget *parent, const char *name )
    : QWidget( parent, name )
    , mpBase( 0 )
    , mpLay( 0 )
    , mpLayBtn( 0 )
    , mpVbox1( 0 )
    , mpVbox2( 0 )
    , mpListView(0)
    , mpLabel(0)
    , mpBtnSend(0)
    , mArgsEdit()
    , muiMaxParaCount(Trace::TraceArgs)
    , mpCurrent(0)
{
    Q3VBoxLayout *mpBase = new Q3VBoxLayout(this);

    Q3HBoxLayout *mpLay = new  Q3HBoxLayout( mpBase ) ;
    mpLay->setMargin( 5 );

    Q3HBoxLayout *mpLayBtn = new  Q3HBoxLayout( mpBase ) ;
    mpLayBtn->setMargin( 5 );

    // create a widget which layouts its childs in a column
    Q3HBoxLayout *mpVbox1 = new  Q3HBoxLayout( mpLay ) ;
    mpVbox1->setMargin( 5 );

    //First child: a Label
   // vbox1->addWidget( new QLabel( "Scopes", this ) );

    // Second child: the ListView
    mpListView = new  Q3ListView( this ) ;
    mpVbox1->addWidget( mpListView );
    mpListView->addColumn( "Callbacks" );
    mpListView->setRootIsDecorated( TRUE );
    mpListView->setSorting(-1);
    mpListView->setResizeMode(Q3ListView::LastColumn);

    // Second child: the LogView
    Q3VBoxLayout *mpVbox2 = new  Q3VBoxLayout( mpLay ) ;
    mpVbox2->setMargin( 5 );

    //First child: a Label
    //vbox2->addWidget( new QLabel( "Messages", this ) );

    for( unsigned idx = 0; idx < muiMaxParaCount; idx++)
    {
      QLineEdit* p = new QLineEdit(this);
      mArgsEdit.append(p);
      mpVbox2->addWidget( p );
    }
    mpVbox2->addStretch();

    mpListView->setSortColumn(0);
    mpListView->sort();

    mpBtnSend = new  QPushButton(tr("Send"),this) ;

    mpLayBtn->addWidget(mpBtnSend);

    connect( mpBtnSend, SIGNAL(released()), SLOT(sendCallback()));
   connect( mpListView, SIGNAL(clicked(Q3ListViewItem*)), SLOT(clicked(Q3ListViewItem*)));

}

TraceClientCBView::~TraceClientCBView()
{
  if( 0 !=  mpBase )
  {
    delete  mpBase  ;
    mpBase = 0;
  }

  if( 0 !=  mpLay )
  {
    delete   mpLay ;
    mpLay = 0;
  }

  if( 0 != mpLayBtn  )
  {
    delete   mpLayBtn ;
    mpLayBtn = 0;
  }

  if( 0 !=  mpVbox1 )
  {
    delete  mpVbox1  ;
    mpVbox1 = 0;
  }

  if( 0 != mpVbox2  )
  {
    delete  mpVbox2  ;
    mpVbox2 = 0;
  }

  if( 0 != mpListView )
    {
      delete  mpListView ;
      mpListView = 0;
    }
    if( 0 != mpLabel )
    {
      delete  mpLabel ;
      mpLabel = 0;
    }
    if( 0 != mpBtnSend )
    {
      delete  mpBtnSend ;
      mpBtnSend = 0;
    }

    for( QLineEdit* p = mArgsEdit.first(); 0 != p; p=mArgsEdit.next())
    {
      delete p;
      p=0;
    }

    // remove all elements from the callback list
    // dont forget sub elements
   // moList[it.key()] = new  TCCBListItem(mpListView,it.key()) );

}

void TraceClientCBView::updateCallback(const Trace::TraceCallbackMap& rCallback)
{
  if( 0 < rCallback.size())
  {
    for(Trace::TraceCallbackMap::const_iterator it = rCallback.begin(); it != rCallback.end();++it)
    {
      TCCBListItem* pParent = 0;
      QString pid;

      if(!moList.contains(it.key()))
      {
        moList[it.key()] = new  TCCBListItem(mpListView,it.key()) ;
      }

      pParent = moList[it.key()];
      pid = it.key();

      for( Trace::TraceCallback::const_iterator itCallback = it.data().begin(); itCallback != it.data().end(); ++itCallback )
      {
        QStringList list = QStringList::split(QChar('_'),itCallback.key());
        pParent = moList[it.key()];

        for (QStringList::iterator it = list.begin(); it != list.end(); ++it)
        {
          if( 0 != pParent)
          {
            TCCBListItem* ptr = static_cast<TCCBListItem*>(pParent->firstChild());
            TCCBListItem* pItem = 0;
            QString strIt = (*it);
            while( (0 != ptr) && (0 == pItem) )
            {
                if(ptr->text(0) == strIt)
                {
                    pItem = static_cast<TCCBListItem*>(ptr);
                }
                ptr = static_cast<TCCBListItem*>(ptr->nextSibling());
            }
            if( 0 == pItem)
            {
              pItem = new  TCCBListItem(pParent,strIt) ;
              pItem->setCallback(pid,itCallback.key(),itCallback.data());
            }
            pParent = pItem; // set parent to next level
          }
        }
      }
    }
  }
  else
  {
    // empty filter;
    moList.clear();
    mpListView->clear();
  }
}



void TraceClientCBView::clicked(Q3ListViewItem* p )
{
  mpCurrent = 0;
  TCCBListItem* pCheckListItem = static_cast<TCCBListItem*>(p);
  if( 0 != pCheckListItem )
  {
    if( pCheckListItem->isCallback())
    {
      const QStringList& args = pCheckListItem->getCallbackArg();
      unsigned int cnt = args.count();
      cnt = (muiMaxParaCount < cnt) ? muiMaxParaCount : cnt;
      unsigned int idx = 0;
      for( idx = 0; idx < cnt; idx++ )
      {
        mArgsEdit.at(idx)->setText(args[idx]);
      }
      for( ; idx < muiMaxParaCount; idx++)
      {
        mArgsEdit.at(idx)->setText("");
      }
      mpCurrent = pCheckListItem;
    }
    else
    {
      for( unsigned int idx = 0; idx < muiMaxParaCount; idx++ )
      {
        mArgsEdit.at(idx)->setText("");
      }
    }
  }

}

void TraceClientCBView::sendCallback()
{
  if( 0 != mpCurrent )
  {
    Trace::TraceCallbackMap callback;
    QStringList args;
    unsigned int cnt = mpCurrent->getCallbackArg().count();
    cnt = (muiMaxParaCount < cnt) ? muiMaxParaCount : cnt;
    for( unsigned int idx = 0; idx < cnt; idx++ )
    {
      args.append(mArgsEdit.at(idx)->text());
    }
    callback[mpCurrent->getPID()].insert(mpCurrent->getCallback(),args);
    emit setCallback(callback);

  }

}


