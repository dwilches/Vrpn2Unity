
#include "stdafx.h"

//*****************************************************************************
// Include VRPN headers
//*****************************************************************************
#include <vrpn_Tracker.h>
#include <vrpn_Button.h>
#include <vrpn_Analog.h>

#include <vector>


//*****************************************************************************
// Each system needs different annotations to define which functions are
// exported from a DLL
//*****************************************************************************
#if _MSC_VER
#define DLL_EXPORT __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define DLL_EXPORT // XCode does not need annotating exported functions, so define is empty
#endif


//*****************************************************************************
// These are the types of the callbacks that are defined in Unity (see the
// example scene). There is a callback for each type of VRPN event:
// Analog, Button and Tracker.
//*****************************************************************************
typedef void(__stdcall *Vrpn2UnityAnalogCallback)(int num_channel, float value);
typedef void(__stdcall *Vrpn2UnityButtonCallback)(int num_button, int state);
typedef void(__stdcall *Vrpn2UnityTrackerCallback)(int num,
	float pos_x, float pos_y, float pos_z,
	float quat_x, float quat_y, float quat_z, float quat_w);



//*****************************************************************************
// This is the VRPN wrapper: a class that keeps track of the VRPN connection.
// There can be several instances of this class running at the same time.
//*****************************************************************************
class Vrpn2UnityWrapper
{
public:
	// Creates a wrapper that opens connections towards the specified device.
	// You can specify up to 3 callbacks, one for each event type that you are
	// expecting from your VRPN server. In case you are not expecting a certain
	// type of event just pass null for that callback.
	Vrpn2UnityWrapper(const char* device,
		Vrpn2UnityAnalogCallback analogCallback,
		Vrpn2UnityButtonCallback buttonCallback,
		Vrpn2UnityTrackerCallback trackerCallback);
	// The destructor of this class closes the connections and frees any memory
	// that had been allocated.
	virtual ~Vrpn2UnityWrapper();

	// VRPN callbacks (actually VRPN invokes the top-level callbacks, which
	// in turn invoke these 3 methods.
	void HandleAnalog(const vrpn_ANALOGCB& analogData);
	void HandleButton(const vrpn_BUTTONCB& buttonData);
	void HandleTracker(const vrpn_TRACKERCB& trackerData);

	// Executes the mainloop of the VRPN connections (one mainloop for the
	// analog connection, other for the button conenction and another one for
	// the tracker's.
	void MainLoop();

private:
	// These are the actual VRPN handlers with which you normally connect to
	// VRPN.
	vrpn_Analog_Remote* vrpnAnalog;
	vrpn_Button_Remote* vrpnButton;
	vrpn_Tracker_Remote* vrpnTracker;

	// These are references to the Unity callbacks that were passed to the
	// constructor.
	Vrpn2UnityAnalogCallback analogCallback;
	Vrpn2UnityButtonCallback buttonCallback;
	Vrpn2UnityTrackerCallback trackerCallback;
};


//*****************************************************************************
// VRPN callbacks.
// These are the actual callbacks invoked directly by the VRPN library.
//*****************************************************************************

void VRPN_CALLBACK handle_analog(void* v2u_wrapper, const vrpn_ANALOGCB analogData)
{
	reinterpret_cast<Vrpn2UnityWrapper*>(v2u_wrapper)->HandleAnalog(analogData);
}

void VRPN_CALLBACK handle_button(void* v2u_wrapper, const vrpn_BUTTONCB buttonData)
{
	reinterpret_cast<Vrpn2UnityWrapper*>(v2u_wrapper)->HandleButton(buttonData);
}

void VRPN_CALLBACK handle_tracker(void* v2u_wrapper, const vrpn_TRACKERCB trackerData)
{
	reinterpret_cast<Vrpn2UnityWrapper*>(v2u_wrapper)->HandleTracker(trackerData);
}


//*****************************************************************************
// VRPN wrapper method implementations
//*****************************************************************************

Vrpn2UnityWrapper::Vrpn2UnityWrapper(const char* device,
										Vrpn2UnityAnalogCallback analogCallback,
										Vrpn2UnityButtonCallback buttonCallback,
										Vrpn2UnityTrackerCallback trackerCallback)
{
	this->analogCallback = analogCallback;
	this->buttonCallback = buttonCallback;
	this->trackerCallback = trackerCallback;

	if (analogCallback)
	{
		vrpnAnalog = new vrpn_Analog_Remote(device);
		vrpnAnalog->register_change_handler(this, handle_analog);
	}

	if (buttonCallback)
	{
		vrpnButton = new vrpn_Button_Remote(device);
		vrpnButton->register_change_handler(this, handle_button);

	}

	if (trackerCallback)
	{
		vrpnTracker = new vrpn_Tracker_Remote(device);
		vrpnTracker->register_change_handler(this, handle_tracker);
	}
}

Vrpn2UnityWrapper::~Vrpn2UnityWrapper()
{
	if (analogCallback)
	{
		analogCallback = nullptr;
		delete vrpnAnalog;
		vrpnAnalog = nullptr;
	}

	if (buttonCallback)
	{
		buttonCallback = nullptr;
		delete vrpnButton;
		vrpnButton = nullptr;
	}

	if (trackerCallback)
	{
		trackerCallback = nullptr;
		delete vrpnTracker;
		vrpnTracker = nullptr;
	}
}

void Vrpn2UnityWrapper::HandleAnalog(const vrpn_ANALOGCB& analogData)
{
	// In case of doubt: this is not paranoic, actually there is a chance
	// of invoking this function a moment after the callback has been
	// deregistered as this is multithreaded.
	if (!analogCallback)
		return;

	for (int i = 0; i < analogData.num_channel; i++)
	{
		analogCallback(i, (float)analogData.channel[i]);
	}
}

void Vrpn2UnityWrapper::HandleButton(const vrpn_BUTTONCB& buttonData)
{
	// In case of doubt: this is not paranoic, actually there is a chance
	// of invoking this function a moment after the callback has been
	// deregistered as this is multithreaded.
	if (!buttonCallback)
		return;

	buttonCallback(buttonData.button, buttonData.state);
}

void Vrpn2UnityWrapper::HandleTracker(const vrpn_TRACKERCB& trackerData)
{
	// In case of doubt: this is not paranoic, actually there is a chance
	// of invoking this function a moment after the callback has been
	// deregistered as this is multithreaded.
	if (!trackerCallback)
		return;

	trackerCallback(trackerData.sensor,
		(float)trackerData.pos[0], (float)trackerData.pos[1], (float)trackerData.pos[2],
		(float)trackerData.quat[0], (float)trackerData.quat[1],
		(float)trackerData.quat[2], (float)trackerData.quat[3]);
}

void Vrpn2UnityWrapper::MainLoop()
{
	if (analogCallback)
		vrpnAnalog->mainloop();

	if (buttonCallback)
		vrpnButton->mainloop();

	if (trackerCallback)
		vrpnTracker->mainloop();
}


//*****************************************************************************
// List of VRPN wrappers
//*****************************************************************************

// Each VRPN wrapper is registered in this collection, and the assigned spot
// is never reassigned to other wrapper even after the original wrapper has
// been freed.
std::vector<Vrpn2UnityWrapper*> v2u_wrappers;


//*****************************************************************************
// Functions that will be exported from the DLL
//*****************************************************************************

// From Unity you will call these functions using the "[DllImport]" feature
extern "C" {

	// This function must be called to initialize a device. You can pass NULL
	// for any of the callbacks if you don't need them.
	// This function returns a DEVICE_ID which you need to keep in Unity
	// and pass as a parameter for the other functions.
	//
	// Recommendation: call this function form the "Start" function of one
	// of your MonoBehaviours.
	//
	// DO NOT ASSUME anything about the value of this DEVICE_ID.
	DLL_EXPORT int __stdcall VrpnStart(const char* device,
										Vrpn2UnityAnalogCallback analogCallback,
										Vrpn2UnityButtonCallback buttonCallback,
										Vrpn2UnityTrackerCallback trackerCallback)
	{
		v2u_wrappers.push_back(new Vrpn2UnityWrapper(device,
								analogCallback,
								buttonCallback,
								trackerCallback));

		// Return ID of this VRPN device
		return v2u_wrappers.size() - 1;
	}

	// This function tells this DLL when is a good moment to communicate with
	// VRPN and trigger callback invocations.
	// You need to pass the same DEVICE_ID that was obtained from the call to
	// "VrpnStart()".
	//
	// Recommendation: call this function form the "Update" function of one
	// of your MonoBehaviours. if that is too often for you, use Time.deltaTime
	// to keep track of the last time it was called and invoke it as often as
	// you want.
	DLL_EXPORT void VrpnUpdate(int device_idx)
	{
		// This is better than causing a SEGFAULT
		if (!v2u_wrappers[device_idx])
			return;

		v2u_wrappers[device_idx]->MainLoop();
	}

	// This function tells this DLL to deallocate any memory used for the
	// device DEVICE_ID. After this call the actual VRPN connection is closed
	// and you will not receive any further callback invocations.
	//
	// You need to pass the same DEVICE_ID that was obtained from the call to
	// "VrpnStart()".
	//
	// Recommendation: call this function form the "OnDestroy" function of one
	// of your MonoBehaviours.
	DLL_EXPORT void VrpnOnDestroy(int device_idx)
	{
		if (!v2u_wrappers[device_idx])
			return;

		delete v2u_wrappers[device_idx];
		v2u_wrappers[device_idx] = nullptr;
	}

}
