#include "lightsettings.h"

#include <cassert>
#include <iostream>

#include <ren-script/script.h>

LightSettings::LightSettings(const String &Location) :
	Filename(Location)
	{ Refresh(); }

void LightSettings::Save(void)
{
	OutputStream Out(Filename.c_str());
	Out << "return {\n";

	for (std::map<String, String>::iterator CurrentValue = Values.begin();
		CurrentValue != Values.end(); CurrentValue++)
		Out << "\t[\"" << CurrentValue->first << "\"] = \"" << CurrentValue->second << "\",\n";

	Out << "}\n";
}

void LightSettings::Refresh(void)
{
	Values.clear();

	Script In;
	In.Do(Filename);

	while (In.PullNext())
	{
		String Value = In.GetString();
		In.GrabValue(-1);
		Values[In.GetString()] = Value;
	}
}

void LightSettings::Unset(const String &Value)
{
	int Count = Values.erase(Value);
	assert(Count == 1);
}
