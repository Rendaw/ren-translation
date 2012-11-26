#ifndef lightsettings_h
#define lightsettings_h

#include <map>

#include "../ren-general/string.h"
#include "../ren-general/inputoutput.h"

class LightSettings
{
	public:
		LightSettings(const String &Location);

		void Save(void);
		void Refresh(void);

		template <typename Type> Type Get(const String &Name, const Type &Default)
		{
			if (Values.find(Name) == Values.end()) return Default;

			Type Out = Default; 
			(void) (MemoryStream(Values[Name]) >> Out);
			return Out;
		}

		template <typename Type> void Set(const String &Name, const Type &NewValue)
		{
			Values[Name] = MemoryStream() << NewValue;
		}

		void Unset(const String &Value);

	private:
		String Filename;
		std::map<String, String> Values;
};

#endif
