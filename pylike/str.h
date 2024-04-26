#include <iostream>
#include <string>
#include <vector>


class pystring {

    std::string str_;

public:

    pystring() {

    }

    pystring(std::string str) {
        str_ = str;
    }

    pystring(const char * str) {
        str_ = str;
    }

    bool operator==(pystring &str) {
        return str_ == str.str();
    }

    bool operator==(std::string &str) {
        return str_ == str;
    }

    bool operator==(const char str[]) {
        return str_ == str;
    }

    pystring operator+(pystring &str) {
        return pystring(str_ + str.str());
    }

    pystring operator+(std::string str) {
        return pystring(str_ + str);
    }

    pystring operator+(const char * str) {
        return pystring(str_ + str);
    }

    void operator+=(pystring &str) {
        str_ += str.str();
    }

    void operator+=(std::string str) {
        str_ += str;
    }

    void operator+=(const char * str) {
        str_ += str;
    }

    char operator[](int idx){
        return str_[idx<0?str_.length()+idx:idx];
    }

    bool startswith(const std::string& prefix) {
		size_t str_len = str_.length();
		size_t prefix_len = prefix.length();
		if (prefix_len > str_len) return false;
		return str_.find(prefix) == 0;
	}

	bool endswith(const std::string& suffix) {
		size_t str_len = str_.length();
		size_t suffix_len = suffix.length();
		if (suffix_len > str_len) return false;
		return (str_.find(suffix, str_len - suffix_len) == (str_len - suffix_len));
	}

    bool startswith(pystring &prefix) {
		size_t str_len = str_.length();
		size_t prefix_len = prefix.str().length();
		if (prefix_len > str_len) return false;
		return str_.find(prefix.str()) == 0;
	}

	bool endswith(pystring &suffix) {
		size_t str_len = str_.length();
		size_t suffix_len = suffix.str().length();
		if (suffix_len > str_len) return false;
		return (str_.find(suffix.str(), str_len - suffix_len) == (str_len - suffix_len));
	}

    inline char &at(size_t idx) {
        return str_.at(idx);
    }

    inline void append(std::initializer_list<char> __l) {
        str_.append(__l);
    }

    size_t length() {
        return str_.length();
    }

    inline bool empty(){
        return str_.empty();
    }

    inline char &back() {
        return str_.back();
    }

    inline void pop_back() {
        return str_.pop_back();
    }

    friend std::ostream& operator<<(std::ostream& _os, pystring obj) {
        _os << obj.str_;
        return _os;
    }

    std::vector<pystring> split(std::string delimiter) {
        std::vector<pystring> tokens;
	    size_t pos = 0;
	    std::string::size_type prev_pos = 0;
	    while ((pos = str_.find(delimiter, prev_pos)) != std::string::npos) {
	        tokens.push_back(pystring(str_.substr(prev_pos, pos - prev_pos)));
	        prev_pos = pos + delimiter.length();
	    }
	    if (prev_pos < str_.length()) tokens.push_back(pystring(str_.substr(prev_pos)));
	    return tokens;
    }

    std::vector<pystring> split(pystring delimiter) {
        return this->split(delimiter.str());
    }

    std::vector<pystring> split(const char* delimiter) {
        return this->split(pystring(delimiter).str());
    }

    pystring replace(std::string old_substr, std::string new_substr) {
        size_t pos = 0;
		while ((pos = str_.find(old_substr, pos)) != std::string::npos) {
			str_.replace(pos, old_substr.length(), new_substr);
			pos += new_substr.length(); // 更新位置以继续搜索
		}
		return str_;
    }

    pystring replace(pystring old_substr, pystring new_substr) {
        return this->replace(old_substr.str(), new_substr.str());
    }

    pystring replace(const char* old_substr, const char* new_substr) {
        return this->replace(pystring(old_substr), pystring(new_substr));
    }

    pystring zfill(int len) {
        std::string ret = str_;
        while (ret.length() < len) {
            ret = "0" + ret;
        }
        return pystring(ret);
    }

    const std::string str() {
        return str_;
    }

    inline const char* c_str() {
        return str_.c_str();
    }

};