#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "node.h"

namespace node {
class ShaderNode;
}

namespace node::registry {

struct NodeEntry {
  NodeInfo spec;
  std::function<std::shared_ptr<ShaderNode>()> Create;
};

/// Register built-in Nodes
void RegisterAllNodes();

void RegisterNode(const NodeEntry &entry);

std::vector<std::string> GetNodeNames();

std::optional<NodeEntry> GetNodeEntry(const std::string &name);

std::shared_ptr<ShaderNode> CreateNode(const std::string &name);

std::shared_ptr<ShaderNode> CreateNode(const NodeInfo &info);

}  // namespace node::registry
