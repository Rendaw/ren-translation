#include <cassert>
#include <iostream>

#include "../translation.h"

void AssertEqual(std::string const &Got, std::string const &Expected)
{
	std::cout << "Got is: [[" << Got << "]]\nExpected is : [[" << Expected << "]]" << std::endl;
	assert(Got == Expected);
}

int main(int argc, char **argv)
{
	InitializeTranslation("donottranslate");
	assert(HexPlaces(4) == 1);
	assert(HexPlaces(0) == 1);
	assert(HexPlaces(15) == 1);
	assert(HexPlaces(16) == 2);
	assert(HexPlaces(17) == 2);
	assert(ReadHex("0") == 0);
	assert(ReadHex("1") == 1);
	assert(ReadHex("9") == 9);
	assert(ReadHex("a") == 10);
	assert(ReadHex("A") == 10);
	assert(ReadHex("F") == 15);
	assert(ReadHex("10") == 16);
	assert(ReadHex("1F") == 31);
	AssertEqual(Local("test"), "test");
	AssertEqual(Local("^0test", "q "), "q test");
	AssertEqual(Local("test^0", " q"), "test q");
	AssertEqual(Local("^1test^0", " q", "q "), "q test q");
	AssertEqual(Local("^1test^0", " q", "q "), "q test q");
	AssertEqual(Local("test^^0"), "test^^0");
	AssertEqual(Local("^0test", 1), "1test");
	AssertEqual(Local("^0test^0", 4), "4test4");
	AssertEqual(Local("^00test", "q "), "q 0test");
	AssertEqual(Local("^00test^01^02^03^04^05^06^07^08^09^0a^0b^0c^0d^0e^0f^10", "q ", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), "q test12345678910111213141516");
	return 0;
}

