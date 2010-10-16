#include "CKAll.h"
#include "BB_InitHaptic.h"
#include "ManagerExample.h"
#include <time.h>

CKBehavior* beh;

CKObjectDeclaration	*FillBehaviorBB_InitHapticDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration(BB_NAME);
	
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(BB_VERSION);
	od->SetCreationFunction(CreateBB_InitHapticPrototype);
	od->SetDescription(BB_COMMENT);
	od->SetCategory(BB_CATEGORY);
	od->SetGuid(CKGUID(BB_UID1,BB_UID2));
	od->SetAuthorGuid(CKGUID(AUTHOR_UID1,AUTHOR_UID2));
	od->SetAuthorName(AUTHOR_NAME);
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateBB_InitHapticPrototype(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype(BB_NAME);
	if (!proto) {
		return CKERR_OUTOFMEMORY;
	}
	
	proto->DeclareInput("In"); // input 1

	//proto->DeclareOutput("nom");
	proto->DeclareOutput("Out"); // output 1
	proto->DeclareOutput("OutERROR"); // output 2

	//proto->DeclareInParameter("nom", CKPGUID_LAYERTYPE, default_value );
	proto->DeclareInParameter("doExit"	, CKPGUID_BOOL  ); //input 0
	proto->DeclareInParameter("position", CKPGUID_VECTOR); //input 1
	proto->DeclareInParameter("force"	, CKPGUID_VECTOR); //input 2
	proto->DeclareInParameter("Mode"	, CKPGUID_INT	); //input 3
	proto->DeclareInParameter("Speed"	, CKPGUID_FLOAT ); //input 4

	//proto->DeclareInParameter("nom", CKPGUID_LAYERTYPE, default_value );
	proto->DeclareOutParameter("button1", CKPGUID_BOOL);// output 0
	proto->DeclareOutParameter("button2", CKPGUID_BOOL);// output 1
	proto->DeclareOutParameter("button3", CKPGUID_BOOL);// output 2
	proto->DeclareOutParameter("position", CKPGUID_VECTOR);// output 3

	//----	Settings Declaration
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetFunction(BB_InitHaptic);

	*pproto = proto;
	return CK_OK;
}

int BB_InitHaptic(const CKBehaviorContext& behcontext)
{
	beh = behcontext.Behavior;

	// Get the manager associated to this building block
	ManagerExample* bm = (ManagerExample*)behcontext.Context->GetManagerByGuid(CKGUID(MANAGER_UID1,MANAGER_UID2));

		bm->setArmUse(true);

		//recuperation
		beh->GetInputParameterValue(0,&bm->hapticParameters.g_doExit	 );
		beh->GetInputParameterValue(1,&bm->hapticParameters.g_position_in);
		beh->GetInputParameterValue(2,&bm->hapticParameters.g_force		 );
		beh->GetInputParameterValue(3,&bm->hapticParameters.g_selecMode	 );
		beh->GetInputParameterValue(4,&bm->hapticParameters.g_speed		 );
		
		CKBOOL b1 = bm->getg_button1() ? TRUE : FALSE;
		CKBOOL b2 = bm->getg_button2() ? TRUE : FALSE;
		CKBOOL b3 = bm->getg_button3() ? TRUE : FALSE;

		beh->SetOutputParameterValue(0, &b1, sizeof(CKBOOL));
		beh->SetOutputParameterValue(1, &b2, sizeof(CKBOOL));
		beh->SetOutputParameterValue(2, &b3, sizeof(CKBOOL));
		beh->SetOutputParameterValue(3, &(bm->getg_position_out()), sizeof(VxVector));


	if(!bm->hapticParameters.g_doExit){
		
		beh->ActivateOutput(0); // activate OUT
	}
	else{//ERROR
		bm->ExitArmUse();
		beh->ActivateOutput(1); // activate OUTERROR
	}

	return CKBR_OK;
}

