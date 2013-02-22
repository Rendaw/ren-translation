#ifndef translation_h
#define translation_h

#include <libintl.h>
#include <locale.h>
#include <limits>

#include "ren-general/inputoutput.h"
#include "ren-general/range.h"

void InitializeTranslation(String const &PackageName);
bool IsTranslationInitialized(void);

constexpr unsigned int HexPlaces(unsigned int Input)
{
	return Input >= 16 ? 1 + HexPlaces(Input >> 4) : 1;
}

inline unsigned int ReadHex(String const &Input)
{
	unsigned int Out = 0;
	for (char const &Place : Input)
	{
		Out = Out << 4;
		if (RangeD('0', '9').Contains(Place))
			Out += Place - '0';
		else if (RangeD('a', 'f').Contains(Place))
			Out += Place - 'a' + 10;
		else if (RangeD('A', 'F').Contains(Place))
			Out += Place - 'A' + 10;
		else assert(false);
	}
	return Out;
}

// Convert multiple variables into an array of strings
inline void AsStringVector(std::vector<String> &Out) { }

template <typename NextArgumentType, typename ...ArgumentTypes> void AsStringVector(std::vector<String> &Out, NextArgumentType NextArgument, ArgumentTypes ...Arguments)
{
	Out.push_back(AsString(NextArgument));
	AsStringVector(Out, Arguments...);
}

template <typename ...ArgumentTypes> std::vector<String> AsStringVector(ArgumentTypes const & ...Arguments)
{
	std::vector<String> Out;
	Out.reserve(sizeof...(ArgumentTypes));
	AsStringVector(Out, Arguments...);
	return Out;
}

// Translate value with 
template <typename ...ArgumentTypes> String Local(String const &Input, ArgumentTypes const & ...Arguments)
{
	assert(IsTranslationInitialized());
	constexpr unsigned int Places = HexPlaces(sizeof...(ArgumentTypes));

	String LocalString = gettext(Input.c_str());

	if ((LocalString.length() < Places + 1) || (sizeof...(ArgumentTypes) == 0)) 
		return LocalString;

#ifndef NDEBUG
	for (unsigned int Index = 0; Index < sizeof...(ArgumentTypes); ++Index)
	{
		MemoryStream Marker("^");
		Marker << OutputStream::StringHex(Index).PadTo(Places);
		assert(Input.find((String)Marker) < Input.size());
	}
#endif

	std::vector<String> Replacements = AsStringVector(Arguments...);
	unsigned int NewLength = 0;
	for (auto const &Replacement : Replacements)
		NewLength += Replacement.length();
	MemoryStream Out(NewLength + LocalString.length()), Number;

	size_t Position = 0, LastPosition = 0;
	while ((Position = LocalString.find('^', Position)) <= (LocalString.size() - (Places + 1)))
	{
		if (LocalString[Position + 1] == '^')
		{
			// Allow escapes
			Position += 2;
			continue;
		}

		// Add the unchanged part of the string
		Out << LocalString.substr(LastPosition, Position - LastPosition);

		// Add the marker replacement
		unsigned int Index = ReadHex(LocalString.substr(Position + 1, Places));
		assert(Index < sizeof...(ArgumentTypes));
		Out << Replacements[Index];

		LastPosition = Position += Places + 1;
	}

	// Add the final bit of the string
	Out << LocalString.substr(LastPosition, LocalString.length());

	return Out;
}

#endif
