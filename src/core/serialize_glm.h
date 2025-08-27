#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace glm {

// glm::vec
template <glm::length_t N, typename T, glm::qualifier Q>
void to_json(nlohmann::json& j, const glm::vec<N, T, Q>& v) {
  j = nlohmann::json::array();
  for (glm::length_t i = 0; i < N; ++i) {
    j.push_back(v[i]);
  }
}

template <glm::length_t N, typename T, glm::qualifier Q>
void from_json(const nlohmann::json& j, glm::vec<N, T, Q>& v) {
  for (glm::length_t i = 0; i < N; ++i) {
    v[i] = j.at(i).get<T>();
  }
}

// glm::mat, using glm::vec serializer
template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
void to_json(nlohmann::json& j, const glm::mat<C, R, T, Q>& m) {
  j = nlohmann::json::array();
  for (glm::length_t i = 0; i < C; ++i) {
    j.push_back(m[i]);
  }
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
void from_json(const nlohmann::json& j, glm::mat<C, R, T, Q>& m) {
  for (glm::length_t i = 0; i < C; ++i) {
    m[i] = j.at(i).get<glm::vec<R, T, Q>>();
  }
}

}  // namespace glm
