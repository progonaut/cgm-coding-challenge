/**
 * CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 * 
 * UNIT TESTS FOR CATALOG
 */

#include"Test.h"
#include"../Catalog.h"
#include<stdexcept>
#include"../globals.h"

TEST(C_ADD_QUESTION_AND_QUERY) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2"} );
	return { c.getAnswersFor( "Q1?").size() == 2, ""};
} NOEXCEPT

TEST(C_QUERY_EMPTY) {
	cgm::Catalog c;

	return { c.getAnswersFor( "Q1?").size() == 1 && c.getAnswersFor("Q1?").begin()->starts_with("the answer to life, universe"), ""};
} NOEXCEPT

TEST(C_QUERY_NOT_FOUND) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2"} );
	return { c.getAnswersFor( "Q2?").size() == 1 && c.getAnswersFor("Q2?").begin()->starts_with("the answer to life, universe"), ""};
} NOEXCEPT

TEST(C_QUERY_MULTIPLE) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2"} );
	c.addQuestion("Q2?", {"A1","A2", "A3"} );
	return { c.getAnswersFor( "Q2?").size() == 3 && c.getAnswersFor("Q1?").size() == 2, "" };
} NOEXCEPT

TEST(C_ADD_MERGE_3 ) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2"} );
	auto [position, new_or_changed, nr] = c.addQuestion("Q1?", {"A1","A2", "A3"} );
	return { c.getAnswersFor( "Q1?").size() == 3
		&& c.getAnswersFor("Q1?") == std::list<std::string>{"A1","A2","A3"}
		&& position->getQuestionText() == "Q1?"
		&& new_or_changed == cgm::Catalog::NewOrChanged::Changed
		&& nr == 1
		, "" };
} NOEXCEPT

TEST(C_ADD_REPLACE_3 ) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2", "A3", "A4"} );
	auto [position, new_or_changed, nr]= c.addQuestion("Q1?", {"A1","A2", "A3"}, cgm::Catalog::MergeOrReplace::Replace  );
	return { c.getAnswersFor( "Q1?").size() == 3
		&& c.getAnswersFor("Q1?") == std::list<std::string>{"A1","A2","A3"}
		&& position->getQuestionText() == "Q1?"
		&& new_or_changed == cgm::Catalog::NewOrChanged::Changed
		&& nr == 3
		, "" };
} NOEXCEPT

TEST(C_ADD_MERGE_QUESTION_NO_ANS ) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {} );
	auto [position, new_or_changed, nr]= c.addQuestion("Q1?", {"A1","A2", "A3"}, cgm::Catalog::MergeOrReplace::Replace  );
	return { c.getAnswersFor( "Q1?").size() == 3
		&& c.getAnswersFor("Q1?") == std::list<std::string>{"A1","A2","A3"}
		&& position->getQuestionText() == "Q1?"
		&& new_or_changed == cgm::Catalog::NewOrChanged::Changed
		&& nr == 3
		, "" };
} EXPECT(std::invalid_argument);

TEST(C_ADD_MERGE_QUESTION_INVALID ) {
	cgm::Catalog c;

	c.addQuestion("ThisIsFarTooLongForTheTest", {"A1"} );
	return { false, "Expected exception 'TooLongException'" };
} EXPECT(cgm::TooLongException);

TEST(C_ADD_MERGE_QUESTION_INVALID_SHORT ) {
	cgm::Catalog c;

	c.addQuestion("?", {"A1"} );
	return { false, "Expected exception 'TooShortException'" };
} EXPECT(cgm::TooShortException);

TEST(C_ADD_MERGE_QUESTION_INVALID_ANS_SHORT ) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {""} );
	return { false, "Expected exception 'TooShortException'" };
} EXPECT(cgm::TooShortException);

TEST(C_ADD_MERGE_QUESTION_INVALID_ANS_LONG ) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"123123123123123123123123123123"} );
	return { false, "Expected exception 'TooLongException'" };
} EXPECT(cgm::TooLongException);

TEST(C_ADD_MERGE_NON_UNIQUE) {
	cgm::Catalog c;

	c.addQuestion("Q1?", {"A1","A2", "A3", "A4"} );
	auto [position, new_or_changed, nr]= c.addQuestion("Q1?", {"A1","A2", "A3"}, cgm::Catalog::MergeOrReplace::Merge, false );
	return { c.getAnswersFor( "Q1?").size() == 7
		&& c.getAnswersFor("Q1?") == std::list<std::string>{"A1","A2","A3", "A4", "A1", "A2", "A3"}
		&& position->getQuestionText() == "Q1?"
		&& new_or_changed == cgm::Catalog::NewOrChanged::Changed
		&& nr == 3
		, "" };
} NOEXCEPT;
