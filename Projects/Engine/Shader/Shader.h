
#ifndef _Shader_Shader_h_
#define _Shader_Shader_h_


// Library includes
#include <string>

// Project includes
#include <Common/Logger.h>
#include <Driver/GLWrapper.h>
#include <Interfaces/Core/Object.h>

// Forward declarations

// Namespace declarations


namespace ShaderSpace {


static const std::string FragmentShader = "shader.frag";
static const std::string GeometryShader = "shader.geo";
static const std::string VertexShader   = "shader.vert";


class Shader : public Core::Object,
			   private Common::Logger
{
public:
	Shader(const Common::Logger *p, const std::string& name, const std::string& path);
	~Shader();

public:
	GLhandleARB program();
	GLint variable(const std::string& variable);

	void begin();
	void end();
	void set(const std::string& name, bool value);
	void set(const std::string& name, float value);
	void set(const std::string& name, int value);

protected:

private:
	void init(const std::string& path);
	std::string load(const std::string& filename);
	void unload();

private:
	GLhandleARB	mFragmentShader;
    GLhandleARB	mGeometryShader;
	GLhandleARB	mProgramObject;
	GLhandleARB	mVertexShader;
};


}

#endif
