#pragma once

#include <string>

#include "project.h"

namespace ops {

/// Cancel Current running task in [App::taskManager]
bool CancelTask();

/// Load / Replace currently running shader
bool LoadShader(std::string path, bool reload = false);

/// Load a single shader file as a new project
bool LoadSingleShaderProject(const std::string& path);

/// Open Dialog for loading a shader.
bool OpenOpenShaderDialog();

/// Show/Hide every overlays, leaving only the scene texture
bool ShowOverlays(bool set);

/// Align View to Render Output
///
/// Reset the viewport size (and window size) to render output size.
bool AlignViewportToOutput();

/// Align Output to Viewport
///
/// Set the render output size to viewport size.
bool AlignOutputToViewport();

/// Maximize the viewport to window size
bool MaximizeViewport(bool set);

/// Reload current shader and reset timeline.
/// NOTE: This should be called in the thread rendering
/// viewport texture.
bool ReloadShader();

/// Render to output file.
/// if `animation` is set, renders frames in the range defined in the Setting.
/// Otherwise, renders current iTime in `Timeline`.
bool Render(bool animation = false);

/// Reset Camera's position & rotation to default values.
bool ResetCamera();

/// Quit the App.
bool Quit();

}  // namespace ops

#include "report.h"
