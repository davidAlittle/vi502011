using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
[AddComponentMenu("Camera-Control/OrbitHD")]

public class OrbitHD : MonoBehaviour {
	
	[DllImport("dllVI50")]
	public static extern bool HDIsACtivate();	
	
	[DllImport("dllVI50")]
	public static extern double getRelativeXForCamera();
	
	[DllImport("dllVI50")]
	public static extern double getRelativeYForCamera();
	
	[DllImport("dllVI50")]
	public static extern double getRelativeZForCamera();	
	
	[DllImport("dllVI50")]
	public static extern double getRelativeXForPointeur();
	
	[DllImport("dllVI50")]
	public static extern double getRelativeYForPointeur();

	[DllImport("dllVI50")]
	public static extern bool isButton2Activate(); 
	
	public Transform target;
	
	private float angleViewMax = 90.0f;
	private float angleViewMin = 30.0f;

    public float distance= 10.0f;
	
	private float distanceMin = 10.0f;
	private float distanceMax = 10.0f;

    public float xSpeed= 250.0f;
    public float ySpeed= 120.0f;

    public float yMinLimit= -20;
    public float yMaxLimit= 80;

    private float px= 0.0f;
    private float py= 0.0f;

    void Start (){
		/* initialisation pour les déplacements en mode rotation */
        Vector3 angles= transform.eulerAngles;
        this.px = angles.y;
        this.py = angles.x;

        // Make the rigid body not change rotation
        if (rigidbody)
            rigidbody.freezeRotation = true;
    }

    void LateUpdate (){
		// si le bras est désactivé on sort
		if(!HDIsACtivate())
			return;
	
		
		/* si on est en mode rotation */
        if (target&&isButton2Activate()) {			
			
			Camera.main.fieldOfView = Camera.main.fieldOfView + Camera.main.fieldOfView*(float)getRelativeZForCamera() * 0.002f;
			if(Camera.main.fieldOfView>angleViewMax )
				Camera.main.fieldOfView = angleViewMax;
			
			if(Camera.main.fieldOfView<angleViewMin )
				Camera.main.fieldOfView = angleViewMin;
			
			// on récupère la positon du bras
            px -= (float)getRelativeXForCamera() * 0.02f;
            py += (float)getRelativeYForCamera()  * 0.02f;
			
            py = ClampAngle(py, yMinLimit, yMaxLimit);
			
            Quaternion rotation = Quaternion.Euler(py, px, 0);
            Vector3 position = rotation * new Vector3(0.0f, 0.0f, -distance) + target.position;

            transform.rotation = rotation;
            transform.position = position;
        }
    }

    static float ClampAngle (float angle, float min, float max) {
        if (angle < -360)
            angle += 360;
        if (angle > 360)
            angle -= 360;
        return Mathf.Clamp (angle, min, max);
    }

}
