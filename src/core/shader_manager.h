#pragma once

#include <FileWatch/FileWatch.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "error.h"

struct Shader;

class ShaderManager : public std::enable_shared_from_this<ShaderManager> {
  using FileWatch = filewatch::FileWatch<std::string>;
  using Key = std::string;

 public:
  bool needRefresh = false;

  static std::shared_ptr<ShaderManager> Create();

  bool HasCompileErrors();

  std::shared_ptr<Shader> GetShader(std::string path);

  void Refresh(std::vector<error::Error>& reports);

 private:
  std::unordered_map<Key, std::weak_ptr<Shader>> cache;
  std::unordered_map<Key, std::unique_ptr<FileWatch>> watchers;

  std::vector<error::Error> compileErrors;

  ShaderManager() = default;

  std::shared_ptr<Shader> CreateShader(const std::string& path,
                                       const std::string& id);

  void DeleteShader(Shader& sp);

  std::shared_ptr<Shader> GetShaderFromCache(const Key& path);

  void Watch(Shader& shader);
};
