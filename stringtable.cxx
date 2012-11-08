#include "stringtable.h"

StringTable::StringTable(FilePath const &File)
{
	std::vector<std::pair<String, String> > Variables;
	Variables.push_back(std::pair<String, String>("StringsLocation", File.Directory()));

	Script Source;

	for (auto &Variable : Variables)
	{
		Source.PushString(Variable.second);
		Source.SaveGlobal(Variable.first);
	}

	Source.PushTable();
	Source.PushFunction([&](Script &State) -> int
	{
		State.AssertString("String takes a string as its first argument.");
		String To = State.GetString();
		State.AssertString("String takes a string as its first argument.");
		Add(State.GetString(), To);
		return 0;
	});
	Source.PutElement("String");
	Source.SaveGlobal("ext");

	Source.Do(File, true);
}

String StringTable::operator ()(const String &Name) const
{
	std::map<String, String>::const_iterator Result = Translations.find(Name);
	if (Result == Translations.end()) return String("[UNT]\"") + Name + "\"";
	else return Result->second;
}

void StringTable::Add(const String &From, const String &To)
	{ Translations[From] = To; }

