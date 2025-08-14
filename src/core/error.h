#pragma once

#include <string>
#include <vector>

namespace error {

struct Error {
  std::string label;
  std::string log;
};

std::string MergeErrors(std::vector<Error>& errors);

}  // namespace error