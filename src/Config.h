#pragma once
// clang-format off
// order matters
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <format>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Camera.h"
#include "DebugStat.h"
#include "Postprocess.h"
#include "ShaderLoader.h"
#include "UI.h"