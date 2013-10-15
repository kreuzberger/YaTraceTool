#include "TraceTest.h"
#include <QtTest>


QTEST_APPLESS_MAIN(TraceTest);

#include <lib/tracelib/Trace.h>
//Added by qt3to4:
#include <QtCore/QDataStream>
#include "lib/tracelib/NTrace.h"

void TraceTest::callback_serialisation_single()
{
  Trace::TraceCallback callbackEmpty;
  Trace::TraceCallback callbackSingle;
  callbackSingle["callbackSingle"]= QStringList("first line");
  QVERIFY("first line" == callbackSingle["callbackSingle"].first());
  QVERIFY(callbackSingle.contains("callbackSingle"));
  QCOMPARE(1,callbackSingle["callbackSingle"].count());

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,callbackSingle);
  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,callbackEmpty);

  QVERIFY(callbackEmpty.contains("callbackSingle"));
  QCOMPARE(1,callbackEmpty["callbackSingle"].count());
  QVERIFY("first line" == callbackEmpty["callbackSingle"].first());
}

void TraceTest::callback_serialisation_multi()
{
  Trace::TraceCallback callbackEmpty;
  Trace::TraceCallback callbackMulti;
  callbackMulti["callbackMulti"]= QStringList("first line");
  callbackMulti["callbackMulti"].append("second line");

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,callbackMulti);
  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,callbackEmpty);

  QVERIFY(callbackEmpty.contains("callbackMulti"));
  QCOMPARE(2,callbackEmpty["callbackMulti"].count());
  QVERIFY("first line" == callbackEmpty["callbackMulti"][0]);
  QVERIFY("second line" == callbackEmpty["callbackMulti"][1]);

}

void TraceTest::callback_serialisation_multidigits()
{
  Trace::TraceCallback callbackEmpty;
  Trace::TraceCallback callbackMulti;
  callbackMulti["callbackMulti"]= QStringList("1");
  callbackMulti["callbackMulti"].append("2");

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,callbackMulti);
  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,callbackEmpty);

  QVERIFY(callbackEmpty.contains("callbackMulti"));
  QCOMPARE(2,callbackEmpty["callbackMulti"].count());
  QVERIFY("1" == callbackEmpty["callbackMulti"][0]);
  QVERIFY("2" == callbackEmpty["callbackMulti"][1]);

}

void TraceTest::all_serialisation_single()
{
  Trace::TraceCallback callbackRcv;
  Trace::TraceCallback callbackSingle;
  callbackSingle["callbackSingle"]= QStringList("first line");
  QVERIFY("first line" == callbackSingle["callbackSingle"].first());
  QVERIFY(callbackSingle.contains("callbackSingle"));
  QCOMPARE(1,callbackSingle["callbackSingle"].count());

  Trace::TraceFilter filterRcv;
  Trace::TraceFilter filterSingle;
  filterSingle["test_single_filter"]=Trace::TraceID_Warn;
  QCOMPARE(Trace::TraceID_Warn, filterSingle["test_single_filter"]);

  Trace::TraceMessage messageRcv;
  Trace::TraceMessage messageSingle;
  messageSingle = QStringList() << "a single message";

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,filterSingle);
  Trace::serialize(ts,messageSingle);
  Trace::serialize(ts,callbackSingle);

  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,filterRcv,messageRcv,callbackRcv);

  QVERIFY(callbackRcv.contains("callbackSingle"));
  QCOMPARE(1,callbackRcv["callbackSingle"].count());
  QVERIFY("first line" == callbackRcv["callbackSingle"].first());

  QVERIFY(filterRcv.contains("test_single_filter"));
  QCOMPARE(Trace::TraceID_Warn, filterRcv["test_single_filter"]);

  QCOMPARE(messageSingle.count(),messageRcv.count());
  QCOMPARE(qPrintable(messageSingle.first()),qPrintable(messageRcv.first()));

}

void TraceTest::callback_serialisation_pid()
{
  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  QString name="myname";
  unsigned int pid = 4711;
  Trace::serialize(ts,name,pid);
  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  QString nameExpected;
  unsigned int pidExpected;
  Trace::deserialize(ds,nameExpected, pidExpected);

  QCOMPARE(qPrintable(nameExpected),qPrintable(name));
  QCOMPARE(pidExpected, pid);

}


void TraceTest::filtermap_serialisation_map()
{
  Trace::TraceCallbackMap callbackMapRcv;
  Trace::TraceFilter filterRcv;
  Trace::TraceFilter filterSingle;
  Trace::TraceFilterMap filterMap;
  Trace::TraceFilterMap filterMapRcv;
  filterSingle["test_single_filter"]=Trace::TraceID_Warn;
  QCOMPARE(Trace::TraceID_Warn, filterSingle["test_single_filter"]);

  filterMap["testme(0)"]=filterSingle;

  Trace::TraceMessage messageRcv;
  Trace::TraceMessage messageSingle;
  messageSingle = QStringList() << "a single message";

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,filterMap);
  Trace::serialize(ts,messageSingle);
 // Trace::serialize(ts,callbackMapRcv);

  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,filterMapRcv,messageRcv,callbackMapRcv);

  QCOMPARE(0,callbackMapRcv.count());
  QCOMPARE(1,filterMap.count());

  QVERIFY(filterMapRcv["testme(0)"].contains("test_single_filter"));
  //QCOMPARE(Trace::TraceID_Warn, filterMapRcv["testme(0)"] ["test_single_filter"]);

  QCOMPARE(messageSingle.count(),messageRcv.count());
  QCOMPARE(qPrintable(messageSingle.first()),qPrintable(messageRcv.first()));

}
