#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

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
	vec3 g_force;
	vec3 g_position_in;
	Mode g_selectMode;
	
	hduVector3Dd force_active;
	hduVector3Dd standard_force(10, 10, 10);

	bool g_doExit;
	// variable de définition de la désactivation

	// Output variables
	bool g_button1; //BLACK
	bool g_button2; //WHITE
	bool g_button3; //INK
	vec3 g_position_out;

static const hduVector3Dd v_initial_position(0, 0, 0);

static void init(){
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


static void setForce(hduVector3Dd f){
	force_active = f;
	// todo : ajouter une force active maximale et minimale.
}

static void addForce(hduVector3Dd f){
	force_active +=f;
	// todo : ajouter une force active maximale et minimale.
}

static hduVector3Dd getForce(){
	return force_active;
}

// les réalisation des manipulations
// retourne les valeurs possible HD_CALLBACK_DONE ou HD_CALLBACK_CONTINUE
HDCallbackCode HDCALLBACK startManipulationCallBack(void *pUserData){
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

				//hduVecSubtract(force, v_initial_position, v_position);
				//hduVecScaleInPlace(force, stiffness);
				
				//force = 
				//hdSetDoublev(HD_CURRENT_FORCE, force);
				// définition de la force de retour à donner au bras

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
				hdSetDoublev(HD_CURRENT_FORCE, standard_force);

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