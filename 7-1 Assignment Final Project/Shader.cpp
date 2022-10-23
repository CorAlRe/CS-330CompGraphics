#include "Shader.h"

const GLchar* Shader::DefaultVertexShaderSource =
	"#version 440 core"
	"\nlayout(location = 0) in vec3 position;  // Vertex data from Vertex Attrib Pointer 0 "
	"\nlayout(location = 1) in vec3 color;     // Color data from Vertex Attrib Pointer 1 "

	"\nout vec3 vertexColor; // variable to transfer color data to the fragment shader "

	"\n//Global variables for the  transform matrices "
	"\nuniform mat4 model; "
	"\nuniform mat4 view; "
	"\nuniform mat4 projection; "

	"\nvoid main() "
	"\n{"
	"\n    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates "
	"\n    vertexColor = color;   // references incoming color data "
	"\n}";


const GLchar* Shader::DefaultVertexFragmentShaderSource =
	"#version 440 core"
	"\nin vec3 vertexColor;   // Variable to hold incoming color data from vertex shader "

	"\nout vec4 fragmentColor; "

	"\nvoid main() "
	"\n{"
	"\n		fragmentColor = vec4(vertexColor, 1.0f);"
	"\n}";

const GLchar* Shader::TextureVertexShaderSource =
	"#version 440 core"
	"\nlayout(location = 0) in vec3 position;		// Vertex data from Vertex Attrib index 0"
	"\nlayout(location = 1) in vec3 color;          // vertex data from vertext attrib index 1"
	"\nlayout(location = 2) in vec2 textureCoord;   // Texture data from Vertex Attrib index 2"

	"\nout vec2 vertexTextureCoordinate;	// variable to transfer texture data to the fragment shader"

	"\n//Global variables for the transform matrices"
	"\nuniform mat4 model;"
	"\nuniform mat4 view;"
	"\nuniform mat4 projection;"

	"\nvoid main()"
	"\n{"
	"\n    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates"
	"\n    vertexTextureCoordinate = textureCoord;	    // texture vector2"
	"\n}";

const GLchar* Shader::TextureFragmentShaderSource =
	"#version 440 core"
	"\nin vec2 vertexTextureCoordinate;   // holds texture coordinate from vertex shader, used"

	"\nout vec4 fragmentColor;			  // output color"

	"\nuniform sampler2D uTexture;		  // uniform for texture unit, used"

	"\nvoid main()"
	"\n{"
	"\n		fragmentColor = texture(uTexture, vertexTextureCoordinate);"
	"\n}";


const GLchar* Shader::LampVertexShaderSource =
	"#version 440 core"
	"\nlayout(location = 0) in vec3 position;" // lamp positions from vbo

	"\nuniform mat4 model;"
	"\nuniform mat4 view;"
	"\nuniform mat4 projection;"

	"\nvoid main()"
	"\n{"
	"\n		gl_Position = projection * view * model * vec4(position, 1.0f);" // transform to clip coordinates
	"\n}";

const GLchar* Shader::LampFragmentShaderSource =
	"#version 440 core"
	"\n out vec4 fragmentColor; " // output color

	"\nvoid main()"
	"\n{"
	"\n		fragmentColor = vec4(1.0f); " // color white rgba (1, 1, 1, 1)
	"\n}";


const GLchar* Shader::LightingVertexShaderSource =
	"#version 440 core"
	"\nlayout(location = 0) in vec3 position;"				// positions from vbo
	"\nlayout(location = 1) in vec3 color;"					// colors from vbo
	"\nlayout(location = 2) in vec2 textureCoordinate;"		// texture coords from vbo
	"\nlayout(location = 3) in vec3 normal;"				// normals from vbo

	"\nout vec3 vertexNormal;"				// For outgoing normals to fragment shader
	"\nout vec3 vertexFragmentPos;"			// For outgoing color / pixels to fragment shader
	"\nout vec2 vertexTextureCoordinate;"	// For outgoing texture coords to fragment shader

	"\nuniform mat4 model;"					// model matrix transforms to world space
	"\nuniform mat4 view;"					// view matrix transforms to view space
	"\nuniform mat4 projection;"			// projection matrix transforms to clip space

	"\nvoid main()"
	"\n{"
	"\n		gl_Position = projection * view * model * vec4(position, 1.0f);" // transform to clip coordinates
	"\n		vertexFragmentPos = vec3(model * vec4(position, 1.0f));"		 // fragment position in world space pass to frag shader
	"\n		vertexNormal = mat3(transpose(inverse(model))) * normal;"        // normal vectors in world space but remove translation by converting to mat3 pass to frag shader
	"\n		vertexTextureCoordinate = textureCoordinate;"					 // pass UV coordinate to frag shader 
	"\n}";

const GLchar* Shader::LightingFragmentShaderSource =
	"#version 440 core"

	"\n#define NR_DIFF_LIGHTS 2"		// number of diffuse light sources for computing diffuse and specular

	// structure to hold diffuse light properties
	"\nstruct DiffLight {"
		"\nvec3 position;"
		"\nvec3 color;"
		"\nfloat intensity;"
	"\n};"

	"\nin vec3 vertexNormal;"				// For incoming normals 
	"\nin vec3 vertexFragmentPos;"			// For incoming fragment position
	"\nin vec2 vertexTextureCoordinate;"	// U V coordinate

	"\nout vec4 fragmentColor;"				// output color to GPU

	// Uniform / Global variables for object color, light color, light position, and camera/view position
	"\nuniform float ambientStrength;"		// ambient strength
	"\nuniform float specularIntensity;"	// specular strength
	"\nuniform float highlightSize;"		// specular size (pow)
	"\nuniform vec3 viewPosition;"			// position of the camera
	"\nuniform sampler2D uTexture;"			// texture unit
	"\nuniform DiffLight diffLights[NR_DIFF_LIGHTS];" // diffuse light properties

	"\nvoid main()"
	"\n{"
		// normalize normal
		"\nvec3 normal = normalize(vertexNormal);"

		// object color 
		"\nvec3 textureColor = texture(uTexture, vertexTextureCoordinate).xyz;"			// texture color is object color

		// ambient lighting 
		"\nvec3 ambient = ambientStrength * textureColor;"								// adjust color for ambient lighting

		// diffuse lighting
		"\nvec3 lightDir = normalize(diffLights[0].position - vertexFragmentPos);"
		"\nvec3 diffuse = max(dot(normal, lightDir), 0.0f) * diffLights[0].intensity * diffLights[0].color;"		// compute amount of diffuse light from lightsource 1

		// diffuse light 2 
		"\nvec3 lightDir2 = normalize(diffLights[1].position - vertexFragmentPos);"
		"\nvec3 diffuse2 = max(dot(normal, lightDir2), 0.0f) * diffLights[1].intensity * diffLights[1].color;"	// compute amount of diffuse light from lightsource 2

		// specular lighting 1
		"\nvec3 viewDir = normalize(viewPosition - vertexFragmentPos);"
		"\nvec3 reflectDir = reflect(-lightDir, normal);"
		"\nfloat spec = pow(max(dot(viewDir, reflectDir), 0.0f), highlightSize);"
		"\nvec3 specular = specularIntensity * spec * diffLights[0].color;"				// compute amount of specular light from light source 1

		// specular lighting 2
		"\nvec3 specular2 = vec3(0.0f);"
		"\nif (diffLights[1].intensity > 0.0f) {" // only compute if light has a value
		"\n  vec3 reflectDir2 = reflect(-lightDir2, normal);"
		"\n  float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0f), highlightSize);"
		"\n  specular2 = specularIntensity * spec2 * diffLights[1].color;"			// compute amount of specular light from lightsource 2
		"\n}"

		// output final color
		"\nfragmentColor = vec4((ambient + diffuse + diffuse2 + specular + specular2) * textureColor, 1.0f);"
	"\n}";

Shader::Shader() {
	CompileProgram(Shader::DefaultVertexShaderSource, Shader::DefaultVertexFragmentShaderSource);
}

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	CompileProgram(vertexShaderSource, fragmentShaderSource);
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	CompileProgram(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::use() {
	if (ID != 0)
		glUseProgram(ID);
}

void Shader::setProjectionMatrix(const glm::mat4& projection) {
	GLint location = glGetUniformLocation(ID, "projection");
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::setModelMatrix(const glm::mat4& model) {
	GLint location = glGetUniformLocation(ID, "model");
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::setViewMatrix(const glm::mat4& view) {
	GLint location = glGetUniformLocation(ID, "view");
	if (location != -1)	
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
}

// tell shader which texture unit to use
void Shader::setTextureUnit(GLint unit) {
	GLint location = glGetUniformLocation(ID, "uTexture");
	if (location != -1)	
		glUniform1i(location, 0);  
}

void Shader::setUniformValue(std::string name, GLfloat value) {
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location != -1)
		glUniform1f(location, value);
}

void Shader::setUniformValue(std::string name, glm::vec3 value) {
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location != -1)
		glUniform3f(location, value.x, value.y, value.z);
}

Shader::~Shader() {
	if (ID != 0)
		glDeleteProgram(ID);
}

void Shader::CompileProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	ID = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
	glShaderSource(fragShaderId, 1, &fragmentShaderSource, NULL);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile the vertex shader
	// check for shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return;
	}

	glCompileShader(fragShaderId); // compile the fragment shader
	// check for shader compile errors
	glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(ID, vertexShaderId);
	glAttachShader(ID, fragShaderId);

	glLinkProgram(ID);   // links the shader program
	// check for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return;
	}

//	glUseProgram(ID);    // Uses the shader program

	// clean up source files
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragShaderId);

	return;
}