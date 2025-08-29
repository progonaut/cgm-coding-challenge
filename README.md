# CGM Coding Challenge

## Minutiae

* Matthias Wagner (wagner.matthias@gmx.at)
* 2025-08-29

## Purpose

This is the repository for my submission for the CGM coding challenge as part of the application process for the 
C++ senior developer position at CGM/Innomed.

## Task

The following task description was provided

1.  Write a C++ command line program that gives me two options. One to ask a specific question and the other option is
to add questions and their answers

	Therefore the following restrictions apply:

	*   A Question is a String with max 255 chars
	*   An Answer is a String with max 255 chars
	*   A Question can have multiple answers (like bullet points)
	*   If the user asks a question it has to be exactly the same as entered – no “best match”.
	*   If the user asks a question which is not stored yet the program should print “the answer to life, universe and everything is 42” according to “The hitchhikers guide to the Galaxy”
	*   If the user asks a question whish is  stored the program should print all answers to that question. Every Answer in a separate line
	*   Adding a question looks like:
		*   <question>? “<answer1>” “<answer2>” “<answerX>”
		*   Char “?” is the separator between question and answers
		*   Every Question needs to have at least one answer but can have unlimited answers all inside of char “

2.  Provide tests for the functionality described in 1)
3.  Provide the source code on any GIT repo worldwide (GITLAB, GITHUB, whatever you prefer and/or use)
4.  No other restrictions apply

**Examples**:

*   Adding a question:
	*   What is Peters favorite food? “Pizza” “Spaghetti” “Ice cream”
*   Asking a question which is in the system:
	*   What is Peters favorite food?
		*   Answers will be
			* Pizza
			* Spaghetti
			* Ice cream
*   Asking a question which is not in the system:
	*   When is Peters birthday?
		*   Answer will be
			*   the answer to life, universe and everything is 42

## External libraries or dependencies, Standard version

C++23 with no external libraries.

The program compiles with  clang 16.06 and g++ 14.1 on a Linux 6.12.15-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.12.15-1 (2025-02-18) x86_64 GNU/Linux
machine and runs all tests successfully.

The following **defines** modify the build: 

* `MAX_LENGTH` sets the expected max length of questions and answers
* `DEBUG` to compile debug version
* `RUN_TESTS` to compile test version

The test version is a debug compilation, but instead of the normal functionality the program runs tests and 
displays the result.

Catch2 was considered to implement tests, but because I don't have ready access to a Windows machine and could not
verify that the set up of the inclusion of the library (which is not ("no longer") header only) works, I opted to
implement a very (very) simple home-spun "test framework".

## Assumptions

* There is no input across line-feeds, i.e. "What is my <LINEFEED> name?" is not a valid question
	- the program would treat this as one illegal input, "What is my", followed by a query ("name?")

* A query ("Question?") and a definition ("Question? \"answer\"") are "statements". One line of input can contain
  multiple statements

* Input errors are handled by means of an error message; unrecoverable errors terminate the program (but ought not
  happen ordinarily)

* The application is not performance-critical, such that quadratic Big-O function performance characteristics are not
  optimised further

* Answers are stored and retrieved in insertion order

## Discussion

I opted for an architecture that is considerably over-engineered given the small
size of the task. However, this is a deliberate decision taken to provide a basis for discussion in the context of
enterprise grade or professional software development, in contrast to the ability to quickly code a solution for the
problem.

A simple solution put together in 15 minutes could look like this, sans testing:

```
#include <iostream>
#include <map>
#include <string>
#include <vector>
int main() {
  std::map<std::string, std::vector<std::string>> questions;
  std::string input_line;
  std::cout << "Exit with Ctrl-D" << std::endl;
  std::cout << "? ";
  while (std::getline(std::cin, input_line)) {
    bool error{false};
    if (!input_line.contains("?")) {
      std::cerr << "Questions must end with '?'" << std::endl;
      error = true;
    } else {
      auto [first_part, second_part] = [input_line]() {
        return std::make_pair(input_line.substr(0, input_line.find("?")),
                              input_line.substr(input_line.find("?")));
      }();
      if (first_part.length() > 255) {
        std::cerr << "Questions can not be longer than 255 characters."
                  << std::endl;
      } else {
        std::vector<std::string> answers;
        auto begin = second_part.find("\"");
        do {
          if (begin != std::string::npos) {
            begin++;
            auto end = second_part.find("\"", begin);
            if (end != std::string::npos) {
              if (end - begin > 255) {
                std::cerr << "Answers can not be longer than 255 characters."
                          << std::endl;
                error = true;
              } else {
                answers.push_back(second_part.substr(begin, end - begin));
                begin = second_part.find("\"", end + 1);
              }
            } else {
              std::cerr << "Answers must be surrounded by '\"'" << std::endl;
              error = true;
            }
          }
        } while (begin != std::string::npos && !error);
        if (error) {
          std::cerr << "Aborting due to previous errors." << std::endl;
        } else {
          if (answers.size() >= 1) {
            questions[first_part] = answers;
          } else {
            if (questions.contains(first_part)) {
              for (auto const &ans : questions[first_part]) {
                std::cout << "* " << ans << "\n";
              }
              std::cout << "[no further answers]" << std::endl;
            } else {
              std::cout << "the answer to life, universe and everything is 42"
                        << std::endl;
            }
          }
        }
      }
    }
    std::cout << "? ";
  }
}
```

With that said, the solution consists of

* `Question` and `Catalog` to represent a single Question and a collection of Questions, respectively
* Interfaces to provide Input, a Parser, and a Processor/Interpreter
* Implementations of said Interfaces
* Implementations of "mocks" and test versions of said interfaces (where necessary)
* A facade class to tie the interface implementations together and provide a single point of deployment/execution
* (unfinished) error handling with (some) custom exceptions
* Unit-Tests for `Question` and `Catalog`
* Simple integration tests for good and bad inputs
* A very simple test framework

I have chosen this approach because it would be a reasonable first draft architecture for a larger program in a similar
domain.

### Possible improvements and considerations

* Instead of a bespoke `Question` class, a `std::map` in `Catalog` could suffice
* Passable output streams instead of `cout` in objects
* replace `std::pair` and similar with bespoke structs (clearer)
* replace "test framework" with sensible library
* reduce use of `std::string` and copies; `std::string_view`?

Not everything is polished; nor necessarily bug-free.

This program and all of its source and project files are intented for the coding challenge as outlined above; publishing
any part of it publically is frowned upon.
