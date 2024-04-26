#ifndef PYTHONLIKE_OS_H
#define PYTHONLIKE_OS_H

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "./str.h"

template <class T>
bool pyin(T element, std::vector<T> list) {
    for (T &e: list) {
        if (e == element) return true;
    }
    return false;
}


namespace os {

    namespace path {

        pystring join(std::vector<pystring> paths) {
            pystring joined_path = paths[0];
            if (!joined_path.empty() && joined_path.back() == '/') {
                joined_path.pop_back();
            }

            for (int i=1;i<paths.size();i++) {
                if (!(paths[1][0] == '/' || paths[1][0] == '\\')) {
                    joined_path += "/";
                }
                joined_path += paths[i];
                if (!joined_path.empty() && joined_path.back() == '/') {
                    joined_path.pop_back();
                }
            }
            return pystring(joined_path);
        }

        pystring basename(pystring path) {
            path = path.replace("\\", "/");
            std::vector<pystring> ps_ = path.split("/");
            return ps_[ps_.size()-1];
        }

        pystring abspath(pystring path) {
            path = path.replace("\\", "/");
            if (path.startswith("/")) {
                return path;
            }
            else if (path[1] == ':' && path[2] == '/') {
                return path;
            }

            char buff[FILENAME_MAX];
            getcwd(buff, FILENAME_MAX);
            std::string currPath_(buff);
            pystring currPath = currPath_;
            currPath = currPath.replace("\\", "/");

            if (!currPath.endswith("/")) {
                currPath += "/";
            }

            currPath += path;
            return currPath;
        }

        pystring dirname(pystring path) {
            path = path.replace("\\", "/");
            std::vector<pystring> ps_ = path.split("/");
            pystring ret = "";
            for (int i=0;i<ps_.size()-1;i++) {
                ret += ps_[i];
                if (i < ps_.size() - 2) ret += "/";
            }
            return ret;
        }

        bool isdir(pystring path) {
            struct stat info;
            if (stat(path.c_str(), &info) != 0) {
                return false; // 目录不存在
            } else if (info.st_mode & S_IFDIR) {
                return true; // 目录存在
            } else {
                return false; // 路径存在但不是目录
            }
        }

        bool isfile(pystring path) {
            struct stat info;
            if (stat(path.c_str(), &info) != 0) {
                return false; // 目录不存在
            } else if (info.st_mode & S_IFDIR) {
                return false; // 目录存在
            } else {
                return true; // 路径存在但不是目录
            }
        }

        bool exist(pystring path) {
            struct stat info;
            return (stat(path.c_str(), &info) == 0);
        }

        // pystring join(std::vector<std::string> paths) {
        //     std::string joined_path = paths[0];
        //     if (!joined_path.empty() && joined_path.back() == '/') {
        //         joined_path.pop_back();
        //     }
        //     for (int i=1;i<paths.size();i++) {
        //         if (!(paths[1][0] == '/' || paths[1][0] == '\\')) {
        //             joined_path += "/";
        //         }
        //         joined_path += paths[i];
        //         if (!joined_path.empty() && joined_path.back() == '/') {
        //             joined_path.pop_back();
        //         }
        //     }
        //     return pystring(joined_path);
        // }

        // pystring join(std::vector<const char*> paths_) {
        //     std::vector<std::string> paths;
        //     for (auto p_: paths_) {
        //         paths.push_back(p_);
        //     }
        //     return join(paths);
        // }

        // pystring basename(std::string path) {
        //     return basename(pystring(path));
        // }

        // pystring basename(const char* path) {
        //     return basename(pystring(path));
        // }

        // pystring abspath(std::string path) {
        //     return abspath(pystring(path));
        // }

        // pystring abspath(const char* path) {
        //     return abspath(pystring(path));
        // }

        // pystring dirname(std::string path) {
        //     return dirname(pystring(path));
        // }

        // pystring dirname(const char* path) {
        //     return dirname(pystring(path));
        // }

        // bool isdir(std::string path) {
        //     struct stat info;
        //     if (stat(path.c_str(), &info) != 0) {
        //         return false; // 目录不存在
        //     } else if (info.st_mode & S_IFDIR) {
        //         return true; // 目录存在
        //     } else {
        //         return false; // 路径存在但不是目录
        //     }
        // }

        // bool isdir(const char* path) {
        //     return isdir(pystring(path));
        // }

        // bool isfile(std::string path) {
        //     struct stat info;
        //     if (stat(path.c_str(), &info) != 0) {
        //         return false; // 目录不存在
        //     } else if (info.st_mode & S_IFDIR) {
        //         return false; // 目录存在
        //     } else {
        //         return true; // 路径存在但不是目录
        //     }
        // }

        // bool isfile(const char* path) {
        //     return isfile(pystring(path));
        // }

        // bool exist(std::string path) {
        //     struct stat info;
        //     return (stat(path.c_str(), &info) == 0);
        // }

        // bool exist(const char* path) {
        //     return exist(pystring(path));
        // }

    }

    void makedirs(pystring path_) {
        std::string path = path_.str();
        std::string _path = "";
        bool hasLetter=false;
        for (int i=0;i<path.length();i++) {
            if (path[i] != '/' && path[i] != '\\') {
                hasLetter = true;
            }

            if (path[i] == '/' ||  path[i] == '\\') {
                if (hasLetter) {
                    if (!path::exist(_path)) mkdir(_path.c_str(), 0755);
                }
            }
            _path += path[i];
        }
        if (hasLetter) {
            if (!path::exist(path)) mkdir(path.c_str(), 0755);
        }
    }
    
    // void makedirs(const char* path) {
    //     makedirs(pystring(path));
    // }


}

#endif