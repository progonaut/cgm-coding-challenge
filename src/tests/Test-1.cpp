#include"Test.h"
#include<iostream>
#include<exception>

#include"../globals.h"

/* These tests are meta-tests to test if the test system works. 
 **/

TEST(_META_TEST_________PASS) {
	return {true, "This should pass."};
} PASS;

TEST(_META_TEST_FAILURE_FAIL) {
	return {false, "This shouldn't pass."};
} PASS;

TEST(_META_TEST_EXC_____PASS) {
	throw std::logic_error{"This is expected (PASS)"};
	return {false, "This test should throw an exception and the exception should be expected (PASS)"};
} EXPECT(std::logic_error);

TEST(_META_FALSE_EXC____FAIL) {
	throw std::underflow_error{"This is not the expected exception (FAIL)"};
	return {false, "This test should throw an exception, but the exception is not the correct exception (FAIL)"};
} EXPECT(std::logic_error);

TEST(_META_NO_EXC_______FAIL) {
	return {false, "This test expects an exception, but no exception happened (FAIL)"};
} EXPECT(std::logic_error);

TEST(_META_MAX_LENGTH___PASS) {
	return { MAX_LENGTH == 20, "Many tests assume MAX_LENGTH to be 20." };
} PASS;

//EXCEPT(std::length_error)
