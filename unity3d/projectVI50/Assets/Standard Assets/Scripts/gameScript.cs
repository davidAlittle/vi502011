using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
//private var motor : CharacterMotor;
//private var motor : CharacterMotor;

public class gameScript : MonoBehaviour {
	
//	var target : Transform;
	[DllImport("dllVI50")]
	public static extern int init();
	
	[DllImport("dllVI50")]
	public static extern double getX();
	
	[DllImport("dllVI50")]
	public static extern double getY();
	
	[DllImport("dllVI50")]
	public static extern double getZ();
	
	[DllImport("dllVI50")]
	public static extern bool isButton1Activate(); 

	[DllImport("dllVI50")]
	public static extern bool isButton2Activate(); 
	
	[DllImport("dllVI50")]
	public static extern bool exitHD(); 
	
	[DllImport("dllVI50")]
	public static extern double getRelativeXForCamera();
	
	[DllImport("dllVI50")]
	public static extern double getRelativeYForCamera();
	
	[DllImport("dllVI50")]
	public static extern double getRelativeZForCamera();
	
	[DllImport("dllVI50")]
	public static extern bool setForceOnAxisX(double a, double b, double c); 
	
	void Start() {
		int know;
		know = init();
	}
	
	// Update is called once per frame
	void Update () {
		//print(getX() + " / " + getY() +  " / " + getZ());
		/*
		double x = getRelativeXForCamera();
		double y = getRelativeYForCamera();
		double z = getRelativeZForCamera();
		
		transform.localEulerAngles = new Vector3((float)-y,(float)x,0);
		/*
		if(Mathf.Abs((float)x)<5)
			x = 0;
		
		if(Mathf.Abs((float)z)<5)
			z = 0;
		//transform.Translate(0, (float)y/30, 0);
		
		//transform.Rotate((float)z/20, (float)x/20, 0);
		transform.Rotate(0, (float)x/20, 0);
		transform.Rotate((float)z/20, 0, 0);
		*/
		if(Input.GetKeyDown ("escape")){
			exitHD(); 
			//Application.Quit();
		}
		
	}

}
