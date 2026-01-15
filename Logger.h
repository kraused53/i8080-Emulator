#pragma once

#include <string>

class Logger {
private:
  // None so far

public:
  // Basic log
  static void Log( const std::string& message );
  // Error log
  static void Err( const std::string& message );
};