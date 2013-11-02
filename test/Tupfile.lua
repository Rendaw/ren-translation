DoOnce 'app/translation/Tupfile.lua'

local TranslationTest = Define.Executable
{
	Name = 'test',
	Sources = Item 'test.cxx',
	Objects = TranslationObjects
}

Define.Test { Executable = TranslationTest }
