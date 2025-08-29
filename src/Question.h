#pragma once

/**
 * CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * Question represents a Question in the form of text, as well as a (possible) set of answers.
 *
 * Question provides a custom comparator, AnswerAgnosticComparator, which represents equality
 * of questions based solely on their text. This is useful in the context of using std::set to
 * store multiple questions in Catalog.
 */

#include <cstddef>
#include <string>
#include <list>

#include "globals.h"

#include<iostream>

namespace cgm {
class Question {
  public:

	using AnswerSet = std::list<std::string>;//< AnswerSet is using a list because of O(1) insertion (at the end),
											 // removal, and traversal; if large lists are to be saved,
											 // the use of a custom allocator or a different container type 
											 // may be advisable; especially if the uniqueness constraint 
											 // that has a worst-case O(n^2) becomes a performance constraint,
											 // containers like *set* could be used. However, unlike std::list, std::set
											 // does not preserve insertion order

	// Constructs a question
	// A question without answers is technically not desired; however, that is not a concern of question itself
	// but rather has to be implemented in f.e. the catalog. Together with the custom comparator defined below,
	// Question provides ease of use with container types, in particular with various look-up methods (find, ...)
	// In order to facilitate the use of Question to allow or deny empty answer sets, @in allow_empty can be set to 
	// true or false, respectively.
	//
	// @in question_text The question to be asked/stored
	// @in answers A set of answers to the question
	// @in max_size A length constraint for question and answer texts
	// @in allow_empty a switch to allow or deny empty answer sets 
	Question( std::string question_text, AnswerSet answers = { }, std::size_t max_size = MAX_LENGTH, bool allow_empty = true );

	~Question( ) noexcept        = default;
	Question( Question const & ) = default;
	Question( Question && )      = default;

	AnswerSet const &getAnswers( ) const noexcept;
	inline AnswerSet &getAnswers( ) noexcept {
		return const_cast<AnswerSet &>( const_cast<Question const &>( *this ).getAnswers( ) );
	}

	inline std::string const& getQuestionText() const noexcept {
		return _question_text;
	}

	// A custom comparator that can be used with STL-containers, or STL-algorithms, that
	// compares Questions based on their question text only, rather than the (default/standard) 
	// element-wise comparison that would include answers and the max_size constraint in the
	// comparison.
	//
	// Note that the use of this comparator with STL containers implies that insertion into the container
	// *MUST* consider that two Question objects can compare equal when having different answers, 
	// and *MUST* decide what to do in such a case (f.e. overwrite the "older" answers, or merge them)
	static struct {
		bool operator( )( Question const &lhs, Question const &rhs ) const noexcept {
			return lhs._question_text < rhs._question_text;
		}
	} AnswerAgnosticComparator;

  private:
	void checkLengthInvariant( ) const;  //< throws on error

	std::string _question_text;
	AnswerSet _answers;
	std::size_t _max_size_of_text;
};
}  // namespace cgm
