#ifndef HOARD_COMMON_LDPRELOAD_TESTING_H
#define HOARD_COMMON_LDPRELOAD_TESTING_H

#include "tracing.h"

static int ldpreload_test_result = 0;
int TestResult() {

    if (ldpreload_test_result == 0) {
        hoard::println("TESTS OK!!!");
    } else {
        hoard::println("TESTS FAILED!!!");
    }
    return ldpreload_test_result;
}


void SetTestsFailed() {
    ldpreload_test_result = EXIT_FAILURE;
}

#define LDPRELOADTEST_NONFATAL_FAILURE_ SetTestsFailed
#define LDPRELOADTEST_FATAL_FAILURE_ std::abort

void PrintlnAssertionFailureMessage(
    const char* expression_text,
    const char* actual_predicate_value,
    const char* expected_predicate_value) {
  hoard::println("Value of: ", expression_text,
       "\n Actual: ", actual_predicate_value,
       "\n Expected: ", expected_predicate_value);
}


// Implements Boolean test assertions such as EXPECT_TRUE. expression can be
// either a boolean expression or an AssertionResult. text is a textual
// represenation of expression as it was passed into the EXPECT_TRUE.
#define LDPRELOADTEST_TEST_BOOLEAN_(expression, text, actual, expected, fail) \
  if (expression){} \
  else {  \
    PrintlnAssertionFailureMessage(\
        text, #actual, #expected);\
    fail();\
  }

#define EXPECT_TRUE(condition) \
  LDPRELOADTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      LDPRELOADTEST_NONFATAL_FAILURE_)
#define EXPECT_FALSE(condition) \
  LDPRELOADTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      LDPRELOADTEST_NONFATAL_FAILURE_)

#define ASSERT_TRUE(condition) \
  LDPRELOADTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      LDPRELOADTEST_FATAL_FAILURE_)
#define ASSERT_FALSE(condition) \
  LDPRELOADTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      LDPRELOADTEST_FATAL_FAILURE_)


#endif //HOARD_COMMON_LDPRELOAD_TESTING_H

