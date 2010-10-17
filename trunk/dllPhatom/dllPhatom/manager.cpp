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
	MOVE,
	CAM
}Mode;

/* d�finition de varialbe */
// d�finition du scheduler (pour la gestion des threads pour envoi de force au bras notamment)
HDSchedulerHandle gSchedulerCallback;

static void start(){

}

static void stop(){

}

static void exit(){
 	hdStopScheduler();
	// arr�t du scheduler
	hdUnschedule(gSchedulerCallback);
	// suppression des informations � envoyer
}

//simulation loop
static void simStep(){

}

static void simLoop(){

}

static void setForce(){

}

// les actions � r�aliser
void startManipulationCallBack(void *pUserData){

}


// boucle de manipulation ??
void startManipulation(){
    hdScheduleSynchronous(startManipulationCallback, 0,
                          HD_DEFAULT_SCHEDULER_PRIORITY);
	// synchronization du scheduer avec les "actions"  ???
}