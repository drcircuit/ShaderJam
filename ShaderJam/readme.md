# ShaderJam
A shader coding sandbox for creating demos. Sort of like ShaderToy, however it is running on DirextX on Windows, and you use High Level Shading Language.

## Building
You will need a Windows 10 or later machine, with DirectX11 installed, as well as the DirectXTK builds for your platform. Populate the Includes and Lib directories with the header files as well as the lib files for x86 and x64 for both Debug and Release builds.
Use the config file settings.cfg to control what shaders are loaded and other options. Also if you want to publish your demo for constrained competitions, you will need to compile your shaders and load the compiled byte code instead of the raw shader text.