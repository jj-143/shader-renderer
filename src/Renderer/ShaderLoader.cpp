#include "ShaderLoader.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

#include "../Global.h"

std::optional<std::string> ReadFileContent(const std::string& filepath) {
  std::stringstream buffer;
  std::ifstream file;
  std::string line;

  file.open(filepath);
  if (!file.is_open()) return {};

  while (std::getline(file, line)) {
    buffer << line << "\n";
  }
  return buffer.str();
}

ShaderCompileResult MakeModule(const std::string& filepath, GLuint moduleType) {
  // Read file contents
  std::vector<std::string> sources;
  std::vector<std::string> paths = {Global::CHUNK_COMMON};
  const int nPrependingPaths = paths.size();

  // Append user file
  paths.push_back(filepath);

  // Read `paths` into `sources`
  for (int i = 0; i < paths.size(); i++) {
    auto& path = paths[i];
    std::string src;
    std::optional<std::string> content = ReadFileContent(path);

    if (!content) {
      return ShaderCompileResult{
          .isSuccess = false,
          .error = std::format("Cannot read file \"{:s}\"", path)};
    }

    // Prepend line directive for user paths
    bool isUserFile = i >= nPrependingPaths;

    if (isUserFile) {
      src.append(std::format("#line 1 \"USER_{:d}\"\n", i - nPrependingPaths));
    }

    src.append(content.value());

    sources.push_back(src);
  }

  // Prepare arguments
  std::vector<const GLchar*> srcPtrs;

  for (const auto& src : sources) {
    srcPtrs.push_back(src.c_str());
  }

  // Create shader program
  GLuint shaderModule = glCreateShader(moduleType);
  glShaderSource(shaderModule, srcPtrs.size(), srcPtrs.data(), nullptr);
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