#include "Shader.h"

#include <client/components/graphics/Texture.h>
#include <fstream>
#include <shared/Console.h>
#include <shared/System.h>
#include <vector>

std::string filetobuf(std::string file) {
	std::ifstream in(file);
	if (!in.good())
		return "";
	return std::string((std::istreambuf_iterator<char>(in)),
	                   std::istreambuf_iterator<char>());
}
void logShader(GLuint id) {
	int maxLength;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
	char *infoLog = new char[maxLength];
	glGetShaderInfoLog(id, maxLength, &maxLength, infoLog);
	g_Console()->Err(std::string(infoLog));
	delete[] infoLog;
}
void logProgram(GLuint id) {
	int maxLength;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
	char *infoLog = new char[maxLength];
	glGetProgramInfoLog(id, maxLength, &maxLength, infoLog);
	g_Console()->Err(std::string(infoLog));
	delete[] infoLog;
}
bool endsWith(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 ==
		        fullString.compare(fullString.length() - ending.length(),
		                           ending.length(), ending));
	} else {
		return false;
	}
}
Shader::Shader(const std::string &filepath, glm::vec2 viewport, GLenum culling,
               GLboolean colormask0, GLboolean colormask1, GLboolean colormask2,
               GLboolean colormask3, GLboolean depthmask, GLbitfield clear) {
	registred.push_back(this);

	this->culling = culling;
	this->colormask[0] = colormask0;
	this->colormask[1] = colormask1;
	this->colormask[2] = colormask2;
	this->colormask[3] = colormask3;
	this->depthmask = depthmask;
	this->clear = clear;
	this->viewport = viewport;

	std::string libraries = "\n";
	std::vector<std::string> libs =
	    g_System()->GetFilesInDirectory(g_System()->GetDataFile("shaders"));
	for (auto &s : libs) {
		s = "shaders/" + s;
		if (endsWith(s, ".slib")) {
			std::string libsource = filetobuf(g_System()->GetDataFile(s));
			if (libsource.length() == 0)
				continue;
			g_Console()->Info("Shader library loaded " + s);
			libraries += libsource + "\n";
		}
	}
	std::string absolutepath = g_System()->GetDataFile(filepath);
	std::string vertexsource = libraries + filetobuf(absolutepath + ".vert");
	std::string fragmentsource = libraries + filetobuf(absolutepath + ".frag");

	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *ptr = vertexsource.c_str();
	glShaderSource(vertexshader, 1, (const GLchar **)&ptr, 0);
	glCompileShader(vertexshader);
	int IsCompiled_VS;
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == GL_FALSE) {
		logShader(vertexshader);
		return;
	}

	GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	ptr = fragmentsource.c_str();
	glShaderSource(fragmentshader, 1, (const GLchar **)&ptr, 0);
	glCompileShader(fragmentshader);
	int IsCompiled_FS;
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == GL_FALSE) {
		logShader(fragmentshader);
		return;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexshader);
	glAttachShader(id, fragmentshader);
	glLinkProgram(id);
	int IsLinked;
	glGetProgramiv(id, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == GL_FALSE) {
		logProgram(id);
		id = 0;
		return;
	}
	g_Console()->Info("Shader loaded " + filepath);
}
Shader::~Shader() {
	if (framebuffer)
		glDeleteFramebuffers(1, &framebuffer);
	glDeleteProgram(id);
	id = 0;
	registredModels.clear();
	registred.remove(this);
}
void Shader::RegisterModel(Model *model) { registredModels.insert(model); }
void Shader::UnregisterModel(Model *model) { registredModels.erase(model); }
void Shader::Render() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glCullFace(culling);
	glColorMask(colormask[0], colormask[1], colormask[2], colormask[3]);
	glDepthMask(depthmask);
	glUseProgram(id);
	glViewport(0, 0, viewport.x, viewport.y);
	glClear(clear);
}
std::list<Shader *> Shader::registred;
void Shader::RenderShaders() {
	for (Shader *&shader : registred)
		shader->Render();
}
void Shader::ClearShaders() {
	while (!registred.empty())
		delete registred.back();
}
GLuint Shader::Uniform(const std::string &name) {
	auto cached = std::find(uniformNames.begin(), uniformNames.end(), name);
	if (cached == uniformNames.end()) {
		uniformNames.push_back(name);
		uniformIds.push_back(glGetUniformLocation(*this, name.c_str()));
		cached = uniformNames.end() - 1;
	}
	return uniformIds[cached - uniformNames.begin()];
	// GLuint Shader::Uniform(const std::string &name) {
	// 	auto cached = uniforms.find(name);
	// 	if (cached == uniforms.end()) {
	// 		uniforms[name] = glGetUniformLocation(*this, name.c_str());
	// 		cached = uniforms.find(name);
	// 	}
	// 	return (*cached).second;
	// }
}
void Shader::SetAttribute(const std::string &name, SHADER_BINDINGS id) {
	glBindAttribLocation(*this, id, name.c_str());
}
void Shader::AddOutputTexture(Texture &t) {
	if (!framebuffer)
		glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	if (t.flags & TEXTURE_DEPTH) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t,
		                       0);
	} else {
		GLuint id = GL_COLOR_ATTACHMENT0 + colorTargets.size();
		glFramebufferTexture2D(GL_FRAMEBUFFER, id, GL_TEXTURE_2D, t, 0);
		colorTargets.push_back(id);
	}
	glReadBuffer(GL_NONE);
	glDrawBuffers(colorTargets.size(), colorTargets.data());
}

template <> void Shader::SetUniform(const std::string &name, const int &value) {
	glUniform1i(Uniform(name), value);
};
template <>
void Shader::SetUniform(const std::string &name, const float &value) {
	glUniform1f(Uniform(name), value);
};
template <>
void Shader::SetUniform(const std::string &name, const glm::vec2 &value) {
	glUniform2f(Uniform(name), value.r, value.g);
};
template <>
void Shader::SetUniform(const std::string &name, const glm::vec3 &value) {
	glUniform3f(Uniform(name), value.r, value.g, value.b);
};
template <>
void Shader::SetUniform(const std::string &name, const glm::vec4 &value) {
	glUniform4f(Uniform(name), value.r, value.g, value.b, value.a);
};
template <>
void Shader::SetUniform(const std::string &name, const glm::mat4 &value) {
	glUniformMatrix4fv(Uniform(name), 1, GL_FALSE,
	                   (const float *)glm::value_ptr(value));
};
