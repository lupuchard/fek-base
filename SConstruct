platform = ARGUMENTS.get('OS', Platform())

#env = Environment(CXX = 'clang++', CXXFLAGS = '-std=c++1y -Wall')
env = Environment(CC = 'gcc', CCFLAGS = '-std=c++1y -Wall')

sources = Glob('src/*.cpp')
sources.extend(Glob('src/util/*.cpp'))

release = ARGUMENTS.get('release', 0)
if int(release):
	env.Append(CXXFLAGS = ' -O2')
else:
	env.Append(CXXFLAGS = ' -g')

tests   = ARGUMENTS.get('tests', 0)
if int(tests):
	sources.extend(Glob('tests/*.cpp'))
else:
	sources.append('main.cpp')

cpppath = ['include', 'include/util', 'irrlicht', 'tests']
libs    = ['yaml-cpp', 'Irrlicht', 'boost_regex', 'boost_system', 'boost_filesystem', 'GL']
libpath = ['lib']

# all the boost used:
#   preprocessor in Enum
#   filesystem in ResourceManager
#   regex in Formula
#   variant in Formula and Type

if str(platform) == "posix":
	libs.append('X11')
	libs.append('Xxf86vm')

env.Program('fek', sources, CPPPATH = cpppath, LIBPATH = libpath, LIBS = libs)