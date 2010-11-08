using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class NewBehaviourScript : MonoBehaviour {
	 [DllImport("dllPhatom")]
	
	private static extern void init();
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
