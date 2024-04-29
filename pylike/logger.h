#ifndef PYLIKE_LOGGER_H
#define PYLIKE_LOGGER_H

#include <cstdlib>
#include <thread>
#include "./str.h"
#include "./os.h"
#include "./datetime.h"

#define liststr std::vector<pystring>
#define osp os::path
#define LOG_BASEINFO (liststr{osp::basename(pystring(__FILE__)), std::to_string(__LINE__)})


enum LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};


enum LogSettings {
    LOG_COLOR_BLUE,
    LOG_COLOR_GREEN,
    LOG_COLOR_YELLOW,
    LOG_COLOR_RED,
    LOG_FONT_BOLD,
    LOG_NORMAL,
};


struct SingleLog
{
    enum Content {
        Time,
        Level,
        BaseInfo,
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
        if (!format.length() || format.empty()) format = "$TIME | $LEVEL | $BASEINFO - $MSG";
        infos.clear();
        int count = 0;
        PartInfo info_;
        info_.content = Other;
        info_.text = format;
        infos = {info_};
        for (pystring kw: {"$TIME", "$LEVEL", "$BASEINFO", "$MSG"}) {
            infos = _decodeFormat(infos, kw, (Content)count);
            count++;
        }
    }

};



class Logger {
public:

    Logger() {
        stdoutLog.level = Debug;
        stdoutLog.format = "$TIME | $LEVEL | $MSG";
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

    void log(LogLevel level, pystring msg, liststr baseinfo={}) {
        baseinfo_ = baseinfo;
        _log(level, msg);
    }

    void debug(pystring debug_, liststr baseinfo={}) {
        log(Debug, debug_, baseinfo);
    }

    void info(pystring info_, liststr baseinfo={}) {
        log(Info, info_, baseinfo);
    }

    void warning(pystring warning_, liststr baseinfo={}) {
        log(Warning, warning_, baseinfo);
    }

    void error(pystring error_, liststr baseinfo={}) {
        log(Error, error_, baseinfo);
    }

    std::ostringstream& log(LogLevel level, liststr baseinfo={}) {
        oss.str("");
        baseinfo_ = baseinfo;
        oss_level = level;
        return oss;
    }

    std::ostringstream& debug(liststr baseinfo={}) {
        return log(Debug, baseinfo);
    }

    std::ostringstream& info(liststr baseinfo={}) {
        return log(Info, baseinfo);
    }

    std::ostringstream& warning(liststr baseinfo={}) {
        return log(Warning, baseinfo);
    }

    std::ostringstream& error(liststr baseinfo={}) {
        return log(Error, baseinfo);
    }

    pystring end() {
        pystring oss_str = oss.str();
        _log(oss_level, oss_str);
        return "";
    }

private:
    std::vector<SingleLog> logs;
    SingleLog stdoutLog;
    bool msg_color = false;

    liststr baseinfo_;

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

        std::unordered_map<LogSettings, pystring> logset = {
            {LOG_COLOR_BLUE, "\033[34m"},
            {LOG_COLOR_GREEN, "\033[32m"},
            {LOG_COLOR_YELLOW, "\033[33m"},
            {LOG_COLOR_RED, "\033[31m"},
            {LOG_FONT_BOLD, "\033[1m"},
            {LOG_NORMAL, "\033[0m"}
        };

        bool enabled = true;

        pystring setText(pystring text, pystring logset_) {
            if (enabled) return logset_ + text + logset.at(LOG_NORMAL);
            else return text;
        }

        pystring setText(pystring text, LogSettings set_) {
            if (enabled) return setText(text, logset.at(set_));
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
            else if (c.content == SingleLog::Content::BaseInfo)
            {
                pystring info = "";
                for (int i=0;i<baseinfo_.size();i++) {
                    if (i) info += ":";
                    info += baseinfo_[i];
                }
                logstr += LogSet.setText(info, LOG_COLOR_BLUE);
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

    void _log(LogLevel level, pystring msg) {
        writeOneLog(stdoutLog, level, msg);
        for (SingleLog& log: logs) {
            writeOneLog(log, level, msg);
        }
    }

};

Logger logger;


#endif
