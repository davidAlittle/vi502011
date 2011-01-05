using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class DragRigidbodyWithHDLevel1 : MonoBehaviour {

	[DllImport("dllVI50")]
	public static extern bool HDIsACtivate();	
	
	[DllImport("dllVI50")]
	public static extern double getX();	
	
	[DllImport("dllVI50")]
	public static extern double getY();	
	
	[DllImport("dllVI50")]
	public static extern double getZ();	
	
	[DllImport("dllVI50")]
	public static extern double setForceOnAxis(double x, double y, double z);	
	
	[DllImport("dllVI50")]
		public static extern bool isButton1Activate();
	
	
    public float spring = 50.0f;
    public float damper = 5.0f;
    public float drag = 10.0f;
    public float angularDrag = 5.0f;
    public float distance = 0.2f;
    public bool attachToCenterOfMass = false;
   
    private SpringJoint springJoint;
	private Vector3 positionLast = new Vector3(0,0,0);
	private Vector3 positionActual = new Vector3(0,0,0);
	private Vector3 forceHD = new Vector3(0, 0, 0);
	private Vector3 vInertiaTensor;
	private Vector3 diffposition = new Vector3(0, 0, 0);
	
	private bool isPick = false;
	private bool HaveBeenContited = false;
    // Use this for initialization
    void Start () {
   
    }
   
    // Update is called once per frame
    void Update () {
		
	//Update score
	var g0 = new Vector3();
	g0 = transform.position;
	if(g0.y<10){
		if(isPick){
			if(!HaveBeenContited){
				HaveBeenContited = true;
				GUIScriptLevel1.score++;
			}
		}else{
			if(!HaveBeenContited){
				HaveBeenContited = true;
				GUIScriptLevel1.score--;
			}
		}
		Destroy(gameObject, 5);
		//Destroy(this);
		
	}
        // Make sure the user pressed the mouse down
        if (!isButton1Activate())
            return;

        Camera mainCamera = FindCamera();
       
        // We need to actually hit an object
        RaycastHit hit;
        if (!Physics.Raycast(mainCamera.ScreenPointToRay(GUIScriptLevel1.positionCurseur), out hit, 100))
            return;
        // We need to hit a rigidbody that is not kinematic
        if (!hit.rigidbody || hit.rigidbody.isKinematic)
            return;
       
       if(hit.rigidbody == rigidbody){
		audio.Play();
		isPick = true;
	}
       
        if (!springJoint)
        {
            GameObject go = new GameObject("Rigidbody dragger");
           
            Rigidbody body = go.AddComponent ("Rigidbody") as Rigidbody;
            springJoint = (SpringJoint)go.AddComponent ("SpringJoint");
            body.isKinematic = true;
        }
       
        springJoint.transform.position = hit.point;
        if (attachToCenterOfMass)
        {
            Vector3 anchor = transform.TransformDirection(hit.rigidbody.centerOfMass) + hit.rigidbody.transform.position;
            anchor = springJoint.transform.InverseTransformPoint(anchor);
            springJoint.anchor = anchor;
        }
        else
        {
            springJoint.anchor = Vector3.zero;
        }
       
        springJoint.spring = spring;
        springJoint.damper = damper;
        springJoint.maxDistance = distance;
        springJoint.connectedBody = hit.rigidbody;
       
        StartCoroutine ("DragObject", hit.distance);
    }

    IEnumerator DragObject (float distance)
    {
        float oldDrag = springJoint.connectedBody.drag;
        float oldAngularDrag = springJoint.connectedBody.angularDrag;
        springJoint.connectedBody.drag = drag;
        springJoint.connectedBody.angularDrag = angularDrag;
        Camera mainCamera = FindCamera();
       
		positionActual = new Vector3((float)getX(), (float)getY(), (float)getZ());
        while (isButton1Activate())
        {
			positionLast = positionActual;
			positionActual.x = (float)getX();
			positionActual.y = (float)getY();
			positionActual.z = (float)getZ();
			
			//positionActual = transform.forward;
			
            // ajout de la force au bras haptique
			diffposition = (positionActual - positionLast);
			vInertiaTensor = rigidbody.inertiaTensor;
			if (vInertiaTensor.x>0)
				if(diffposition.x<0)
					forceHD.x = 0.2f;
				else
					forceHD.x = -0.2f;
				
			if (vInertiaTensor.y>0)
				if(diffposition.y<0)
					forceHD.y = 0.2f;
				else
					forceHD.y = -0.2f;
				
			if (vInertiaTensor.z>0)
				if(diffposition.z<0)
					forceHD.z = 0.2f;
				else
					forceHD.z = -0.2f;
			
			// ajout des forces de retour
			setForceOnAxis((double)forceHD.x, (double)forceHD.y, (double)forceHD.z);
		

			Vector3 positionCurseur = new Vector3((float)((getX() + 225)*(Screen.width/320)), (float)(((getY() + 80)*(Screen.height/190))), 0);		
			Ray ray = mainCamera.ScreenPointToRay(positionCurseur);
			distance -= diffposition.z*50;
			springJoint.transform.position = ray.GetPoint(distance);
			
			//isPick = true;
            yield return null;
            //new yield;
        }
		
		// on réinitialise les forces
		setForceOnAxis(0, 0, 0);
        if (springJoint.connectedBody)
        {
            springJoint.connectedBody.drag = oldDrag;
            springJoint.connectedBody.angularDrag = oldAngularDrag;
            springJoint.connectedBody = null;
        }
    }
	
    Camera FindCamera ()
    {
        if (camera)
            return camera;
        else
            return Camera.main;
    }
}
