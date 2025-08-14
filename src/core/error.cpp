#include "error.h"

#include <format>

namespace error {

std::string MergeErrors(std::vector<error::Error>& errors) {
  std::string result = "";

  for (auto& error : errors) {
    result.append(std::format("[{}] {}\n\n", error.label, error.log));
  }

  return result;
}

}  // namespace error
