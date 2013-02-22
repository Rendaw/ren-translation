#ifndef translation_h
#define translation_h

constexpr unsigned int HexPlaces(unsigned int Input)
{
	return Input > 0 ? 1 + HexPlaces(Input >> 1) : 0;
}

// Convert multiple variables into an array of strings
void AsString(std::vector<String &Out) { }

template <typename NextArgumentType, typename... ArgumentTypes> void AsString(std::vector<String> &Out, NextArgumentType NextArgument, ArgumentTypes... Arguments)
{
	Out.push_back(AsString(NextArgument));
	AsString(Out, Arguments...);
}

template <typename... ArgumentTypes> std::vector<String> AsString(ArgumentTypes... const &Arguments)
{
	std::vector<String> Out;
	Out.reserve(sizeof...(ArgumentTypes));
	AsString(Out, Arguments...);
	return Out;
}

// Translate value with 
template <typename... ArgumentTypes> String Local(String const &Input, ArgumentTypes... Arguments)
{
	constexpr unsigned int Places = HexPlaces(sizeof...(ArgumentTypes));

	String LocalString = gettext(Input.c_str());

	if ((LocalString.length() < Places + 1) || (sizeof...(ArgumentTypes) == 0)) 
		return LocalString;

#ifndef NDEBUG
	for (unsigned int Index = 0; Index < sizeof...(ArgumentTypes); ++Index)
	{
		MemoryStream Marker("\\");
		Marker << OutputStream::StringHex(Index).PadTo(Places);
		assert(Input.find(Marker.str()) < Input.size());
	}
#endif

	std::vector<String> Replacements = AsString(Arguments...);
	unsigned int NewLength = 0;
	for (auto const &Replacement : Replacements)
		NewLength += Replacement.length();
	MemoryStream Out(NewLength + LocalString.length()), Number;

	size_t Position = 0, LastPosition = 0;
	while (Position = LocalString.find('\\', Position) <= LocalString.size() - (Places + 1))
	{
		if (LocalString[Position + 1] == '\\')
		{
			// Allow escapes
			Position += 2;
			continue;
		}

		// Add the unchanged part of the string
		Out << LocalString.substr(LastPosition, Position - LastPosition);
		IndexString = LocalString.substr(Position + 1, Places);

		// Add the marker replacement
		unsigned int Index; 
		IndexString >> Index; 
		assert(Index < sizeof...(ArgumentTypes));

		Out << Replacements[IndexString];

		LastPosition = Position += Places + 1;
	}

	// Add the final bit of the string
	Out << LocalString.substr(LastPosition, LocalString.length());

	return Out;
}

#endif
