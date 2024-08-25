#include "Ops.h"

#include <string>

#include "../app.h"

namespace Ops {

bool Report(std::string message) {
  App& app = App::GetInstance();
  app.ui.UpdateStatus(message);
  return true;
}

}  // namespace Ops
