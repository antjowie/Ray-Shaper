#pragma once
#include <map>

class Config
{
public:
	struct Data
	{
		enum class Type
		{
			Mouse,
			Keyboard,
			Integer
			//Boolean
		}type;
		int code;
		Data(const int code, const Type type);
	};
private:
	std::map<std::string, Data>m_data;

	bool load();
	void save();

	Config();
	~Config();

public:
	void pushData(const std::string &key, Data &data);
	Data getData(const std::string &key);


	static Config &getInstance(); 
};