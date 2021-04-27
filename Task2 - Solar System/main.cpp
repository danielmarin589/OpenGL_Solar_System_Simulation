// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "shader.hpp"
#include "Camera/camera.h"
#include "Sphere.h"
#include "stb_image.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

//variables
GLFWwindow* window;
const int width = 1024, height = 768;

int tWidth, tHeight, nrChannels;
unsigned char* pixel;

float elapsedTimeT = 0.0f;
float elapsedTimeR = 0.0f;


///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(float radius, int sectors, int stacks) 
{
    set(radius, sectors, stacks);
}


///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Sphere::set(float radius, int sectors, int stacks)
{
    this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;

    buildVerticesSmooth();
    
}


///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void Sphere::clearArrays()
{
    std::vector<float>().swap(positions);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    
}


///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void Sphere::buildVerticesSmooth()
{
    const float PI = acos(-1);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addPosition(x, y, z);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
            }

            if (i != (stackCount - 1))
            {
                addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}


///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = positions.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(positions[i]);
        interleavedVertices.push_back(positions[i + 1]);
        interleavedVertices.push_back(positions[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}


///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addPosition(float x, float y, float z)
{
    positions.push_back(x);
    positions.push_back(y);
    positions.push_back(z);
}


///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}


///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}


//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Add the camera parameters here and create a camera object

//side view
//glm::vec3 cameraPos = glm::vec3(0.0f, -10000.0f, 0.0f);
//glm::vec3 cameraDir = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

//top view
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10000.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//angled view
//glm::vec3 cameraPos = glm::vec3(2000.0f, -10000.0f, 10000.0f);
//glm::vec3 cameraDir = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f)-cameraPos);
//glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

Camera camObj(cameraPos, cameraDir, cameraUp);
 
void window_callback(GLFWwindow * window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

bool flagRot = false;
bool flagTrans = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        
        //toggle rotation 
        flagRot = flagRot == true ? false : true;
             
        /*if (flagRot == true) {
            flagRot = false;
        }
        else flagRot = true;*/

        if (flagRot) {
            std::cout << "Planets started rotating." << std::endl;
        }

        if (!flagRot) {
            std::cout << "Planets stopped rotating." << std::endl;
        }

    
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        
        //toggle translation
        flagTrans = flagTrans == true ? false : true;
       
        
        if (!flagTrans) {
            
            std::cout << "Planets stopped orbiting around the Sun." << std::endl;
        }

        if (flagTrans) {
            
            std::cout << "Planets started orbiting around the Sun." << std::endl;
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
       
        flagRot = false;
        flagTrans = false;
        elapsedTimeT = 0.0f;
        elapsedTimeR = 0.0f;
    }
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camObj.translateFront(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camObj.translateBack(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		camObj.translateUp(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		camObj.translateDown(deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camObj.translateRight(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camObj.translateLeft(deltaTime);
    }
}

struct cBody {
    
    float distance; //distance from center of the sun/origin(0,0,0)
    float radius;
    float oSpeed;  //orbiting speed
    float rSpeed;  //rotating speed for own axis
    glm::vec4 color;
	GLuint texture;

    cBody(float distance, float radius, float oSpeed, float rSpeed, glm::vec4 color, const char* filePath) {
        this->distance = distance;
        this->radius = radius;
        this->oSpeed = oSpeed;
        this->rSpeed = rSpeed;
        this->color = color;
		this->setTexture(filePath);
    }
	
private:
	void setTexture(const char* filePath)
	{
		int texWidth, texHeight, nrChannnels;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		unsigned char* pixels = stbi_load(filePath, &texWidth, &texHeight, &nrChannels, 0);
		if (pixels)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture from " << filePath << std::endl;
		}

		stbi_image_free(pixels);
	}
};

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "3D demo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	
	
    Sphere sphere(1.0f, 36, 18);

	cBody sun(0.0f, 10.0f, 0.0f, 10.0f, glm::vec4(1.0f, 0.309f, 0.019f, 1.0f), "textures/sunmap.jpg");
	
	std::vector<cBody> planets;
	planets.push_back(cBody(20.0f, 2.0f, 150.0f, 25.0f, glm::vec4(0.976f, 0.450f, 0.082f, 1.0f), "textures/mercurymap.jpg")); //mercur
	planets.push_back(cBody(30.0f, 3.0f, 125.0f, 10.0f, glm::vec4(0.819f, 0.654f, 0.537f, 1.0f), "textures/venusmap.jpg")); //venus
	planets.push_back(cBody(40.0f, 3.0f, 110.0f, 80.0f, glm::vec4(0.294f, 0.298f, 0.945f, 1.0f), "textures/earthmap.jpg")); //earth
	planets.push_back(cBody(50.0f, 4.0f, 100.0f, 75.0f, glm::vec4(0.780f, 0.235f, 0.019f, 1.0f), "textures/marsmap.jpg")); //mars
	planets.push_back(cBody(65.0f, 8.0f, 85.0f, 160.0f, glm::vec4(0.647f, 0.388f, 0.113f, 1.0f), "textures/jupitermap.jpg")); //jupiter
	planets.push_back(cBody(85.0f, 5.5f, 70.0f, 150.0f, glm::vec4(0.890f, 0.6f, 0.349f, 1.0f), "textures/saturnmap.jpg")); //saturn
	planets.push_back(cBody(100.0f, 2.5f, 40.0f, 110.0f, glm::vec4(0.596f, 0.847f, 0.964f, 1.0f), "textures/uranusmap.jpg")); //uranus 
	planets.push_back(cBody(115.0f, 7.0f, 20.0f, 100.0f, glm::vec4(0.062f, 0.549f, 0.835f, 1.0f), "textures/neptunemap.jpg")); //neptun   
    
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store 
	// the information for a complete rendered object.
	GLuint vbo1, vao1, ibo1;

	//Bindings for sphere
	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	glBindVertexArray(vao1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

    GLuint attribVertex = 0;
    GLuint attribTexCoord = 1;

    // activate attrib arrays
    glEnableVertexAttribArray(attribVertex);
    glEnableVertexAttribArray(attribTexCoord);

    // set attrib arrays with stride and offset
    int stride = 5 * sizeof(float); 
    glVertexAttribPointer(attribVertex, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

	glfwSetFramebufferSizeCallback(window, window_callback);
    glfwSetKeyCallback(window, key_callback);
	//Change accordingly for camera
	glm::mat4 view;
	view = glm::lookAt(camObj.getCameraPosition(), camObj.getCameraViewDirection(), camObj.getCameraUp());

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 20000.0f);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
	
        // Check for events
        glfwPollEvents();

        processInput(window);

		//calculate delta time 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame; 

		glm::mat4 view = glm::lookAt(camObj.getCameraPosition(), camObj.getCameraPosition() + camObj.getCameraViewDirection(), camObj.getCameraUp());

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

        //creating the sun
        glm::mat4 model;
		model = glm::rotate(model, elapsedTimeT * sun.oSpeed / 10, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(sun.distance, 0.0f, 0.0f));

		model = glm::rotate(model, elapsedTimeR * sun.rSpeed / 10, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(sun.radius, sun.radius, sun.radius));
        
        glm::mat4 matrix = projection * view * model;

        unsigned int transformLoc = glGetUniformLocation(programID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

        unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
        glUniform4fv(transformLoc2, 1, glm::value_ptr(sun.color));

		//bind the appropriate texture
		glBindTexture(GL_TEXTURE_2D, sun.texture);
        
		//!!! bind the appropriate VAO before drawing 
        glBindVertexArray(vao1);
       
        //draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,         // primitive type
            sphere.getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        
        for (int i = 0; i < planets.size(); i++) {


            model = glm::mat4(1.0f);

            if (flagTrans == true) {
                elapsedTimeT += deltaTime;
            }
            
           
            model = glm::rotate(model, elapsedTimeT * planets[i].oSpeed/10, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(planets[i].distance, 0.0f, 0.0f));        
            
            

            if (flagRot == true) {
                elapsedTimeR += deltaTime;
            }
            
            
            model = glm::rotate(model, elapsedTimeR * planets[i].rSpeed/10, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(planets[i].radius, planets[i].radius, planets[i].radius));
            

            //calculate MVP matrix
            matrix = projection * view * model;

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
            glUniform4fv(transformLoc2, 1, glm::value_ptr(planets[i].color));

			//bind the appropriate texture
			glBindTexture(GL_TEXTURE_2D, planets[i].texture);

            //!!! bind the appropriate VAO before drawing
            glBindVertexArray(vao1);

            //draw a sphere with VBO
            glDrawElements(GL_TRIANGLES,         // primitive type
                sphere.getIndexCount(),          // # of indices
                GL_UNSIGNED_INT,                 // data type
                (void*)0);                       // offset to indices

        }

            // Swap buffers
            glfwSwapBuffers(window);
	}

	// Cleanup VBO

    // deactivate attrib arrays
    glDisableVertexAttribArray(attribVertex);
    glDisableVertexAttribArray(attribTexCoord);

    // unbind VBOs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
    glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &ibo1);
	glDeleteVertexArrays(1, &vao1);	
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}