#ifndef SHADERTEXTURE_H
#define SHADERTEXTURE_H

#include "../Shader.h"
#include <client/components/graphics/Texture.h>
#include <client/components/graphics/geometry/Geometry2d.h>
#include <tools/vmath.h>

class ShaderTexture : public Shader {
public:
	ShaderTexture(const std::string &file, const glm::uvec2 &size, const std::vector<std::string> &uniforms, const std::vector<Texture> &textures);
	ShaderTexture(const std::string &file, const glm::uvec2 &size, const std::string &uniform, const Texture &texture);
	~ShaderTexture() override;
	virtual void Render() override;

	operator const Texture&() const { return out; };

protected:
	std::vector<GLuint> colorUniforms;
	std::vector<Texture> colorTextures;

private:
	Texture out;

	Geometry2d screen;
	GLuint VAO,vbuffer,tbuffer;
};

#endif