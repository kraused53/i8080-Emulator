#include "Logger.h"

#include <iostream>
#include <chrono>

std::string get_date_time( void ) {
  auto raw_time = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t( raw_time );

  char time_buff[ 30 ];

  ctime_s( time_buff, sizeof( time_buff ), &time );

  // remove \n
  for ( int index = 0; index < sizeof( time_buff ); index++ ) {
    if ( time_buff[ index ] == '\n' ) {
      time_buff[ index ] = '\0';
    }
  }

  return time_buff;
}

void Logger::Log( const std::string& message ) {
  std::string log = " INFO: [ " + get_date_time() + " ] - " + message;

  // Print log message to terminal
  std::cout << "\x1b[32m" << log << "\033[0m" << std::endl;
}

void Logger::Err( const std::string& message ) {
  // Create new log entry
  std::string err = "ERROR: [ " + get_date_time() + " ] - " + message;

  std::cout << "\x1b[31m" << err << "\033[0m" << std::endl;
}