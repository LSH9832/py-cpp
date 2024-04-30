# easycpp: make you coding c++ easier
all files are h files, just add to your project and include them and start your easier coding!
## modules
### pylike: code your c++ code like python
now supports:
- os::path::XXX, os::makedirs, os::getcwd
- str in python (class pystring) (startswith, endswith, split, replace, zfill, ljust, rjust, lower, upper)  convert to std::string -> pystring::str, convert to char* pystring::c_str
- logger (package loguru), usage:
```cpp
#include <pylike/logger.h>
#include <pylike/str.h>
int main()
{
    // stdout settings
    logsetMsgColored(true);
    logsetStdoutLevel(Debug);
    logsetLevelColor(Info, LOG_COLOR_PURPLE);
    logsetStdoutFormat("$TIME | $LEVEL | $LOCATION - $MSG");
    logsetStdoutTimeFormat("%Y%m%d %H:%M:%S");
    logsetLocationColor(LOG_COLOR_PURPLE);
    logsetTimeColor(LOG_COLOR_CYAN);

    logadd("log/log.txt",  // log file
           Debug,   // log level, default is Info
           "$TIME | $LEVEL | $LOCATION - $MSG",   // default format
           "%Y-%m-%d %H:%M:%S.%ms",   // default time format
           true);                   // makedirs while dirpath is not exist, default is true 
    
    logdebug("test0");
    loginfo("test define loginfo 123");
    logsuccess("test success");
    logwarning("test2");
    logerror(pystring("123\n456\n789\n") + 3.1415926535);
    
    INFO << "test good " << 123 << ENDL;
    WARN << "sounds not good" << 456 << ENDL;
    ERROR << "feels bad" << 555 << ENDL;
    DEBUG << "find something wrong" << ENDL;
    SUCCESS << "done!" << 5.678 << ENDL;
}
```

output in terminal and file is

```
2024-04-30 10:44:49.343 | DEBUG    | main.cpp:20:<main> - test0
2024-04-30 10:44:49.343 | INFO     | main.cpp:21:<main> - test define loginfo 123
2024-04-30 10:44:49.344 | SUCCESS  | main.cpp:22:<main> - test success
2024-04-30 10:44:49.344 | WARNING  | main.cpp:23:<main> - test2
2024-04-30 10:44:49.344 | ERROR    | main.cpp:24:<main> - 123
456
789
3.141593
2024-04-30 10:44:49.344 | INFO     | main.cpp:26:<main> - test good 123
2024-04-30 10:44:49.345 | WARNING  | main.cpp:27:<main> - sounds not good456
2024-04-30 10:44:49.345 | ERROR    | main.cpp:28:<main> - feels bad555
2024-04-30 10:44:49.345 | DEBUG    | main.cpp:29:<main> - find something wrong
2024-04-30 10:44:49.345 | SUCCESS  | main.cpp:30:<main> - done!5.678
```

