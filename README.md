# GraphGame

Win-32 requirements
1. cocos2d-x 4.0
2. Visual Studio 2017 or 2019
3. cmake

To build app with Visual Studio run in terminal:
	cd GraphGame
	cocos run --proj-dir . -p win32

If cocos can't find Visual Studio you can edit the script
	cocos2d-x-4.0\tools\cocos2d-console\bin\utils.py
go to definition of 'get_newest_devenv(min_ver=None)' function and comment out the return value and paste the new one:
	# return (need_upgrade, find_path, find_ver)
	return (False, "c:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\Common7\\IDE\\devenv.com", 16)
    
