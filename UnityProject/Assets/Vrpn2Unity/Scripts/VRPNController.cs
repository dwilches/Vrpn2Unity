/**
 * Vrpn2Unity
 * Author: Daniel Wilches <dwilches@gmail.com>
 *
 * This work is released under the Creative Commons Attributions license.
 * https://creativecommons.org/licenses/by/2.0/
 */

using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

//*****************************************************************************
// This class encapsulates all the boilerplate of communicating with VRPN.
// All you need to do is create an instance of it and invoke the following
// functions:
//
// - VrpnStart(): I recommend to invoke this on the "Start()" of your class
//                so you are 100% sure this executes only once.
//
// - VrpnOnDestroy(): I recommend to invoke this on the "OnDestroy()" of your
//                    class so you are 100% sure this always executes, and
//                    always once.
//
// - VrpnUpdate(): You need to invoke this function to allow the VRPN
//                 main loop to execute. I recommend you call it in the
//                 "Update()" of your MonoBehaviour. If you think that is too
//                 much for your needs, then don't call it in each "Update()"
//                 invocation, you can call it once each 100ms with the help
//                 of Time.deltaTime
//
// An example class that uses Vrpn2Unity is found on "MouseVRPNBehaviour.cs".
//*****************************************************************************
public class VRPNController
{
    //*************************************************************************
    // These are the callback delegates. They are needed for the interface
    // between the DLL and Unity. The DLL will call whatever you pass to it
    // using these delegates.
    //*************************************************************************
    public delegate void AnalogCallback(int num_channel,
                    float value);
    public delegate void ButtonCallback(int num_button,
                    int state);
    public delegate void TrackerCallback(int sensor_id,
                    float pos_x, float pos_y, float pos_z,
                    float quat_x, float quat_y, float quat_z,
                    float quat_w);


    //*************************************************************************
    // These are the function exported from the DLL. You can call them as you
    // would call any other function.
    //*************************************************************************

    // This function must be called to initialize a device. You can pass NULL
    // for any of the callbacks if you don't need them.
    // This function returns a DEVICE_ID which you need to keep in Unity
    // and pass as a parameter for the other functions.
    //
    // Recommendation: call this function form the "Start" function of one
    // of your MonoBehaviours.
    //
    // DO NOT ASSUME anything about the value of this DEVICE_ID.
    [DllImport ("Vrpn2Unity", CallingConvention=CallingConvention.StdCall)]
    public static extern int VrpnStart (string device,
                                          AnalogCallback analogCallback,
                                          ButtonCallback buttonCallback,
                                          TrackerCallback trackerCallback);

    // This function tells this DLL when is a good moment to communicate with
    // VRPN and trigger callback invocations.
    // You need to pass the same DEVICE_ID that was obtained from the call to
    // "VrpnStart()".
    //
    // Recommendation: call this function form the "Update" function of one
    // of your MonoBehaviours. if that is too often for you, use Time.deltaTime
    // to keep track of the last time it was called and invoke it as often as
    // you want.
    [DllImport ("Vrpn2Unity")]
    public static extern void VrpnUpdate (int device_index);

    // This function tells this DLL to deallocate any memory used for the
    // device DEVICE_ID. After this call the actual VRPN connection is closed
    // and you will not receive any further callback invocations.
    //
    // You need to pass the same DEVICE_ID that was obtained from the call to
    // "VrpnStart()".
    //
    // Recommendation: call this function form the "OnDestroy" function of one
    // of your MonoBehaviours.
    [DllImport ("Vrpn2Unity")]
    public static extern void VrpnOnDestroy (int device_index);

}
