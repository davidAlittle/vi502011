static var score = 0;
var curseur : Texture2D;

function Start(){
	Screen.showCursor = false;
	
}

function OnGUI () {
	/* définition de la box de score */
	GUI.Box(Rect(10, 10, 100, 90), "jenga");
	
	if(GUI.Button(Rect(20, 40, 80, 20), "Reset")){
		Application.LoadLevel(0);
	}
	
	GUI.Label(Rect(20, 70, 80, 20), "Score : " + score);
	
	if(Input.GetButton("Jump")){
		score = 0;
		Application.LoadLevel(0);
	}
	
	GUI.Label(Rect(Input.mousePosition.x - 20, Input.mousePosition.x + 20, 80, 80), "Sasdsdsf");

	// définition du curseur
	GUI.DrawTexture(new Rect(Input.mousePosition.x, Screen.height- Input.mousePosition.y, 32, 32), curseur); 

} 