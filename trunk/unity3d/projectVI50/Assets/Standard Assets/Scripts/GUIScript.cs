using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class GUIScript : MonoBehaviour {
	public static int score;
	public static Vector3 positionCurseur;
	public Texture2D curseur;
	
	
	[DllImport("dllVI50")]
	public static extern bool HDIsACtivate(); 

	[DllImport("dllVI50")]
	public static extern double getX(); 

	[DllImport("dllVI50")]
	public static extern double getY(); 	
	
	void Start(){
		Screen.showCursor = false;
	}

	void OnGUI () {
		/* définition de la box de score */
		GUI.Box(new Rect(10, 10, 100, 90), "jenga");
		
		if(GUI.Button(new Rect(20, 40, 80, 20), "Reset")){
			Application.LoadLevel(0);
		}
		
		GUI.Label(new Rect(20, 70, 80, 20), "Score : " + score);
		
		if(Input.GetButton("Jump")){
			score = 0;
			Application.LoadLevel(0);
		}
		
		// définition du curseur
		print((float)(getX()/450.0*Screen.width+225) + " / " + (float)(Screen.height- getY()/276*Screen.height+138) + "  screen : " + Screen.width+"x" + Screen.height);
		
		positionCurseur = new Vector3((float)(getX()/250.0*Screen.width+225), (float)(Screen.height- getY()/220*Screen.height+110),0);
		GUI.DrawTexture(new Rect(positionCurseur.x, positionCurseur.y, 32, 32), curseur); 

	} 
}
