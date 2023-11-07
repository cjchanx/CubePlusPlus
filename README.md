<div align="center">
<img alt="Cube++" src="https://github.com/cjchanx/CubePlusPlus/assets/78698227/c3764e36-0ced-4585-81e8-78a98a1e3723" width="425"/>
</div>

# Introduction

# Setup Instructions 
## CubeIDE Project Setup
- Create a new project for your specific chip and select C++ when it asks you for Language

## IOC Setup
### Middleware Setup
- Under `Middleware and Software Packs` enable FreeRTOS using `CMSIS_V1` (unless you are using a multi-core CPU in which case `CMSIS_V2` may be preferred
- Change the following values in config parameters
    - FreeRTOS Heap Size to fairly large (eg. 64KB on a 90KB RAM chip)
    - Minimum task size to at least 192 words
    - Under Software Timer Defines "USE_TIMERS" should be enabled
- Under `Middleware and Software Packs` > FreeRTOS > Advanced Settings
    - Enable USE_NEWLIB_REENTRANT
- Under Computing > CRC 
    - Enable Activated

### IOC Peripheral Setup



## Cube++ Setup
Clone the Cube++ repository into the root folder using one of these commands
```
git clone <repo_link> Cube++
```

Please add the following to include paths:
(right click on the folder and select `Add/Remove Include Path...`)
- Cube++/Core
- Cube++/System/Inc
- Cube++/Drivers/Inc
- Cube++/Libraries/embedded-template-library/include

Please ensure the following folders are not in the exclude from build option:
(right click on the folder, select properties, select configuration 'all configuration' and untick "select `Exclude Resource from Build`")

### Codebase Setup
- It is recommended to setup a new folder called `Components` or `Modules` in the root where all the code goes.
- There are a few files that you should have in `Components`/`Modules`:
	- SystemDefines.hpp : An example can be found TODO
	- main_system.cpp   : This can be named anything you want, but should contain the run_main() function that is the entry point for your codebase, example TODO
	- main_system.hpp   : Header file for main_system.cpp, example TODO
	- Core/Inc/RunInterface.hpp : Header file for the run interface which allows C code to call into the C++ codebase without errors, example TODO
	- Core/RunInterface.cpp : Code file for the run interface, example TODO
- Setup Debug UART
	- Setup one UART line as the UART debug line by following theinstructions uder Cube++/Drivers/UARTDriver_README.md, this should correspond to UART Driver linked to the 
	`constexpr UARTDriver* const DEFAULT_DEBUG_UART_DRIVER = UART::Debug;` line that is required to be in the SystemDefines.hpp file
	- In the case that you do not want a UART line dedicated to debug, then set DEFAULT_DEBUG_UART_DRIVER to nullptr, and
	define the macro DISABLE_DEBUG using `#define DISABLE_DEBUG` inside SystemDefines.hpp
- Setup main.c
	- Under Private includes in the first USER CODE BEGIN section, add the run interface as an include `RunInterface.hpp`
	- Inside the int main(void) function definition, add the following inside `USER CODE BEGIN 2`:
```
  run_interface();
  #if 0
```
	- Then inside `USER CODE BEGIN 3` after the `}` you want to put an `#endif`
	- This ensures that the only tasks that will be started up are the ones you add to your run_main() function
 
