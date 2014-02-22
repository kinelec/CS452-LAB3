#include "initShaders.h"
#include <cstdlib>
using namespace std;

void rotate(GLuint locate);

GLuint vaoID,vboID[2],eboID;
GLuint prog;

GLfloat pit,yaw,scalar=1;
glm::vec3 diamondTran;

GLfloat vertices[]={
	0.0f,10.0f,0.0f,
	0.0f,-10.0f,0.0f,
	0.0f,0.0f,3.0f,
	0.0f,0.0f,-3.0f,	
	2.0f,0.0f,0.0f,
	-2.0f,0.0f,0.0f
					};

GLfloat colors[]={
	0.0f,0.0f,1.0f,1.0f,
	1.0f,0.0f,1.0f,1.0f,
	0.0f,0.5f,1.0f,1.0f,
	1.0f,0.5f,0.0f,1.0f,
	0.5f,1.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f						
	              		};
											
 GLubyte elems[]={
	1,2,4,
	1,5,2,
	1,3,5,
	1,4,3,
	0,2,4,
	0,5,2,
	0,3,5,
	0,4,3
			};

void init(){
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 600, 600);	
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	
	glGenBuffers(2, vboID);
	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
	glGenBuffers(1,&eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

	Shader s[]={
  	{ GL_VERTEX_SHADER , "vertexshader.glsl"},
  	{ GL_FRAGMENT_SHADER , "fragmentshader.glsl"}, 
  	{ GL_NONE , NULL} 
  	};
		
  	prog=initShaders(s);
  
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);  
}


void display(SDL_Window* screen){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 trans;
	
	trans = glm::translate(trans,diamondTran);
  	trans = glm::rotate(trans,pit,glm::vec3(1,0,0));//rotate around x axis
  	trans = glm::rotate(trans,yaw,glm::vec3(0,1,0));//rotate arround y axis
  	trans = glm::scale(trans,glm::vec3(scalar));//scaling
    
	//transformation matrix
  	GLint tmpLocation = glGetUniformLocation(prog,"modelMatrix");
	glUniformMatrix4fv(tmpLocation,1,GL_FALSE,&trans[0][0]);
	
	glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_BYTE,NULL);
	glFlush();
	SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){
	
	//keyboard events
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_QUIT:exit(0);break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:exit(0);
					case SDLK_w:diamondTran.y+=2;break;
					case SDLK_s:diamondTran.y-=2;break;
					case SDLK_a:diamondTran.x-=2;break;
					case SDLK_d:diamondTran.x+=2;break;
					case SDLK_e:scalar+=.1f;break;
					case SDLK_q:scalar-=.1f;break;
					case SDLK_i:pit+=2;break;
					case SDLK_k:pit-=2;break;
					case SDLK_j:yaw+=2;break;
					case SDLK_l:yaw-=2;break;
				}
		}
	}
}


int main(int argc, char **argv){
	SDL_Window *window;
	
	//initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    	SDL_Quit();
    	exit(1);
	}

	window = SDL_CreateWindow("Kinel Lab 3",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_OPENGL);
	
	if(window==NULL){
		fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
	}

	SDL_GLContext glcontext=SDL_GL_CreateContext(window);
	
	//initialize glew
  	glewExperimental=GL_TRUE;
  	if(glewInit()){
    	fprintf(stderr, "Unable to initalize GLEW");
    	exit(EXIT_FAILURE);
  	}
  
	init();
	
	while(true){
		input(window);
		display(window);
	}

	SDL_GL_DeleteContext(glcontext);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
 
  	return 0;
}
