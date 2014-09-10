rem Instructions to generate MSVC project files
rem
rem Please update CMake generator.
rem Remember that "Win64" in CMake generator targets a 64 bit executable
rem
rem Set the correct environment variables
rem For example:
rem     set BOOST_ROOT=D:\ThirdParty\boost_1_56_0__MSVC
mkdir binMSVC
cd binMSVC
mkdir buildRelease
cd buildRelease
cmake ..\.. -G"Visual Studio 12 Win64" -DCMAKE_BUILD_TYPE=Release -DINSTALL_PREFIX=installRelease
cd ..
cd ..
