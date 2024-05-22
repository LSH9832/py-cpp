#ifndef PYLIKE_LOGGER_H
#define PYLIKE_LOGGER_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "./str.h"
#include "./os.h"
#include "./datetime.h"
#include <functional>

#define liststr std::vector<pystring>
#define osp os::path


#define LOG_LOC (liststr{pystring(__FILE__), std::to_string(__LINE__), pystring("<") + __func__ + ">"})


#define LOG_DEFAULT_FORMAT "$TIME | $LEVEL | $LOCATION - $MSG"

#define logadd logger.add
#define logdebug(debug_) logger.debug((debug_), LOG_LOC)
#define loginfo(info_) logger.info((info_), LOG_LOC)
#define logsuccess(success_) logger.success((success_), LOG_LOC)
#define logwarning(warning_) logger.warning((warning_), LOG_LOC)
#define logerror(error_) logger.error((error_), LOG_LOC)

#define DEBUG logger.debug(LOG_LOC)
#define INFO logger.info(LOG_LOC)
#define SUCCESS logger.success(LOG_LOC)
#define WARN logger.warning(LOG_LOC)
#define ERROR logger.error(LOG_LOC)
#define ENDL logger.end()

#define logsetMsgColored logger.setMsgColored
#define logsetStdoutLevel logger.setStdoutLevel
#define logsetStdoutFormat logger.setStdoutFormat
#define logsetStdoutTimeFormat logger.setStdoutTimeFormat
#define logsetLevelColor logger.setLevelColor
#define logsetLocationColor logger.setLocationColor
#define logsetTimeColor logger.setTimeColor
#define logsetStdoutFuncNameShow logger.setStdoutFuncNameShow


enum LogLevel
{
    Debug,
    Info,
    Success,
    Warning,
    Error
};


enum LogSettings {
    LOG_COLOR_BLUE,
    LOG_COLOR_GREEN,
    LOG_COLOR_YELLOW,
    LOG_COLOR_RED,
    LOG_COLOR_PURPLE,
    LOG_COLOR_CYAN,
    LOG_COLOR_WHITE,
    LOG_FONT_BOLD,
    LOG_NORMAL,
};


enum PathType {
    LOG_PATH_ABS,
    LOG_PATH_REL,
    LOG_PATH_BASE
};


struct SingleLog
{
    enum Content {
        Time,
        Level,
        Location,
        Msg,
        Other
    };

    struct PartInfo {
        Content content;
        pystring text;
    };
    bool isStdout = false;
    bool showFuncName = true;
    pystring filePath;
    LogLevel level=Info;
    pystring format;
    pystring timeformat = "%Y-%m-%d %H:%M:%S.%ms";
    std::vector<PartInfo> infos;

    PathType pathtype = LOG_PATH_BASE;
    

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
        if (!format.length() || format.empty()) format = LOG_DEFAULT_FORMAT;
        infos.clear();
        int count = 0;
        PartInfo info_;
        info_.content = Other;
        info_.text = format;
        infos = {info_};
        for (pystring kw: {"$TIME", "$LEVEL", "$LOCATION", "$MSG"}) {
            infos = _decodeFormat(infos, kw, (Content)count);
            count++;
        }
    }

};



class Logger {
public:

    Logger() {
        stdoutLog.level = Debug;
        stdoutLog.format = LOG_DEFAULT_FORMAT;
        stdoutLog.isStdout = true;
        stdoutLog.decodeFormat();
    }

    void showStdout(bool enabled=true) {
        show_ = enabled;
    }

    void setStdoutFuncNameShow(bool flag=true) {
        stdoutLog.showFuncName = flag;
    }

    void setStdoutTimeFormat(pystring format) {
        stdoutLog.timeformat = format;
    }

    void setStdoutLevel(LogLevel level) {
        stdoutLog.level = level;
    }

    void setStdoutLevel(int level) {
        level = MIN((int)Error, level);
        stdoutLog.level = (LogLevel)level;
    }

    void setStdoutFormat(pystring format) {
        stdoutLog.format = format;
        stdoutLog.decodeFormat();
    }

    void setLevelColor(LogLevel level_, LogSettings color_) {
        levelColor_.at(level_) = color_;
    }

    void setTimeColor(LogSettings color_) {
        timeColor_ = color_;
    }

    void setLocationColor(LogSettings color_) {
        locationColor_ = color_;
    }

    void setMsgColored(bool flag) {
        msg_color = flag;
    }

    bool add(pystring filepath, LogLevel level=Info, pystring format="", pystring timeformat="", bool generate_path=true) {
        if (!filepath.length()) return false;
        pystring dirname = osp::dirname(osp::abspath(filepath));
        if (!osp::isdir(dirname)) {
            if (generate_path) os::makedirs(dirname);
            else {
                error(LOG_LOC) << "path '" << dirname 
                               << "' not exist! Skip adding log file '" 
                               << filepath << "'." << end();
                return false;
            }
        }
        
        SingleLog log2add;
        log2add.filePath = filepath;
        log2add.level = level;
        log2add.format = format;
        log2add.timeformat = timeformat;
        log2add.decodeFormat();
        logs.push_back(log2add);
        return true;
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

    void log(LogLevel level, pystring msg, liststr location={}) {
        location_ = location;
        _log(level, msg);
    }

    void debug(pystring debug_, liststr location={}) {
        log(Debug, debug_, location);
    }

    void info(pystring info_, liststr location={}) {
        log(Info, info_, location);
    }

    void success(pystring info_, liststr location={}) {
        log(Success, info_, location);
    }

    void warning(pystring warning_, liststr location={}) {
        log(Warning, warning_, location);
    }

    void error(pystring error_, liststr location={}) {
        log(Error, error_, location);
    }

    std::ostringstream& log(LogLevel level, liststr location={}) {
        oss.str("");
        location_ = location;
        oss_level = level;
        return oss;
    }

    std::ostringstream& debug(liststr location={}) {
        return log(Debug, location);
    }

    std::ostringstream& info(liststr location={}) {
        return log(Info, location);
    }

    std::ostringstream& success(liststr location={}) {
        return log(Success, location);
    }

    std::ostringstream& warning(liststr location={}) {
        return log(Warning, location);
    }

    std::ostringstream& error(liststr location={}) {
        return log(Error, location);
    }

    pystring end() {
        pystring oss_str = oss.str();
        _log(oss_level, oss_str);
        return "";
    }

// protected:
    SingleLog* getLogByName(pystring filepath) {
        if (filepath.length()) {
            std::vector<SingleLog> logs_;
            for (int i=0;i<logs.size();i++) {
                if (logs[i].filePath == filepath) {
                    return &logs[i];
                }
            }
            logs = logs_;
        }
        return nullptr;
    }

private:
    std::vector<SingleLog> logs;
    SingleLog stdoutLog;

    std::unordered_map<LogLevel, LogSettings> levelColor_ = {
        {Debug, LOG_COLOR_BLUE},
        {Info, LOG_FONT_BOLD},
        {Success, LOG_COLOR_GREEN},
        {Warning, LOG_COLOR_YELLOW},
        {Error, LOG_COLOR_RED}
    };

    std::unordered_map<LogLevel, pystring> levelFlag_ = {
        {Debug, "DEBUG   "},
        {Info, "INFO    "},
        {Success, "SUCCESS "},
        {Warning, "WARNING "},
        {Error, "ERROR   "}
    };

    LogSettings timeColor_ = LOG_COLOR_GREEN;
    LogSettings locationColor_ = LOG_COLOR_CYAN;

    bool msg_color = true;

    liststr location_;

    std::ostringstream oss;
    LogLevel oss_level = Info;

    bool show_ = true;

    struct _LogSet
    {
        std::unordered_map<LogSettings, pystring> logset = {
            {LOG_COLOR_BLUE, "\033[34m"},
            {LOG_COLOR_GREEN, "\033[32m"},
            {LOG_COLOR_YELLOW, "\033[33m"},
            {LOG_COLOR_RED, "\033[31m"},
            {LOG_COLOR_PURPLE, "\033[35m"},
            {LOG_COLOR_CYAN, "\033[36m"},
            {LOG_COLOR_WHITE, "\033[37m"},
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
        ret = LogSet.setText(date, timeColor_);
        return ret;
    }

    pystring timestr(pystring format_, datetime::Datetime date_) {
        pystring ret = "";
        ret = LogSet.setText(date_.strftime(format_), timeColor_);
        return ret;
    }

    pystring getColorByLevel(LogLevel level) {
        return LogSet.logset.at(levelColor_.at(level));
    }

    pystring levelstr(LogLevel level) {
        return LogSet.setText(LogSet.setText(levelFlag_.at(level), levelColor_.at(level)), LOG_FONT_BOLD);
    }

    void writeOneLog(SingleLog& log, LogLevel level, pystring msg, datetime::Datetime date, std::vector<pystring> location) {
        if (level < log.level) return;
        if (log.isStdout && !show_) return;
        
        LogSet.enabled = log.isStdout;

        pystring logstr = "";

        for (auto c: log.infos) {
            if (c.content == SingleLog::Content::Other) {
                logstr += c.text;
            }
            else if (c.content == SingleLog::Content::Time)
            {
                logstr += timestr(log.timeformat, date);
            }
            else if (c.content == SingleLog::Content::Location)
            {
                pystring info = "";
                for (int i=0;i<location.size();i++) {
                    if(!i) {
                        if (log.pathtype == LOG_PATH_BASE) location[i] = osp::basename(location[i]);
                        else if (log.pathtype == LOG_PATH_REL) location[i] = osp::relpath(location[i]);
                    }
                    else if(!log.showFuncName && i == location.size()-1) continue;
                    if (i) info += ":";
                    info += LogSet.setText(location[i], locationColor_);
                }
                logstr += info; //LogSet.setText(info, locationColor_);
            }
            else if (c.content == SingleLog::Content::Level)
            {
                logstr += levelstr(level);
            }
            else if (c.content == SingleLog::Content::Msg)
            {
                auto text = LogSet.setText(msg, LOG_FONT_BOLD);
                if (msg_color) text = LogSet.setText(text, levelColor_.at(level));
                logstr += text;
            }
        }

        if (log.isStdout) {
            // printf(logstr.c_str());
            logstr += "\n";
            std::cout << logstr;
        } else {
            writefile(logstr, log.filePath);
        }
    }

    void writefile(pystring msg, pystring filepath) {
        std::ofstream outfile(filepath.c_str(), std::ios::app);  // 创建ofstream对象，并打开文件
        if (!outfile.is_open()) { // 检查文件是否成功打开
            error(LOG_LOC) << "failed to open " << filepath << end();
            return;
        }
        outfile << msg << std::endl; // 写入文件
        outfile.close(); // 关闭文件

        // pystring command = pystring("echo '") + msg + "' >> " + filepath;
        // popen(command.c_str(), "r");
        // // system(command.c_str());
    }

    void __logfile(LogLevel level, pystring msg, datetime::Datetime d, std::vector<pystring> loc) {
        for (SingleLog& log: logs) {
            writeOneLog(log, level, msg, d, loc);
        }
    }

    void _log(LogLevel level, pystring msg) {
        datetime::Datetime d = datetime::datetime::now();
        std::vector<pystring> loc = location_;
        writeOneLog(stdoutLog, level, msg, d, loc);
        std::thread _t(std::mem_fn(&Logger::__logfile), this, level, msg, d, loc);
        _t.detach();
    }

};

Logger logger;


#endif
