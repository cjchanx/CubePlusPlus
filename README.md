<div align="center">
<img alt="Cube++" src="https://github.com/cjchanx/CubePlusPlus/assets/78698227/025c59ea-36be-4131-9b09-06c37ff81b52" width="300"/>
</div>

<a href="https://en.cppreference.com/w/" rel="CppReference">![Foo](https://user-images.githubusercontent.com/78698227/185344746-ace6a502-a9a3-43d6-9379-daad6f4ffcd3.svg)</a>
<a href="https://www.st.com/" rel="STMicro">![STMicro](https://user-images.githubusercontent.com/78698227/185344511-0296b5ed-15a3-4013-a98a-6dcd38222382.svg)</a>
<a href="https://www.freertos.org/features.html" rel="FreeRTOS">![FreeRTOS](https://camo.githubusercontent.com/da874464f191d72e47b3f6834b0ae26e8c3b5edbaeafcdacf526a844a972f87c/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f4672656552544f532d2532333234373133332e7376673f7374796c653d666f722d7468652d6261646765)</a>
<a href="https://github.com/cjchanx/CubePlusPlus.git" rel="GitHub">![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white)</a>

![](https://img.shields.io/github/repo-size/cjchanx/CubePlusPlus?label=Size)
![](https://img.shields.io/github/commit-activity/m/cjchanx/CubePlusPlus)
![](https://img.shields.io/github/contributors/cjchanx/CubePlusPlus)

# Introduction
Cube++ simplifies the process of creating a C/C++ codebase for an STM32 microcontroller in STM32CubeIDE while fully supporting CubeIDE's native code generation. This project is intended to be a submodule in a CubeIDE project to give access to some useful C++ wrappers and a method of routing control from the C main.c file to the C++ codebase located in one dedicated folder to support cleaner code compared to mixing it with code generation, and preventing the need for constantly renaming `main.c` to `main.cpp` every time code generation happens. Please refer to the [Setup Instructions](#Setup-Instructions) below for a guide on how to setup the project with Cube++

Cube++ is based on the core code of the [Student Organization for Aerospace Research's Avionics Software](https://github.com/StudentOrganisationForAerospaceResearch/AvionicsSoftware/tree/d3b55eb04511bf8b2d5ea3fb2d51fbd1329d9e81). As the codebase was originally intended for use in a specific way there are still updates to be made to improve compatability and capabilities.

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
- Under `System Core` > SYS > Timebase Source
    - Change the timebase source to a timer instead of SysTick (CubeIDE will recommend you do this upon code generation)

### IOC Peripheral Setup
#### For UART Debug Line 
(if not defining `DEBUG_DISABLED` as mentioned in the [Codebase Setup](#Codebase-Setup) section below)

- Inside Pinout and Configuration > Connectivity
    - Select a UART line for debug (if you choose to enable it) and set to Asynchronous mode with hardware flow control off
- At the top select Project Manager tab, go to the Advanced Settings and select the UART line for debug under Driver Selector
    - Change this UART line driver from `HAL` to `LL`


## Cube++ Setup
Add the Cube++ repository into the root folder as a submodule using one of these commands
```
git submodule add git@github.com:cjchanx/CubePlusPlus.git Cube++
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
 
