#ifndef stringtable_h
#define stringtable_h

#include "../ren-general/filesystem.h"
#include "../ren-script/script.h"

#include <map>

class StringTable
{
	public:
		StringTable(FilePath const &File);
		String operator ()(const String &Name) const;

	private:
		void Add(const String &From, const String &To);
		std::map<String, String> Translations;
};

#endif
