#include "shader_manager.h"

#include <filesystem>

#include "logger.h"
#include "shader_loader.h"

namespace {

void CompileShader(Shader& shader) {
  shader.isDirty = false;

  ShaderCompileResult result;

  switch (shader.type) {
    case ShaderType::ComputeShader:
      result = MakeComputeShader(shader.path);
      break;
    case ShaderType::FragShader:
      result = MakeFragShader(shader.path);
      break;
    default:
      result = {
          .isSuccess = false,
          .program = 0,
          .error = "Not Implemented ShaderType",
      };
      break;
  }

  if (!result.isSuccess) {
    shader.isValid = false;
    shader.error = error::Error{
        .label = shader.path,
        .log = result.error,
    };
    return;
  }

  shader.isValid = true;
  shader.program = result.program;
}

}  // namespace

std::shared_ptr<ShaderManager> ShaderManager::Create() {
  return std::shared_ptr<ShaderManager>(new ShaderManager());
}

bool ShaderManager::HasCompileErrors() { return compileErrors.size(); }

std::shared_ptr<Shader> ShaderManager::GetShader(std::string path,
                                                 ShaderType type) {
  if (path.empty()) return nullptr;

  Key id = path;

  if (auto cached = GetShaderFromCache(id); cached) {
    return cached;
  }

  if (auto shader = CreateShader(path, id, type); shader) {
    Watch(*shader);
    cache[id] = shader;
    needRefresh = true;
    return shader;
  }

  return nullptr;
}

void ShaderManager::Refresh(std::vector<error::Error>& reports) {
  compileErrors.clear();

  std::erase_if(cache, [](auto& item) { return item.second.expired(); });

  for (auto& [key, shaderCache] : cache) {
    if (auto shader = shaderCache.lock()) {
      if (shader->isDirty) {
        CompileShader(*shader);
      };

      if (!shader->isValid && shader->error) {
        compileErrors.emplace_back(shader->error.value());
      }
    }
  }

  reports.insert(reports.end(), compileErrors.begin(), compileErrors.end());

  needRefresh = false;
}

std::shared_ptr<Shader> ShaderManager::CreateShader(const std::string& path,
                                                    const std::string& id,
                                                    ShaderType type) {
  if (path.empty() || !std::filesystem::exists(path)) return nullptr;

  auto deleter = [weakSelf = weak_from_this()](Shader* ptr) {
    if (auto self = weakSelf.lock()) {
      self->DeleteShader(*ptr);
    }
    delete ptr;
  };

  std::shared_ptr<Shader> shader(new Shader(), deleter);

  shader->id = id;
  shader->path = path;
  shader->type = type;
  shader->isDirty = true;

  return shader;
}

void ShaderManager::DeleteShader(Shader& sp) {
  watchers.erase(sp.id);
  cache.erase(sp.id);
  DeleteProgram(sp.program);
}

std::shared_ptr<Shader> ShaderManager::GetShaderFromCache(const Key& path) {
  if (cache.contains(path)) return cache[path].lock();
  return nullptr;
}

void ShaderManager::Watch(Shader& shader) {
  /**
   * Debounce needed for modified event firing 2 times.
   * see: https://github.com/ThomasMonkman/filewatch/issues/27
   * They said it's Windows API related, but it happens on Linux too.
   */

  shader.lastModified = glfwGetTime();

  auto callback = [this, &shader]([[maybe_unused]] const std::string& path,
                                  const filewatch::Event changeType) {
    if (changeType != filewatch::Event::modified) return;
    float now = glfwGetTime();
    if (now - shader.lastModified < .1f) return;

    needRefresh = true;
    shader.isDirty = true;
    shader.lastModified = now;
  };

  watchers[shader.id] = std::make_unique<FileWatch>(shader.path, callback);
}
