#pragma once

#include <vector>
#include <string>

template< typename T >
class ConfigItem
{
	std::string category, name;
	T* value;
public:
	ConfigItem(std::string category, std::string name, T* value)
	{
		this->category = category;
		this->name = name;
		this->value = value;
	}
};

template< typename T >
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_, const T& def)
	{
		category = category_;
		name = name_;
		value = value_;
		defaultValue = def;
	}

	std::string category, name;
	T* value;
	T defaultValue;
};

class CConfig
{
protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
	std::vector<ConfigValue<char>*> chars;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);
	void SetupValue(char*, char*, std::string, std::string);

	void SetupSkins();
	void SetupMisc();
public:
	CConfig()
	{
	}

	void Setup();

	void Load(const std::string& name);
	void Save(const std::string& name);
};

extern CConfig* Config;
