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


	// In order to add answers to questions, one must restate the question
	// For this reason, it becomes relevant whether or not the answers provided
	// with a question to be added should be merged or replace the existing answers 
	// if the question is already known
	// 
	// Additionally, when merging, duplicates can either be dropped or remain in the answer set
	//
	// @in question_text  
	// @in answers 
	// @in merge_or_replace Indicates whether two questions with the same question text should have their answers
	// merged, or if the newer question should overwrite the old
	// @in unique indicates whether the answer set is pruned of duplicates (unique = true) or retains them (unique =
	// false)
	std::tuple<typename CatalogContainerType::const_iterator, NewOrChanged, std::size_t> addQuestion(
	  std::string question_text, Question::AnswerSet &&answers, MergeOrReplace merge_or_replace = MergeOrReplace::Merge,
	  OnlyUnique unique = true );

	std::tuple<typename CatalogContainerType::const_iterator, NewOrChanged, std::size_t> addQuestion(
	 Question &&question, MergeOrReplace merge_or_replace = MergeOrReplace::Merge,
	  OnlyUnique unique = true );

	// getAnswersFor returns the stored answers for a given question
	// or a default answer if the question can not be found in the catalog
	Question::AnswerSet const &getAnswersFor( Question question ) const noexcept;

	// getAnswersFor is a helper function to allow searching for strings directly
	// without violating the noexcept guarantee of the main fucntion
	// for long input
	Question::AnswerSet const &getAnswersFor( std::string question_text ) const {
		return getAnswersFor( Question{question_text, {}} );
	}
  private:
	CatalogContainerType _questions;
	std::size_t _max_length_of_contained_questions;
};
}  // namespace cgm
