#include "Input.h"
#include "Utils.h"

MakeInternal Input GetInput(GLFWwindow* window, std::function<void(int)> callback) {
  Input input;

  input.window = window;
  input.callback = callback;

  return input;
}

MakeInternal void InputAddKeyToTrack(Input *input, int key) {
  input->keys_to_track.push_back(key);
}

MakeInternal bool IsPressed(Input *input, int key) {
  int state = glfwGetKey(input->window, key);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

MakeInternal void InputUpdate(Input *input, float dt) {
  for (auto key : input->keys_to_track) {
    if (IsPressed(input, key)) {
      input->callback(key);
    }
  }
}