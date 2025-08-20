#include "node_entries.h"

namespace node::registry {

void RegisterAllNodes() {
  RegisterComputeShaderNode();
  RegisterFragShaderNode();
}

}  // namespace node::registry