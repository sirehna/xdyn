echo "Target 64 bits with Microsoft Visual Studio 12 2013"
set BOOST_ROOT=D:\ThirdParty\boost_1_56_0__MSVC
set buildType=Release
set suffix=64
rem set cmakeGenerator="Visual Studio 12"
set cmakeGenerator="Visual Studio 12 Win64"
mkdir -p ..\buildMsvc%buildType%%suffix%
cd ..\buildMsvc%buildType%%suffix%
cmake ..\code -G%cmakeGenerator% -DCMAKE_BUILD_TYPE=%buildType% -DINSTALL_PREFIX=..\installMsvc%buildType%%suffix%
