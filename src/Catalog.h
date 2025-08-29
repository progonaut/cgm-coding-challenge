#pragma once

/**
 * CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 *
 * A Catalog stores a set of Questions and offers methods to add questions (addQuestion) and retrieve answers
 * (getAnswersFor). Catalog also maintains the requirement of returning a default answer for
 * questions that are not found within the Catalog.
 */
#include <set>
#include <tuple>

#include "Question.h"
#include "globals.h"

namespace cgm {
class Catalog {
  public:
	// Catalog can use a std::set as its container, as questions are to be unique. It also uses the
	// custom comparator provided by Question, which compares equal if two Questions have the same Question
	// text, even if other parameters, most notably the set of associated answers, differ.
	using CatalogContainerType = std::set<Question, decltype( Question::AnswerAgnosticComparator )>;
	using OnlyUnique           = bool;

	enum class MergeOrReplace { Merge, Replace };

	enum class NewOrChanged { New, Changed };

	Catalog( std::size_t max_size = MAX_LENGTH ) noexcept;

	~Catalog( )                = default;
	Catalog( Catalog const & ) = delete;
	Catalog( Catalog && )      = delete;

	std::tuple<typename CatalogContainerType::const_iterator, NewOrChanged, std::size_t> addQuestion(
	  std::string question_text, Question::AnswerSet &&answers, MergeOrReplace merge_or_replace = MergeOrReplace::Merge,
	  OnlyUnique unique = true );

	std::tuple<typename CatalogContainerType::const_iterator, NewOrChanged, std::size_t> addQuestion(
	 Question &&question, MergeOrReplace merge_or_replace = MergeOrReplace::Merge,
	  OnlyUnique unique = true );

	Question::AnswerSet const &getAnswersFor( std::string question_text ) const noexcept;

  private:
	CatalogContainerType _questions;
	std::size_t _max_length_of_contained_questions;
};
}  // namespace cgm
