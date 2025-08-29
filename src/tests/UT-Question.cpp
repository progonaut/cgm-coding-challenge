/**
 * CGM Coding Challenge
 * Author: wagner.matthias@gmx.at
 * 
 * UNIT TESTS FOR QUESTION
 */

#include"Test.h"
#include"../Question.h"
#include<stdexcept>
#include"../globals.h"

TEST(Q_length_10__1_answer) {
	cgm::Question q{"123456789?", {"Answer1234"}};
	return { q.getQuestionText() == "123456789?" && q.getAnswers().size() == 1 && *(q.getAnswers().begin()) == "Answer1234", ""};
} NOEXCEPT;

TEST(Q_length_10__no_answer) {
	cgm::Question q{"123456789?", {}};
	return { q.getQuestionText() == "123456789?" && q.getAnswers().size() == 0, ""};
} NOEXCEPT;

TEST(Q_length_10__no_answer_NEG) {
	cgm::Question q{"123456789?", {}, MAX_LENGTH, false};
	return { false, "Expected exception 'invalid_argument'" };
} EXPECT(std::invalid_argument);

TEST(Q_length_30__too_long) {
	cgm::Question q{"123456789?123456789?123456789?", {}};
	return { false, "Expected exception 'TooLongException'" };
} EXPECT(cgm::TooLongException);


TEST(Q_length_20__edge_case) {
	cgm::Question q{"123456789?123456789?", {}};
	return { q.getQuestionText().length() == 20, "" };
} NOEXCEPT;

TEST(Q_length_21__too_long) {
	cgm::Question q{"123456789?123456789?1?", {}};
	return { false, "Expected exception 'TooLongException'" };
} EXPECT(cgm::TooLongException);

TEST(Q_length_1__too_short) {
	cgm::Question q{"?", {}};
	return { false, "Expected exception 'TooShortException'" };
} EXPECT(cgm::TooShortException);

TEST(Q_Comparison_equality) {
	cgm::Question q1{"Q1?", {}};
	cgm::Question q2{"Q1?", {"A1"}};

	return { cgm::Question::AnswerAgnosticComparator(q1,q2) == cgm::Question::AnswerAgnosticComparator(q2,q1), "" };
} NOEXCEPT;

TEST(Q_Comparison_equality_unequal) {
	cgm::Question q1{"Q1?", {}};
	cgm::Question q2{"Q2?", {"A1"}};

	return { cgm::Question::AnswerAgnosticComparator(q1,q2) != cgm::Question::AnswerAgnosticComparator(q2,q1), "" };
} NOEXCEPT;
