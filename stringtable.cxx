#include "stringtable.h"

StringTable::StringTable(const String &Directory, const String &File)
{
	std::vector<std::pair<String, String> > Variables;
	Variables.push_back(std::pair<String, String>("StringsLocation", Directory));
	Script Source;
	Source.RegisterFunction("String", extString, this);
	Source.Do(Directory + File, Variables);
}

String StringTable::operator ()(const String &Name) const
{
	std::map<String, String>::const_iterator Result = Translations.find(Name);
	if (Result == Translations.end()) return String("[UNT]\"") + Name + "\"";
	else return Result->second;
}

void StringTable::Add(const String &From, const String &To)
	{ Translations[From] = To; }

int StringTable::extString(lua_State *StateSource)
{
	Script State(StateSource);

	String To = State.GetString();
	((StringTable *)State.GetAssociatedData())->Add(State.GetString(), To);

	return 0;
}
