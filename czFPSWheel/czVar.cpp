#include "czVar.h"
#include "czApi.h"
#include <algorithm>
#include <cctype>
#include <string>
czVar::czVar()//constructor
{
	value.Integer = 0;
}
czVar::~czVar()//desconstructor
{

}

std::string toLower(std::string data)
{
	auto x = std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return data;
}

czVar* czVar::find(const std::string& name, const std::string& owner)
{
	if (czApi::czInfo == nullptr)
		return nullptr;
	std::string lname = toLower(name);
	std::string lowner = toLower(owner);
	for (auto n : czApi::czInfo->czVars)
	{
		if (toLower(n->name) == lname && (lowner == "" || toLower(n->owner) == lowner))
			return n;
	}
	return nullptr;
}

czVar* czVar::add(const std::string& name, const czVarType& type, const czVarValue& value, bool autosave, const std::string& desc)
{
	czVar* alreadyExists = find(name, plugin_name);
	if (alreadyExists != nullptr)
	{
		alreadyExists->desc = desc;
		return alreadyExists;
	}
	czVar* nVar = new czVar();
	nVar->name = name;
	nVar->desc = desc;
	nVar->owner = plugin_name;
	nVar->type = type;
	nVar->value = value;
	nVar->autoSave = autosave;
	czApi::czInfo->czVars.push_back(nVar);
	nVar->index = czApi::czInfo->czVars.size();
	return nVar;
}
bool czVar::remove(const std::string& name, const std::string& owner)
{
	return remove(find(name, owner));
}
bool czVar::remove(czVar* var)
{
	if (var == nullptr)
		return false;

	czApi::czInfo->czVars.erase(czApi::czInfo->czVars.begin() + var->index);
}
