
#include <string>

namespace Ops {

/// Cancel Current running task in [App::taskManager]
bool CancelTask();

/// Load / Replace currently running shader
bool LoadShader(std::string path);

/// Open Dialog for loading a shader.
bool OpenOpenShaderDialog();

/// Show/Hide every overlays, leaving only the scene texture
bool ShowOverlays(bool set);

/// Reload current shader and reset timeline.
/// NOTE: This should be called in the thread rendering
/// viewport texture.
bool ReloadShader();

/// Display a message in Status Bar.
bool Report(std::string message);

/// Render to output file.
/// if `animation` is set, renders frames in the range defined in the Setting.
/// Otherwise, renders current iTime in `Timeline`.
bool Render(bool animation = false);

/// Quit the App.
bool Quit();

}  // namespace Ops
