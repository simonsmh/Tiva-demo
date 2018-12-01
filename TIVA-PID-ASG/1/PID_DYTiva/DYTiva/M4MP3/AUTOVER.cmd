@ECHO OFF
SET ver_file=m4_ver.h
FOR /F "tokens=3" %%i IN (%ver_file%) DO (
	SET /A j=%%i+1
	ECHO #define BUILD_NUM %j%>> %ver_file%
)