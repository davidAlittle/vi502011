#include "ManagerExample.h"

ManagerExample::ManagerExample(CKContext *Context) : 
CKBaseManager(Context, CKGUID(MANAGER_UID1,MANAGER_UID2), MANAGER_NAME)
{
	Context->RegisterNewManager(this);
	this->context = Context;
}

ManagerExample::~ManagerExample()
{
	
}

CKDWORD	ManagerExample::GetValidFunctionsMask()
{
	return CKMANAGER_FUNC_OnCKInit | CKMANAGER_FUNC_OnCKEnd;
}	

CKERROR ManagerExample::OnCKInit()
{

	hapticParameters.ghHD = HD_INVALID_HANDLE;
	hapticParameters.gSchedulerCallback = HD_INVALID_HANDLE;
	int returnValue = 0;

	hapticParameters.g_doExit = false;

	//Initialize Haptic device

	HDErrorInfo error;
    
    // Initialize the device.  This needs to be called before any actions on the
    // device.
    hapticParameters.ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		returnValue = -1;
		return returnValue;
	}
    
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_MAX_FORCE_CLAMPING);

	// Initialize amplitude for move vribration
	hdGetDoublev(HD_NOMINAL_MAX_CONTINUOUS_FORCE, &hapticParameters.gVibrationAmplitude);
	hapticParameters.gVibrationAmplitude *= 0.75;
    
	hapticParameters.gSchedulerCallback = hdScheduleAsynchronous(touchScene, this,HD_MAX_SCHEDULER_PRIORITY);

    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = -2;

	context->OutputToConsole("INIT OK");
	return CK_OK;
}

CKERROR ManagerExample::OnCKEnd(){

	ExitHandler();

	return CK_OK;

}


const bool& ManagerExample::getg_button1() 
{
	return hapticParameters.g_button1;
}

const bool& ManagerExample::getg_button2() 
{
	return hapticParameters.g_button2;
}

const bool& ManagerExample::getg_button3() 
{
	return hapticParameters.g_button3;
}

const VxVector& ManagerExample::getg_position_out()
{
	return hapticParameters.g_position_out;
}


void ManagerExample::ExitArmUse(){
	ArmUse = false;
}


void ManagerExample::ExitHandler(){

	hdStopScheduler();
	hdUnschedule(hapticParameters.gSchedulerCallback);
	hapticParameters.g_doExit = false;

}

hduVector3Dd shakeBaby(ManagerExample * bm){

		static const hduVector3Dd direction(0, 1, 0);
		hduVector3Dd force; //current force applied to the arm 
		HDdouble instRate;
		static HDdouble timer = 0;

		bm->hapticParameters.gVibrationFreq = (HDint) bm->hapticParameters.g_speed * 10;
		bm->hapticParameters.gVibrationFreq = bm->hapticParameters.gVibrationFreq < 0 ? 0	  : bm->hapticParameters.gVibrationFreq; 
		bm->hapticParameters.gVibrationFreq = bm->hapticParameters.gVibrationFreq > 500 ? 500 : bm->hapticParameters.gVibrationFreq; 

		/* Use the reciprocal of the instantaneous rate as a timer. */
		hdGetDoublev(HD_INSTANTANEOUS_UPDATE_RATE, &instRate);
		timer += 1.0 / instRate;

		/* Apply a sinusoidal force in the direction of motion. */
		hduVecScale(force, direction, bm->hapticParameters.gVibrationAmplitude * 
						sin(timer * bm->hapticParameters.gVibrationFreq));
	    
		return force;
}

HDCallbackCode HDCALLBACK touchScene(void *pUserData){

	ManagerExample * bm = (ManagerExample *)pUserData;

	static const HDdouble stiffness = 0.05;

	hduVector3Dd position;
	hduVector3Dd initialPosition(0, 0, 0);

	hduVector3Dd force( (double) bm->hapticParameters.g_force.x, 
						(double) bm->hapticParameters.g_force.y, 
						(double) bm->hapticParameters.g_force.z);
	int button = 0;

	// Get Haptic Arm State
	hdGetIntegerv(HD_CURRENT_BUTTONS,&button);

	bm->hapticParameters.g_button1 = ((button & HD_DEVICE_BUTTON_1)==HD_DEVICE_BUTTON_1);
	bm->hapticParameters.g_button2 = ((button & HD_DEVICE_BUTTON_2)==HD_DEVICE_BUTTON_2);
	bm->hapticParameters.g_button3 = ((button & HD_DEVICE_BUTTON_3)==HD_DEVICE_BUTTON_3);

	if(bm->IsArmUse()){

		hdBeginFrame(bm->hapticParameters.ghHD);

		hdGetDoublev(HD_CURRENT_POSITION, position);

		switch(bm->hapticParameters.g_selecMode){
			case MOVE:
				{
					hduVector3Dd tempForce = shakeBaby(bm);

					hduVecSubtract(force, initialPosition, position);
					hduVecScaleInPlace(force, stiffness);

					force += tempForce;

					hdSetDoublev(HD_CURRENT_FORCE, force);

					bm->hapticParameters.g_position_out.v[0] =  (float)position[0];
					bm->hapticParameters.g_position_out.v[1] =  0.f;
					bm->hapticParameters.g_position_out.v[2] = -(float)position[2];

					break;
				}
			case CAM:
				{ 
					hduVecSubtract(force, initialPosition, position);
					hduVecScaleInPlace(force, stiffness);
					hdSetDoublev(HD_CURRENT_FORCE, force);

					bm->hapticParameters.g_position_out.v[0] = (float)position[0];
					bm->hapticParameters.g_position_out.v[1] = (float)position[1];
					bm->hapticParameters.g_position_out.v[2] = 0.f;

					break;
				}
			case ARM :
				{
					HDdouble pos_in[3]; 
						pos_in[0] = bm->hapticParameters.g_position_in.x;
						pos_in[1] = bm->hapticParameters.g_position_in.y;
						pos_in[2] = bm->hapticParameters.g_position_in.z;
					hduVecSubtract(force, pos_in, position);

					bm->hapticParameters.g_position_out.v[0] =  (float)position[0];
					bm->hapticParameters.g_position_out.v[1] =  (float)position[1];
					bm->hapticParameters.g_position_out.v[2] = -(float)position[2];
					
					hduVecScaleInPlace(force, stiffness);
					hdSetDoublev(HD_CURRENT_FORCE, force);
					break;
				}
		}

		hdEndFrame(bm->hapticParameters.ghHD);

	}

	return HD_CALLBACK_CONTINUE;
}