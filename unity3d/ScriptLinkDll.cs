using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour {
	[DllImport ("dllPhatom.dll")]
	
	private static extern void init();
	private static extern void exit();
	private static extern void changeMode();
	private static extern void startManipulation();
	
	// Use this for initialization
	void Start () {
		init();
		startManipulation();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
