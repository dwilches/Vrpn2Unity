Vrpn2Unity
==========

This repository contains the following 2 folders:

Vrpn2Unity_DLL
====
Contains everything needed to build the DLL by yourself.
For example, if you want to use a different compiler or operating system
I also included the vrpn_server.exe and vrpn_print_devices.exe in the folder
vrpn_executables in case you want  to use them without compiling everything by yourself.

SampleUnityProject
====
This is a sample project. Just open it with Unity Pro and it will work.
Note that it expects to read the analog and button data from the
*Mouse0@localhost* device, so you will need to start your own
VRPN server and expose that device.

The code of the Unity project is very straightforward:

 * **VRPNController.cs:** This file is the one that communicates with the DLL.
   You don't need to modify it at all. Just let it inside your project's folder.
 * **SampleMouseBehaviour.cs:** This is the sample file that reads and logs
   the data that is read from the VRPN server. This file is just one
   example of how you should do with your custom devices.

