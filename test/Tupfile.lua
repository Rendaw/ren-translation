DoOnce 'app/translation/Tupfile.lua'

local LinkFlags = ''
if tup.getconfig 'PLATFORM' == 'windows'
then
	LinkFlags = LinkFlags .. ' -lintl'
end

local TranslationTest = Define.Executable
{
	Name = 'test',
	Sources = Item 'test.cxx',
	Objects = TranslationObjects,
	LinkFlags = LinkFlags
}

Define.Test { Executable = TranslationTest }
