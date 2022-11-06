#include "dVar.h"
char* dVar::getEmptyArray(const int length)
{
	char* emptyarray = new char[length];
	for (int i = 0; i < length; i++)
		emptyarray[i] = 0x0;
	return emptyarray;
}


//void dVar::removeCmd(const char* name)
//{
//	reinterpret_cast<void(__cdecl*)(const char* name)>(0x4f99e0)(name);
//}
//
//void dVar::removeFromMainList(dVar* var)
//{
//	DWORD baseList = 0xCBA7404;
//	DWORD count = *(DWORD*)0xCBA73F8;
//	bool isFound = false;
//	int i = 0;
//	while (((DWORD**)baseList)[i] != 0)
//	{
//		if (((DWORD**)baseList)[i] == (DWORD*)var)
//			isFound = true;
//		if (isFound)
//		{
//			((DWORD**)baseList)[i] = ((DWORD**)baseList)[i + 1]; //shift the pointers up in memory by 4 bytes
//		}
//		i += 1;
//	}
//	((DWORD**)baseList)[i - 1] = 0; //last pointer null since we are removing 1
//}
//
//dVar* dVar::getFromHashTable(int index)
//{
//	return ((dVar**)0xCBAB408)[index];
//}
//
//void dVar::removeFromHashTable(dVar* var)
//{
//	DWORD baseHashTable = 0xCBAB408;
//	DWORD count = 255;
//	for (int i = 0; i < count; i++)
//	{
//		dVar* cVar = getFromHashTable(i);
//		if (cVar == var)
//			((DWORD**)baseHashTable)[i] = (DWORD*)cVar->HashNext; //was base of hash table element just replace with next hash element
//		else
//		{
//			dVar* pVar = cVar;
//			while (cVar != nullptr)
//			{
//				cVar = cVar->HashNext;
//				if (cVar == var)
//				{
//					if (cVar->HashNext == nullptr && i < 254)
//						cVar = getFromHashTable(i + 1);
//
//					if (cVar != nullptr)
//						pVar->HashNext = cVar->HashNext; //set the previous variables hash pointer to the current hash pointer to skip over this element
//				}
//				pVar = cVar;
//			}
//		}
//	}
//}
//
//dVar* dVar::byName(const std::string& name)
//{
//	DWORD baseVar = 0xCBAB808;
//	DWORD count = 0xCBA73F8;
//	for (int i = 0; i < *(int*)count; i++)
//	{
//		dVar* cVar = &reinterpret_cast<dVar*>(baseVar)[i];//(baseVar + (i * sizeof(dVar)));
//		if (cVar->Name != nullptr)
//		{
//			if (cVar->Name == name)
//				return cVar;
//		}
//	}
//	return nullptr;
//}

dVar* dVar::byName(const char* name)
{
	DWORD addr = 0x56b5d0;
	__asm
	{
		mov edi, name
		call [addr]
	}
}
//
//void dVar::fixHashTable(DWORD* oldPTR, DWORD* newPTR)
//{
//	DWORD baseHashTable = 0xCBAB408;
//	DWORD count = 255;
//
//	bool isFound = false;
//	for (int i = 0; i < count; i++)
//	{
//		dVar* cVar = dVar::getFromHashTable(i);
//		if (((DWORD**)baseHashTable)[i] == oldPTR)
//			((DWORD**)baseHashTable)[i] = newPTR; //was base of hash table element just replace with next hash element
//		else
//		{
//			while (cVar != nullptr)
//			{
//				if ((DWORD*)cVar->HashNext == oldPTR)
//					cVar->HashNext = (dVar*)newPTR;
//				cVar = cVar->HashNext;
//			}
//		}
//	}
//}
//void dVar::remove(const std::string& name)
//{
//	dVar* findVar = dVar::byName(name.c_str());
//	if (findVar == nullptr)
//	{
//		czApi::writeConsole("Couldn't find variable " + name);
//		return;
//	}
//	findVar->remove();
//}
//
//void dVar::remove()
//{
//	dVar::removeCmd(this->Name);
//	dVar::removeFromHashTable(this);
//	dVar::removeFromMainList(this);
//
//
//	DWORD baseVar = 0xCBAB808;
//	DWORD count = 0xCBA73F8;
//
//	bool isFound = false;
//	dVar* toRemove = nullptr;
//	for (int i = 0; i < *(int*)count; i++)
//	{
//		dVar* cVar = &reinterpret_cast<dVar*>(baseVar)[i];//(baseVar + (i * sizeof(dVar)));
//		if (cVar->Name != nullptr)
//		{
//			if (std::string(cVar->Name) == std::string(this->Name))
//			{
//				isFound = true;
//				toRemove = cVar;
//
//			}
//		}
//		if (isFound)
//		{
//			dVar* next_var = &reinterpret_cast<dVar*>(baseVar)[i + 1];
//			if (i == (*(int*)count) - 1)
//			{
//				memcpy(cVar, getEmptyArray(sizeof(dVar)), sizeof(dVar));
//				fixHashTable((DWORD*)cVar, 0);
//			}
//			else
//			{
//				memcpy(cVar, next_var, sizeof(dVar));
//				fixHashTable((DWORD*)next_var, (DWORD*)cVar);
//			}
//		}
//	}
//	*(int*)count -= 1;
//}


dVar* dVar::registerString(const char* name, std::uint16_t type, std::uint16_t flags, const char* desc, const char* value, std::int32_t iVal1, std::int32_t iVal2, std::int32_t iVal3, std::int32_t iVal4, std::int32_t iVal5)
{
	return reinterpret_cast<dVar * (__cdecl*)(const char*, std::uint16_t, std::uint16_t, const char*, const char*, std::int32_t, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>(0x56C130)(name, (std::uint16_t)type, flags, desc, value, iVal1, iVal2, iVal3, iVal4, iVal5);
}
dVar* dVar::registerFloat(const char* name, std::uint16_t type, std::uint16_t flags, const char* desc, float value, float iVal1, float iVal2, float iVal3, float iVal4, float iVal5)
{
	return reinterpret_cast<dVar * (__cdecl*)(const char*, std::uint16_t, std::uint16_t, const char*, float, float, float, float, float, float)>(0x56C130)(name, (std::uint16_t)type, flags, desc, value, iVal1, iVal2, iVal3, iVal4, iVal5);
}
dVar* dVar::registerInt(const char* name, std::uint16_t type, std::uint16_t flags, const char* desc, std::int32_t value, std::int32_t iVal1, std::int32_t iVal2, std::int32_t iVal3, std::int32_t iVal4, std::int32_t iVal5)
{
	return reinterpret_cast<dVar* (__cdecl*)(const char*, std::uint16_t, std::uint16_t, const char*, std::int32_t, std::int32_t, std::int32_t, std::int32_t, std::int32_t, std::int32_t)>(0x56C130)(name, type, flags, desc, value, iVal1, iVal2, iVal3, iVal4, iVal5);
}

dVar* dVar::add(const char* name, const char* desc, CvarValue_t value, DvarLimits domain, DvarType vtype, std::uint16_t flags)
{
	dVar* isValid = byName(name);
	if (isValid != nullptr)
		return isValid;
	
	char* nName = new char[32];
	char* nDesc = new char[256];

	memcpy(nName, name, 32);
	memcpy(nDesc, desc, 256);

	dVar* rVal = nullptr;
	switch (vtype)
	{
		case DvarType::DVAR_TYPE_INT:
		{
			rVal = registerInt(nName, (std::uint16_t)vtype, flags, nDesc, value.integer, 0, 0, 0, domain.Integer.Min, domain.Integer.Max);
			break;
		}
		case DvarType::DVAR_TYPE_BOOL:
		{
			rVal = registerInt(nName, (std::uint16_t)vtype, flags, nDesc, value.boolean, 0, 0, 0, 0, 0);
			break;
		}
		case DvarType::DVAR_TYPE_FLOAT:
		{
			rVal = registerFloat(nName, (std::uint16_t)vtype, flags, nDesc, value.floatval, 0, 0, 0, domain.Float.Min, domain.Float.Max);
			break;
		}
		case DvarType::DVAR_TYPE_STRING:
		{
			rVal = registerString(nName, (std::uint16_t)vtype, flags, nDesc, value.string, 0, 0, 0, 0, 0);
			break;
		}
		case DvarType::DVAR_TYPE_VECTOR_2:
		{
			rVal = registerFloat(nName, (std::uint16_t)vtype, flags, nDesc, value.vec2[0], value.vec2[1], 0, 0, domain.Float.Min, domain.Float.Max);
			break;
		}
		case DvarType::DVAR_TYPE_VECTOR_3:
		{
			rVal = registerFloat(nName, (std::uint16_t)vtype, flags, nDesc, value.vec3[0], value.vec3[1], value.vec3[2], 0, domain.Float.Min, domain.Float.Max);
			break;
		}
		case DvarType::DVAR_TYPE_VECTOR_4:
		{
			rVal = registerFloat(nName, (std::uint16_t)vtype, flags, nDesc, value.vec4[0], value.vec4[1], value.vec4[2], value.vec4[3], domain.Float.Min, domain.Float.Max);
			break;
		}
	}
	return rVal;
}