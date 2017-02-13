Project Dependencies:
- [OpenGL Bindings](http://glew.sourceforge.net)

- [SDL2 for Windowing](https://www.libsdl.org/download-2.0.php)

- [SOIL for Texture Loading](http://www.lonesock.net/soil.html)

- [GLM for Matrix Math](http://glm.g-truc.net/0.9.8/index.html)

### For Visual Studio 15

1. Download the above libraries
2. Add glm folder to VC++ directories
3. Add include folders of GLEW and SDL2 to VC++ additional include folders
4. Add lib/Release/x86/, lib/x86/, and lib/x86/ to linker additional libraries from GLEW, SDL2, and SOIL respectively.


