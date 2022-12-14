/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "s21_smart_calc-test.check" instead.
 */

#include <check.h>

#include "polishManager.h"
#include "stackManager.h"
#include "calculation.h"
#define E_TEST 1e-6

START_TEST(polish_test_1) {
#line 7
    char *expression = "tan(1)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.55740772;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_2) {
#line 14
    char *expression = "tan(1+2)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = -0.14254654;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_3) {
#line 21
    char *expression = "-1";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = -1.0;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_4) {
#line 28
    char *expression = "a";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_5) {
#line 33
    char *expression = "+1.2";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.2;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_6) {
#line 40
    char *expression = "2.2.";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_7) {
#line 45
    char *expression = "ln(2*2+2)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.791759;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_8) {
#line 52
    char *expression = "ln";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_9) {
#line 57
    char *expression = "8ln";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_10) {
#line 62
    char *expression = "8+-";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_11) {
#line 67
    char *expression = "(8))";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_12) {
#line 72
    char *expression = "((8+6)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_13) {
#line 77
    char *expression = "(9 9 +)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_14) {
#line 82
    char *expression = "2^2";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 4.0;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_15) {
#line 89
    char *expression = "cos(sin(3))";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 0.99005908;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_16) {
#line 96
    char *expression = "asin(1)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.570796;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_17) {
#line 103
    char *expression = "acos(0.23)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.3387186;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_18) {
#line 110
    char *expression = "tan(0.4125)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 0.4376067;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_19) {
#line 117
    char *expression = "atan(-0.241)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = -0.2364903;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_20) {
#line 124
    char *expression = "sqrt(421)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 20.518284;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_21) {
#line 131
    char *expression = "log(23.342)";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.3681381;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_22) {
#line 138
    char *expression = "x*132";
    double x = 0.3;
    stack *notation = polishConverter(expression);
    checkX(notation);
    replaceXWithValue(&notation, &x);
    double result = calculate(&notation);
    double expected = 39.6;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_23) {
#line 148
    char *expression = "3x";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    fail_if(result == result);
}
END_TEST

START_TEST(polish_test_24) {
#line 153
    char *expression = "2/2";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 1.0;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

START_TEST(polish_test_25) {
#line 160
    char *expression = "2%2";
    stack *notation = polishConverter(expression);
    double result = calculate(&notation);
    double expected = 0.0;
    expected = fabs(expected - result);
    fail_if(expected > E_TEST);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;
    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, polish_test_1);
    tcase_add_test(tc1_1, polish_test_2);
    tcase_add_test(tc1_1, polish_test_3);
    tcase_add_test(tc1_1, polish_test_4);
    tcase_add_test(tc1_1, polish_test_5);
    tcase_add_test(tc1_1, polish_test_6);
    tcase_add_test(tc1_1, polish_test_7);
    tcase_add_test(tc1_1, polish_test_8);
    tcase_add_test(tc1_1, polish_test_9);
    tcase_add_test(tc1_1, polish_test_10);
    tcase_add_test(tc1_1, polish_test_11);
    tcase_add_test(tc1_1, polish_test_12);
    tcase_add_test(tc1_1, polish_test_13);
    tcase_add_test(tc1_1, polish_test_14);
    tcase_add_test(tc1_1, polish_test_15);
    tcase_add_test(tc1_1, polish_test_16);
    tcase_add_test(tc1_1, polish_test_17);
    tcase_add_test(tc1_1, polish_test_18);
    tcase_add_test(tc1_1, polish_test_19);
    tcase_add_test(tc1_1, polish_test_20);
    tcase_add_test(tc1_1, polish_test_21);
    tcase_add_test(tc1_1, polish_test_22);
    tcase_add_test(tc1_1, polish_test_23);
    tcase_add_test(tc1_1, polish_test_24);
    tcase_add_test(tc1_1, polish_test_25);
    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? 0 : 1;
}
