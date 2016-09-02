Vrpn2Unity
==========
This library lets you read data from your devices through VRPN and into Unity. It works with Unity 5.x (even the Personal version). With this library you don't need to worry about hogging your Unity thread, as all the polling to VRPN is made in a separate thread. You just call Vrpn2Unity methods and it does all the heavy lifting behind the scenes.

This project's repository contains the following folders.

UnityPackages
====
This is the one you need to start working with [VRPN][2] and [Unity][4] together.
If you are in a hurry, just download the Unity Package and import it to your project. There is a sample scene you can check to understand how to make your own device work.

UnityProject
====
This folder contains a sample project where I imported the aforementioned package. Just open it with Unity and it will work.
Note that it expects to read the analog and button data from the
*Mouse0@localhost* device, so you will need to start your own
VRPN server and expose that device.

The code of the Unity project is very straightforward:

 * **VRPNController.cs:** This file is the one that communicates with the DLL.
   You don't need to modify it at all. Just let it inside your project's folder.
 * **SampleMouseBehaviour.cs:** This is the sample file that reads and logs
   the data that is read from the VRPN server. This file is just one
   example of how you should do with your custom devices.

So for communicating with your own devices just copy&paste *SampleMouseBehaviour.cs* and modified as you want.

Vrpn2Unity_DLL
====
This is the heart of this project. This folder contains everything needed to build the DLL by yourself (you don't actually need to do this as the DLL is already in the provided Unity Package, but if you are curious, take a look).

I also included the *vrpn_server.exe* and *vrpn_print_devices.exe* in the folder
*vrpn_executables* in case you want to use them without compiling everything by yourself. Note "everything VRPN" is released under their own license: https://github.com/vrpn/vrpn/wiki/License.


Foot notes:
====
* You need the 32-bit version of the Unity editor as VRPN is compiled for 32-bits
* The version used for building this DLL was VRPN 07.33 so if you want to use another version, just download the full source-code.
* I'm providing the compiled DLLs inside those ZIP files. These DLLs don't depend on other 3rd-party libraries, but you will need the [redistributable package of Microsoft's Visual Studio 2013][1]. I have also tested it with the redistributables of VS2012 and it works, other versions may work as well but I haven't tested them.

License
=======
This work is released under the [Creative Commons Attributions][3] license.

If you use this library please let me know, so I know my work has been useful to you :)

![CC Attribution](images/CC-BY_icon.png?raw=true)


[1]: http://www.microsoft.com/en-us/download/details.aspx?id=40784
[2]: https://github.com/vrpn/vrpn/wiki
[3]: https://creativecommons.org/licenses/by/2.0/
[4]: https://unity3d.com/
