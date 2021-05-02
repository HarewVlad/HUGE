#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

MakeGlobal int Global_Width = 800;
MakeGlobal int Global_HWidth = Global_Width * 0.5f;
MakeGlobal int Global_Height = 600;
MakeGlobal int Global_HHeight = Global_Height * 0.5f;
MakeGlobal int Global_Grid_Amount = 10;
MakeGlobal std::string Global_Title = "Game!";
MakeGlobal float Global_M2P = 60.0f;
MakeGlobal float Global_P2M = 1.0f / Global_M2P;
MakeGlobal b2Vec2 Global_Gravity = { 0, -9.8f };
MakeGlobal int Global_Velocity_Iterations = 6;
MakeGlobal int Global_Position_Iterations = 2;
MakeGlobal float Global_MovementEpsilon = 0.2f;
MakeGlobal glm::mat4 Global_Rotate90 = glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
MakeGlobal glm::mat4 Global_RotateMinus90 = glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));