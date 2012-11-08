#include "lightsettings.h"

#include <cassert>
#include <iostream>

#include "../ren-script/script.h"
#include "../ren-script/databuilder.h"

LightSettings::LightSettings(const String &Location) :
	Filename(Location)
	{ Refresh(); }

void LightSettings::Save(void)
{
	FileOutput Out(Filename.c_str());
	Out << "return {\n";

	ScriptDataBuilder ScriptOut(Out, 1);

	for (auto const &CurrentValue : Values)
		ScriptOut.Key(CurrentValue.first).Value(CurrentValue.second);

	Out << "}\n";
}

void LightSettings::Refresh(void)
{
	Values.clear();

	Script In;
	In.Do(Filename, true);

	if ((In.Height() > 0) && In.IsTable())
		In.Iterate([this](Script &State) -> bool
		{
			String Value = State.GetString();
			State.Duplicate(-1);
			Values[State.GetString()] = Value;
			return true;
		});
}

void LightSettings::Unset(const String &Value)
{
	int Count = Values.erase(Value);
	assert(Count == 1);
}
