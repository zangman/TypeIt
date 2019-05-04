# TypeIt Plugin for X-Plane 11

This is a plugin that allows you to use your regular keyboard to configure the
Radio and the FMC (Zibo mod only) for X-Plane 11.

TypeIt supports the following:

*   FMC - Full support for flight planning (Zibo Mod only).
*   Radio
    *   COM1 and COM2 Standby frequencies.
    *   NAV1 and NAV2 Standby frequencies.
    *   Transponder/Squawk code.

# Usage

## FMC

The key configuration is setup as follows for the FMC:

*   Alphabets and numbers as per normal.
*   Special keys using SHIFT followed by the first letter of the key. Ex: EXEC
    is pressed by SHIFT+E and RTE is pressed by SHIFT+R. Only exceptions to this
    rule are CRZ (SHIFT+Z) and DES (SHIFT+S).
*   CLR is pressed with both DELETE and BACKSPACE.
*   LEFT buttons are activated by F1 to F6.
*   RIGHT buttons are activated by F7 to F12.

## Radios

Radios are as simple as calling pressing the right hotkey and entering the value
into the textbox that pops up.

# Configuration

All the hotkeys can be configured in SETTINGS->KEYBOARD.
One suggested configuration is:

*   Set COM1 Standby Frequency: `;`
*   Set COM2 Standby Frequency: `Shift + ;`
*   Set NAV1 Standby Frequency: `'`
*   Set NAV2 Standby Frequency: `Shift + '`
*   Set Transponder/Squawk code: `\`
*   Toggle FMC Input mode: `Shift + Ctrl + o`

# Development

This project has been built using Visual Studio Community 2017. Create a new project and copy the source and header files into the project.

## Download X-Plane Plugin SDK

Download the X-Plane SDK (v3.0.1) from [here](https://developer.x-plane.com/sdk/plugin-sdk-downloads/). This plugin was developed using the `XPLM301` macros. The configuration for this is explained later.

## Set up the SDK

Extract the sdk zip file downloaded above. Within you will find the headers in a folder called `CHeaders`. If you already have an existing C++ development environment where you put all your headers, copy the contents of this folder (not the folder itself) into the headers folder. 

If not, create an empty folder on your system anywhere (I prefer Documents) and call it `Includes`. Copy the contents of this folder into the `Includes` folder.

Also in the sdk you should find a folder called `Libraries`. Go into that folder and under the `Win` folder there should be 2 `.lib` files. If you have an existing C++ dev environment where you put your libs, copy the contents of this folder there.

If not, create an empty folder on your system anywhere (I prefer Documents) and call it `Libs`. Copy the contents of this folder into the `Libs` folder.

## Set up the project

1.   Right click on your project in Visual Studio and select `Properties`.
1.   In the left menu select `General` and under `Configuration Type` select `Dynamic Library (.dll)`. X-Plane plugins are pretty much dlls.
1.   In the left menu select VC++ Directories and under `Include Directories` select `Edit` and put in the path to the `Includes` folder from above.
1.   Repeat the same for the `Library Directories` with the `Libs` folder.
1.   In the left menu, under `C/C++`, select `Command Line` and under `Additional Options` copy and paste the following:
```
-DIBM=1 -DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1
```
1.   In the left men, under `Linker` > `Input`, select `Additional Dependencies`and hit `Edit`. In the box, type in each of the following on a newline:
```
opengl32.lib
XPLM_64.lib
XPWidgets_64.lib
```
1.   That should complete the project setup. Try and compile the code. If it works then you're golden! If not, I would suggest checking the steps again or looking at the documentation on the [plugin page](https://developer.x-plane.com/sdk/).


## Testing the project

Compile everything and you should have a `.dll` file generated. In your X-Plane 11 folder, go to `Resources` > `Plugins` and create a new folder called `TypeIt`. Within that folder create another folder called `64`. Copy your generated `dll` file into this folder and rename it as `win.xpl`.

Start X-Plane 11 and you should be able to use the plugin.
