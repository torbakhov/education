//internal includes
#include "custom_include/common.h"
#include "custom_include/Object.h"
#include "custom_include/ShaderProgram.h"


int initGL()
{
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "MashGraph Torbakhov", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;
	
	//Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex_normal_parallax.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment_normal_parallax.glsl";
	ShaderProgram program(shaders); GL_CHECK_ERRORS;

	glEnable(GL_DEPTH_TEST);

	glfwSwapInterval(1); // force 60 frames per second

	//Object
	Object cube;
	Mesh cubeMesh;
	Texture cubeTexture;
	Texture cubeNormalMap;
	Texture cubeHeightMap;

	cubeMesh.load("meshes/cube.obj");
	cube.mesh = &cubeMesh;

	cubeTexture.loadTexture("textures/Metal_Grill_Base_Color.jpg");
	cube.diffuseMap = &cubeTexture;
	cubeNormalMap.loadTexture("textures/Metal_Grill_Normal.jpg");
	cube.normalMap = &cubeNormalMap;
	cubeHeightMap.loadTexture("textures/Metal_Grill_Height.bmp");
	cube.heightMap = &cubeHeightMap;
	cube.shader = &program;
	cube.position = glm::vec3(0, 0, 0);
	cube.scale = 1;



	//Camera
	Camera cam;
	cam.pos = glm::vec3(4.0f, 2.5f, 3.0f);
	cam.dir = -cam.pos;
	cam.moveSpeed = 2.0f;

	double time_prev = glfwGetTime();
	float deltaTime;

	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		deltaTime = glfwGetTime() - time_prev;
		time_prev = deltaTime + time_prev;

		cam.moveCam(window, deltaTime);

		//очищаем экран каждый кадр
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

		program.StartUseShader();                           GL_CHECK_ERRORS;


		// очистка и заполнение экрана цветом
		//
		glViewport  (0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		cube.drawObject(&cam, NULL);


		program.StopUseShader();

		glfwSwapBuffers(window); 
	}

	glfwTerminate();
	return 0;
}
