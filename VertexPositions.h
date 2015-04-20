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

	//Cube front face
	0.0f, 0.25f, 0.0f, 1.0f, 
	-0.0f, -0.25f, 0.0f, 1.0f, 
	0.5f, -0.25f, 0.0f, 1.0f,

	0.5f, -0.25f, 0.0f, 1.0f, 
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.0f, 0.25f, 0.0f, 1.0f, 

	//Cube back face
	0.0f, 0.25f, 0.5f, 1.0f,
	0.5f, -0.25f, 0.5f, 1.0f, 
	-0.0f, -0.25f, 0.5f, 1.0f, 

	0.5f, -0.25f, 0.5f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f,

	//Cube bottom face
	-0.0f, -0.25f, 0.5f, 1.0f,
	0.5f, -0.25f, 0.0f, 1.0f,
	-0.0f, -0.25f, 0.0f, 1.0f, 

	0.5f, -0.25f, 0.0f, 1.0f, 
	-0.0f, -0.25f, 0.5f, 1.0f, 
	0.5f, -0.25f, 0.5f, 1.0f,

	//Cube top face
	-0.0f, 0.25f, 0.5f, 1.0f, 
	-0.0f, 0.25f, 0.0f, 1.0f, 
	0.5f, 0.25f, 0.0f, 1.0f, 

	0.5f, 0.25f, 0.0f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f, 
	-0.0f, 0.25f, 0.5f, 1.0f, 

	//Cube left face
	-0.0f, -0.25f, 0.5f, 1.0f, 
	-0.0f, -0.25f, 0.0f, 1.0f,
	0.0f, 0.25f, 0.0f, 1.0f,

	0.0f, 0.25f, 0.0f, 1.0f, 
	0.0f, 0.25f, 0.5f, 1.0f, 
	-0.0f, -0.25f, 0.5f, 1.0f, 

	//Cube right face
	0.5f, -0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.0f, 1.0f, 
	0.5f, -0.25f, 0.0f, 1.0f,

	0.5f, 0.25f, 0.0f, 1.0f,
	0.5f, -0.25f, 0.5f, 1.0f, 
	0.5f, 0.25f, 0.5f, 1.0f, 


	//########################################################################### GUI
	-4.0f, 0.5f, -0.4f, 1.0f,
	-4.0f, 0.5f, 0.0f, 1.0f,
	-4.0f, 0.3f, 0.0f, 1.0f,
	
	-4.0f, 0.3f, 0.0f, 1.0f,
	-4.0f, 0.3f, -0.4f, 1.0f,
	-4.0f, 0.5f, -0.4f, 1.0f,


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
};