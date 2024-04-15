#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>


#include <iostream>

// resize callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// process input callback function prototype
void processInput(GLFWwindow* window);
// mouse callback function prototype
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// zoom callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float mixValue = 0.2f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(5.25f, 0.0f, 5.0f); // was 0.0f, 0.0f, 3.0f
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



// camera movement 
Camera camera(glm::vec3(5.25f, 0.0f, 5.0f));  // was 0.0f, 0.0f, 3.0f
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// light
glm::vec3 lightPos(5.25f, 5.0f, 5.0f);


int main() 
{
	
	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// load glad before opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// glViewport(0, 0, 800, 600);

	// auto resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// cursor position
	glfwSetCursorPosCallback(window, mouse_callback);

	// scroll capture
	glfwSetScrollCallback(window, scroll_callback);


	// global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	/*Shader ourShader("Shaders/vertex.vert", "Shaders/fragment.frag");*/
	Shader lightingShader("Shaders/lighting_colors.vert", "Shaders/lighting_colors.frag");
	Shader lightCubeShader("Shaders/light_cube.vert", "Shaders/light_cube.frag");

	

	

	

	// ::INITIALIZATION CODE DONE::
	// set up vertex data (and buffers) and configure vertex attributes
	// -----------------------------------------------------------------


	// first three - positions, other two - (was)texture coords, now other 3 - normal vectors
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
			/*glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f),*/




			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.5f),
			glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 4.5f),
			glm::vec3(0.0f, 0.0f, 6.0f),
			glm::vec3(0.0f, 0.0f, 7.5f),
			glm::vec3(0.0f, 0.0f, 9.0f),
			



			glm::vec3(1.5f, 0.0f, 0.0f),
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(4.5f, 0.0f, 0.0f),
			glm::vec3(6.0f, 0.0f, 0.0f),
			glm::vec3(7.5f, 0.0f, 0.0f),
			glm::vec3(9.0f, 0.0f, 0.0f),



			glm::vec3(10.5f, 0.0f, 0.0f),
			glm::vec3(10.5f, 0.0f, 1.5f),
			glm::vec3(10.5f, 0.0f, 3.0f),
			glm::vec3(10.5f, 0.0f, 4.5f),
			glm::vec3(10.5f, 0.0f, 6.0f),
			glm::vec3(10.5f, 0.0f, 7.5f),
			glm::vec3(10.5f, 0.0f, 9.0f),

			glm::vec3(1.5f, 0.0f, 9.0f),
			glm::vec3(3.0f, 0.0f, 9.0f),
			glm::vec3(4.5f, 0.0f, 9.0f),
			glm::vec3(6.0f, 0.0f, 9.0f),
			glm::vec3(7.5f, 0.0f, 9.0f),
			glm::vec3(9.0f, 0.0f, 9.0f),
			
	};


	//unsigned int VAO[1], VBO[1];

	//glGenVertexArrays(1, VAO);
	//glGenBuffers(1, VBO);
	//// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO[0]);

	//unsigned int lightVAO;
	//glGenVertexArrays(1, &lightVAO);
	//glBindVertexArray(lightVAO);



	//// 2. copy our vertices array in a buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//// 4. set the vertex attributes pointers
	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//// texture coordinates attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//// light source attribute
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(3);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.


	// first, configure the cube's VAO (and VBO)
	unsigned int cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);





	//glBindVertexArray(0);


	//// texture generation
	//
	//unsigned int texture1;
	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);

	//// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// load and generate the texture
	//int width, height, nrChannels;

	//// flip the image
	//stbi_set_flip_vertically_on_load(true);
	////
	//unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);





	//unsigned int texture2;
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);

	//// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//data = stbi_load("Textures/fresh.png", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	//// ::TEXTURE GENERATION DONE:: 
	//
	//ourShader.use(); // don't forget to activate the shader before setting uniforms!  
	//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	//ourShader.setInt("texture2", 1);

	



	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// delta

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		//input
		processInput(window);

		// rendering commands here

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		


		/*lightPos = glm::vec3(5.25f+6*cos(glfwGetTime()), 5.0f, 5.0f+6*sin(glfwGetTime()));*/
		lightPos.x = 5.25f + sin(glfwGetTime()) * 3.0f;
		lightPos.y = 2.0f + sin(glfwGetTime() / 2.0f) * 5.0f;

		// 5. Drawing code (in render loop)
		// use shader program to render an object




		//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		lightingShader.use();

		/*lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);*/
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);


		// material properties
		lightingShader.setVec3("material.ambient", 0.0215f,0.1745f,0.0215f);
		lightingShader.setVec3("material.diffuse", 0.07568f,0.61424f,0.07568f);
		lightingShader.setVec3("material.specular", 0.633f,0.727811f,0.633f);
		lightingShader.setFloat("material.shininess", 0.6f*128.0f);

		// light properties
		lightingShader.setVec3("light.ambient", 0.24725f,0.1995f,0.0745f);
		lightingShader.setVec3("light.diffuse", 0.75164f,0.60648f,0.22648f);
		lightingShader.setVec3("light.specular", 0.628281f,0.555802f,0.366065f);



		// -----------------------------------------------------------------------------

		/*glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f);


		lightingShader.setVec3("light.ambient", ambientColor);
		lightingShader.setVec3("light.diffuse", diffuseColor);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);*/

		// -----------------------------------------------------------------------------





		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);


		for (unsigned int i = 0; i < (sizeof(cubePositions)/sizeof(*cubePositions)); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			/*model = glm::scale(model, glm::vec3(1.0f, 2.0f, 4.0f));*/
			model = glm::translate(model, cubePositions[i]);
			float angle = 0.0f;
			/*if (i % 3 == 0)
			{
				angle = glfwGetTime() * 25.0f;
			}*/
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, lightPos);
		/*float angle = 40.0f * glfwGetTime();
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));*/
		model = glm::scale(model, glm::vec3(2.0f));
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	/*glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);*/
	/*glDeleteBuffers(1, &EBO);*/

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// auto resize callback-function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}
// input function
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mixValue += 0.01f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) 
	{
		mixValue -= 0.01f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyBoard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyBoard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		lightPos.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		lightPos.y -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		lightPos.x += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		lightPos.x -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.MovementSpeed -= 0.1f;
		if (camera.MovementSpeed <= 0.0f)
			camera.MovementSpeed = 0.1;
	}
	


}

void mouse_callback(GLFWwindow* window, double xposIN, double yposIN)
{
	float xpos = static_cast<float>(xposIN);
	float ypos = static_cast<float>(yposIN);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
