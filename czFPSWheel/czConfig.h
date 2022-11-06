#pragma once

//#pragma comment(lib,"libs\\libconfig++r.lib")
#include "libconfig.hh"

namespace czConfig
{
	inline std::string configName = "plugins\\cfg\\czExample.cfg";
	void loadSettings();
	void setEnabled(std::string name, bool isEnabled);
	bool checkEnabled(std::string name);
	template <typename n>
	void update_or_add(libconfig::Setting& val, std::string name, libconfig::Setting::Type type, n value);
	void update_or_add_array(libconfig::Setting& val, int elementCount, const libconfig::Setting::Type type, float value[]);
	void saveSettings();
};

