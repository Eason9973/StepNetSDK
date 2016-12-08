/**
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
	class WholeBody;
	class SingleNode;
	
	STEPVR_API enum BoneName{
		BONE_NON = 0,
		BONE_START,
		Bone_Sacrum,     
		Bone_Lumbar,     
		Bone_Thoracic,   
		Bone_Cervical,   
		Bone_Skull,
		Bone_LeftHand,
		Bone_LeftForearm,
		Bone_LeftUpperArm,
		Bone_LeftClavicle, 
		Bone_RightHand,
		Bone_RightForearm,
		Bone_RightUpperArm,
		Bone_RightClavicle,
		Bone_LeftPelvis,   
		Bone_LeftThigh,
		Bone_LeftShank,
		Bone_LeftFoot,
		Bone_RightPelvis,
		Bone_RightThigh,
		Bone_RightShank,
		Bone_RightFoot,
		BONE_END
	};

	STEPVR_API enum JointName{
		JOINT_NON = 0,
		JOINT_START,

		Joint_CoH,      
		Joint_S1,       
		Joint_L1,       
		Joint_T1,       
		Joint_C1,       
		Joint_Vertex,   

		Joint_LeftShoulder,
		Joint_LeftElbow,
		Joint_LeftWrist,
		Joint_LeftFingerTip,
		Joint_RightShoulder,
		Joint_RightElbow,
		Joint_RightWrist,
		Joint_RightFingerTip,

		Joint_LeftHip,
		Joint_LeftKnee,
		Joint_LeftAnkle,
		Joint_LeftToeTip,
		Joint_RightHip,
		Joint_RightKnee,
		Joint_RightAnkle,
		Joint_RightToeTip,
		JOINT_END
	};


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
	* The DevConfig struct represents the hardware information.
	*
	* It includes all sensors frequency, data updated frequency.
	* You could get the most recent hardware information by
	* calling Manager::GetDevConfig() function.
	* 
	* @since 1.0
	**/
	STEPVR_API struct DevConfig
	{
		int LeftWristMocapFrequency;
		int RightWristMocapFrequency;
		int LeftAnkleMocapFrequency;
		int RightAnkleMocapFrequency;
		int NeckMocapFrequency;
		int LeftUpperArmMocapFrequency;
		int RightUpperArmMocapFrequency;
		int HeadMocapFrequency;
		int WeaponMocapFrequency;

		int LeftWristPOTFrequency;
		int RightWristPOTFrequency;
		int LeftAnklePOTFrequency;
		int RightAnklePOTFrequency;
		int NeckPOTFrequency;
		int LeftUpperArmPOTFrequency;
		int RightUpperArmPOTFrequency;
		int HeadPOTFrequency;
		int WeaponPOTFrequency;

		int LeftWristBase1Frequency;
		int RightWristBase1Frequency;
		int LeftAnkleBase1Frequency;
		int RightAnkleBase1Frequency;
		int NeckBase1Frequency;
		int LeftUpperArmBase1Frequency;
		int RightUpperArmBase1Frequency;
		int HeadBase1Frequency;
		int WeaponBase1Frequency;

		int LeftWristBase2Frequency;
		int RightWristBase2Frequency;
		int LeftAnkleBase2Frequency;
		int RightAnkleBase2Frequency;
		int NeckBase2Frequency;
		int LeftUpperArmBase2Frequency;
		int RightUpperArmBase2Frequency;
		int HeadBase2Frequency;
		int WeaponBase2Frequency;

		int LeftWristBase3Frequency;
		int RightWristBase3Frequency;
		int LeftAnkleBase3Frequency;
		int RightAnkleBase3Frequency;
		int NeckBase3Frequency;
		int LeftUpperArmBase3Frequency;
		int RightUpperArmBase3Frequency;
		int HeadBase3Frequency;
		int WeaponBase3Frequency;

		int LeftWristBase4Frequency;
		int RightWristBase4Frequency;
		int LeftAnkleBase4Frequency;
		int RightAnkleBase4Frequency;
		int NeckBase4Frequency;
		int LeftUpperArmBase4Frequency;
		int RightUpperArmBase4Frequency;
		int HeadBase4Frequency;
		int WeaponBase4Frequency;

		int ParseFrequency;
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
	* The WholeBody class represents a set of motion capture tracking 
	* data detected in a single frame.
	*
	* The StepVR SDK detects the whole body within the tracking area, 
	* reporting the hip position and hip, right upper leg, right leg, 
	* right foot, left upper leg, left leg, left foot, spine0, spine1,
	* spine2, spine3, neck, head, right shoulder, right arm, right 
	* forearm, right hand, left shoulder, left arm, left forearm, 
	* left hand rotations.
	*
	* Access WholeBody objects through the instance of the Frame
	* class. WholeBody instance created by the constructor is invalid.
	*
	* @since 1.0
	**/
	STEPVR_API class WholeBody{
	public:

		/**
		* Enumerates the names of Skeleton nodes.
		*
		* @since 1.0
		**/
		STEPVR_API enum SkeletonID
		{
			SkeletonID_Hip = 1,
			SkeletonID_RightUpperLeg = 2,
			SkeletonID_RightLeg = 3,
			SkeletonID_RightFoot = 4,
			SkeletonID_LeftUpperLeg = 5,
			SkeletonID_LeftLeg = 6,
			SkeletonID_LeftFoot = 7,
			SkeletonID_Spine0 = 8,
			SkeletonID_Spine1 = 9,
			SkeletonID_Spine2 = 10,
			SkeletonID_Spine3 = 11,
			SkeletonID_Neck = 12,
			SkeletonID_Head = 13,
			SkeletonID_RightShoulder = 14,
			SkeletonID_RightArm = 15,
			SkeletonID_RightForearm = 16,
			SkeletonID_RightHand = 17,
			SkeletonID_LeftShoulder = 18,
			SkeletonID_LeftArm = 19,
			SkeletonID_LeftForearm = 20,
			SkeletonID_LeftHand = 21,
		};

		/**
		* Constructs a MocapFrame object.
		*
		* MocapFrame instances created with this constructor are 
		* invalid. Get valid MocapFrame objects by calling the 
		* Frame::GetMocapFrame() function.
		*
		* @since 1.0
		**/
		STEPVR_API WholeBody(float* data);
		STEPVR_API ~WholeBody();

		/**
		* The hip position relative to the origin of the calibrated
		* room.
		* 
		* @returns StepVR::Vector3f The rotation of a specified skeleton
		                            node.
		* @since 1.0
		**/
		STEPVR_API Vector3f GetHipPostion();

		/**
		* The rotation of a specified skeleton node relative to 
		* its parent node.
		*
		* @param SkeletonID A specified SkeletonID.
		* @param Engine A specified Engine you use.
		* @returns Vector3f The rotation of a specified
		*                   skeleton node.
		* @since 1.0
		**/
		STEPVR_API Vector3f GetRotation(SkeletonID id, Engine engine);
		
		/**
		* The quaternion of a specified skeleton node relative to
		* its parent node.
		*
		* @param id The SkeletonID of a single frame.
		* @returns Vector4f The quaternion of a specified
		*                   skeleton node.
		* @since 1.0
		**/
		STEPVR_API Vector4f GetQuaternion(SkeletonID id);
	
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
			//手柄左ID是0x81
			//手柄右ID是0x82
			//枪是0x04
			//头是0x06
			//动捕背部0x0A
			//动捕左手0x0B
			//动捕右手0x0C
			//动捕左脚0x0D
			//动捕右脚0x0E

			NodeID_LeftWrist = 1,
			NodeID_RightWrist = 2,
			NodeID_LeftAnkle = 3,
			NodeID_RightAnkle = 4,
			NodeID_Neck = 5,
			NodeID_LeftUpperArm = 6,
			NodeID_RightUpperArm = 7,
			NodeID_Head = 8,
			NodeID_Weapon = 9,
			NodeID_Back = 10,
			NodeID_McpLeftHand=11,
			NodeID_McpRightHand=12,
			NodeID_McpLeftFoot=13,
			NodeID_McpRightFoot=14
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
		* Returns the most recent frame of motion capture data.
		*
		* @returns MocapFrame The specified MocapFrame.
		* @since 1.0
		**/
		STEPVR_API WholeBody GetWholeBody();

		/**
		* Returns the most recent frame of each single-node data.
		*
		* @returns PositionFrame The specified PositionFrame.
		* @since 1.0
		**/
		STEPVR_API SingleNode GetSingleNode();

	};

	STEPVR_API struct Model{
		float LenSacrum;         
		float LenLumbar;         
		float LenThoracic;       
		float LenCervical;       
		float LenSkull;          
		float LenPelvis;         
		float LenThigh;          
		float LenShank;          
		float LenFoot;           
		float LenClavicle;       
		float LenUpperArm;       
		float LenForearm;        
		float LenHand;          
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
		* Call Calibrate() function to calibrate after starting 
		* capture data, in order to get correct tracking data. 
		*
		* @since 1.0
		**/
		STEPVR_API void CalibrateMocap();

		/**
		* Returns the most recent frame of skeleton posture data.
		*
		* @returns The specified MocapFrame.
		* @since 1.0
		**/
		STEPVR_API Frame GetFrame();

		/**
		* Returns the most recent hardware information.
		*
		* @returns The specified DeviceConfiguration.
		* @since 1.0
		**/
		STEPVR_API DevConfig GetDevConfig();

		/**
		* Record and replay raw data, which records from serial port.
		* The data will be recorded into a file called "RawData.txt", 
		* which locates in the software folder.
		*
		* @returns true, if success; false, if fail.
		* @since 1.0
		**/
		STEPVR_API bool StartRecRawData();
		STEPVR_API bool StopRecRawData();
		STEPVR_API bool StartReplayRawData();
		STEPVR_API bool StopReplayRawData();

		STEPVR_API bool BindBone(unsigned POTNodeID, BoneName boneName);
		STEPVR_API bool ResetBindBone();                                    
		STEPVR_API Model GetModelInfo();
		STEPVR_API void ScaleModel(float _xscale = 1.0f, float _yscale = 1.0f, float _zscale = 1.0f);
		
		STEPVR_API Vector3f GetRawP(int nodeid,int stationid);

		STEPVR_API Vector3f GetVel(int nodeid);

		STEPVR_API bool GetPortStatus();

		STEPVR_API bool SendVibrateCmd(int objType, int nodeId, unsigned short usDuration=10, unsigned short usFreq=0);
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