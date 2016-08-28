using UnityEngine;
using System.Collections;


//*****************************************************************************
// This class is a sample of how to use the Vrpn2Unity bridge.
// You can use it to have an idea of what to do for your own needs.
//
// You can have as many of these classes as you want, so you can receive
// updates from several devices in the same Unity application.
//*****************************************************************************
public class SampleMouseBehaviour : MonoBehaviour
{

    //*************************************************************************
    // Public variables (editables in the editor)
    //*************************************************************************

    // VRPN address for the device we want to open
    public string device_address = "Mouse0@localhost";


    //*************************************************************************
    // Private variables
    //*************************************************************************

	// Used to keep track of the current position of the mouse, as Analog
	// callbacks are invoked once for each channel independently.
	private static float mouse_x, mouse_y;
	// You need to keep track of the DEVICE_ID as it is needed for invoking
	// some functions
	private int device_id = -1;


    //*************************************************************************
    // Unity callbacks necessary to react for obejct's lifetime events
    //*************************************************************************

    // Start the VRPN connection through Vrpn2Unity.
    // Don't use Start for this because if you modify your script while Unity is
    // running the only two methods called to notify of it are: OnEnable() and
    // OnDisable().
    void OnEnable()
    {
        // Open the device for button+analog updates. Note that as a mouse
        // doesn't generate tracker callbacks we are passing 'null' as the
        // tracker delegate.
        device_id = VRPNController.VrpnStart(device_address,
                          new VRPNController.AnalogCallback(onMouseMove),
                          new VRPNController.ButtonCallback(onMouseButton),
                          null);
        Debug.Log("Opened VRPN device " + device_address +
                  " with device_id:" + device_id);
    }

    // Free the VRPN and Vrpn2Unity resources.
    // Don't use OnDestory for this because if you modify your script while Unity is
    // running the only two methods called to notify of it are: OnEnable() and
    // OnDisable().
    void OnDisable()
    {
        if (device_id != -1)
        {
            Debug.Log("Freeing resources used by VRPN and Vrpn2Unity for " +
                      "device_id:" + device_id);

            VRPNController.VrpnOnDestroy(device_id);
            device_id = -1;
        }
    }

	// Update is called once per frame
	void Update ()
    {
        // Poll new data from Unity
        VRPNController.VrpnUpdate(device_id);

        // This is an example of how to move a cube with the mouse using VRPN
        transform.localPosition = new Vector3((mouse_x - 0.5f) * 5, -(mouse_y - 0.5f) * 5, 0);
	}


    //*************************************************************************
    // Static callbacks
    //*************************************************************************

    // Button callback
    public static void onMouseButton(int num_button, int state)
    {
        Debug.Log("A mouse button has changed its state. Button: " + num_button
                    + " state: " + (state == 1 ? "PRESSED" : "RELEASED"));
    }

    // Analog callback
    public static void onMouseMove(int num_channel, float value)
    {
        if (num_channel == 0)
            mouse_x = value;
        else
            mouse_y = value;
    }
}
