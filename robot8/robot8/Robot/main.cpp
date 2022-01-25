#include "main.h"
#include "FreeImage.h"
void testpicture(char *imgpath);
GLfloat skyboxVertices[] = {
		// 背面
		-50.0f, 50.0f, -50.0f,		// A
		-50.0f, -50.0f, -50.0f,	// B
		50.0f, -50.0f, -50.0f,		// C
		50.0f, -50.0f, -50.0f,		// C
		50.0f, 50.0f, -50.0f,		// D
		-50.0f, 50.0f, -50.0f,		// A

		// 左侧面
		-50.0f, -50.0f, 50.0f,		// E
		-50.0f, -50.0f, -50.0f,	// B
		-50.0f, 50.0f, -50.0f,		// A
		-50.0f, 50.0f, -50.0f,		// A
		-50.0f, 50.0f, 50.0f,		// F
		-50.0f, -50.0f, 50.0f,		// E

		// 右侧面
		50.0f, -50.0f, -50.0f,		// C
		50.0f, -50.0f, 50.0f,		// G
		50.0f, 50.0f, 50.0f,		// H
		50.0f, 50.0f, 50.0f,		// H
		50.0f, 50.0f, -50.0f,		// D
		50.0f, -50.0f, -50.0f,		// C

		// 正面
		-50.0f, -50.0f, 50.0f,  // E
		-50.0f, 50.0f, 50.0f,  // F
		50.0f, 50.0f, 50.0f,  // H
		50.0f, 50.0f, 50.0f,  // H
		50.0f, -50.0f, 50.0f,  // G
		-50.0f, -50.0f, 50.0f,  // E

		// 顶面
		-50.0f, 50.0f, -50.0f,  // A
		50.0f, 50.0f, -50.0f,  // D
		50.0f, 50.0f, 50.0f,  // H
		50.0f, 50.0f, 50.0f,  // H
		-50.0f, 50.0f, 50.0f,  // F
		-50.0f, 50.0f, -50.0f,  // A

		// 底面
		-50.0f, -50.0f, -50.0f,  // B
		-50.0f, -50.0f, 50.0f,   // E
		50.0f, -50.0f, 50.0f,    // G
		50.0f, -50.0f, 50.0f,    // G
		50.0f, -50.0f, -50.0f,   // C
		-50.0f, -50.0f, -50.0f,  // B
	};
//Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));

int main(int argc, char** argv){
	glutInit(&argc, argv);
	//glutInitContextVersion(4,3);//以OpenGL version4.3版本為基準
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
   
    initshader();
	init();
	//glutDisplayFunc(display2);
	glutDisplayFunc(display);
	
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(processSpecialKeys);
	int ActionMenu,ModeMenu,ShaderMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("run_and_jump",1);
	glutAddMenuEntry("javelin",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯


	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutMouseFunc(Mouse);
	glutTimerFunc (100, idle, 0); 
	glutMainLoop();
	return 0;
}
void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}
void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	int out = 0;
	
	if(action == WALK){
		flag = 0;
		walk_flag = 1;
		
		if(javelin_flag == 1)
		{
		dummy = 0;
		javelin_flag = 0;
		resetObj(dummy);
		out = 0;
		
		}
		updateObj(dummy);
		out = dummy+1;
		if(out > 27) out = 1;
	}
	else if(action == IDLE){
		flag = 0;
		walk_flag = 0;
		javelin_flag = 0;
		resetObj(dummy);
		out = 0;
	}
	else if(action == Javelin){
		flag = 1;
		javelin_flag = 1;
		if(walk_flag == 1){
			walk_flag = 0;
			dummy = 0;
			resetObj(dummy);
		    out = 0;
			
		}
		
		updateObj2(dummy);
		
		out = dummy+1;
		if(out > 36) out = 1;
	}
	glutPostRedisplay();
	
	glutTimerFunc (150, idle, out);
	//cout << out << endl;
}
void resetObj(int f){
	for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
	}
	position_Longinus_y=0.0;
	position_Longinus_z =0.0;
	position =0.0;
    position_lefthand_y = 0.0;
    position_lefthand_z = 0.0;
	position_righthand_y = 0.0;
	position_righthand_z = 0.0;
	position_leftfoot_x = 0.0;
	position_leftfoot_y = 0.0;
    position_leftfoot_z = 0.0;
	position_rightfoot_x = 0.0;
    position_rightfoot_y = 0.0;
    position_rightfoot_z = 0.0;

	position_lefthanddown_y = 0.0;
    position_lefthanddown_z = 0.0;

    position_righthanddown_y = 0.0;
    position_righthanddown_z = 0.0;

    position_leftfootdown_x = 0.0;
    position_leftfootdown_y = 0.0;
    position_leftfootdown_z = 0.0;

    position_rightfootdown_x = 0.0;
    position_rightfootdown_y = 0.0;
    position_rightfootdown_z = 0.0;
}
void updateObj(int frame){
	switch(frame){
	case 0:
		//左手
		//angles[2] = -45;
		//右手

		//腿
		//angles[13] = 15;
		//position_leftfootdown_y -= 3.0;
        //position_leftfootdown_z += 10.0;	
		
		break;
	case 1:
	case 2:
	case 3:
		angles[1] +=15;
		angles[6] -=15;
		angles[12] -=15;
		angles[15] +=15;
		position += 0.1;
		position_leftfoot_z -= 0.3;
        position_rightfoot_z += 0.3;

		
		position_leftfootdown_x -= 0.2;
        position_leftfootdown_z -= 2.0;

		position_rightfootdown_x += 0.2;
        position_rightfootdown_z += 2.0;

		break;
	case 4:
	case 5:
	case 6:
		angles[1] -=15;
		angles[6] +=15;
		angles[12] +=15;
		angles[15] -=15;
		position -= 0.1;
		position_leftfoot_z += 0.3;
        position_rightfoot_z -= 0.3;

		//position_leftfootdown_y -= 3.0;
		position_leftfootdown_x += 0.2;
        position_leftfootdown_z += 2.0;

		position_rightfootdown_x -= 0.2;
        position_rightfootdown_z -= 2.0;


		break;
	case 7:
	case 8:
	case 9:
		angles[1] -=15;
		angles[6] +=15;
		angles[12] +=15;
		angles[15] -=15;
		//angles[13] -=15;
		position += 0.1;
		position_leftfoot_z += 0.3;
        position_rightfoot_z -= 0.3;

        position_leftfootdown_x -= 0.3;
		position_leftfootdown_z += 2.0;

		position_rightfootdown_x += 0.3;
        position_rightfootdown_z -= 2.0;


		//position_leftfootdown_y += 3.0;
        //position_leftfootdown_z -= 10.0;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] +=15;
		angles[6] -=15;
		angles[12] -=15;
		angles[15] +=15;
		//angles[13] +=15;
		position -= 0.1;
		position_leftfoot_z -= 0.3;
        position_rightfoot_z += 0.3;

        position_leftfootdown_x += 0.3;
		position_leftfootdown_z -= 2.0;

		position_rightfootdown_x -= 0.3;
        position_rightfootdown_z += 2.0;
		//position_leftfootdown_y -= 3.0;
        //position_leftfootdown_z += 10.0;
		break;
	case 13:
	case 14:
	case 15:
		angles[1] -=40;
		angles[6] -=50;
		angles[12] -=15;
		angles[13] +=15;
		angles[15] -=15;
		position += 3.0;
		position_lefthand_y += 1.0;
		position_righthand_y += 1.0;
		position_leftfoot_x += 0.3;
		position_rightfoot_x -= 0.3;
		position_leftfoot_y += 0.6;
        position_rightfoot_y += 0.6;
		position_leftfoot_z += 0.6;
        position_rightfoot_z += 0.6;

        position_leftfootdown_x -= 0.45;
		position_leftfootdown_y += 3.3;
		//position_leftfootdown_z += 0.1;

        position_rightfootdown_x += 0.45;
		position_rightfootdown_y += 3.3;
        //position_rightfootdown_z += 0.1;
		/*angles[1] -=20;
		angles[6] -=25;
		angles[12] -=8;
		angles[15] -=8;
		position += 1.5;
		position_lefthand_y += 0.5;
		position_righthand_y += 0.5;
		position_leftfoot_x += 0.15;
		position_rightfoot_x -= 0.15;
		position_leftfoot_y += 0.3;
        position_rightfoot_y += 0.3;
		position_leftfoot_z += 0.3;
        position_rightfoot_z += 0.3;*/
		break;
	case 16:
	case 17:
	case 18:
	   /* angles[1] -=20;
		angles[6] -=25;
		angles[12] -=7;
		angles[15] -=7;
		position += 1.5;
		position_lefthand_y += 0.5;
		position_righthand_y += 0.5;
		position_leftfoot_x += 0.15;
		position_rightfoot_x -= 0.15;
		position_leftfoot_y += 0.3;
        position_rightfoot_y += 0.3;
		position_leftfoot_z += 0.3;
        position_rightfoot_z += 0.3;*/
		
		break;
	case 19:
	case 20:
	case 21:
		
		break;
	case 22:
	case 23:
	case 24:
	   
		angles[1] +=40;
		angles[6] +=50;
		angles[12] +=15;
		angles[15] +=15;
		position -= 3.0;
		position_lefthand_y -= 1.0;
		position_righthand_y -= 1.0;
		position_leftfoot_x -= 0.3;
		position_rightfoot_x += 0.3;
		position_leftfoot_y -= 0.6;
        position_rightfoot_y -= 0.6;
		position_leftfoot_z -= 0.6;
        position_rightfoot_z -= 0.6;
        
		angles[13] -=15;
		position_leftfootdown_x += 0.45;
		position_leftfootdown_y -= 3.3;
		//position_leftfootdown_z -= 0.1;

        position_rightfootdown_x -= 0.45;
		position_rightfootdown_y -= 3.3;
        //position_rightfootdown_z -= 0.1;

		break;
	case 25:
	case 26:
	case 27:
		
		break;
	}
}
void updateObj2(int frame){
	switch(frame){
	case 0:
		//左手
		//angles[2] = -45;
		//右手

		//腿
		//angles[13] = 45;	
		
		break;
	case 1:
	case 2:
	case 3:
	    angles[13] += 15;
		angles[6] -=70;
		angles[1] -= 20;
		angles[9] -= 40;
		angles[12] -=10;
		position -= 1.0;
		position_Longinus_y += 2.0;
		position_Longinus_z -= 5.0;
		position_righthand_y += 0.4;
		position_righthand_z -= 0.4;
		position_leftfoot_z -= 0.5;
    
        position_rightfoot_z += 0.5;

        position_leftfootdown_x -= 0.1;
		position_leftfootdown_y -= 0.5;
		position_leftfootdown_z += 2.3;

        position_rightfootdown_x += 0.1;
		position_rightfootdown_y -= 1.7;
        position_rightfootdown_z += 2.0;

		break;
	case 4:
	case 5:
	case 6:
		//angles[1] -=8;
		//angles[12] +=8;
		//angles[13] -= 15;
		//position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		//angles[1] -=8;
		//angles[12] +=8;
		//angles[13] = 0;
		//position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		//angles[1] +=8;
		//angles[12] -=8;
		//angles[13] += 15;
		//position -= 0.1;
		break;
	case 13:
	case 14:
	case 15:
	    //angles[1] -=8;
		//angles[12] -=8;
		//angles[13] = 0;
		//position += 0.1;
		break;
	case 16:
	case 17:
	case 18:
	    //angles[1] +=8;
		//angles[12] +=8;
		//angles[13] += 15;
		//position -= 0.1;
		break;
	case 19:
	case 20:
	case 21:
		angles[13] -=15;
		angles[6] +=70;
		angles[1] += 20;
		//angles[9] += 30;
		angles[12] +=10;
		//angles[13] = 0;
		position += 1.0;
		position_Longinus_y += 10.0;
		position_Longinus_z += 20.0;
		position_righthand_y -= 0.4;
		position_righthand_z += 0.4;
		position_leftfoot_z += 0.5;
        position_rightfoot_z -= 0.5;


		position_leftfootdown_x += 0.1;
		position_leftfootdown_y += 0.5;
		position_leftfootdown_z -= 2.3;

        position_rightfootdown_x -= 0.1;
		position_rightfootdown_y += 1.7;
        position_rightfootdown_z -= 2.0;
		break;
    case 22:
	   
	case 23:
	case 24:
	    //angles[1] -=8;
		//angles[12] -=8;
		//angles[13] = 0;
		position_Longinus_y += 10.0;
		position_Longinus_z += 20.0;
		break;
	case 25:
	case 26:
	case 27:
	    //angles[1] -=8;
		//angles[12] -=8;
		//angles[13] = 0;
		position_Longinus_y += 10.0;
		position_Longinus_z += 20.0;
		break;
	case 28:
	case 29:
	case 30:
	    for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
	    }
		position_Longinus_z -= 55.0;
		position_Longinus_y -= 32.0;
		break;
	case 31:
	case 32:
	case 33:
	    break;
	case 34:
	case 35:
	case 36:
	    break;
	

	}
}

 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;
 GLuint texLOC;
 void initshader()
 {
	 ShaderInfo shaders[] = {
		 {GL_VERTEX_SHADER, "DSPhong_Material.vp"},	  //vertex shader
		 {GL_FRAGMENT_SHADER, "DSPhong_Material.frag"}, //fragment shader
		 {GL_NONE, NULL}};

	 program = LoadShaders(shaders); //讀取shader
    
	ShaderInfo shaders2[] = {
		 {GL_VERTEX_SHADER, "skybox.vertex"},	  //vertex shader
		 {GL_FRAGMENT_SHADER, "skybox.frag"}, //fragment shader
		 {GL_NONE, NULL}};
    
	program2 = LoadShaders(shaders2); //讀取shader
    
	ShaderInfo shadersid[] = {
		 {GL_VERTEX_SHADER, "particle/Particle.vertexshader"},	  //vertex shader
		 {GL_FRAGMENT_SHADER, "particle/Particle.fragmentshader"}, //fragment shader
		 {GL_NONE, NULL}};
	programID = LoadShaders(shadersid); //讀取shader
	//programID = LoadShaders( "particle/Particle.vertexshader", "particle/Particle.fragmentshader" );
    program3 = LoadShaders("advanced.vs", "advanced.frag");
    program4 = LoadShaders("screen.vs", "screen.frag");
}
void initparticle(){
	// Vertex shader
	CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	// fragment shader
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	for(int i=0; i<MaxParticles; i++){
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}
    Texture = loadDDS("particle/14.DDS");
	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	cout << "1"<<endl;
}
void initFBO(){
   glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
    // Setup plane VAO 
    /*glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);*/
    // Setup screen VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    // Load textures
    //cubeTexture = loadTexture((char*)"picture/123.jpg",false);
	//cubeTexture = loadBMP_custom((char *)"picture/123.bmp");
	cubeTexture = createTextureFromImage((char*)"picture/test.jpg");
    floorTexture = loadTexture((char*)"picture/Up_Tex2.png",false);
	//#pragma endregion

    // Framebuffers
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  
    // Create a color attachment texture
    textureColorbuffer = generateAttachmentTexture(false, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

	// Framebuffers2
    glGenFramebuffers(1, &framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);  
    // Create a color attachment texture
    textureColorbuffer2 = generateAttachmentTexture(false, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo2);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo2); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void init(){
	isFrame = false;
	pNo = 0;
	for(int i = 0;i<PARTSNUM;i++)//初始化角度陣列
		angles[i] = 0.0;
    
	
	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glUseProgram(program);//uniform參數數值前必須先use shader
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");
	ModelID =  glGetUniformLocation(program,"Model");
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	texLOC=glGetUniformLocation(program,"Material.tex0"); 
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;

	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,30) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);
    
	//Obj2Buffer();
	//if(num==0){
	//Obj2Buffer((char*)"my_obj2/background2.obj",5);	
    //Obj2Buffer((char*)"my_obj2/background.obj",5);
	//}
	//if(num==1){
	Obj2Buffer((char*)"my_obj2/body.obj",0);
	Obj2Buffer((char*)"my_obj2/lefthand.obj",1);
	Obj2Buffer((char*)"my_obj2/lefthand_down.obj",2);
	Obj2Buffer((char*)"my_obj2/righthand.obj",6);
	Obj2Buffer((char*)"my_obj2/righthand_down.obj",7);
	Obj2Buffer((char*)"my_obj2/leftfoot.obj",12);
	Obj2Buffer((char*)"my_obj2/leftfoot_down.obj",13);
	Obj2Buffer((char*)"my_obj2/rightfoot.obj",15);
	Obj2Buffer((char*)"my_obj2/rightfoot_down.obj",16);

	Obj2Buffer((char*)"my_obj2/Longinus.obj",9);
	//}
	glUseProgram(program2);
	glUniformMatrix4fv(glGetUniformLocation(program2, "projection"),
		1, GL_FALSE, glm::value_ptr(Projection));

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	initparticle();
	initFBO();
	cout << "2" << endl;
	
	//UBO
	glUniformBlockBinding(program, MatricesIdx,0);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);  
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);

	glClearColor(0.0,0.0,0.0,1);//black screen
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	background();
	//LoadTexture("picture/111.png");
}

#define DOR(angle1) (angle1*3.1415/180);
void display(){
	
//SKYBOX
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glGenVertexArrays(1, &skyBoxVAOId);
	glGenBuffers(1, &skyBoxVBOId);
	glBindVertexArray(skyBoxVAOId);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	// 顶点位置数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//texture2 = LoadTexture("picture/111.png", 640, 640);
    glDepthMask(GL_TRUE);
	//glDepthMask(GL_FALSE); // 禁止写入深度缓冲区
	glUseProgram(program2);
	//background();
	//Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//glm::mat4 projection = glm::perspective(camera.mouse_zoom,
		//(GLfloat)w / h, 0.1f, 100.0f); // 投影矩阵
	float sky = DOR(rotatesky);
	float sky2 = DOR(rotatesky2);
	//vec3(0,sin(sky),cos(sky))+vec3(sin(sky2),0,cos(sky2))
	glm::mat4 view       = glm::lookAt(
		vec3(sin(sky2),0,cos(sky2)) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);
	//glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // 视变换矩阵 移除translate部分
	
	glUniformMatrix4fv(glGetUniformLocation(program2, "view"),
		1, GL_FALSE, glm::value_ptr(view));
	
	glBindVertexArray(skyBoxVAOId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextId); // 注意绑定到CUBE_MAP
	//glUniform1i(glGetUniformLocation(program2, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glUseProgram(program);
	//glUniform1i(glGetUniformLocation(program, "skybox"), 0);
//SKYBOX
//--------------------------------------------------------------------------------
// Floor
    
	/*glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glm::mat4 model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(program3, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);		*/
	
//PARTICLE
	if (particle_NUM == 1)
	{	
		glDepthMask(GL_TRUE);
		glUseProgram(programID);
		ProjectionMatrix = glm::perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
		ViewMatrix = glm::lookAt(
		glm::vec3(0,10,30) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);
		glm::vec3 CameraPosition(glm::inverse(ViewMatrix  )[3]);
        cout << "2.2"<<endl;
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix  ;
		newparticles = (int)(delta*10000.0);
		if (newparticles > (int)(0.016f*10000.0))
			newparticles = (int)(0.016f*10000.0);
		//新的粒子噴出來時的數值
		for(int i=0; i<newparticles; i++){
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 3.0f; // This particle will live 5 seconds.
			ParticlesContainer[particleIndex].pos = glm::vec3(0,-20.f,10.0f);
           
			float spread = 30.0f;
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(//0.f,10.f,0.f
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f
			);
			
			ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


			// Very bad way to generate a random color
			ParticlesContainer[particleIndex].r = 0;
			ParticlesContainer[particleIndex].g = 255;
			ParticlesContainer[particleIndex].b = 0;
			//rand() % 256;
			ParticlesContainer[particleIndex].a = 200;
			//(rand() % 256) / 3;
			ParticlesContainer[particleIndex].size = 0.1f;
			//(rand()%1000)/2000.0f + 0.1f;
			
		}
		
        //每個粒子的變化
		// Simulate all particles
		ParticlesCount = 0;
		for(int i=0; i<MaxParticles; i++){

			Particle& p = ParticlesContainer[i]; // shortcut

			if(p.life > 0.0f){

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f){

					// Simulate simple physics : gravity only, no collisions
					//p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;//往下掉出現重力的感覺
					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length2( p.pos - CameraPosition );
					p.size += 0.01f;
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
					g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
					g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;
												   
					g_particule_position_size_data[4*ParticlesCount+3] = p.size;

												   
					g_particule_color_data[4*ParticlesCount+0] = p.r;
					g_particule_color_data[4*ParticlesCount+1] = p.g;
					g_particule_color_data[4*ParticlesCount+2] = p.b;
					g_particule_color_data[4*ParticlesCount+3] = p.a;

				}else{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;
                
			}
			
		}
    
		SortParticles();
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shadersss
		glDepthMask(GL_TRUE);
		glUseProgram(programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// Same as the billboards tutorial
		glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		
		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		//break;
	} // Check if the ESC key was pressed or the window was closed
//PARTICLE
//--------------------------------------------------------------------------------

//ROBOT
    if(mtl_flag==0){
	//glDepthMask(GL_TRUE);
	
	glBindVertexArray(VAO);
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);//
    glBindTexture(GL_TEXTURE_2D, id);//
	float eyey = DOR(eyeAngley);
	View       = lookAt(
		               vec3(eyedistance*sin(eyey),2,eyedistance*cos(eyey)) , // Camera is at (0,0,20), in World Space
		               vec3(0,0,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	updateModels();
	glUniform3f(glGetUniformLocation(program, "cameraPos"), eyedistance*sin(eyey), 2, eyedistance*cos(eyey));
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
    
	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);
        
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,固定點,連續點的偏移量,buffer point)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextId); 
		for(int j = 0;j <mtls[i].size() ;j++){//	
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			glUniform1i(texLOC,1);
			
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model
}
//ROBOT
//--------------------------------------------------------------------------------*/
//計時傳給noise
   if(noise_flag==1){
  float timer[cnt];
        //printf("Start timing...\n");    
        
        // 開始計時 
        for(i = 1; i <= cnt; i++)
        {
            //printf("Start No.%d timer!(press any key to stop)\n", i);   
            timer[i] = crrentTime;
            totalTime += timer[i];
        }
    if(TRUE)  
    {
        crrentTime = t++ / 100.0;
        //printf(" \r %.2f", crrentTime);  // 利用 \r 歸位字元，讓時間始終顯示在同一位置 
		noiseLOC = glGetUniformLocation(program, "u_time"); //
        glUniform1f(noiseLOC, crrentTime);
        Sleep(10); // 10 ms = 0.01 s
        //if(hitAnyKey())
        //{
           // printf("\a\n");  // 發出聲音並且換行 
           // break;
        //}
    }
    crrentTime = (int)(crrentTime * 100 + 0.5) / 100.0;;
  }
//計時傳給noise
//--------------------------------------------------------------------------------*/

    if(control==9){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//if(screenshot==0){
//SKYBOX
	glGenVertexArrays(1, &skyBoxVAOId);
	glGenBuffers(1, &skyBoxVBOId);
	glBindVertexArray(skyBoxVAOId);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	// 顶点位置数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//texture2 = LoadTexture("picture/111.png", 640, 640);
    //glDepthMask(GL_TRUE);
	glUseProgram(program2);
	//Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//glm::mat4 projection = glm::perspective(camera.mouse_zoom,
		//(GLfloat)w / h, 0.1f, 100.0f); // 投影矩阵
	float sky = DOR(rotatesky);
	float sky2 = DOR(rotatesky2);
	//vec3(0,sin(sky),cos(sky))+vec3(sin(sky2),0,cos(sky2))
	glm::mat4 view       = glm::lookAt(
		vec3(sin(sky2),0,cos(sky2)) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);
	//glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // 视变换矩阵 移除translate部分
	
	glUniformMatrix4fv(glGetUniformLocation(program2, "view"),
		1, GL_FALSE, glm::value_ptr(view));
	
	glBindVertexArray(skyBoxVAOId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextId); // 注意绑定到CUBE_MAP
	//glUniform1i(glGetUniformLocation(program2, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glUseProgram(program);
	//glUniform1i(glGetUniformLocation(program, "skybox"), 0);
//SKYBOX
	//ROBOT
    if(mtl_flag==0){
	glBindVertexArray(VAO);
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);//
    glBindTexture(GL_TEXTURE_2D, id);//
	float eyey = DOR(eyeAngley);
	View       = lookAt(
		               vec3(eyedistance*sin(eyey),2,eyedistance*cos(eyey)) , // Camera is at (0,0,20), in World Space
		               vec3(0,0,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	updateModels();
	glUniform3f(glGetUniformLocation(program, "cameraPos"), eyedistance*sin(eyey), 2, eyedistance*cos(eyey));
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
    
	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);
        
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,固定點,連續點的偏移量,buffer point)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextId); 
		for(int j = 0;j <mtls[i].size() ;j++){//	
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//glUniform1i(texLOC,1);
			
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model
}
//ROBOT
	//}
	//else 
	if(screenshot==1){

    // Set uniforms
	glUseProgram(program3);
	glm::mat4 model;
	//glm::mat4 viewtest = camera.GetViewMatrix();
	glm::mat4 projectiontest = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//float eyey = DOR(eyeAngley);
	glm::mat4 viewtest       = glm::lookAt(
		               vec3(0,0,1) , // Camera is at (0,0,20), in World Space
		               vec3(0,0,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	//glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(program3, "view"), 1, GL_FALSE, glm::value_ptr(viewtest));
	glUniformMatrix4fv(glGetUniformLocation(program3, "projection"), 1, GL_FALSE, glm::value_ptr(projectiontest));

	// Cubes
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);  
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(program3, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, nrOfindex, GL_UNSIGNED_INT,0);
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(glGetUniformLocation(program3, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);	
	/////////////////////////////////////////////////////
	// Bind to default framebuffer again and draw the 
	// quad plane with attched screen texture.
	// //////////////////////////////////////////////////
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
	// Draw Screen
	glUseProgram(program4);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	}
//--------------------------------------------------------------------------------

	glDeleteVertexArrays(1, &skyBoxVAOId);
	glDeleteBuffers(1, &skyBoxVBOId);
	glFlush();		   //強制執行上次的OpenGL commands
	glutSwapBuffers(); //調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

//PARTICLE RESTART
void reset(){
	if(particle_NUM == 0&& particle_flag==2){
		newparticles = (int)(delta*10000.0);
		ParticlesCount = 0;
		//delete[] g_particule_position_size_data;
		// Cleanup VBO and shader
		glDeleteBuffers(1, &particles_color_buffer);
		glDeleteBuffers(1, &particles_position_buffer);
		glDeleteBuffers(1, &billboard_vertex_buffer);
		glDeleteProgram(programID);
		glDeleteTextures(1, &Texture);
		glDeleteVertexArrays(1, &VertexArrayID);
		ShaderInfo shadersid[] = {
			{GL_VERTEX_SHADER, "particle/Particle.vertexshader"},	  //vertex shader
			{GL_FRAGMENT_SHADER, "particle/Particle.fragmentshader"}, //fragment shader
			{GL_NONE, NULL}};

		programID = LoadShaders(shadersid); //讀取shader
		//programID = LoadShaders( "particle/Particle.vertexshader", "particle/Particle.fragmentshader" );
		initparticle();
		particle_flag = 1;
	}
}

void background(){		
	if(background_num==0 ||background_num==1){	
		glUseProgram(program);
		//LoadTexture("picture/sky2.png");	
		std::vector<const char*> faces;
		faces.push_back("picture/diversespace-left.jpg");
		faces.push_back("picture/diversespace-right.jpg");			
		faces.push_back("picture/diversespace-down.jpg");
		faces.push_back("picture/diversespace-up.jpg");						
		faces.push_back("picture/diversespace-front.jpg");
		faces.push_back("picture/diversespace-back.jpg");						
		skyBoxTextId = TextureHelper::loadCubeMapTexture(faces);								//
		backgroundLOC = glGetUniformLocation(program, "background_num"); 
		glUniform1i(backgroundLOC, background_num);
		if(light==true)
		{
			lightLOC=glGetUniformLocation(program,"light");   
			glUniform1i(lightLOC,light);//
		}
		else
		{	   
			lightLOC = glGetUniformLocation(program, "light");				
			glUniform1i(lightLOC, light);									
		}			
		}			
	else if(background_num==2){
		glUseProgram(program);
		//LoadTexture("picture/city2.png");//
		std::vector<const char*> faces2;
		faces2.push_back("picture/Right_Tex2.png");
		faces2.push_back("picture/Left_Tex2.png");
		faces2.push_back("picture/Up_Tex.png");
		faces2.push_back("picture/Down_Tex.png");			
		faces2.push_back("picture/Back_Tex.png");
		faces2.push_back("picture/Front_Tex2.png");	
		skyBoxTextId = TextureHelper::loadCubeMapTexture(faces2);
		backgroundLOC=glGetUniformLocation(program,"background_num");//    
		glUniform1i(backgroundLOC,background_num);//
		if(light==true)
		{
			lightLOC=glGetUniformLocation(program,"light");//    
			glUniform1i(lightLOC,light);//
		}
		else
		{	   
			lightLOC = glGetUniformLocation(program, "light");				//
			glUniform1i(lightLOC, light);									//
		}
	}
}

void Obj2Buffer(char* obj,int i){
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;

	if(mtl_flag==0)
	    loadMTL("my_obj2/EVA2.mtl",Kds,Kas,Kss,Materials,texture);
	//else if(mtl_flag==1)
	   // loadMTL("my_obj2/blank.mtl",Kds,Kas,Kss,Materials,texture);
	//if(texture.c_str()!=NULL)
	    //printf("%s\n",texture.c_str());
	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}
    load2Buffer((char*)obj,i);
	//background();
	
	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&uVBO);
	glGenBuffers(1,&nVBO);
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));
		offset[0] += vertices_size[i]*sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	mat4 Scale[PARTSNUM];
	for(int i = 0 ; i < PARTSNUM;i++){
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f); 
		Scale[i] = mat4(1.0f); 
	}
	float r,pitch,yaw,roll;
	float alpha, beta ,gamma;

	//Body============================================================
	beta = angle1;
	Rotatation[0] = rotate(beta,0,1,0);
	Translation[0] = translate(0,2.9+position,0);
	Models[0] = Translation[0]*Rotatation[0];
	//============================================================

	//左手=======================================================
	yaw = DOR(beta);r = 3.7;
	alpha = angles[1];
    gamma = 0;
	//gamma = 10;
	//Translation[1] = translate(3.7,10,-0.5);
    Translation[1] = translate(0,-0.1+position_lefthand_y,-0.05);
	
		//alpha = angles[1];
	    //gamma = 0;
	Rotatation[1] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Models[1] = Models[0]*Translation[1]*Rotatation[1];

	
	Rotatation[1] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Models[1] = Models[0]*Translation[1]*Rotatation[1];
	//============================================================
	    
	     
	
	//左肩膀
	Rotatation[4] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Translation[4] =translate(3.7,1,-0.5);//位移到左上手臂處
	
	Models[4] =Models[0]*Translation[1]*Rotatation[1];
	
	//左下手臂
	pitch = DOR(alpha);r = 3;
	roll = DOR(gamma);
	static int i=0;
	i+=5;
	alpha = angles[2];
	//上手臂+下手臂向前旋轉*向右旋轉
	Rotatation[2] = rotate(alpha,1,0,0);
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin)
	//延z軸位移以上手臂為半徑角度:translate(r*sin,-rcos,0)
	Translation[2] = translate(0,0,0);

	Models[2] = Models[1]*Translation[2]*Rotatation[2];
	

	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//手掌角度與下手臂相同
	//Rotatation[3] = Rotatation[2];
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin) ,角度為上手臂+下手臂
	Translation[3] = translate(0,-4.8,10);
	Models[3] = Models[2]*Translation[3]*Rotatation[3];
	
	//背景==========================================================
	alpha = eyeAngley;
	//alpha =angles[5];
	Rotatation[5] = rotate(alpha,0,1,0);
	Translation[5] = translate(0 ,10,0);
	//Scale[5] = scale(10.0, 10.0, 1.0);
	Models[5] = Models[5]*Translation[5]*Rotatation[5];
	//============================================================

	//右手=========================================================
	gamma = 0;
	if(flag==0){
	    alpha =  angles[6];
	    //cout << "angles[1]"<<angles[1] << endl;
		}
	else if(flag==1){
	    alpha = angles[6] ;	
		//cout << "angles[6]"<<angles[6] << endl;
		}
	Rotatation[6] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	//Translation[6] = translate(-3.9,1.7,-0.2);
	Translation[6] = translate(0,-0.1+position_righthand_y,-0.05+position_righthand_z);
	Models[6] = Models[0]*Translation[6]*Rotatation[6];
	//=============================================================

	//槍=============================================================
	alpha = angles[9];
	Rotatation[9] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	if(javelin_flag==1)
	    Translation[9] = translate(4.8,2.0+position_Longinus_y,7.0+position_Longinus_z);
	else if(javelin_flag==0)
        Translation[9] = translate(4.8,2.0+position_Longinus_y-position_Longinus_y,7.0+position_Longinus_z-position_Longinus_z);
	Models[9] = Models[9]*Translation[9]*Rotatation[9];
    //=============================================================

	angles[7] = angles[2];
	pitch = DOR(alpha);r = -3;
	roll = DOR(gamma);
	alpha = angles[7];
	Rotatation[7] = rotate(alpha,1,0,0);
	Translation[7] = translate(0,0,0);
	Models[7] = Models[6]*Translation[7]*Rotatation[7];

	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	Translation[8] =translate(0,-6,0);
	Models[8] = Models[7]*Translation[8]*Rotatation[8];
	
	//back&DBody===================================================
	Translation[10] =translate(0,2,-4.5);
	Models[10] = Models[0]*Translation[10]*Rotatation[10];

	Translation[11] =translate(-0.9,0,0.1);
	Models[11] = Models[0]*Translation[11]*Rotatation[11];
	//=============================================================

	//左腳
	alpha = angles[12];gamma = 2;
	Rotatation[12] = rotate(alpha,0.5,0,0)*rotate(gamma,0,0,1);
	//Translation[12] =translate(1.8,-4.5,0);
	//Translation[12] =translate(-0.9,2.6,0.1);
	Translation[12] =translate(-0.3+position_leftfoot_x,-0.25+position_leftfoot_y,-0.1+position_leftfoot_z);
	Models[12] = Translation[12]*Rotatation[12]*Models[0];
    //=============================================================

	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[13]+angles[12];
	//Translation[13] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[12];
	Translation[13] = translate(0.5+position_leftfootdown_x,3+position_leftfootdown_y,0+position_leftfootdown_z)*Translation[12];
	Rotatation[13] = rotate(alpha,1,0,0);
	Models[13] = Translation[13]*Rotatation[13]*Models[13];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	Translation[14] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-1)*Translation[13];
	Rotatation[14] = rotate(alpha,1,0,0);
	Models[14] = Translation[14]*Rotatation[14]*Models[14];
	
	//右腳============================================================
	if(flag==0){
	    alpha = angles[15] ;
	    //cout << "angles[1]"<<angles[1] << endl;
		}
	else if(flag==1){
	    alpha =  -angles[12];
		//cout << "angles[6]"<<angles[6] << endl;
		}
	
	gamma = -2;
	Rotatation[15] = rotate(alpha ,0.5,0,0)*rotate(gamma ,0,0,1);
	//Translation[15] =translate(-1.8,-4.5,0);
	Translation[15] =translate(0.3+position_rightfoot_x,-0.25+position_rightfoot_y,-0.1+position_rightfoot_z);
	Models[15] = Translation[15]*Rotatation[15]*Models[0];
    //=============================================================
    
	angles[16] = angles[13];

	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[16]+angles[15];
	Rotatation[16] = rotate(alpha,1,0,0);
	//Translation[16] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[15];
	Translation[16] = translate(-0.5+position_rightfootdown_x,3.1+position_rightfootdown_y,0+position_rightfootdown_z)*Translation[15];
	Models[16] = Translation[16]*Rotatation[16]*Models[16];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[15]+angles[16];
	Translation[17] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-0.5)*Translation[16];
	Rotatation[17] = rotate(alpha,1,0,0);
	Models[17] = Translation[17]*Rotatation[17]*Models[17];
	
}
//
void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);
	if(!res) printf("load failed\n");
    //cout << mtls[i].size() << endl;
	

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x*1,0,0,0);
	vec4 c2 = vec4(0,y*1,0,0);
	vec4 c3 = vec4(0,0,z*1,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle1,float x,float y,float z){
	float r = DOR(angle1);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	
	case '1':
	    if(background_num==0 ||background_num==2){
			background_num = 1;
			background();
			//cout <<"1  :"<< background_num << endl;
		    }
			else if (background_num == 1){
				background_num = 0;
				background();
				//cout <<"2  :"<< background_num << endl;
		    }
			//angle += 5;
			//if(angle>=360) angle = 0;
			//printf("beta:%f\n",angle);
			break;
	case '2':
	    if(background_num==0 ||background_num==1){
			background_num = 2;
			background();
			//cout <<"3  :"<< background_num << endl;
			}
		else if(background_num==2){
		    background_num = 0;
		    background();
			//cout <<"4  :"<< background_num << endl;
		    }
		//angle -= 5;
		//if(angle<=0) angle = 360;
		//printf("beta:%f\n",angle);
		break;
	case '3':
	    if(background_num==1 ||background_num==2){
			if(light==true)
			{
				light = false;
				background();	
			}
			else if(light==false)
			{
                light = true;
				background();	
			}
			}
		
		break;
	case '4':
	    if(particle_NUM==0&&particle_flag == 0){
		    particle_NUM = 1;
			particle_flag = 1;

		 }
		 else if(particle_NUM==0&&particle_flag == 1){
			 particle_NUM = 1;
			particle_flag = 1;

		 }
		 else if(particle_NUM==1&&particle_flag == 1){
		    particle_NUM = 0;
			particle_flag = 2;
			reset();
		}
		 break;
	case '5':
	    if(mtl_flag==0){
		    mtl_flag = 1;
			//mtl_flag2 = 1;
			//reset2();

		 }
		 else if(mtl_flag==1){
			mtl_flag =0;
			//mtl_flag2 = 1;
			//reset2();
			
		 }
		 break;
	case '6':
	    
		if(control==6)
		{
			noise_flag = 0;
			control = 0;
			noisecontrolLOC = glGetUniformLocation(program, "control"); //
            glUniform1i(noisecontrolLOC, control);
			cnt=1;
			i=0;
				
			totalTime = 0;
			crrentTime=0;
			t = 0;
		}
		else{
		   noise_flag = 1;
		   control = 6;
		   noisecontrolLOC = glGetUniformLocation(program, "control"); //
           glUniform1i(noisecontrolLOC, control);
		}
		break;
	case '7':
	    
		if(control==7)
		{
			control = 0;
			noisecontrolLOC = glGetUniformLocation(program, "control"); //
            glUniform1i(noisecontrolLOC, control);
		}
		else{
		   control = 7;
		   noisecontrolLOC = glGetUniformLocation(program, "control"); //
           glUniform1i(noisecontrolLOC, control);
		}
		break;
	case '8':
	    
		if(control==8)
		{
			control = 0;
			noisecontrolLOC = glGetUniformLocation(program, "control"); //
            glUniform1i(noisecontrolLOC, control);
		}
		else{
		   control = 8;
		   noisecontrolLOC = glGetUniformLocation(program, "control"); //
           glUniform1i(noisecontrolLOC, control);
		}
		break;
	case '9':
	    
		if(control==9)
		{
			control = 0;
			//noisecontrolLOC = glGetUniformLocation(program, "control"); //
            //glUniform1i(noisecontrolLOC, control);
		}
		else{
		   control = 9;
		   //noisecontrolLOC = glGetUniformLocation(program, "control"); //
           //glUniform1i(noisecontrolLOC, control);
		}
		break;
	case '0':
		testpicture((char*)"picture/123.jpg");
		cubeTexture = createTextureFromImage((char*)"picture/test.jpg");
		break;
	case 'w':
		eyedistance -= 1.0;
		rotatesky -= 1.0;
		//if(rotatesky==-360.00)
			//rotatesky = 0.0;
		break;
	case 's':
		eyedistance += 1.0;
		rotatesky += 1.0;
		//if(rotatesky==360.00)
			//rotatesky = 0.0;
		break;
	case 'a':
	    //eyedistance -= 1.0;
		rotatesky2 += 1.0;
		//if(rotatesky2==360.00)
			//rotatesky2 = 0.0;
		
		break;
	case 'd':
	    //eyedistance += 1.0;
		rotatesky2 -= 1.0;
		//if(rotatesky2==-360.00)
			//rotatesky2 = 0.0;
		
		break;
	case 'r':
		angles[1] -= 5; 
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
	    eyeAngley -=30;
		//rotatesky += 1.0;
		break;
	case 'e':
	    eyeAngley +=30;
	    //rotatesky -= 1.0;
		break;
	}
	glutPostRedisplay();
}
void processSpecialKeys(int key, int x, int y) {
	switch(key)
	{
		case GLUT_KEY_F1:
			picturetype = 0;
			glUseProgram(program4);
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F2:
			picturetype = 1;
			glUseProgram(program4);
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F3:
			picturetype = 2;
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F4:
			picturetype = 3;
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F5:
			picturetype = 4;
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F6:
			picturetype = 5;
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F7:
			picturetype = 6;
			pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			glUniform1i(pictureLOC, picturetype);
			break;
		case GLUT_KEY_F9:
		    if(screenshot == 0){
			    screenshot = 1;
				picturetype = 0;
				glUseProgram(program4);
			    pictureLOC = glGetUniformLocation(program4, "picturetype"); //
			    glUniform1i(pictureLOC, picturetype);
				//mtl_flag = 1;
				//cubeTexture = createTextureFromImage((char*)"picture/test.jpg");
			}
			else if(screenshot == 1){
			    screenshot = 0;
				//mtl_flag = 0;
				//cubeTexture = createTextureFromImage((char*)"picture/test.jpg");
			}
			break;
	}
}
void menuEvents(int option){}
void ActionMenuEvents(int option){
	switch(option){
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		action = Javelin;
		break;
	}
}
void ModeMenuEvents(int option){
	switch(option){
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}
void ShaderMenuEvents(int option){
	pNo = option;
}


// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle(){

	for(int i=LastUsedParticle; i<MaxParticles; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<LastUsedParticle; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles(){
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


/*void reset2(){
    
    
	if(mtl_flag2==1){
	glDeleteProgram(program);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &uVBO);
	glDeleteBuffers(1, &nVBO);
	for(int i = 0;i < PARTSNUM ;i++){
		glDeleteBuffers(1, &VBOs[i]);
		glDeleteBuffers(1, &uVBOs[i]);
		glDeleteBuffers(1, &nVBOs[i]);
	}
	glDeleteBuffers(1, &UBO);
	
	glDeleteVertexArrays(1, &VAO);
44
	ShaderInfo shaders[] = {
	{GL_VERTEX_SHADER, "DSPhong_Material.vp"},	  //vertex shader
	{GL_FRAGMENT_SHADER, "DSPhong_Material.fp"}, //fragment shader
	{GL_NONE, NULL}};

	program = LoadShaders(shaders); //讀取shader
	init();
	mtl_flag2 = 0;
	}
}*/

void testpicture(char *imgpath)
{
    //std::cout << "z" << std::endl;
    BYTE* pixels = new BYTE[3 * 800 * 600];
    
    glReadPixels(0, 0, 800, 600, GL_BGR, GL_UNSIGNED_BYTE, pixels);

// Convert to FreeImage format & save to file
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, 800, 600, 3 * 800, 24, 255, 255, 255, false);
    FreeImage_Save(FIF_BMP, image,"picture/test.jpg", 0);

    // Free resources
    FreeImage_Unload(image);
    delete [] pixels; 
}
static unsigned createTexture(int w, int h, const void* data) {
	unsigned    texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w,h, 0, GL_RGBA, GL_UNSIGNED_BYTE,data);
    //glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return  texId;
}

/**
*   使用FreeImage加载图片
*/
static unsigned createTextureFromImage(const char* fileName) {
	//1 获取图片格式
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	if(fifmt == FIF_UNKNOWN) {
		return  0;
	}
	//2 加载图片
	FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);

	//获取加载图片格式
	FREE_IMAGE_COLOR_TYPE type    =   FreeImage_GetColorType(dib);

	// 获取数据指针
	FIBITMAP*   temp    =   dib;
	dib =   FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(temp);

	BYTE*   pixels =   (BYTE*)FreeImage_GetBits(dib);
	int     width   =   FreeImage_GetWidth(dib);
	int     height  =   FreeImage_GetHeight(dib);

	//默认加载出来的图片数据格式是bgra 这里要转换成rgba
	for(int i = 0; i < width * height * 4; i+=4) {
		BYTE temp       =   pixels[i];
		pixels[i]       =   pixels[i + 2];
		pixels[i + 2]   =   temp;
	}

	unsigned    res =   createTexture(width, height, pixels);
	FreeImage_Unload(dib);
	return      res;
}




