#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#include <tchar.h>

typedef struct{
	union{
		struct{
			float x,y,z;
		};
		float v[3];
	};
}vec3;

typedef enum{
	CATCH,
	CAM
}Mode;

/* définition de variable */
	HHD ghHD;
	// définition de la device bras haptique
	HDSchedulerHandle gSchedulerCallback;
	// définition du scheduler (pour la gestion des threads pour envoi de force au bras notamment)

	static HDint gVibrationFreq = 100; /* Hz */
	static HDdouble gVibrationAmplitude = 0; /* N */

	vec3 g_force;
	vec3 g_position_in;
	Mode g_selectMode;
	
	hduVector3Dd force_active(100.0, 100.0, 100.0);
	hduVector3Dd standard_force(100.0, 100.0, 100.0);

	bool g_doExit;
	// variable de définition de la désactivation

	// Output variables
	bool g_button1; //BLACK
	bool g_button2; //WHITE
	bool g_button3; //INK
	vec3 g_position_out;

static const hduVector3Dd v_initial_position(0, 0, 0);

static void init(){
	//DeviceSpace* OmniSpace = new DeviceSpace;
	

	ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
	// initialisation du bras haptique
	g_selectMode = CAM;
	/* initialisation des booléens */
	g_doExit = false;
}

static void stop(){

}

static void exit(){
 	hdStopScheduler();
	// arrêt du scheduler
	hdUnschedule(gSchedulerCallback);
	// suppression des informations à envoyer
}

// changement de mode.
static void changeMode(){
	if(g_selectMode == CAM){g_selectMode = CATCH;}
	else{g_selectMode = CAM;}
}


static void setForce(double x, double y, double z){
	force_active[0] = x;
	force_active[1] = y;
	force_active[2] = z;
		// todo : ajouter une force active maximale et minimale.
}

static void addForce(double x, double y, double z){
	force_active[0] +=x;
	force_active[1] +=y;
	force_active[2] +=z;
	// todo : ajouter une force active maximale et minimale.
}

static hduVector3Dd getForce(){
	return force_active;
}

// les réalisation des manipulations
// retourne les valeurs possible HD_CALLBACK_DONE ou HD_CALLBACK_CONTINUE
HDCallbackCode HDCALLBACK startManipulationCallBack(void *pUserData){  //cf touchBack
	hduVector3Dd v_position;
	// vector3d de type double	
	hduVector3Dd v_force;
	// définition du vecteur force que l'on souhaite associer
	int button;

	// récupération des valeurs des boutons du bras haptic
	hdGetIntegerv(HD_CURRENT_BUTTONS,&button);

	g_button1 = (button & HD_DEVICE_BUTTON_1);
	g_button2 = (button & HD_DEVICE_BUTTON_2);
	g_button3 = (button & HD_DEVICE_BUTTON_3);

	if(g_button2) g_doExit = true;
	// si activation du bouton sortie ==> sortie.

	/* début de la manipulation */
	hdBeginFrame(ghHD);

	hdGetDoublev(HD_CURRENT_POSITION, v_position);
	// récupération de la position du bras
	
	switch(g_selectMode){
		case CAM:
		// si on est dans le mode camera
			{

				g_position_out.v[0] = (float)v_position[0];
				g_position_out.v[1] = (float)v_position[1];
				g_position_out.v[2] = (float)v_position[2];


				break;
			}
		case CATCH:
		// si on est dans le mode "attraper un bloc"
			{

				/* déplacement de la main */
				g_position_out.v[0] = (float)v_position[0];
				g_position_out.v[1] = (float)v_position[1];
				g_position_out.v[2] = (float)v_position[2];
				
				/* ajout de la force de retour*/
				hdSetDoublev(HD_CURRENT_FORCE, force_active);

				break;
		    }
	}


	/* fin de la manipulation */
	hdEndFrame(ghHD);

	return HD_CALLBACK_CONTINUE;
}


// boucle de manipulation ??
void startManipulation(){

	ghHD = HD_INVALID_HANDLE;
	gSchedulerCallback = HD_INVALID_HANDLE;
	bool returnValue = true;
	int devideID = 0;

	HDErrorInfo error;
    // Initialize the device.  This needs to be called before any actions on the
    // device.
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;
    
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_MAX_FORCE_CLAMPING);

	// initialize amplitude for vibration
	// TODO 

	// mise en attente du device
    hdScheduleSynchronous(startManipulationCallBack, 0,
                          HD_DEFAULT_SCHEDULER_PRIORITY);
	// synchronization du scheduer avec les "actions"  ???


    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;

	atexit(exit);

}

static void changeAleaForce(){
	static HDdouble timer = 0;
	static const hduVector3Dd direction(0, 1, 0);
	HDdouble instRate;
	hduVector3Dd force;
	hdGetDoublev(HD_INSTANTANEOUS_UPDATE_RATE, &instRate);
	timer += 1.0 / instRate;

	hduVecScale(force, direction, 300 * sin(timer));
}

static float getPositionX(){
	return  (float) g_position_out.v[0];
}

static float getPositionY(){
	return (float) g_position_out.v[1];
}

static float getPositionZ(){
	return (float) g_position_out.v[2];
}
 
 int _tmain(int argc, _TCHAR* argv[])
{
	
	ghHD = HD_INVALID_HANDLE;
	gSchedulerCallback = HD_INVALID_HANDLE;
	bool returnValue = true;
	int devideID = 0;

	g_doExit = false;

	//Initialize Haptic device

	HDErrorInfo error;
    
    // Initialize the device.  This needs to be called before any actions on the
    // device.
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;
    
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_MAX_FORCE_CLAMPING);

	// Initialize amplitude for move vribration
	hdGetDoublev(HD_NOMINAL_MAX_CONTINUOUS_FORCE, &gVibrationAmplitude);
	gVibrationAmplitude *= 0.75;
    
	gSchedulerCallback = hdScheduleAsynchronous(startManipulationCallBack, 0, 
		HD_MAX_SCHEDULER_PRIORITY);

    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;

	atexit(exit);

	while(!g_doExit){
		//printf("%i %i %i\n",g_button1,g_button2,g_button3);
	}

	return 0;
}