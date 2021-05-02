#include "Utils.h"

#include <GL/glew.h>
#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <ShlObj.h>
#include <thread>

MakeInternal bool glHandleError(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL error] (" << error << ")" << function << " " << file
      << ":" << line << '\n';
    return false;
  }
  return true;
}

MakeInternal std::string UtilsSelectFolder() {
  std::string result;

  BROWSEINFO info;

  OleInitialize(NULL);
  memset((void *)&info, 0, sizeof(info));
  info.hwndOwner = GetForegroundWindow();
  info.lpszTitle = "Select";
  info.ulFlags = 0;
  info.lParam = 0;

  TCHAR display_name[MAX_PATH];
  info.pszDisplayName = display_name;

  ITEMIDLIST *pidl = SHBrowseForFolder(&info);
  if (pidl == NULL) return result;

  TCHAR path[MAX_PATH];
  if (SHGetPathFromIDList(pidl, path)) {
    result = path;
  }

  IMalloc *imalloc = NULL;
  if (SUCCEEDED(SHGetMalloc(&imalloc))) {
    imalloc->Free(pidl);
    imalloc->Release();
    imalloc = NULL;
  }

  return result;
}

MakeInternal void UtilsRunInDifferentThread(const std::function<void()> function) {
  std::thread thread([&]() {
    function();
  });
  thread.detach();
}