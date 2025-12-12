
#include "cfig.h"

std::string strip(const std::string& str)
{
	if (str.empty())
		return str;
				
	if (str.at(0) == '"' && str.at(str.size() - 1) == '"')
		return str.substr(1, str.length() - 2);

	if (str.at(0) == '\'' && str.at(str.size() - 1) == '\'')
		return str.substr(1, str.length() - 2);
		
	return str;
}

const CfigValue& Cfig::get(const std::string& section, const std::string& key) const
{
	auto sec_it = data.find(section);

	if (sec_it == data.end())
		throw SectionError();

	auto key_it = sec_it->second.find(key);

	if (key_it == sec_it->second.end())
		throw KeyError();

	return key_it->second;
}

std::ostream& operator<<(std::ostream& out, const Type& type)
{
	switch(type)
	{
		case STRING:
			out << "string"; break;
		case INT:
			out << "int";break;
		case FLOAT:
			out << "float";break;
		case BOOL:
			out << "bool";break;
		case CHAR:
			out << "char"; break;
		default:
			out << "string";			
	}
	return out;
}

Type CfigValue::detectType(const std::string& val)
{
	if (val.empty())
		return STRING;

	if (val == "true" || val == "false")
		return BOOL;

	const char* start = val.c_str();
	char* endptr;

	std::strtod(start, &endptr);

	if (endptr == start + val.size())
	{
		const char* p = start;
		while (p < endptr)
		{
			if (*p == '.' || *p == 'e' || *p == 'E') return FLOAT;
			p++;
		}
		return INT;
	}	

	if (strip(val).size() == 1)
		return CHAR;
	
	return STRING;		
}

char CfigValue::toChar() const
{
	switch (type_)
	{
		case INT:
			try {
				int value = std::stoi(raw_);
				return static_cast<char>(value);
			} catch (const std::exception& e) {
				throw std::runtime_error("Cannot convert to char: " + raw_);
			}
		case FLOAT:
			try {
				double value = std::stod(raw_);
				return static_cast<char>(value);
			} catch (const std::exception& e) {
				throw std::runtime_error("Cannot convert to char: " + raw_);
			}
		case BOOL:
			return toBool() ? '1' : '0';
		case STRING:
		case CHAR:
			try {
				return raw_[0];
			} catch (const std::exception& e) {
				throw std::runtime_error("Cannot convert to char: " + raw_);
			}
		default:
			throw std::runtime_error("Cannot convert to char: " + raw_);			
	}
}

int CfigValue::toInt() const
{
	switch (type_)
	{
		case INT:
		case FLOAT:
			try { 
				return std::stoi(raw_); 
			} catch (const std::exception& e) {
				throw std::runtime_error("Cannot convert to int: " + raw_);
			}
			break;
		case BOOL:
			return toBool() ? 1 : 0;
		default:
			throw std::runtime_error("Cannot convert to int: " + raw_);
	}
}

double CfigValue::toDouble() const
{
	switch (type_)
	{
		case INT:
		case FLOAT:
			try { 
				return std::stod(raw_); 
			} catch (const std::exception& e) {
				throw std::runtime_error("Cannot convert to double: " + raw_);
			}
			break;
		case BOOL:
			return toBool() ? 1.0 : 0.0;
		default:
			throw std::runtime_error("Cannot convert to double: " + raw_);
	}
}

float CfigValue::toFloat() const
{
	return static_cast<float>(toDouble());
}

bool CfigValue::toBool() const
{

	switch (type_)
	{
		case BOOL:
			return raw_ == "true";
		case INT:
			return toInt() != 0;
		case FLOAT:
			return toDouble() != 0.0;
		default:
			throw std::runtime_error("Cannot convert to bool: " + raw_);
	}
}

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

void Cfig::load(const std::string& filename)
{

	std::ifstream in(filename);

	if (!in.is_open())
		throw FileNotExistence();

	std::string line;
	while (getline(in, line))
		parse(line);
	in.close();
		
}

void Cfig::setDelimiter(const Delimiter& del_)
{
	switch (del_)
	{
		case EQUAL:
			DELIMITER = '='; break;
		case COLON:
		default:
			DELIMITER = ':';
	}
}

void Cfig::setCommentPrefix(const CommentStyle& comment_style_)
{
	switch (comment_style_)
	{
		case HASH:
			COMMENT_PREFIX = '#'; break;
		case SEMICOLON:
		default:
			COMMENT_PREFIX = ';';
	}
}

void Cfig::parse(const std::string& line)
{

	std::string cline = trim(line.substr(0, line.find(COMMENT_PREFIX)));

	if (cline.empty())
		return;

	if (cline == MAIN_SECTION)
	{
		section = MAIN_SECTION_TITLE;
		return;
	}

	if (cline[0] == OPEN_SECTION && cline[cline.length() - 1] == CLOSE_SECTION)
	{
		section = trim(cline.substr(1, cline.length() - 2));
		return;
	}

	size_t del_pos = cline.find(DELIMITER);

	if (del_pos == std::string::npos)
		return;

	std::string	key = trim(cline.substr(0, del_pos));
	CfigValue val = CfigValue(
		strip(trim(cline.substr(del_pos + 1)))
	);
	
	if (!key.empty())
		data[section][key] = val;
}

bool Cfig::has(const std::string& target_section) const
{
	return data.find(target_section) != data.end();
}

bool Cfig::has(const std::string& target_section, const std::string& key) const
{
	auto it = data.find(target_section);
	return it != data.end() && it->second.find(key) != it->second.end();
}


template<typename T>
T Cfig::get(const std::string& section, const std::string& key, const T& defaultValue) const
{
    throw std::runtime_error("Unsupported type");
}

// template<typename T>
// T Cfig::get(const std::string& key, const T& defaultValue) const
// {
// 	throw std::runtime_error("Unsupported type");
// }

template<>
int Cfig::get<int>(const std::string& section, const std::string& key, const int& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.toInt();
	} catch (...) {
		return defaultValue;
	}
}

template<> double Cfig::get<double>(const std::string& section, const std::string& key, const double& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.toDouble();
	} catch (...) {
		return defaultValue;
	}
}
template<> bool Cfig::get<bool>(const std::string& section, const std::string& key, const bool& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.toBool();
	} catch (...) {
		return defaultValue;
	}
}
template<> float Cfig::get<float>(const std::string& section, const std::string& key, const float& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.toFloat();
	} catch (...) {
		return defaultValue;
	}
}
template<> char Cfig::get<char>(const std::string& section, const std::string& key, const char& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.toChar();
	} catch (...) {
		return defaultValue;
	}
}
template<> std::string Cfig::get<std::string>(const std::string& section, const std::string& key, const std::string& defaultValue) const
{
	try {
		const CfigValue& value = get(section, key);
		return value.raw();
	} catch (...) {
		return defaultValue;
	}
}

template<>
int Cfig::get<int>(const std::string& key, const int& defaultValue) const
{
	return get<int>(MAIN_SECTION_TITLE, key, defaultValue);
}


template<>
bool Cfig::get<bool>(const std::string& key, const bool& defaultValue) const
{
	return get<bool>(MAIN_SECTION_TITLE, key, defaultValue);
}

template<>
double Cfig::get<double>(const std::string& key, const double& defaultValue) const
{
	return get<double>(MAIN_SECTION_TITLE, key, defaultValue);
}

template<>
float Cfig::get<float>(const std::string& key, const float& defaultValue) const
{
	return get<float>(MAIN_SECTION_TITLE, key, defaultValue);
}

template<>
char Cfig::get<char>(const std::string& key, const char& defaultValue) const
{
	return get<char>(MAIN_SECTION_TITLE, key, defaultValue);
}

template<>
std::string Cfig::get<std::string>(const std::string& key, const std::string& defaultValue) const
{
	return get<std::string>(MAIN_SECTION_TITLE, key, defaultValue);
}
