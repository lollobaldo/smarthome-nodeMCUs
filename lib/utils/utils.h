#if defined MYDEBUG && defined ARDUINO
    #define DebugPrint(a) (Serial.print(a))
    #define DebugPrintln(a) (Serial.println(a))
#else
    #define DebugPrint(a)
    #define DebugPrintln(a)
#endif

const char* concat(const char* s1, const char* s2);