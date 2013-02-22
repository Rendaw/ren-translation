#include "translation.h"

bool Initialized = false;

void InitializeTranslation(String const &PackageName)
{
	assert(Initialized == false);
	setlocale(LC_ALL, "");
	textdomain(PackageName.c_str());
	Initialized = true;
}

bool IsTranslationInitialized(void) { return Initialized; }

