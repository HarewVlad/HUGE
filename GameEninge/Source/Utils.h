#pragma once

#include <string>
#include <functional>

#define MakeInternal static
#define MakeGlobal static

#define ASSERT(x) if (!(x)) __debugbreak();

#define glCall(x) \
  x;              \
  ASSERT(glHandleError(#x, __FILE__, __LINE__));

bool glHandleError(const char *function, const char *file, int line);

#define CONCAT_INTERNAL(x,y) x##y
#define CONCAT(x,y) CONCAT_INTERNAL(x,y)

template<typename T>
struct ExitScope {
  T lambda;
  ExitScope(T lambda) :lambda(lambda) {}
  ~ExitScope() { lambda(); }
  ExitScope(const ExitScope&);
private:
  ExitScope& operator =(const ExitScope&);
};

class ExitScopeHelp {
public:
  template<typename T>
  ExitScope<T> operator+(T t) { return t; }
};

#define defer const auto& CONCAT(defer__, __LINE__) = ExitScopeHelp() + [&]()

std::string UtilsSelectFolder();
void UtilsRunInDifferentThread(const std::function<void()> function);