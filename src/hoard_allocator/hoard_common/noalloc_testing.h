#ifndef HOARD_HOARD_COMMON_NOALLOC_TESTIN_H
#define HOARD_HOARD_COMMON_NOALLOC_TESTIN_H

#include "tracing.h"

static int no_alloc_test_result = 0;
int TestResult() {

    if (no_alloc_test_result == 0) {
        hoard::println("TESTS OK!!!");
    } else {
        hoard::println("TESTS FAILED!!!");
    }
    return no_alloc_test_result;
}


void SetTestsFailed() {
    no_alloc_test_result = EXIT_FAILURE;
};

#define NOALLOCTEST_NONFATAL_FAILURE_ SetTestsFailed
#define NOALLOCTEST_FATAL_FAILURE_ std::abort

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
#define NOALLOCTEST_TEST_BOOLEAN_(expression, text, actual, expected, fail) \
  if (expression){} \
  else {  \
    PrintlnAssertionFailureMessage(\
        text, #actual, #expected);\
    fail();\
  }

#define EXPECT_TRUE(condition) \
  NOALLOCTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      NOALLOCTEST_NONFATAL_FAILURE_)
#define EXPECT_FALSE(condition) \
  NOALLOCTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      NOALLOCTEST_NONFATAL_FAILURE_)

#define ASSERT_TRUE(condition) \
  NOALLOCTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      NOALLOCTEST_FATAL_FAILURE_)
#define ASSERT_FALSE(condition) \
  NOALLOCTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      NOALLOCTEST_FATAL_FAILURE_)

#endif //HOARD_HOARD_COMMON_NOALLOC_TESTIN_H

