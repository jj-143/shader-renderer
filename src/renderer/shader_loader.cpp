#include "shader_loader.h"

#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "global.h"
#include "logger.h"

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

std::expected<std::vector<std::string>, std::string> PrepareSources(
    const std::string& filepath, [[maybe_unused]] GLuint moduleType) {
  std::vector<std::string> result;

  std::vector<ShaderSource> sources = {
      {.text = "#version 430"},
      {.text = "#extension GL_ARB_shading_language_include : enable"},
      global::CHUNK_COMMON,
      {.text = "#line 1"},
      {.path = filepath},
      global::CHUNK_COMMON_POST,
  };

  for (auto& file : sources) {
    std::optional<std::string> content;

    if (!file.path.empty()) {
      content = ReadFileContent(file.path);
      if (!content) {
        return std::unexpected(
            std::format("Cannot read file \"{}\"", file.path));
      }
    } else {
      content = file.text + "\n";
    }
    result.push_back(content.value());
  }

  return result;
}

ShaderCompileResult MakeModule(const std::string& filepath, GLuint moduleType) {
  // Prepare sources
  auto sources = PrepareSources(filepath, moduleType);

  if (!sources) {
    return ShaderCompileResult{
        .isSuccess = false,
        .program = 0,
        .error = sources.error(),
    };
  }

  // Prepare arguments
  std::vector<const GLchar*> srcPtrs;

  for (const auto& src : *sources) {
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
    return ShaderCompileResult{
        .isSuccess = false,
        .program = 0,
        .error = errorLog,
    };
  }

  return ShaderCompileResult{.isSuccess = true, .program = shaderModule};
}

ShaderCompileResult MakeShader(const std::string& vertexFilepath,
                               const std::string& fragmentFilepath) {
  std::vector<GLuint> modules;
  auto vertResult = MakeModule(vertexFilepath, GL_VERTEX_SHADER);
  auto fragResult = MakeModule(fragmentFilepath, GL_FRAGMENT_SHADER);

  if (!vertResult.isSuccess) return vertResult;
  if (!fragResult.isSuccess) return fragResult;

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
    logger::Error("Shader Linking error: {}", errorLog);
    return {
        .isSuccess = false,
        .program = 0,
        .error = errorLog,
    };
  }

  // shaders are separate from module
  // delete them after linking
  for (GLuint shaderModule : modules) {
    glDeleteShader(shaderModule);
  }

  return {.isSuccess = true, .program = shader};
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

void DeleteProgram(GLuint program) { glDeleteProgram(program); }