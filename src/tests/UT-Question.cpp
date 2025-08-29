#include"Test.h"
#include"../Question.h"

TEST(Create_Good_Question_10_length) {
	cgm::Question q{"123456789?", {"Ans1"}};
	return { q.getQuestionText() == "123456789?" && q.getAnswers().size() == 1 && *(q.getAnswers().begin()) == "Ans1", ""};
} NOEXCEPT;

TEST(Create_Good_Question_10_length_no_answers) {
	cgm::Question q{"123456789?", {}};
	return { q.getQuestionText() == "123456789?" && q.getAnswers().size() == 0, ""};
} NOEXCEPT;
