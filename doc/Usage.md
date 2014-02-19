Usage
-----

To add scopes to your application, the class or code must be first filled with scope definitions.
For usage of callbacks, the class must also be prepared for handlinge the callbacks.

This is all done by adding macros to your class and source files.

Example 1: Prepare a class for callbacks

    class MyDlg
    {
    
    TRACECALLBACK_CLASS(MyDlg)
    
    public:
      MyDlg();
      virtual ~MyDlg();
      void dump(); // a callback
    };
