#include <Infra/glErrors/glErrors.hpp>
#include <Infra/Shader/Shader.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

std::vector<unsigned int> Shader::TotalRendererIDList_;

std::string getDirectory(std::string filepath) {
	std::string directory;
	const size_t last_slash_idx = filepath.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		directory = filepath.substr(0, last_slash_idx);
	}
	return directory;
}

void ShaderProgramSource::saveShaders(std::string& folder) {

	std::string vertexSourceSave = folder + "/vertex.glsl";
	std::string fragmentSourceSave = folder + "/fragment.glsl";

    std::ofstream outVertex(vertexSourceSave);
    outVertex << VertexSource;
    outVertex.close();

    std::ofstream outFragment(fragmentSourceSave);
    outFragment << FragmentSource;
    outFragment.close();

}

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0), m_LoadFailedFlag(false) {

	ShaderProgramSource source = ParseShader(filepath);

	std::string folder = getDirectory(filepath);
	source.saveShaders(folder);

	glCall(m_RendererID=CreateShader(source.VertexSource,source.FragmentSource));

	if(m_RendererID) TotalRendererIDList_.push_back(m_RendererID);
	else m_LoadFailedFlag=true;

}

Shader::~Shader() {
	glCall(glDeleteProgram(m_RendererID));
	if(m_RendererID) {
		auto it = std::find(TotalRendererIDList_.begin(), TotalRendererIDList_.end(), m_RendererID);
		TotalRendererIDList_.erase(it);
	}
}

bool Shader::isLoadFailed() const {return m_LoadFailedFlag;}

void Shader::Bind() const {
	glCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const {
	glCall(glUseProgram(0));
}

// Set Uniforms //
void Shader::SetUniformVec3f(const std::string& name,const glm::vec3& data) {
	glCall(glUniform3f(GetUniformLocation(name),data.x,data.y,data.z));
}
void Shader::SetUniformVec2f(const std::string& name,const glm::vec2& data) {
	glCall(glUniform2f(GetUniformLocation(name),data.x,data.y));
}
void Shader::SetUniform1iv(const std::string& name,unsigned int count ,const int* data) {
	glCall(glUniform1iv(GetUniformLocation(name),count,data));
}
void Shader::SetUniform1i(const std::string& name,int value) {
	glCall(glUniform1i(GetUniformLocation(name),value));
}
void Shader::SetUniform1f(const std::string& name,float value) {
	glCall(glUniform1f(GetUniformLocation(name),value));
}
void Shader::SetUniform4f(const std::string& name,float v0,float v1,float v2,float v3) {
	glCall(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3));
}
void Shader::SetUniformMat4f(const std::string& name,const glm::mat4& matrix) {
	glCall(glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&matrix[0][0]));
}


unsigned int Shader::GetUniformLocation(const std::string& name) const {
	/*Setting Data in Shader*/
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

    int location;
	glCall(location = glGetUniformLocation(m_RendererID,name.c_str()));
	if(location == -1) {
        stringstream err;
		std::cerr << "Warning : uniform "<< name << " dosent exist" << std::endl;
	}
	
	m_UniformLocationCache[name] = location;

	return location;
}


ShaderProgramSource Shader::ParseShader(const std::string filePath) {

	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};	

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while(getline(stream,line)) {
		if(line.find("#shader") != std::string::npos) {
			if(line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n"; 
		}
	}

	return {ss[0].str(),ss[1].str()};

}

unsigned int Shader::CompileShader(unsigned int type,const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	/* sharder id , num shaders , source pointer, no array lenths required */
	glCall(glShaderSource(id,1,&src,nullptr));
	glCall(glCompileShader(id));

	int result;
	glCall(glGetShaderiv(id,GL_COMPILE_STATUS,&result));
	if (result == GL_FALSE) {
		int length;
		glCall(glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length));
		char* message = (char *)alloca(length*sizeof(char));

		glCall(glGetShaderInfoLog(id,length,&length,message));

        stringstream err;
		err << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<std::endl;
		err << message <<std::endl;
        throw runtime_error(err.str());

		glCall(glDeleteShader(id));
		return 0;

	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader,const std::string& fragmentShader )  {
    unsigned int program;
    unsigned int vs ;
    unsigned int fs;
    stringstream err;

	glCall(program = glCreateProgram());
	glCall(vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	glCall(fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	if(vs == 0 || fs == 0) {
		glCall(glDeleteProgram(program));
		program = 0;
		if (vs != 0) glCall(glDeleteShader(vs));
		if (fs != 0) glCall(glDeleteShader(fs));
		return 0;
	}
	glCall(glAttachShader(program,vs));
	glCall(glAttachShader(program,fs));
	glCall(glLinkProgram(program));

	int result;
	bool flag_error = false;
	glCall(glGetProgramiv(program,GL_LINK_STATUS,&result));
	if (result == GL_FALSE) {
		int length = 0;
		glCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = (char *)alloca(length*sizeof(char));

		err << "Failed to link program" <<std::endl;
		err << message <<std::endl;
        throw runtime_error(err.str());

		flag_error = true;
	}

	glCall(glValidateProgram(program));
	glCall(glGetProgramiv(program, GL_VALIDATE_STATUS,&result));
	if (result == GL_FALSE) {
		flag_error = true;

        err << "Failed to validate program" <<std::endl;
        throw runtime_error(err.str());
	}

	if(flag_error) {
		glCall(glDeleteProgram(program));
		program = 0;
	}

	glCall(glDeleteShader(vs));
	glCall(glDeleteShader(fs));

	return program;
}