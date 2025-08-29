#pragma once

/**
 * CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 */

#if !defined( MAX_LENGTH )
#define MAX_LENGTH 255
#endif

#include <exception>
#include <stdexcept>

namespace cgm {

extern bool verbose;

class TooLongException : public std::length_error {
	public:
	TooLongException( std::string msg ) noexcept;
	TooLongException(
	  std::size_t length, std::size_t max_length, std::string additional_msg = "" ) noexcept;
};

class TooShortException : public std::length_error {
	public:
	TooShortException( std::string msg ) noexcept;
	TooShortException(
	  std::size_t length, std::size_t min_length, std::string additional_msg = "" ) noexcept;
};
}  // namespace cgm
