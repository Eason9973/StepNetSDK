﻿/**
* StepVR is a Single-Node Location and Motion Capture library
* released by G-Wearables Company. This is a Dynamic-link
* library fit for C++ developer, Unity and C# developer, and
* Unreal developer. The C# library is based on C++ library
* and generated using SWIG. For more information about SWIG,
* see: http://www.swig.org/. The C++ version is a Dynamic-link
* library(DLL) generated via VS2013(Framework 3.5 and x86_64).
* Copy the StepVR.dll and StepVRCSharp.dll to Unity project's
* folder Assets to use it. For more details of the library,
* please contact G-Wearables Company.
*
* The StepVR SDK employs a right-handed Cartesian coordinate
* system. Values given are in units of real-world meters. The
* origin is centered at the center of the calibrated room. The
* x-axis lies in the direction from the calibrated O point to
* the calibrated A point. The z-axis lies in the direction from
* the calibrated O point to the calibrated B point. The y-axis
* is vertical, with positive values increasing upwards.
**/

#ifndef StepVR_h__
#define StepVR_h__

#ifdef STEPVR_EXPORTS
#define STEPVR_API __declspec(dllexport) 
#else
#define STEPVR_API __declspec(dllimport) 
#endif

namespace StepVR {
    class Manager;
    class Frame;
    class SingleNode;


    /**
    * Enumerates the names of Engine.
    *
    * @since 1.0
    **/
    STEPVR_API enum Engine
    {
        Engine_Unreal = 1,
        Engine_Unity = 2,
    };

    /**
    * The Vector3f struct represents a three-component
    * mathematical vector or point such as position or rotation
    * in three-dimensional space.
    *
    * @since 1.0
    */
    STEPVR_API struct Vector3f
    {
        float x;
        float y;
        float z;
        Vector3f(){};
        /**
        * Creates a new Vector with all components set to specified values.
        *
        * @since 1.0
        **/
        Vector3f(float xx, float yy, float zz) :
            x(xx), y(yy), z(zz) {};
    };

    /**
    * The Vector4f struct represents a four-component
    * mathematical vector, such as quaternion.
    *
    * @since 1.0
    */
    STEPVR_API struct Vector4f
    {
        float w;
        float x;
        float y;
        float z;

        /**
        * Creates a new Vector with all components set to specified values.
        *
        * @since 1.0
        **/
        Vector4f(float ww, float xx, float yy, float zz) :
            w(ww), x(xx), y(yy), z(zz) {};
    };


    /**
    * The SingleNode class represents a set of position and rotation
    * tracking data detected in a single frame.
    *
    * The StepVR System detects all the single-nodes within the
    * tracking area, reporting head, back, left foot, right foot, left
    * hand, and right hand positions and rotations.
    *
    * Access SingleNode objects through the instance of the Frame
    * class. SingleNode instance created by the constructor is invalid.
    *
    * @since 1.0
    **/
    STEPVR_API class SingleNode
    {
    public:
        /**
        * Enumerates the names of node ID.
        *
        * @since 1.0
        **/
        STEPVR_API enum NodeID
        {
            NodeID_Weapon = 4,
            NodeID_Head = 6,
        };

        /**
        * Enumerates the names of key ID.
        *
        * @since 1.0
        **/
        STEPVR_API enum KeyID
        {
            KeyA = 1,
            KeyB,
            KeyC,
            KeyD,
            MAX_KEY
        };


        /**
        * Constructs a PositionFrame object.
        *
        * Frame instances created with this constructor are invalid. Get
        * valid Frame objects by calling the Manager::GetPositionFrame()
        * function.
        *
        * @since 1.0
        **/
        STEPVR_API SingleNode(float* data);
        STEPVR_API ~SingleNode();

        /**
        * The position of a specified node in the calibrated room.
        *
        * @param NodeID A specified NodeID.
        * @returns Vector3f The position of a specified
        *                   node.
        * @since 1.0
        **/
        STEPVR_API Vector3f GetPosition(NodeID id);

        /**
        * The rotation of a specified node in the calibrated room.
        *
        * @param NodeID A specified NodeID.
        * @param Engine A specified Engine you use.
        * @returns Vector3f The rotation of a specified
        *                   node.
        * @since 1.0
        **/
        STEPVR_API Vector3f GetRotation(NodeID id, Engine engine);

        /**
        * The quaternion of a specified node in the calibrated room.
        *
        * @param NodeID A specified NodeID.
        * @returns Vector4f The quaternion of a specified node.

        * @since 1.0
        **/
        STEPVR_API Vector4f GetQuaternion(NodeID id);

        /**
        * The IsDown() function detect button down state.
        *
        * @params KeyID A specified key on the weapon. For version 1,
        *               There is only one button, which is KeyA.
        * @return true, the button is down; false, the button is up.
        **/
        STEPVR_API bool GetKeyDown(NodeID _nodeid, KeyID _keyid);

        /**
        * The IsUp() function detect button up state.
        *
        * @params KeyID A specified key on the weapon. For version 1,
        *               There is only one button, which is KeyA.
        * @return true, the button is down; false, the button is up.
        **/
        STEPVR_API bool GetKeyUp(NodeID _nodeid, KeyID _keyid);

        /**
        * The GetKey() function detect once button down and up state.
        *
        * @params KeyID A specified key on the weapon. For version 1,
        *               There is only one button, which is KeyA.
        * @return true, detect button down and up once.
        **/
        STEPVR_API bool GetKey(NodeID _nodeid, KeyID _keyid);

    };

    /**
    * The Frame class contains the all the detected data in a single
    * frame.
    *
    * Access Frame object through the instance of the Manager class.
    * A Frame includes body skeleton posture, which can be polled at
    * any time using GetWholeBody() function; single-node position and
    * rotation, which can be polled at any time using GetSingleNode()
    * function.
    *
    * @since 1.0
    **/
    STEPVR_API class Frame
    {
    public:

        /**
        * Constructs a Frame object.
        *
        * Frame instance created with this constructor is invalid.
        * Get valid Frame object by calling the Manager::GetFrame()
        * function.
        *
        * @since 1.0
        **/
        STEPVR_API Frame(float* data);
        STEPVR_API ~Frame();

        /**
        * Returns the most recent frame of each single-node data.
        *
        * @returns PositionFrame The specified PositionFrame.
        * @since 1.0
        **/
        STEPVR_API SingleNode GetSingleNode();

    };



    /**
    * The Manager class is the main interface to the StepVR SDK.
    *
    * Create an instance of this Manager class to access frames of
    * tracking data. Frame data includes body skeleton posture,
    * single-node position and rotation, and key state.
    *
    * When an instance of Manager is created, call Start() function
    * to prepare and start all devices and StepVR SDK; and call Stop()
    * function to stop all devices and software. To start receive
    * correct whole-body, put your body T-Pose and call CalibrateMocap()
    * function. No need to calibrate, if you just use single-node data.
    *
    * Call GetFrame() function to get the most recent frame of
    * tracking data.
    *
    * Call GetDevConfig() function to get current sensor state.
    *
    * @since 1.0
    **/
    STEPVR_API class Manager{
    public:
        /**
        * Constructs a Manager object.
        *
        * @since 1.0
        **/
        STEPVR_API Manager();
        STEPVR_API ~Manager();

        /**
        * When you create a manager object, Call Start() function to
        * start to capture frames of tracking data.
        *
        * @since 1.0
        **/
        //************************************
        // Method:    Start
        // FullName:  StepVR::Manager::Start
        // Access:    public 
        // Returns:   STEPVR_API int.
        //            0 is ok, 
        //            1 is load matrix fail, 
        //            2 is open port fail, 
        //            3 is start thread fail.
        // Qualifier:
        // Parameter: char * transmatfile
        //************************************
        STEPVR_API int Start(char* transmatfile = "TransMat.txt");

        /**
        * Call Stop() function to stop capturing frames of tracking data.
        *
        * @since 1.0
        **/
        STEPVR_API bool Stop();

        /**
        * Returns the most recent frame of skeleton posture data.
        *
        * @returns The specified MocapFrame.
        * @since 1.0
        **/
        STEPVR_API Frame GetFrame();
    };

    STEPVR_API enum EulerOrd{
        EulerOrder_YXZ,
        EulerOrder_ZYX,
        EulerOrder_XZY,
        EulerOrder_XYZ,
        EulerOrder_YZX,
        EulerOrder_ZXY
    };

    STEPVR_API class StepVR_EnginAdaptor
    {
    public:
        STEPVR_API static Vector4f toUserQuat(const Vector4f &p);
        STEPVR_API static Vector3f toUserEuler(const Vector4f &p);
        STEPVR_API static Vector3f toUserPosition(const Vector3f &p);

        STEPVR_API static void MapCoordinate(Vector3f UserX, Vector3f UserY, Vector3f UserZ);
        //Yaw-Pitch-Roll Order
        STEPVR_API static void setEulerOrder(EulerOrd order);

    private:
        StepVR_EnginAdaptor(){};
    };

}

#endif // StepVR_h__
