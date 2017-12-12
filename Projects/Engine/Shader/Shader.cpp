
// Header
#include "Shader.h"

// Library includes
#include <fstream>

// Project includes
#include <Tools/Strings.h>

// Namespace declarations


namespace ShaderSpace {


std::string getShaderLog(GLhandleARB handle)
{
    int charsWritten  = 0;
    int infologLength = 0;
    char *infoLog;

    glGetObjectParameterivARB(handle, GL_INFO_LOG_LENGTH, &infologLength);

    if ( infologLength > 1 ) {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(handle, infologLength, &charsWritten, infoLog);

		std::string log = infoLog;
		log.erase(log.find_last_not_of(" \n\r\t") + 1);

        free(infoLog);

        return log;
    }

    return "<Blank Log>";
}


Shader::Shader(const Common::Logger *p, const std::string& name, const std::string& path)
: Common::Logger(p, "Shader"),
  Core::Object(Core::invalidID, name),
  mFragmentShader(0),
  mGeometryShader(0),
  mProgramObject(0),
  mVertexShader(0)
{
	init(path);
}

Shader::~Shader()
{
	unload();
}

void Shader::begin()
{
	glUseProgramObjectARB(mProgramObject);
}

void Shader::end()
{
	glUseProgramObjectARB(0);
}

void Shader::init(const std::string& path)
{
	if ( path.empty() ) {
		error("invalid shader path provided!");
	    return;
	}

	std::string frag = load(path + FragmentShader);
	std::string geo  = load(path + GeometryShader);
	std::string vert = load(path + VertexShader);

    const char *szFShader = frag.c_str();
	const char *szGShader = geo.c_str();
	const char *szVShader = vert.c_str();


    // Here we get a pointer to our fragment and vertex shaders
    mFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    (void)szGShader;	//mGeometryShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_EXT);
    mVertexShader   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

    // Now this assigns the shader text file to each shader pointer
    glShaderSourceARB(mFragmentShader,	1, &szFShader, NULL);
    glShaderSourceARB(mVertexShader,	1, &szVShader, NULL);


    // Compile our fragment shader
    glCompileShaderARB(mFragmentShader);
	debug("Compiling fragment shader: " + getShaderLog(mFragmentShader));

    // Compile our geometry shader
	//glCompileShaderARB(mGeometryShader);
	//debug("Compiling geometry shader: " + getShaderLog(mGeometryShader));

    // Compile our vertex shader
	glCompileShaderARB(mVertexShader);
	debug("Compiling vertex shader: " + getShaderLog(mVertexShader));


    // Next we create a program object to represent our shaders
    mProgramObject = glCreateProgramObjectARB();

    // We attach each shader we just loaded to our program object
    glAttachObjectARB(mProgramObject, mFragmentShader);
    //glAttachObjectARB(mProgramObject, mGeometryShader);
    glAttachObjectARB(mProgramObject, mVertexShader);

    // Link our program object with OpenGL
    glLinkProgramARB(mProgramObject);
	debug("Linking shader program: " + getShaderLog(mProgramObject));

	// And finally Validate our shader program
    glValidateProgramARB(mProgramObject);
	//debug("Validate shader program: " + getShaderLog(mProgramObject));
}

std::string Shader::load(const std::string& filename)
{
	debug("load('" + filename + "')");

    // Open the file passed in
    std::ifstream fin(filename.c_str());

    // Make sure we opened the file correctly
	if ( !fin ) {
		warn("could not load '" + filename + "'!");
		return "";
	}

    std::string strLine = "";
    std::string strText = "";

    // Go through and store each line in the text file within a "string" object
    while( getline(fin, strLine) ) {
		strText = strText + "\n" + strLine;
    }

    // Close our file
    fin.close();

    // Return the text file's data
    return strText;
}

GLhandleARB Shader::program()
{
	return mProgramObject;
}

void Shader::set(const std::string& name, bool value)
{
	glUniform1iARB( variable(name), value );
}

void Shader::set(const std::string& name, float value)
{
	glUniform1fARB( variable(name), value );
}

void Shader::set(const std::string& name, int value)
{
	glUniform1iARB( variable(name), value );
}

void Shader::unload()
{
    // If our fragment shader pointer is valid, free it
    if ( mFragmentShader ) {
	    glDetachObjectARB(mProgramObject, mFragmentShader);
	    glDeleteObjectARB(mFragmentShader);
	    mFragmentShader = 0;
    }

    // If our geometry shader pointer is valid, free it
	if ( mGeometryShader ) {
	    glDetachObjectARB(mProgramObject, mGeometryShader);
	    glDeleteObjectARB(mGeometryShader);
		mGeometryShader = 0;
	}

    // If our vertex shader pointer is valid, free it
    if ( mVertexShader ) {
	    glDetachObjectARB(mProgramObject, mVertexShader);
	    glDeleteObjectARB(mVertexShader);
	    mVertexShader = 0;
    }

    // If our program object pointer is valid, free it
    if ( mProgramObject ) {
	    glDeleteObjectARB(mProgramObject);
	    mProgramObject = 0;
    }
}

GLint Shader::variable(const std::string& variable)
{
    // If we don't have an active program object, let's return -1
	if ( !mProgramObject ) {
	    return -1;
	}

    // This returns the variable ID for a variable that is used to find
    // the address of that variable in memory.
    return glGetUniformLocationARB(mProgramObject, variable.c_str());
}


}
