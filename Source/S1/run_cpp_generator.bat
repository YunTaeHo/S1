@echo off
set /p class_name=Enter the class name: 
set /p base_class_name=Enter the base class name: 
set /p module_name=Enter the module name: 
python create_cpp_files.py %class_name% %base_class_name% %module_name%
pause