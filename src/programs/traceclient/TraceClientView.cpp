#include "TraceClientView.h"
#include "TraceClient.h"

#include <q3listview.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3valuelist.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <q3textedit.h>
#include <qpushbutton.h>
#include <q3stylesheet.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

TraceClientView::TraceClientView( QWidget *parent, const char *name )
    : QWidget( parent, name )
    , mpBase( 0 )
    , mpLay( 0 )
    , mpLayBtn( 0 )
    , mpVbox1( 0 )
    , mpVbox2( 0 )
    , mpListView(0)
    , mpLogView(0)
    , mpLabel(0)
    , mpBtnConnect(0)
    , mpBtnDisconnect(0)
    , mpBtnQuit(0)
    , mpBtnClear(0)
    , mpBtnCallback(0)
    , mCallbackView()
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

    // Second child: the ListView
    mpListView = new  Q3ListView( this ) ;
    mpVbox1->addWidget( mpListView );
    mpListView->addColumn( "Scopes" );
    mpListView->setRootIsDecorated( TRUE );
    mpListView->setSorting(-1);
    mpListView->setResizeMode(Q3ListView::LastColumn);

    // Second child: the LogView
    Q3HBoxLayout *mpVbox2 = new  Q3HBoxLayout( mpLay ) ;
    mpVbox2->setMargin( 5 );

    //First child: a Label


    mpLogView = new  Q3TextEdit(this);
    mpLogView->setTextFormat( Qt::LogText );
    mpLogView->setFamily("Courier New");
    mpVbox2->addWidget( mpLogView );

    mpListView->setSortColumn(0);
    mpListView->sort();

  	mpBtnConnect = new  QPushButton(tr("connect"),this) ;
  	mpBtnDisconnect = new  QPushButton(tr("disconnect"), this) ;
  	mpBtnQuit = new  QPushButton( tr("quit"), this) ;
  	mpBtnClear = new  QPushButton( tr("clear"), this) ;
    mpBtnCallback = new  QPushButton( tr("callbacks"), this) ;

    mpLayBtn->addWidget(mpBtnConnect);
    mpLayBtn->addWidget(mpBtnDisconnect);
    mpLayBtn->addWidget(mpBtnClear);
    mpLayBtn->addWidget(mpBtnCallback);
    mpLayBtn->addWidget(mpBtnQuit);

    connect( mpBtnConnect, SIGNAL(released()), SLOT(connectToHost()));
    connect( mpBtnDisconnect, SIGNAL(released()), SLOT(disconnect()));
    connect( mpBtnQuit, SIGNAL(released()), SLOT(close()));
    connect( mpBtnClear, SIGNAL(released()), mpLogView, SLOT(clear()));
    connect( mpBtnCallback, SIGNAL(released()), SLOT(showCallback()));
    connect( mpListView, SIGNAL(clicked(Q3ListViewItem*, const QPoint&, int)), SLOT(clicked(Q3ListViewItem*,const QPoint&, int)));
    connect( &mCallbackView, SIGNAL(setCallback(const Trace::TraceCallbackMap&)),SIGNAL(setCallback(const Trace::TraceCallbackMap&)));
}

TraceClientView::~TraceClientView()
{
  if( 0 !=  mpBase )
  {
    delete  mpBase ;
    mpBase = 0;
  }

  if( 0 !=  mpLay )
  {
    delete   mpLay ;
    mpLay = 0;
  }

  if( 0 != mpLayBtn  )
  {
    delete   mpLayBtn;
    mpLayBtn = 0;
  }

  if( 0 !=  mpVbox1 )
  {
    delete  mpVbox1;
    mpVbox1 = 0;
  }

  if( 0 != mpVbox2  )
  {
    delete  mpVbox2 ;
    mpVbox2 = 0;
  }
  if( 0 != mpListView )
  {
    delete  mpListView;
    mpListView = 0;
  }
  if( 0 != mpLogView )
  {
    delete  mpLogView;
    mpLogView = 0;
  }
  if( 0 != mpLabel )
  {
    delete  mpLabel;
    mpLabel = 0;
  }
  if( 0 != mpBtnConnect )
  {
    delete  mpBtnConnect;
    mpBtnConnect = 0;
  }
  if( 0 != mpBtnDisconnect )
  {
    delete  mpBtnDisconnect;
    mpBtnDisconnect = 0;
  }
  if( 0 != mpBtnQuit )
  {
    delete  mpBtnQuit;
    mpBtnQuit = 0;
  }
  if( 0 != mpBtnClear )
  {
    delete  mpBtnClear;
    mpBtnClear = 0;
  }
  if( 0 != mpBtnCallback )
  {
    delete  mpBtnCallback;
    mpBtnCallback = 0;
  }
}

void TraceClientView::setPara(const TraceClientPara& roPara)
{
  moPara = roPara;

  setCaption(QString("Traceclient (%1:%2)").arg(moPara.moHost).arg(moPara.muiPort));
  if( moPara.mbAutoConnect)
  {
    rcvText(QString("autoconnect to %1:%2").arg(moPara.moHost).arg(moPara.muiPort));
    emit connectToHost(moPara.moHost, moPara.muiPort);
  }
}


void TraceClientView::updateFilter(const Trace::TraceFilterMap& rFilter)
{
  // todo
  moList.clear();
  if( 0 != mpListView )
  {
    mpListView->clear();
  }

  if( 0 < rFilter.size())
  {
    for(Trace::TraceFilterMap::const_iterator itMap = rFilter.begin(); itMap != rFilter.end(); ++itMap )
    {

      TCCheckListItem* pParent = 0;
      QString pid;

      // first find the process assigned to this filter map;
      mpLogView->append(QString("New Filter for PID %1").arg(itMap.key()));
      pid = itMap.key();

      if(!moList.contains(itMap.key()))
      {
        TCCheckListItem* pNew = new  TCCheckListItem(mpListView,itMap.key());
        pNew->setPID(pid);
        moList[itMap.key()] = pNew;
      }
      pParent = moList[itMap.key()];

      for(Trace::TraceFilter::const_iterator itFilter = itMap.data().begin(); itFilter != itMap.data().end(); ++itFilter )
      {
        // first find the process assigned to this filter map;
        mpLogView->append(QString("New Filter %1").arg(itFilter.key()));
        QStringList list = QStringList::split(QChar('_'),itFilter.key());
        pParent = moList[itMap.key()];

        QString strName;

        for (QStringList::iterator it = list.begin(); it != list.end(); ++it)
        {
          if( 0 != pParent)
          {
            TCCheckListItem* ptr = static_cast<TCCheckListItem*>(static_cast<Q3CheckListItem*>((pParent->firstChild())));
            TCCheckListItem* pItem = 0;
            QString strIt = (*it);
            strName += strIt;
            while( (0 != ptr) && (0 == pItem) )
            {
                if(ptr->text() == strIt)
                {
                    pItem = static_cast<TCCheckListItem*>(ptr);
                }
                ptr = static_cast<TCCheckListItem*>(static_cast<Q3CheckListItem*>(ptr->nextSibling()));
            }
            if( 0 == pItem)
            {
              pItem = new  TCCheckListItem(pParent,strIt) ;
              pItem->setName(strName);
              pItem->setPID(pid);
              if( it == list.fromLast())
              {
                pItem->setScope(itFilter.key());
              }
              pItem->setLogLevel(itFilter.data());
            }
            pParent = pItem; // set parent to next level
            strName += "_";
          }
        }
      }
    }
  }
  else
  {
    moList.clear();
    mpListView->clear();
  }
}


void TraceClientView::updateCallback(const Trace::TraceCallbackMap& rCallback)
{
  mCallbackView.updateCallback(rCallback);
}



void TraceClientView::clicked(Q3ListViewItem* p, const QPoint& pnt, int )
{
  const int iTraceMenuID = 3000;
  const int iMenuID = 2000;
  TCCheckListItem* pCheckListItem = static_cast<TCCheckListItem*>(static_cast<Q3CheckListItem*>(p));

  if( 0 != pCheckListItem )
  {
    Q3PopupMenu* pContextMenu = new  Q3PopupMenu() ;
    if( pContextMenu->count() > 0 )
    {
      pContextMenu->insertSeparator();
    }

    Trace::TraceID logLevel = pCheckListItem->getLogLevel();

    pContextMenu->insertItem( "EnterExit", iTraceMenuID + Trace::TraceID_EnterExit);
    pContextMenu->setItemChecked(iTraceMenuID + Trace::TraceID_EnterExit,logLevel & Trace::TraceID_EnterExit);
    pContextMenu->insertItem( "Log", iTraceMenuID + Trace::TraceID_Log);
    pContextMenu->setItemChecked(iTraceMenuID + Trace::TraceID_Log,logLevel & Trace::TraceID_Log);
    pContextMenu->insertItem( "Warn", iTraceMenuID +Trace::TraceID_Warn);
    pContextMenu->setItemChecked(iTraceMenuID + Trace::TraceID_Warn,logLevel & Trace::TraceID_Warn);
    pContextMenu->insertItem( "Error", iTraceMenuID +Trace::TraceID_Error);
    pContextMenu->setItemChecked(iTraceMenuID + Trace::TraceID_Error,logLevel & Trace::TraceID_Error);
    pContextMenu->insertSeparator();
    pContextMenu->insertItem( "Clear All", iMenuID + 0);
    pContextMenu->insertItem( "Set All", iMenuID + 1);

    int iId = pContextMenu->exec(pnt);
    if( -1 != iId )
    {
      int idLevel = logLevel;

      if( iTraceMenuID <= iId)
      {
        int iTraceId = iId - iTraceMenuID;
        idLevel ^= iTraceId;
      }
      else if( iMenuID == iId)
      {
        idLevel = Trace::TraceID_Invalid;
      }
      else if(  (iMenuID +1 ) == iId)
      {
        idLevel = Trace::TraceID_EnterExit;
        idLevel += Trace::TraceID_Log;
        idLevel += Trace::TraceID_Warn;
        idLevel += Trace::TraceID_Error;
      }

      Trace::TraceFilterMap filterMap;
      QString oScope;
      QString pid;
      if(pCheckListItem->isScope()) // do not iterate through child nodes
      {
        pCheckListItem->setLogLevel(idLevel);
        filterMap[pCheckListItem->getPID()].insert(pCheckListItem->getScope(),pCheckListItem->getLogLevel());
      }
      else // iterate through child nodes
      {
        Q3ListViewItemIterator it( pCheckListItem);
        const QString pid = pCheckListItem->getPID();
        const QString name = pCheckListItem->getName();

        while(it.current())
        {
          TCCheckListItem* p = static_cast<TCCheckListItem*>(static_cast<Q3CheckListItem*>(*it));
          if( 0 != p)
          {
            if( !p->getName().startsWith(name) )
            {
              break;
            }

            p->setLogLevel(idLevel);
            if( p->isScope())
            {
              filterMap[pid].insert(p->getScope(),p->getLogLevel());
            }
          }
          ++it;
        }
      }
      emit setFilter(filterMap);
    }
    if( 0 != pContextMenu )
    {
      delete pContextMenu;
      pContextMenu = 0;
    }
    mpListView->update();
    update();
  }
}

void TraceClientView::showCallback()
{
  mCallbackView.show();
}



