# Visual Debugger for Polygon Mesh Program

This debugger has a .exe vtk debugger, a .h file defining debugger connector and a .lib static lib.

## Features

The debugger can instantly display what is happening on your algorithm program. Several features are implemented, highlight vertices, edges and load meshes. Detailed usage can be found in **TestExample** project.

## Requirements

A **vtk** install is required to launch the debugger. 
An **OpenMesh** framework is required to build .lib connector. (has been integrated in the project)

## Getting Started

Clone the project, and open the .sln file in **Visual Studio 2022**. 

## Usage

+ 1. Build Debugger and static lib for debuggee
+ 2. Build your project or try with **TestExample** linked with .lib file
+ 3. **Open Debugger first**, you will see a vtk window and a command prompt
+ 4. **After** debugger loaded, open your program or **TestExample**
+ 5. As the debuggee calling functions of the connector, debugger will render the mesh and annotations in the vtk window
    
## Attention

+ 1. Always open debugger first, and make sure there is only one debugger running.
+ 2. If there is no reaction in the vtk window, click it then it will refresh. It is a bug that I am trying to fix now.
+ 3. This project cannot open .m file format, so convert it before.
