#include "czConfig.h"
#include "../../czApi/czApi.h"
#include "czFPSWheel.h"
#include <sstream>
namespace czConfig
{

	template <typename T>
	void lookupValue(libconfig::Setting& base, const char* name, T& value)
	{
		if (base.exists(name))
			base.lookupValue(name, value);
	}
	void lookupArray(libconfig::Setting& val, const int elementCount, float* arrVal)
	{
		for (int i = 0; i < val.getLength(); i++)
		{
			arrVal[i] = val[i];
		}
		return;
	}

	void loadSettings()
	{
		using namespace std;
		using namespace libconfig;
		Config cfg;
		try
		{
			cfg.readFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			czApi::writeConsole("[error] Configuration file error! Cannot read file " + configName);
			return;
		}
		catch (const ParseException& pex)
		{
			stringstream x;
			x << "[error] Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
			czApi::writeConsole(x.str());
			return;
		}
		//Setting& root = cfg.getRoot();
		//if (!root.exists("Hud_Time"))
		//	root.add("Hud_Time", Setting::Type::TypeGroup);
		//Setting& hud = root["Hud_Time"];
		//lookupValue(hud, "enabled", czFPSWheelHud);
		//lookupValue(hud, "opacity", hud::opacity);
		//lookupValue(hud, "fontSize", hud::fontSize);
		//if (hud.exists("color"))
		//	lookupArray(hud["color"], 4, (float*)&hud::tColor);
	}

	void saveSettings()
	{
		using namespace std;
		using namespace libconfig;
		Config cfg;
		bool isNew = false;
		//static const char* output_file = "cfg\\czMain.cfg";
		try
		{
			cfg.readFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			isNew = true;
		}
		//Setting& root = cfg.getRoot();
		//if (!root.exists("Hud_Time"))
		//	root.add("Hud_Time", Setting::TypeGroup);


		//Setting& e_Hud = root["Hud_Time"];
		///*if (!root.exists("Hud_Time"))
		//	root.add("Hud_Time", Setting::Type::TypeGroup);
		//Setting& hud = root["Hud_Time"];
		//lookupValue(root, "enabled", czFPSWheelHud);
		//lookupValue(root, "opacity", hud::opacity);
		//lookupValue(root, "fontSize", hud::fontSize);
		//lookupValue(root, "color", hud::tColor);*/
		//update_or_add(e_Hud, "enabled", Setting::TypeBoolean, czFPSWheelHud);
		//update_or_add(e_Hud, "opacity", Setting::TypeFloat, hud::opacity);
		//update_or_add(e_Hud, "fontSize", Setting::TypeFloat, hud::fontSize);
		//if (!e_Hud.exists("color"))
		//	e_Hud.add("color", Setting::TypeArray);
		//update_or_add_array(e_Hud["color"], 4, Setting::TypeFloat, (float*)&hud::tColor);
		try
		{
			cfg.writeFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			stringstream x;
			x << "[error] I/O error while writing file: " << configName;
			czApi::writeConsole(x.str());
		}
	}

	template <typename n>
	void update_or_add(libconfig::Setting& val, std::string name, libconfig::Setting::Type type, n value)
	{
		using namespace std;
		using namespace libconfig;

		if (!val.exists(name))
			val.add(name, type);

		val[name] = value;
	}

	void update_or_add_array(libconfig::Setting& val, int elementCount, const libconfig::Setting::Type type, float value[])
	{
		using namespace std;
		using namespace libconfig;
		for (int i = 0; i < elementCount; i++)
		{
			if (i > val.getLength() - 1)
				val.add(type);
			val[i] = value[i];
		}
	}

}
