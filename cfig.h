

#ifndef _CFIG_H_
#define _CFIG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

// // ERRORS
class FileNotExistence {};
class KeyError {};
class SectionError {};
class ValueError {};

// lang class
std::string trim(const std::string& str);

std::string strip(const std::string& str);

enum Type { STRING, INT, FLOAT, BOOL, CHAR };

std::ostream& operator<<(std::ostream& out, const Type& type);

class CfigValue
{
	std::string raw_;
	Type type_;
	Type detectType(const std::string& val);

public:
	CfigValue(const std::string& value) : raw_(trim(value)) {
		type_ = detectType(raw_);
	}
	CfigValue() : type_(STRING), raw_("") {}

	Type type() { return type_; }
	std::string raw() const { return raw_;  }

	std::string toString() const { return raw_; }

	int toInt() const;
	bool toBool() const;
	float toFloat() const;
	double toDouble() const;
	char toChar() const;
	operator std::string() const { return raw_; }
};

class Cfig
{
private:
	char DELIMITER = ':';
	char COMMENT_PREFIX = ';';
	const char OPEN_SECTION = '[';
	const char CLOSE_SECTION = ']';
	const std::string MAIN_SECTION = "[]";
	const std::string MAIN_SECTION_TITLE = "main";

	std::unordered_map<std::string, std::unordered_map<std::string, CfigValue>> data;

	std::string section = MAIN_SECTION_TITLE;
	void parse(const std::string& line);

public:
	enum CommentStyle {
		HASH, SEMICOLON, DOUBLE_SLASH
	};
	enum Delimiter {
		EQUAL, COLON
	};
	void setDelimiter(const Delimiter& del_);	
	void setCommentPrefix(const CommentStyle& comment_style_);
	Cfig() {};
	Cfig(const CommentStyle& style) { setCommentPrefix(style); }
	Cfig(const Delimiter& del) { setDelimiter(del); }
	Cfig(const std::string& filename) { load(filename); }
	bool has(const std::string& target_section) const;
	bool has(const std::string& target_section, const std::string& key) const;
	const CfigValue& get(const std::string& section, const std::string& key) const;
	const CfigValue& get(const std::string& key) const { return get(MAIN_SECTION_TITLE, key); }

	const CfigValue& operator()(const std::string& section, const std::string& key) const {
		return get(section, key);
	}

	template<typename T>
	T get(const std::string& section, const std::string& key, const T& defaultValue) const;
	template<typename T>
	T get(const std::string& key, const T& defaultValue) const;
		
	void load(const std::string& filename);	
};


#endif
