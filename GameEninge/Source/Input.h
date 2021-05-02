#pragma once

#include <GLFW\glfw3.h>
#include <functional>
#include <vector>

struct Input {
  GLFWwindow* window;
  std::function<void(int)> callback;
  std::vector<int> keys_to_track;
};

Input GetInput(GLFWwindow* window, std::function<void(int)> callback);
void InputAddKeyToTrack(Input *input, int key);
void InputUpdate(Input *input, float dt);