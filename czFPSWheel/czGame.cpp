#include "czGame.h"
#include "czApi.h"
namespace czGame
{
	float normalize(float value)
	{
		return fmodf(value + 540, 360) - 180;
	}

	void callCmd(std::string name)
	{
		cmd_function_s* cmd = findCmd(name.c_str());
		if (cmd != nullptr)
			cmd->function();
		else
			czApi::writeConsole("callCmd: not found: " + name);
	}

	cmd_function_s* findCmd(const char* name)
	{
		DWORD rval = 0;
		DWORD addr = 0x4f9950;
		__asm
		{
			mov esi, name
			call addr
			mov rval, eax
		
		}
		if (rval != 0)
			return (cmd_function_s*)rval;
		else
			return nullptr;
	}

	float speed() {
		float x_vel = pow(fabsf(velocity->x), 2);
		float y_vel = pow(fabsf(velocity->y), 2);
		return sqrt(x_vel + y_vel);
	}
	weapon_t* getWeapon(int index) {
		if (index != -1)
			return ((weapon_t**)(*(DWORD*)0x402d8c))[index];
		else
			return ((weapon_t**)(*(DWORD*)0x402d8c))[entity[cg->clientNum].Weapon];
	}

	std::string getArgString(int arg_num)
	{
		if (arg_num == -1)
		{
			DWORD cArgsCount = *(DWORD*)0x1410b84;
			std::string cArgsString = getArgString(0);
			for (int i = 1; i < cArgsCount; i++)
			{
				cArgsString.append(" ");
				cArgsString.append(getArgString(i));
			}
			return cArgsString;
		}

		DWORD cArgsBase = 0x1410ba4;
		std::string cArgString;
		char cMsg[64];
		DWORD cBase = *(DWORD*)(cArgsBase);
		DWORD argBase = *(DWORD*)(cBase + (arg_num * 4));
		memcpy_s(&cMsg, 64, (void*)argBase, 64);
		cArgString = cMsg;
		std::transform(cArgString.begin(), cArgString.end(), cArgString.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return cArgString;
	}

	Vec3 GetViewPoint()
	{
		DWORD CG_GetPlayerViewPoint = 0x456580;

		Vec3* pReturnViewPoint;
		_asm
		{
			mov eax, 0x794474;
			mov edx, dword ptr ds : [pReturnViewPoint] ;
			push edx
				call dword ptr ds : [CG_GetPlayerViewPoint] ;
			add esp, 0x4
		}
		return *pReturnViewPoint;
	}

	bool WorldToScreen(Vec3 vWorldLocation, Vec2* Screen)
	{
		const auto iCenterX = refDef->Width / 2;
		const auto iCenterY = refDef->Height / 2;

		Vec3 vLocal;
		Vec3 vTransForm;

		const auto vright = refDef->Viewaxis[1];
		const auto vup = refDef->Viewaxis[2];
		const auto vfwd = refDef->Viewaxis[0];
		//Tools::VectorSubtract(vWorldLocation, czGame::refDef->Vieworg, &vLocal);
		 //vectorSubtract(vWorldLocation, czGame::refDef->Vieworg, &vLocal);

		 //Tools::VectorSubtract(vWorldLocation, czGame::refDef->Vieworg, vLocal);
		vLocal = vWorldLocation - GetViewPoint();// refDef->Vieworg;
		vTransForm[0] = czMath::DotProduct(vLocal, vright);
		vTransForm[1] = czMath::DotProduct(vLocal, vup);
		vTransForm[2] = czMath::DotProduct(vLocal, vfwd);
		if (vTransForm[2] < 0.01)
		{
			Screen->x = 0;
			Screen->y = 0;
			return false;
		}

		if (Screen)
		{
			Screen->x = iCenterX * (1 - (vTransForm[0] / refDef->Fov.x / vTransForm[2]));
			Screen->y = iCenterY * (1 - (vTransForm[1] / refDef->Fov.y / vTransForm[2]));
		}

		return  (vTransForm[2] > 0);
	}


	Vec2 WorldToScreen(Vec3 vWorldLocation)
	{
		Vec2 vScreen;
		WorldToScreen(vWorldLocation, &vScreen);
		return vScreen;
	}


	
}
Vec2 Vec3::ScreenLocation()
{
	return czGame::WorldToScreen(*this);
}