#ifndef PYLIKE_LOGGER_H
#define PYLIKE_LOGGER_H

#include <cstdlib>
#include <thread>
#include "./str.h"
#include "./os.h"
#include "./datetime.h"

#define BLUE "\033[34m"   /* Blue   */
#define YELLOW "\033[33m" /* Yellow */
#define GREEN "\033[32m"  /* Green  */
#define RED "\033[31m"    /* Red    */


enum LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};


struct SingleLog
{
    enum Content {
        Time,
        Level,
        Msg,
        Other
    };

    struct PartInfo {
        Content content;
        pystring text;
    };
    bool isStdout = false;
    pystring filePath;
    LogLevel level;
    pystring format;
    pystring timeformat = "%Y-%m-%d %H:%M:%S.%ms";
    std::vector<PartInfo> infos;

    std::vector<PartInfo> _decodeFormat(std::vector<PartInfo> texts, pystring kw, Content ctype) {
        std::vector<PartInfo> ret;
        for (PartInfo info: texts) {
            if (info.content == Other) {
                
                std::vector<pystring> splits = info.text.split(kw);
                if (splits.size() < 2) {
                    ret.push_back(info);
                    continue;
                }

                for (int i=0;i<splits.size();i++) {
                    if (i) {
                        PartInfo kwinfo;
                        kwinfo.content = ctype;
                        ret.push_back(kwinfo);
                    }
                    PartInfo newinfo;
                    newinfo.content = Other;
                    newinfo.text = splits[i];
                    ret.push_back(newinfo);
                }
            }
            else {
                ret.push_back(info);
            }
                
        }
        return ret;
    }

    void decodeFormat() {
        if (!format.length() || format.empty()) format = "[$TIME] | $LEVEL | $MSG";
        infos.clear();
        int count = 0;
        PartInfo info_;
        info_.content = Other;
        info_.text = format;
        infos = {info_};
        for (pystring kw: {"$TIME", "$LEVEL", "$MSG"}) {
            infos = _decodeFormat(infos, kw, (Content)count);
            count++;
        }
    }

};



class Logger {
public:

    Logger() {
        stdoutLog.level = Info;
        stdoutLog.format = "[$TIME] | $LEVEL | $MSG";
        stdoutLog.isStdout = true;
        stdoutLog.decodeFormat();
    }

    void showStdout(bool enabled=true) {
        show_ = enabled;
    }

    void setStdoutTimeFormat(pystring format) {
        stdoutLog.timeformat = format;
    }

    void setStdoutLevel(LogLevel level) {
        stdoutLog.level = level;
    }

    void setStdoutFormat(pystring format) {
        stdoutLog.format = format;
        stdoutLog.decodeFormat();
    }

    void setMsgColored(bool flag) {
        msg_color = flag;
    }


    void add(pystring filepath, LogLevel level=Info, pystring format="", pystring timeformat="") {
        SingleLog log2add;
        log2add.filePath = filepath;
        log2add.level = level;
        log2add.format = format;
        log2add.timeformat = timeformat;
        log2add.decodeFormat();
        logs.push_back(log2add);
    }

    void remove(pystring filepath) {
        if (filepath.length()) {
            std::vector<SingleLog> logs_;
            for (int i=0;i<logs.size();i++) {
                if (!(logs[i].filePath == filepath)) {
                    logs_.push_back(logs[i]);
                }
            }
            logs = logs_;
        }
    }

    void debug(pystring debug_) {
        writeOneLog(stdoutLog, Debug, debug_);
        for (SingleLog& log: logs) {
            writeOneLog(log, Debug, debug_);
        }
    }

    void info(pystring info_) {
        writeOneLog(stdoutLog, Info, info_);
        for (SingleLog& log: logs) {
            writeOneLog(log, Info, info_);
        }
    }

    void warning(pystring warning_) {
        writeOneLog(stdoutLog, Warning, warning_);
        for (SingleLog& log: logs) {
            writeOneLog(log, Warning, warning_);
        }
    }

    void error(pystring error_) {
        writeOneLog(stdoutLog, Error, error_);
        for (SingleLog& log: logs) {
            writeOneLog(log, Error, error_);
        }
    }

    std::ostringstream& log(LogLevel level) {
        oss.str("");
        oss_level = level;
        return oss;
    }

    pystring end() {
        pystring oss_str = oss.str();
        // oss.str("");
        writeOneLog(stdoutLog, oss_level, oss_str);
        for (SingleLog& log: logs) {
            writeOneLog(log, oss_level, oss_str);
        }
        return "";
    }

private:
    std::vector<SingleLog> logs;
    SingleLog stdoutLog;
    bool msg_color = false;

    std::ostringstream oss;
    LogLevel oss_level = Info;

    bool show_ = true;

    struct _LogSet
    {
        pystring Blue = "\033[34m";
        pystring Yellow = "\033[33m";
        pystring Green = "\033[32m";
        pystring Red = "\033[31m";
        pystring End = "\033[0m";
        pystring Bold = "\033[1m";

        bool enabled = true;

        pystring setText(pystring text, pystring logset) {
            if (enabled) return logset + text + End;
            else return text;
        }

    };

    _LogSet LogSet;

    pystring timestr(pystring format_) {
        pystring ret = "";
        auto date = datetime::datetime::now().strftime(format_);
        ret = LogSet.setText(date, LogSet.Green);
        return ret;
    }

    pystring getColorByLevel(LogLevel level) {
        pystring ret = LogSet.Green;
        switch (level)
        {
        case Debug:
            ret = LogSet.Blue;
            break;
        case Info:
            ret = LogSet.Green;
            break;
        case Warning:
            ret = LogSet.Yellow;
            break;
        case Error:
            ret = LogSet.Red;
            break;
        }
        return ret;
    }

    pystring levelstr(LogLevel level) {
        pystring ret;
        switch (level)
        {
        case Debug:
            ret = LogSet.setText(LogSet.setText("DEBUG  ", LogSet.Blue), LogSet.Bold);
            break;
        case Info:
            ret = LogSet.setText(LogSet.setText("INFO   ", LogSet.Green), LogSet.Bold);
            break;
        case Warning:
            ret = LogSet.setText(LogSet.setText("WARNING", LogSet.Yellow), LogSet.Bold);
            break;
        case Error:
            ret = LogSet.setText(LogSet.setText("ERROR  ", LogSet.Red), LogSet.Bold);
            break;
        }
        return ret;
    }

    void writeOneLog(SingleLog& log, LogLevel level, pystring msg) {
        if (log.isStdout && !show_) return;

        LogSet.enabled = log.isStdout;

        if (level < log.level) return;

        pystring logstr = "";

        for (auto c: log.infos) {

            if (c.content == SingleLog::Content::Other) {
                logstr += c.text;
            }
            else if (c.content == SingleLog::Content::Time)
            {
                logstr += timestr(log.timeformat);
            }
            else if (c.content == SingleLog::Content::Level)
            {
                logstr += levelstr(level);
            }
            else if (c.content == SingleLog::Content::Msg)
            {
                auto text = LogSet.setText(msg, LogSet.Bold);
                if (msg_color) text = LogSet.setText(text, getColorByLevel(level));
                logstr += text;
            }
        }

        
        
        if (log.isStdout) {
            // printf(logstr.c_str());
            logstr += "\n";
            std::cout << logstr;
        } else {
            pystring command = pystring("echo '") + logstr + "' >> " + log.filePath;
            popen(command.c_str(), "r");
            // system(command.c_str());
        }
    }

};


#endif
