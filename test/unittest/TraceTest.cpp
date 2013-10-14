#include <lib/tracelib/Trace.h>
//Added by qt3to4:
#include <QtCore/QDataStream>
#include "lib/tracelib/NTrace.h"

TEST( callback_serialisation_single )
{
  Trace::TraceCallback callbackEmpty;
  Trace::TraceCallback callbackSingle;
  callbackSingle["callbackSingle"]= QStringList("first line");
  CHECK("first line" == callbackSingle["callbackSingle"].first());
  CHECK(callbackSingle.contains("callbackSingle"));
  CHECK_EQUAL(1,callbackSingle["callbackSingle"].count());

  QByteArray bytearray;
  QDataStream ts( &bytearray,QIODevice::WriteOnly  );
  Trace::serialize(ts,callbackSingle);
  QDataStream ds( &bytearray, QIODevice::ReadOnly );
  Trace::deserialize(ds,callbackEmpty);

  CHECK(callbackEmpty.contains("callbackSingle"));
  CHECK_EQUAL(1,callbackEmpty["callbackSingle"].count());
  CHECK("first line" == callbackEmpty["callbackSingle"].first());
}

TEST( callback_serialisation_multi )
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

  CHECK(callbackEmpty.contains("callbackMulti"));
  CHECK_EQUAL(2,callbackEmpty["callbackMulti"].count());
  CHECK("first line" == callbackEmpty["callbackMulti"][0]);
  CHECK("second line" == callbackEmpty["callbackMulti"][1]);

}

TEST( callback_serialisation_multidigits )
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

  CHECK(callbackEmpty.contains("callbackMulti"));
  CHECK_EQUAL(2,callbackEmpty["callbackMulti"].count());
  CHECK("1" == callbackEmpty["callbackMulti"][0]);
  CHECK("2" == callbackEmpty["callbackMulti"][1]);

}

TEST( all_serialisation_single )
{
  Trace::TraceCallback callbackRcv;
  Trace::TraceCallback callbackSingle;
  callbackSingle["callbackSingle"]= QStringList("first line");
  CHECK("first line" == callbackSingle["callbackSingle"].first());
  CHECK(callbackSingle.contains("callbackSingle"));
  CHECK_EQUAL(1,callbackSingle["callbackSingle"].count());

  Trace::TraceFilter filterRcv;
  Trace::TraceFilter filterSingle;
  filterSingle["test_single_filter"]=Trace::TraceID_Warn;
  CHECK_EQUAL(Trace::TraceID_Warn, filterSingle["test_single_filter"]);

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

  CHECK(callbackRcv.contains("callbackSingle"));
  CHECK_EQUAL(1,callbackRcv["callbackSingle"].count());
  CHECK("first line" == callbackRcv["callbackSingle"].first());

  CHECK(filterRcv.contains("test_single_filter"));
  CHECK_EQUAL(Trace::TraceID_Warn, filterRcv["test_single_filter"]);

  CHECK_EQUAL(messageSingle.count(),messageRcv.count());
  CHECK_EQUAL(qPrintable(messageSingle.first()),qPrintable(messageRcv.first()));

}

TEST( callback_serialisation_pid )
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

  CHECK_EQUAL(qPrintable(nameExpected),qPrintable(name));
  CHECK_EQUAL(pidExpected, pid);

}


TEST( filtermap_serialisation_map )
{
  Trace::TraceCallbackMap callbackMapRcv;
  Trace::TraceFilter filterRcv;
  Trace::TraceFilter filterSingle;
  Trace::TraceFilterMap filterMap;
  Trace::TraceFilterMap filterMapRcv;
  filterSingle["test_single_filter"]=Trace::TraceID_Warn;
  CHECK_EQUAL(Trace::TraceID_Warn, filterSingle["test_single_filter"]);

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

  CHECK_EQUAL(0,callbackMapRcv.count());
  CHECK_EQUAL(1,filterMap.count());

  CHECK(filterMapRcv["testme(0)"].contains("test_single_filter"));
  //CHECK_EQUAL(Trace::TraceID_Warn, filterMapRcv["testme(0)"] ["test_single_filter"]);

  CHECK_EQUAL(messageSingle.count(),messageRcv.count());
  CHECK_EQUAL(qPrintable(messageSingle.first()),qPrintable(messageRcv.first()));

}
