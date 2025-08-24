#include "node_registry.h"

#include <unordered_map>

#include "shader_node.h"

namespace {

using namespace node;
using namespace node::registry;

std::unordered_map<std::string, NodeEntry> entries;

void CopyNodeInfo(NodeInfo& dst, const NodeInfo& src) {
  if (!src.label.empty()) {
    dst.label = src.label;
  }

  if (!src.shaderPath.empty()) {
    dst.shaderPath = src.shaderPath;
  }

  dst.active = src.active;
  dst.initialized = src.initialized;
}

}  // namespace

namespace node::registry {

void RegisterNode(const NodeEntry& entry) { entries[entry.spec.name] = entry; }

std::optional<NodeEntry> GetNodeEntry(const std::string& name) {
  if (entries.contains(name)) {
    return entries[name];
  }
  return std::nullopt;
}

std::vector<std::string> GetNodeNames() {
  std::vector<std::string> result;
  for (auto& [name, _] : entries) {
    result.push_back(name);
  }
  return result;
}

std::shared_ptr<ShaderNode> CreateNode(const std::string& name) {
  auto entry = registry::GetNodeEntry(name);
  if (!entry) return nullptr;

  auto node = entry.value().Create();
  CopyNodeInfo(*node, node->GetSpec());
  return node;
}

std::shared_ptr<ShaderNode> CreateNode(const NodeInfo& info) {
  auto node = CreateNode(info.name);
  if (!node) return nullptr;

  CopyNodeInfo(*node, info);
  return node;
}

}  // namespace node::registry
