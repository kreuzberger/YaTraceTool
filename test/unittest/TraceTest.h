#ifndef TRACETEST_H
#define TRACETEST_H

#include <QtCore/QObject>

class TraceTest: public QObject
{
 Q_OBJECT
  private slots:

  void initTestCase() {}
  void cleanupTestCase() {}
  void init() {}
  void cleanup() {}

  void callback_serialisation_single();
  void callback_serialisation_multi();
  void callback_serialisation_multidigits();
  void all_serialisation_single();
  void callback_serialisation_pid();
  void filtermap_serialisation_map();
};

#endif // TRACETEST_H
