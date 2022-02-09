#include <check.h>
#include "smartcalc.h"

struct type_stack st1;
struct type_double dres;
struct flags errors;

START_TEST(test_1) {
    char out[SIZE] = {};
    char in[SIZE] = "1+2-3*4/5";
    double result = 0.6;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_2) {
    char out[SIZE] = {};
    char in[SIZE] = "(-2)*2";
    double result = -4.0;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_3) {
    char out[SIZE] = {};
    char in[SIZE] = "2-(-2)";
    double result = 4;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_4) {
    char out[SIZE] = {};
    char in[SIZE] = "(-2)/(-2)";
    double result = 1;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_5) {
    char out[SIZE] = {};
    char in[SIZE] = "sin(1)";
    double result = 0.8414709848;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_6) {
    char out[SIZE] = {};
    char in[SIZE] = "cos(1)";
    double result = 0.54030230586;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_7) {
    char out[SIZE] = {};
    char in[SIZE] = "tan(1)";
    double result = 1.55740772465;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_8) {
    char out[SIZE] = {};
    char in[SIZE] = "atan(0.5)";
    double result = 0.463647609;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_9) {
    char out[SIZE] = {};
    char in[SIZE] = "asin(0.5)";
    double result = 0.52359877559;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_10) {
    char out[SIZE] = {};
    char in[SIZE] = "acos(0.5)";
    double result = 1.0471975512;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_11) {
    char out[SIZE] = {};
    char in[SIZE] = "sqrt(0.5)";
    double result = 0.70710678118;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_12) {
    char out[SIZE] = {};
    char in[SIZE] = "ln(0.5)";
    double result = -0.69314718056;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((fabs(dres.array[0] - result)), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_13) {
    char out[SIZE] = {};
    char in[SIZE] = "log(0.5)";
    double result = -0.30102999566;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((fabs(dres.array[0] - result)), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_14) {
    char out[SIZE] = {};
    char in[SIZE] = "sin(cos(sqrt(19 % 4)))";
    double result = -0.15986761197;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((fabs(dres.array[0] - result)), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_15) {
    char out[SIZE] = {};
    char in[SIZE] = "2^2/(7%5+3)*sin(5-3)";
    double result = 0.72743794;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_16) {
    char out[SIZE] = {};
    char in[SIZE] = "sin(0.5";
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_int_eq(st1.error, 1);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

START_TEST(test_17) {
    char out[SIZE] = {};
    char in[SIZE] = "1e4";
    double result = 10000;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le((dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST
START_TEST(test_18) {
    char out[SIZE] = {};
    char in[SIZE] = "2^2^3-4^1*2";
    double result = 248.0;
    init_stack(&st1);
    init_flags(&errors);
    sort(in, out, &st1, &errors);
    create_double(&dres);
    parser(0.0, out, &dres);
    ck_assert_double_le(fabs(dres.array[0] - result), EPS);
    destroy_stack(&st1);
    destroy_double(&dres);
}
END_TEST

int main() {
    Suite *st = suite_create("Calc");
    SRunner *sr = srunner_create(st);
    int nf;

    TCase *case_1 = tcase_create("test_1");
    suite_add_tcase(st, case_1);
    tcase_add_test(case_1, test_1);

    TCase *case_2 = tcase_create("test_2");
    suite_add_tcase(st, case_2);
    tcase_add_test(case_2, test_2);

    TCase *case_3 = tcase_create("test_3");
    suite_add_tcase(st, case_3);
    tcase_add_test(case_3, test_3);

    TCase *case_4 = tcase_create("test_4");
    suite_add_tcase(st, case_4);
    tcase_add_test(case_4, test_4);

    TCase *case_5 = tcase_create("test_5");
    suite_add_tcase(st, case_5);
    tcase_add_test(case_5, test_5);

    TCase *case_6 = tcase_create("test_6");
    suite_add_tcase(st, case_6);
    tcase_add_test(case_6, test_6);

    TCase *case_7 = tcase_create("test_7");
    suite_add_tcase(st, case_7);
    tcase_add_test(case_7, test_7);

    TCase *case_8 = tcase_create("test_8");
    suite_add_tcase(st, case_8);
    tcase_add_test(case_8, test_8);

    TCase *case_9 = tcase_create("test_9");
    suite_add_tcase(st, case_9);
    tcase_add_test(case_9, test_9);

    TCase *case_10 = tcase_create("test_10");
    suite_add_tcase(st, case_10);
    tcase_add_test(case_10, test_10);

    TCase *case_11 = tcase_create("test_11");
    suite_add_tcase(st, case_11);
    tcase_add_test(case_11, test_11);

    TCase *case_12 = tcase_create("test_12");
    suite_add_tcase(st, case_12);
    tcase_add_test(case_12, test_12);

    TCase *case_13 = tcase_create("test_13");
    suite_add_tcase(st, case_13);
    tcase_add_test(case_13, test_13);

    TCase *case_14 = tcase_create("test_14");
    suite_add_tcase(st, case_14);
    tcase_add_test(case_14, test_14);

    TCase *case_15 = tcase_create("test_15");
    suite_add_tcase(st, case_15);
    tcase_add_test(case_15, test_15);

    TCase *case_16 = tcase_create("test_16");
    suite_add_tcase(st, case_16);
    tcase_add_test(case_16, test_16);

    TCase *case_17 = tcase_create("test_17");
    suite_add_tcase(st, case_17);
    tcase_add_test(case_17, test_17);

    TCase *case_18 = tcase_create("test_18");
    suite_add_tcase(st, case_18);
    tcase_add_test(case_18, test_18);

    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
