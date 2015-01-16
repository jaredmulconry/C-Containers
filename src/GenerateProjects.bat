@setlocal
@echo off
if -%1-==-- (
	echo Script requires destination directory
	exit /b -1
)
if not exist CMakeLists.txt (
	echo Current directory does not contain CMakeLists: %cd%
	exit /b -1
)
if -%2-==-- (
	echo No build type specified. Defaulting to Debug!
	set build_mode=Debug
) else (
	set build_mode=%2
)
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
::Set the various variables used for project generation
set src_dir=%cd%
set dest_dir=%1
set mingw_dest_suffix=MinGW
set vs_dest_suffix=VS
set nmake_dest_suffix=NMake
set mingw_dest_dir="%dest_dir%\%mingw_dest_suffix%"
set vs_dest_dir="%dest_dir%\%vs_dest_suffix%"
set nmake_dest_dir="%dest_dir%\%nmake_dest_suffix%"
echo Local variables have been set
::Create the destination directories if they don't exist
mkdir %mingw_dest_dir%
mkdir %vs_dest_dir%
mkdir %nmake_dest_dir%
echo Destination directories have been created
::Generate the project files for the gcc and VS targets
echo Began creating MinGW project files at %mingw_dest_dir%
pushd %mingw_dest_dir%
cmake -D CMAKE_BUILD_TYPE=%build_mode% -G "MinGW Makefiles" %src_dir%
popd
echo Began creating Visual Studio project files at %vs_dest_dir%
pushd %vs_dest_dir%
cmake -G "Visual Studio 14 2015 Win64" %src_dir%
popd
echo Began creating NMake project files at %nmake_dest_dir%
pushd %nmake_dest_dir%
cmake -D CMAKE_BUILD_TYPE=%build_mode% -G "NMake Makefiles JOM" %src_dir%
popd
::Generate batch file for invoking the compilers
echo Generating batch files
>%dest_dir%\build.bat (
	@echo @setlocal
	@echo @echo off
	@echo set flag_num="%%1"
	@echo if %%flag_num%%=="" ^(
	@echo 	echo No flag provided. Defaulting to 0 - Build all.
	@echo 	echo Specify 1 for clean. Specify 2 for test.
	@echo 	set build_flag=
	@echo ^) else ^(
	@echo 	if %%flag_num%%=="0" ^(
	@echo 		set build_flag=
	@echo 	^)
	@echo 	if %%flag_num%%=="1" ^(
	@echo 		set build_flag=clean
	@echo 	^)
	@echo 	if %%flag_num%%=="2" ^(
	@echo 		set build_flag=test
	@echo 	^)
	@echo ^)
	@echo pushd %mingw_dest_suffix%
	@echo echo.
	@echo echo Compiling with mingw32
	@echo echo.
	@echo call mingw32-make -j -Oline %%build_flag%%
	@echo popd
	@echo if not %%errorlevel%%==0 ^(
	@echo	echo.
	@echo	echo Compilation failed with mingw32 with error code %%errorlevel%%
	@echo	goto quick_exit
	@echo ^)
	@echo pushd %nmake_dest_suffix%
	@echo echo.
	@echo echo Compiling with NMake
	@echo echo.
	@echo call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
	@echo call jom /j4 /nologo %%build_flag%%
	@echo popd
	@echo :quick_exit
	@echo endlocal
)
::End of batch file generation
echo Batch file generation complete
endlocal
pushd %1