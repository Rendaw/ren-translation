#ifndef translation_h
#define translation_h

#include <libintl.h>
#include <locale.h>
#include <limits>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <cassert>

void InitializeTranslation(std::string const &PackageName);
bool IsTranslationInitialized(void);

constexpr unsigned int HexPlaces(unsigned int Input)
{
	return Input >= 16 ? 1 + HexPlaces(Input >> 4) : 1;
}

inline unsigned int ReadHex(std::string const &Input)
{
	unsigned int Out = 0;
	for (char const &Place : Input)
	{
		Out = Out << 4;
		if ((Place >= '0') && (Place <= '9'))
			Out += Place - '0';
		else if ((Place >= 'a') && (Place <= 'f'))
			Out += Place - 'a' + 10;
		else if ((Place >= 'A') && (Place <= 'F'))
			Out += Place - 'A' + 10;
		else assert(false);
	}
	return Out;
}

// Convert multiple variables into an array of strings
inline void AsStringVector(std::vector<std::string> &) { }

template <typename NextArgumentType, typename ...ArgumentTypes> void AsStringVector(std::vector<std::string> &Out, NextArgumentType const &NextArgument, ArgumentTypes const &... Arguments)
{
	std::stringstream Convert;
	Convert << NextArgument;
	Out.push_back(Convert.str());
	AsStringVector(Out, std::forward<ArgumentTypes const &>(Arguments)...);
}

template <typename ...ArgumentTypes> std::vector<std::string> AsStringVector(ArgumentTypes const & ...Arguments)
{
	std::vector<std::string> Out;
	Out.reserve(sizeof...(ArgumentTypes));
	AsStringVector(Out, std::forward<ArgumentTypes const &>(Arguments)...);
	return Out;
}

// Translate value with
template <typename ...ArgumentTypes> std::string Local(std::string const &Input, ArgumentTypes const & ...Arguments)
{
	assert(IsTranslationInitialized());
	constexpr unsigned int Places = HexPlaces(sizeof...(ArgumentTypes));

	std::string LocalString = gettext(Input.c_str());

	if ((LocalString.length() < Places + 1) || (sizeof...(ArgumentTypes) == 0))
		return LocalString;

#ifndef NDEBUG
	// All markers must appear at least once in the template string
	{
		size_t Position = 0;
		std::set<unsigned int> Found;
		while ((Position = Input.find('^', Position)) <= (Input.size() - (Places + 1)))
		{
			if (LocalString[Position + 1] == '^')
			{
				// Allow escapes
				Position += 2;
				continue;
			}
			unsigned int Index = ReadHex(LocalString.substr(Position + 1, Places));
			assert(Index < sizeof...(ArgumentTypes));
			Found.emplace(Index);
			Position += Places + 1;
		}
		for (unsigned int Index = 0; Index < sizeof...(ArgumentTypes); ++Index)
			assert(Found.find(Index) != Found.end());
	}
#endif

	std::vector<std::string> Replacements = AsStringVector(std::forward<ArgumentTypes const &>(Arguments)...);
	std::stringstream Out, Number;

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
		assert(Index < sizeof...(ArgumentTypes)); // Arguments must be provided for all markers
		Out << Replacements[Index];

		LastPosition = Position += Places + 1;
	}

	// Add the final bit of the string
	Out << LocalString.substr(LastPosition, LocalString.length());

	return Out.str();
}

#endif
