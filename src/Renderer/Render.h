#include "../app.h"
#include "../common.h"
#include "Renderer.h"

namespace Render {
class RenderContext {
 public:
  Renderer renderer;

  ~RenderContext();

  void Setup(const App::Config& config, const Camera& camera);
  void Teardown();

  /// Render current scene to internal buffer.
  void Render(float iTime);

  /// Write internal buffer to a file.
  /// @return true if success
  bool WriteToFile(const char* filename);

 private:
  struct Output {
    int width;
    int height;
  };

  GLubyte* data;
  GLFWwindow* window;
  Output output;
};
}  // namespace Render
