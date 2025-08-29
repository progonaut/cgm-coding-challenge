#include "Catalog.h"

#include <algorithm>

#include "util.h"
#include"globals.h"

#include<iostream>

namespace cgm {

namespace {
	Question::AnswerSet const default_answer_set = { "the answer to life, universe and everything is 42" };

	void mergeInto( Question::AnswerSet &lhs, Question::AnswerSet rhs, bool unique = true ) {
		for( auto &answer : rhs ) {
			if( !unique || std::find_if( lhs.begin( ), lhs.end( ), [&answer]( auto const& element ) { return element == answer; } ) == lhs.end( ) ) {
				lhs.push_back( std::move( answer ) );
			}
		}
	}
}  // namespace


Catalog::Catalog( std::size_t max_size ) noexcept : _max_length_of_contained_questions{ max_size } {}

std::tuple<typename Catalog::CatalogContainerType::const_iterator, Catalog::NewOrChanged, std::size_t>
Catalog::addQuestion(
  std::string question_text, Question::AnswerSet &&answers, MergeOrReplace merge_or_replace, OnlyUnique unique ) {
	return addQuestion( Question{ std::move(question_text), std::move(answers), _max_length_of_contained_questions, false }, merge_or_replace, unique );
}

std::tuple<typename Catalog::CatalogContainerType::const_iterator, Catalog::NewOrChanged, std::size_t>
Catalog::addQuestion(
  Question &&question, MergeOrReplace merge_or_replace, OnlyUnique unique ) {
	auto existing_question = _questions.find( question);

	std::size_t nr_answers_before{ 0 };

	bool is_new{ true };

	if( existing_question != _questions.end( ) ) {

		auto extracted_node = _questions.extract( existing_question );

		if( merge_or_replace == MergeOrReplace::Merge ) {
			nr_answers_before = extracted_node.value( ).getAnswers( ).size( );
			std::swap( extracted_node.value( ).getAnswers( ), question.getAnswers());
			mergeInto( question.getAnswers(), extracted_node.value( ).getAnswers( ), unique );
		} 


		// At the end of this scope, extracted_node is an owning handle to the question formerly in the catalog
		// and the answers-parameter holds the merged data in insertion order
		// semantically, this is considered an *update* rather than an insert of a new question

		is_new = false;

	}

	auto [position, inserted] = _questions.insert( std::move(question) );

	if( !inserted ) {
		throw std::runtime_error( util::buildString( "Unable to add question to catalog." ) );
	}

	return std::make_tuple(
	  position,
	  is_new ? NewOrChanged::New : NewOrChanged::Changed,
	  position->getAnswers( ).size( ) - nr_answers_before );
}

Question::AnswerSet const &Catalog::getAnswersFor( Question question_obj ) const noexcept {
	auto question = _questions.find( question_obj );

	if( question != _questions.end( ) ) {
		return question->getAnswers( );
	} else {
		return default_answer_set;
	}
}

}  // namespace cgm
