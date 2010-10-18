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
	Mode g_selecMode;

	bool g_doExit;
	// variable de définition de la désactivation

	// Output variables
	bool g_button1; //BLACK
	bool g_button2; //WHITE
	bool g_button3; //INK
	vec3 g_position_out;

static const hduVector3Dd initialPos(0, 0, 0);

static void init(){
	ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
	// initialisation du bras haptique
	g_selecMode = CAM;
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

//simulation loop
static void simStep(){

}

static void simLoop(){

}

static void setForce(){

}

// les réalisation des manipulations
// retourne les valeurs possible HD_CALLBACK_DONE ou HD_CALLBACK_CONTINUE
HDCallbackCode HDCALLBACK startManipulationCallBack(void *pUserData){
	hduVector3Dd v_position;
	// vector3d de type double	
	hduVector3Dd v_force((double) g_force.x, (double) g_force.y, (double) g_force.z);
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
			{
				break;
			}
		case CATCH:
			{
				break;
		    }


	/* fin de la manipulation */
	hdEndFrame(ghHD);

	return HD_CALLBACK_CONTINUE;
}


// boucle de manipulation ??
void startManipulation(){
    hdScheduleSynchronous(startManipulationCallBack, 0,
                          HD_DEFAULT_SCHEDULER_PRIORITY);
	// synchronization du scheduer avec les "actions"  ???
}