using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class DragRigidbodyWithHD : MonoBehaviour {

	[DllImport("dllVI50")]
	public static extern bool HDIsACtivate();	
	
	[DllImport("dllVI50")]
	public static extern double getX();	
	
	[DllImport("dllVI50")]
	public static extern double getY();	
	
	[DllImport("dllVI50")]
	public static extern double getZ();	
	
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
	
    // Use this for initialization
    void Start () {
   
    }
   
    // Update is called once per frame
    void Update () {
        // Make sure the user pressed the mouse down
        if (!isButton1Activate())
            return;

        Camera mainCamera = FindCamera();
       
        // We need to actually hit an object
        RaycastHit hit;
        if (!Physics.Raycast(mainCamera.ScreenPointToRay(GUIScript.positionCurseur), out hit, 100))
            return;
        // We need to hit a rigidbody that is not kinematic
        if (!hit.rigidbody || hit.rigidbody.isKinematic)
            return;
       
        audio.Play();
       
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
			// TODO : faire l'orientation en x et z (changement de rpéère ==> repère caméra/monde)
			positionLast = positionActual;
			positionActual.x = (float)getX();
			positionActual.y = (float)getY();
			positionActual.z = (float)getZ();
			
            //Ray ray = mainCamera.ScreenPointToRay (Input.mousePosition);
			
            //springJoint.transform.position = ray.GetPoint(distance);
			
			springJoint.transform.position += 3*(positionActual - positionLast);
            yield return null;
            //new yield;
        }
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
