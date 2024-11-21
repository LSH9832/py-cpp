# py-cpp: make you coding c++ easier just like python
all files are h files, just add to your project and include them and start your easier coding!
## modules
### pylike
now supports:
- os::path::XXX, os::makedirs, os::getcwd
- str in python (class pystring) (startswith, endswith, split, replace, zfill, ljust, rjust, lower, upper)  convert to std::string -> pystring::str, convert to char* pystring::c_str
- logger (package loguru)
- tabulate (pcakage tabulate)
#### usage:
```cpp
#include <pylike/logger.h>
#include <pylike/str.h>
#include <pylike/tabulate.h>


int main()
{
    // 1.logger
    // stdout settings
    logsetMsgColored(true);   // default is true
    logsetStdoutLevel(LOG_LEVEL_DEBUG); // default is LOG_LEVEL_DEBUG
    logsetLevelColor(LOG_LEVEL_INFO, LOG_COLOR_PURPLE);  // default is no color (gray)
    logsetStdoutFormat("$TIME | $LEVEL | $LOCATION - $MSG");  // default is what this line shows
    logsetStdoutTimeFormat("%Y%m%d %H:%M:%S"); // default is %Y%m%d %H:%M:%S.%ms
    logsetLocationColor(LOG_COLOR_PURPLE);  // default is LOG_COLOR_CYAN
    logsetTimeColor(LOG_COLOR_CYAN);  // default is LOG_COLOR_GREEN

    logadd("log/log.txt",  // log file
           LOG_LEVEL_DEBUG,   // log level, default is LOG_LEVEL_INFO
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

    // 2.tabulate
    TCLEAR;
    TH;
    TABLE << "Key"   << "Value" << TENDL;
    TL << "count" << 1       << TENDL;
    TL;
    TABLE << "path" << "/path/to/your/config/file";
    TABLE << 1234567;
    TABLE << TENDL;

    TSetAlign({TA_LEFT, TA_CENTER});
    TSetStyle(TS_FANCY_GRID);
    INFO << "table:\n" << TSHOW() << ENDL;

    return 0;
}
```

output in terminal and file is

```
2024-04-30 10:44:49.343 | DEBUG    | main.cpp:24:<main> - test0
2024-04-30 10:44:49.343 | INFO     | main.cpp:25:<main> - test define loginfo 123
2024-04-30 10:44:49.344 | SUCCESS  | main.cpp:26:<main> - test success
2024-04-30 10:44:49.344 | WARNING  | main.cpp:27:<main> - test2
2024-04-30 10:44:49.344 | ERROR    | main.cpp:28:<main> - 123
456
789
3.141593
2024-04-30 10:44:49.344 | INFO     | main.cpp:30:<main> - test good 123
2024-04-30 10:44:49.345 | WARNING  | main.cpp:31:<main> - sounds not good456
2024-04-30 10:44:49.345 | ERROR    | main.cpp:32:<main> - feels bad555
2024-04-30 10:44:49.345 | DEBUG    | main.cpp:33:<main> - find something wrong
2024-04-30 10:44:49.345 | SUCCESS  | main.cpp:34:<main> - done!5.678
2024-04-30 10:44:49.346 | SUCCESS  | main.cpp:34:<main> - table:
╒═══════╤═══════════════════════════╤═════════╕
│ Key   │           Value           │         │
╞═══════╪═══════════════════════════╪═════════╡
│ count │             1             │         │
├───────┼───────────────────────────┼─────────┤
│ path  │ /path/to/your/config/file │ 1234567 │
╘═══════╧═══════════════════════════╧═════════╛
```

