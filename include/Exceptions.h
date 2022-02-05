#pragma once
#include <exception>

struct python_error : public std::exception {
  python_error()  {}
  
};