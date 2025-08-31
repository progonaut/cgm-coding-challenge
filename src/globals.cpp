#include "globals.h"

#include<format>

namespace cgm {

bool verbose = false;

TooLongException::TooLongException(std::string msg ) noexcept : std::length_error{ msg } {}
TooLongException::TooLongException(
  std::size_t length, std::size_t max_length, std::string additional_msg ) noexcept
    : std::length_error{ std::format(
        "Input of length {} exceeds allowed maximum length of {} max_length. {}", length, max_length, additional_msg ) } {}

TooShortException::TooShortException(std::string msg ) noexcept : std::length_error{ msg } {}
TooShortException::TooShortException(
  std::size_t length, std::size_t min_length, std::string additional_msg ) noexcept
    : std::length_error{ std::format(
        "Input of length {} is shorter than the allowed minimum length of {}. {}", length, min_length, additional_msg ) } {}

}  // namespace cgm
