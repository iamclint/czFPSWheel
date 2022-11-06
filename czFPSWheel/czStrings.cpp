#include "czStrings.h"
namespace czStrings
{
	bool findStringIC(std::string strHaystack, std::string strNeedle)
	{
		auto it = std::search(
			strHaystack.begin(), strHaystack.end(),
			strNeedle.begin(), strNeedle.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);
		return (it != strHaystack.end());
	}
	bool findStringIC(std::wstring strHaystack, std::wstring strNeedle)
	{
		auto it = std::search(
			strHaystack.begin(), strHaystack.end(),
			strNeedle.begin(), strNeedle.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);
		return (it != strHaystack.end());
	}
	bool findStringIC(std::wstring* strHaystack, std::wstring* strNeedle)
	{
		auto it = std::search(
			strHaystack->begin(), strHaystack->end(),
			strNeedle->begin(), strNeedle->end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);
		return (it != strHaystack->end());
	}

	bool findStringIC(std::string* strHaystack, std::string* strNeedle)
	{
		auto it = std::search(
			strHaystack->begin(), strHaystack->end(),
			strNeedle->begin(), strNeedle->end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);
		return (it != strHaystack->end());
	}
}