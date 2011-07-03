#ifndef stringtable_h
#define stringtable_h

#include <ren-script/script.h>

#include <map>

class StringTable
{
	public:
		StringTable(const String &Directory, const String &File); // File is relative to directory
		String operator ()(const String &Name) const;

	private:
		void Add(const String &From, const String &To);
		static int extString(lua_State *StateSource);

		std::map<String, String> Translations;
};

#endif
