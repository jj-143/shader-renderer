#pragma once

#include <functional>
#include <string>

#include "project.h"

namespace ops {

/// Cancel Current running task in [App::taskManager]
bool CancelTask();

/// Defer small tasks outside to the UI rendering sequence
bool DeferUITask(std::function<void()> task);

/// Open Dialog for loading a shader or a project file
bool OpenOpenProjectDialog();

/// Open Dialog for saving current project
bool OpenSaveAsDialog();

/// Save current project, create new if the current is temporaryProject
bool SaveCurrentProject();

/// Show/Hide every overlays, leaving only the scene texture
bool ShowOverlays(bool set);

/// Set the content compile errors to overlay
bool SetErrorLog(const std::string& content);

bool SetErrorLog();

bool StopTimeline();

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
bool ReloadProject();

/// Render to output file.
/// if `animation` is set, renders frames in the range defined in the Setting.
/// Otherwise, renders current iTime in `Timeline`.
bool Render(bool animation = false);

/// Reset Camera's position & rotation to default values.
bool ResetCamera();

/// Quit the App.
bool Quit();

}  // namespace ops

#include "project.h"
#include "report.h"
