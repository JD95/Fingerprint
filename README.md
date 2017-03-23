Project Dependencies:
- [OpenGL Bindings](http://glew.sourceforge.net)

- [SDL2 for Windowing](https://www.libsdl.org/download-2.0.php)

- [SOIL for Texture Loading]( )

- [GLM for Matrix Math](http://glm.g-truc.net/0.9.8/index.html)

### For Visual Studio 15

Step 1: Get the projects (labrat and labrat-game)
  - Clone both repositories
  - Create a Visual Studio (2015) Project
  - Move both repositories into that project’s source code folder
  
Step 2: Get and build the libraries
  - Download the following library versions:
    - glew-2.0.0-win32
    - SDL2 2.0.4 VC Development Library
    - SOIL (Simple OpenGL Image Library)
      - Build the included VC8 project to make SOIL.lib
    - GLM 0.9.8.4
  
Step 3: Edit your project properties (All paths relative to where you put the libraries)
  - VC++ Directories -> Include Directories
    - glm-0.9.8.4\glm\glm
    - glew-2.0.0-win32\glew-2.0.0\include
    - SLD2-2.0.4\include
    - Simple OpenGL Image Library\src
  - Linker -> General -> Additional Library Directories
    - SDL2-2.0.4\lib\x86
    - glew-2.0.0-win32\glew-2.0.0\lib\Release\Win32
    - SOIL (wherever you put the SOIL.lib you build in Step 2)
  - Linker -> Input -> Additional Dependencies
    - glew32.lib
    - SDL2.lib
    - SDL2main.lib
    - SOIL.lib
    - opengl32.lib

Step 4:
  - Properties -> Linker -> System -> SubSystem: make sure that it is Console (/SUBSYSTEM:CONSOLE)

Step 5:
  - Build the project, it will fail but create a Debug folder in the main directory of the project.
  - From glew-2.0.0\bin\Release\Win32 copy glew32.dll and paste it into the newly created debug folder where the project's .exe has been built
  - From SDL2-2.0.4\lib\x86 copy SDL2.dll into the same location as glew32.dll