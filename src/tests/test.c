/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "tests/test.check" instead.
 */

#include <check.h>

#line 1 "tests/test.check"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../backend/parser.h"

START_TEST(parser_cube) {
#line 14

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj;

  getwd(file_path);
  strcat(file_path, "/objects/cube.obj");

  int err = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err);
  ck_assert_int_eq(obj.v_count, 8);
  ck_assert_int_eq(obj.f_count, 12);

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(obj.f_array[i].v_array);
  }
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

START_TEST(parser_chair) {
#line 34

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj;

  getwd(file_path);
  strcat(file_path, "/objects/chair.obj");

  int err = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err);
  ck_assert_int_eq(obj.v_count, 116);
  ck_assert_int_eq(obj.f_count, 76);

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(obj.f_array[i].v_array);
  }
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

START_TEST(translate_cube) {
#line 56

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj, initial_obj;

  getwd(file_path);
  strcat(file_path, "/objects/cube.obj");

  int err1 = parse_obj_file(file_path, &initial_obj);
  int err2 = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err1);
  ck_assert_int_eq(status_ok, err2);

  translate_object(&obj, X_AXIS, 2.);
  translate_object(&obj, Y_AXIS, 2.);
  translate_object(&obj, X_AXIS, -2.);
  translate_object(&obj, Y_AXIS, -2.);
  translate_object(&obj, Z_AXIS, 3.);
  translate_object(&obj, Z_AXIS, -2.);
  translate_object(&obj, Z_AXIS, -1.);

  for (size_t i = 0; i < obj.v_count; ++i) {
    ck_assert_double_eq_tol(initial_obj.v_array[i].x, obj.v_array[i].x, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].y, obj.v_array[i].y, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].z, obj.v_array[i].z, 1e-6);
  }

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(initial_obj.f_array[i].v_array);
    free(obj.f_array[i].v_array);
  }
  free(initial_obj.f_array);
  free(initial_obj.v_array);
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

START_TEST(rotate_cube) {
#line 93

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj, initial_obj;

  getwd(file_path);
  strcat(file_path, "/objects/cube.obj");

  int err1 = parse_obj_file(file_path, &initial_obj);
  int err2 = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err1);
  ck_assert_int_eq(status_ok, err2);

  rotate_ox_object(&obj, M_PI / 2);
  rotate_ox_object(&obj, 3 * M_PI / 2);
  rotate_oy_object(&obj, -M_PI);
  rotate_oy_object(&obj, -M_PI);
  rotate_oz_object(&obj, 3 * M_PI);
  rotate_oz_object(&obj, -5 * M_PI);

  for (size_t i = 0; i < obj.v_count; ++i) {
    ck_assert_double_eq_tol(initial_obj.v_array[i].x, obj.v_array[i].x, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].y, obj.v_array[i].y, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].z, obj.v_array[i].z, 1e-6);
  }

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(initial_obj.f_array[i].v_array);
    free(obj.f_array[i].v_array);
  }
  free(initial_obj.f_array);
  free(initial_obj.v_array);
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

START_TEST(scale_cube) {
#line 129

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj, initial_obj;

  getwd(file_path);
  strcat(file_path, "/objects/cube.obj");

  int err1 = parse_obj_file(file_path, &initial_obj);
  int err2 = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err1);
  ck_assert_int_eq(status_ok, err2);

  scale_object(&obj, 2.);
  scale_object(&obj, 0.5);
  scale_object(&obj, 0.1);
  scale_object(&obj, 10.);

  for (size_t i = 0; i < obj.v_count; ++i) {
    ck_assert_double_eq_tol(initial_obj.v_array[i].x, obj.v_array[i].x, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].y, obj.v_array[i].y, 1e-6);
    ck_assert_double_eq_tol(initial_obj.v_array[i].z, obj.v_array[i].z, 1e-6);
  }

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(initial_obj.f_array[i].v_array);
    free(obj.f_array[i].v_array);
  }
  free(initial_obj.f_array);
  free(initial_obj.v_array);
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

START_TEST(normalize_chair) {
#line 165

  char *file_path = (char *)calloc(PATH_MAX, sizeof(char));
  object_t obj, normalized_obj;

  getwd(file_path);
  strcat(file_path, "/objects/chair.obj");

  int err = parse_obj_file(file_path, &obj);
  ck_assert_int_eq(status_ok, err);
  normalize_object(obj, &normalized_obj);

  for (size_t i = 0; i < obj.v_count; ++i) {
    ck_assert(fabs(normalized_obj.v_array[i].x) <= 1);
    ck_assert(fabs(normalized_obj.v_array[i].y) <= 1);
    ck_assert(fabs(normalized_obj.v_array[i].z) <= 1);
  }

  free(file_path);
  for (size_t i = 0; i < obj.f_count; ++i) {
    free(obj.f_array[i].v_array);
  }
  free(normalized_obj.v_array);
  free(obj.f_array);
  free(obj.v_array);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("unite_testing");
  TCase *tc1_1 = tcase_create("parser_test");
  TCase *tc1_2 = tcase_create("transformations_test");
  TCase *tc1_3 = tcase_create("normalization_test");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, parser_cube);
  tcase_add_test(tc1_1, parser_chair);
  suite_add_tcase(s1, tc1_2);
  tcase_add_test(tc1_2, translate_cube);
  tcase_add_test(tc1_2, rotate_cube);
  tcase_add_test(tc1_2, scale_cube);
  suite_add_tcase(s1, tc1_3);
  tcase_add_test(tc1_3, normalize_chair);

  srunner_run_all(sr, CK_VERBOSE);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
