#include "ShaderLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

ShaderCompileResult MakeModule(const std::string& filepath, GLuint moduleType) {
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
  GLint success;

  // iv = Integer Value
  glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    char errorLog[1024];
    glGetShaderInfoLog(shaderModule, 1024, nullptr, errorLog);
    return ShaderCompileResult{.isSuccess = false, .error = errorLog};
  }

  return ShaderCompileResult{.isSuccess = true, .program = shaderModule};
}

GLuint MakeShader(const std::string& vertexFilepath,
                  const std::string& fragmentFilepath) {
  std::vector<GLuint> modules;
  auto vertResult = MakeModule(vertexFilepath, GL_VERTEX_SHADER);
  auto fragResult = MakeModule(fragmentFilepath, GL_FRAGMENT_SHADER);

  if (!vertResult.isSuccess || !fragResult.isSuccess) {
    return 0;
  }

  modules.push_back(vertResult.program);
  modules.push_back(fragResult.program);

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

ShaderCompileResult MakeComputeShader(const std::string& filepath) {
  ShaderCompileResult result = MakeModule(filepath, GL_COMPUTE_SHADER);
  if (!result.isSuccess) return result;

  GLuint module = result.program;
  GLuint shader = glCreateProgram();
  glAttachShader(shader, module);
  glLinkProgram(shader);

  int success;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  glDeleteShader(module);

  if (!success) {
    char errorLog[1024];
    glGetProgramInfoLog(shader, 1024, nullptr, errorLog);
    result.isSuccess = false;
    result.error = errorLog;
    return result;
  }

  result.isSuccess = true;
  result.program = shader;
  return result;
}