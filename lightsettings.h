#ifndef lightsettings_h
#define lightsettings_h

#include <map>

#include <ren-general/string.h>

class LightSettings
{
	public:
		LightSettings(const String &Location);

		void Save(void);
		void Refresh(void);

		template <typename Type> Type Get(const String &Name, const Type &Default)
		{
			if (Values.find(Name) == Values.end()) return Default;

			StringStream Conversion(Values[Name]);
			Type Out; Conversion >> Out;
			return Out;
		}

		template <typename Type> void Set(const String &Name, const Type &NewValue)
		{
			StringStream Conversion;
			Conversion << NewValue;
			Values[Name] = Conversion.str();
		}

		void Unset(const String &Value);

	private:
		String Filename;
		std::map<String, String> Values;
};

#endif
