const float vertexPositions[] = {

	//############################################################################################################## POSITIONS

	//############################################################################# PLANE

	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,

	5.0f, -1.0f, -5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,

	//############################################################################# Walls
	-5.0f, -1.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, 5.0f, 1.0f,

	5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,

	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,

	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,
	5.0f, 5.0f, -5.0f, 1.0f,

	-5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,

	-5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,

	5.0f, 5.0f, -5.0f, 1.0f,
	5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, -5.0f, 1.0f,

	5.0f, -1.0f, -5.0f, 1.0f,
	5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,


	//################################################################################## ENEMY CUBE

	//Cube left face
	0.0f, 0.25f, 0.0f, 1.0f, 
	0.0f, -0.25f, 0.0f, 1.0f, 
	0.5f, -0.25f, 0.0f, 1.0f,

	0.5f, -0.25f, 0.0f, 1.0f, 
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.0f, 0.25f, 0.0f, 1.0f, 

	//Cube right face
	0.0f, 0.25f, 0.5f, 1.0f,
	0.5f, -0.25f, 0.5f, 1.0f, 
	0.0f, -0.25f, 0.5f, 1.0f, 

	0.5f, -0.25f, 0.5f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f,

	//Cube bottom face
	0.0f, -0.25f, 0.5f, 1.0f,
	0.5f, -0.25f, 0.0f, 1.0f,
	0.0f, -0.25f, 0.0f, 1.0f, 

	0.5f, -0.25f, 0.0f, 1.0f, 
	0.0f, -0.25f, 0.5f, 1.0f, 
	0.5f, -0.25f, 0.5f, 1.0f,

	//Cube top face
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 
	0.0f, 0.25f, 0.0f, 1.0f, 
	
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 

	//Cube front face
	0.0f, -0.25f, 0.5f, 1.0f, 
	0.0f, -0.25f, 0.0f, 1.0f,
	0.0f, 0.25f, 0.0f, 1.0f,

	0.0f, 0.25f, 0.0f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 
	0.0f, -0.25f, 0.5f, 1.0f, 

	//Cube back face
	0.5f, -0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.5f, -0.25f, 0.0f, 1.0f,

	0.5f, 0.25f, 0.0f, 1.0f,
	0.5f, -0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f, 


	//########################################################################### GUI
	-4.8f, 0.2f, -0.33f, 1.0f,
	-4.8f, 0.2f, 0.33f, 1.0f,
	-4.8f, 0.15f, 0.33f, 1.0f,

	-4.8f, 0.15f, -0.33f, 1.0f,
	-4.8f, 0.2f, -0.33f, 1.0f,
	-4.8f, 0.15f, 0.33f, 1.0f,

	//############################################################################################################## COLOURS

	//############################################################################################################## PLANE

	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,

	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,

	//Blue Wall
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	//Cyan Wall
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	//Pink wall
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,

	//Red Wall
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,


	//############################################################################################################## TEXTURES

	//PLANE
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	//WALLS
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	//CUBE
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	//HUD
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
};