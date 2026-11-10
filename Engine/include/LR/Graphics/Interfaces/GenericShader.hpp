#ifndef LRGENERICSHADER_HPP
#define LRGENERICSHADER_HPP

class GenericShader {
public:
	virtual ~GenericShader() = default;

	virtual void Bind() = 0;
};

#endif