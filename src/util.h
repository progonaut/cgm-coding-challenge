#pragma once

/** CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 * 
 * Various simple utilities in lieu of including and linking existing libraries
 *
 **/

#include <sstream>
#include <stdexcept>
#include<string>
#include<utility>
#include<concepts>
#include<type_traits>

namespace cgm {
	namespace util {

		/** buildString - Simple function to concatinate strings with non-string (primarily numeric) variables
		 **/
		template<typename... Ts>
		std::string buildString( Ts&&... args ) {

			auto to_string = []( auto&& param ) {
				std::stringstream ss;
				ss << std::forward<decltype(param)>(param);
				return ss.str();
			};

			std::string return_string;
			(return_string.append(to_string(std::forward<Ts>(args))),...);
			return return_string;
		}

		/** joinBetween - Returns a string of characters found between two delimiters, or before one delimiter, including
		 * delimiters.
		 */
		template<std::input_iterator IT, std::input_iterator EIT>
		std::string joinBetween( std::string_view begin_delimiter, std::string_view end_delimiter, std::string spacer, IT& begin, EIT end ) {
			if( begin == end ) return {};

			if( begin_delimiter.length() != 0 && !begin->starts_with( begin_delimiter ) ) {
				throw std::invalid_argument( buildString("String does not begin with '", begin_delimiter, "', but '", *(begin->begin()), "'."));
			}

			std::string joined_string{*(begin++)};
			while( begin != end && ( !joined_string.ends_with(end_delimiter) || joined_string.length() <= begin_delimiter.length() + end_delimiter.length() )) {
				joined_string += spacer + *(begin++);
			}

			return joined_string;

		}

	}}
