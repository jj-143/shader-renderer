#include "ShaderLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

GLuint MakeModule(const std::string& filepath, GLuint moduleType) {
  std::ifstream file;
  std::stringstream bufferedLines;
  std::string line;

  file.open(filepath);
  while (std::getline(file, line)) {
    bufferedLines << line << "\n";
  }
  std::string shaderSource = bufferedLines.str();
  const char* shaderSrc = shaderSource.c_str();
  bufferedLines.str("");
  file.close();

  GLuint shaderModule = glCreateShader(moduleType);
  glShaderSource(shaderModule, 1, &shaderSrc, nullptr);
  glCompileShader(shaderModule);
  int success;

  // iv = Integer Value
  glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
  if (!success) {
    char errorLog[1024];
    glGetShaderInfoLog(shaderModule, 1024, nullptr, errorLog);
    std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
  }

  return shaderModule;
}

GLuint MakeShader(const std::string& vertexFilepath,
                  const std::string& fragmentFilepath) {
  std::vector<GLuint> modules;
  modules.push_back(MakeModule(vertexFilepath, GL_VERTEX_SHADER));
  modules.push_back(MakeModule(fragmentFilepath, GL_FRAGMENT_SHADER));

  GLuint shader = glCreateProgram();
  for (GLuint shaderModule : modules) {
    glAttachShader(shader, shaderModule);
  }
  glLinkProgram(shader);

  int success;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (!success) {
    char errorLog[1024];
    glGetProgramInfoLog(shader, 1024, nullptr, errorLog);
    std::cout << "Shader Linking error:\n" << errorLog << std::endl;
  }

  // shaders are separate from module
  // delete them after linking
  for (GLuint shaderModule : modules) {
    glDeleteShader(shaderModule);
  }

  return shader;
}

GLuint MakeComputeShader(const std::string& filepath) {
  GLuint module = MakeModule(filepath, GL_COMPUTE_SHADER);
  GLuint shader = glCreateProgram();
  glAttachShader(shader, module);
  glLinkProgram(shader);

  int success;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (!success) {
    char errorLog[1024];
    glGetProgramInfoLog(shader, 1024, nullptr, errorLog);
    std::cout << "Shader Linking error:\n" << errorLog << std::endl;
  }

  // shaders are separate from module
  // delete them after linking
  glDeleteShader(module);
  return shader;
}