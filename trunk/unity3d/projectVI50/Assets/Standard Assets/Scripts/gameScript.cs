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
	

	
	void Start() {
		int know;
		know = init();
		//Screen.showCursor = false;
		// on cache le curseur
	}
	
	// Update is called once per frame
	void Update () {
	
		if(Input.GetKeyDown ("escape")){
			exitHD(); 
			//Application.Quit();
		}
		
	}

}
