platform = ARGUMENTS.get('OS', Platform())

env = Environment(CC = 'gcc', CCFLAGS = '-std=c++11 -Wall')

sources = Glob('src/*.cpp')
sources.extend(Glob('src/util/*.cpp'))

release = ARGUMENTS.get('release', 0)
if int(release):
	env.Append(CCFLAGS = ' -O2')
else:
	env.Append(CCFLAGS = ' -g')

tests   = ARGUMENTS.get('tests', 0)
if int(tests):
	sources.extend(Glob('tests/*.cpp'))
else:
	sources.append('main.cpp')

env.Program('fek', sources, CPPPATH = ['include', 'include/util', 'tests'], LIBPATH = 'lib', LIBS = 'yaml-cpp')