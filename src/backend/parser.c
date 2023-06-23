#include "parser.h"

#define GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool starts_with(const char *str, const char *pattern);
static void set_vertices_and_faces_count(FILE *f, object_t *obj);
static size_t get_vertices_count_in_f(const char *line);
static size_t fill_face(const char *line, object_t *obj, size_t last_v_index,
                        size_t last_f_index);
void check_minmax(object_t *obj, size_t last_v_index);

bool starts_with(const char *str, const char *pattern) {
  return strncmp(str, pattern, strlen(pattern)) == 0;
}

void set_vertices_and_faces_count(FILE *f, object_t *obj) {
  long pos = ftell(f);

  char *line = NULL;
  size_t linecapp = 0;
  ssize_t character_count = 0;
  obj->v_count = 0;
  obj->f_count = 0;

  while ((character_count = getline(&line, &linecapp, f)),
         character_count > 0) {
    if (starts_with(line, "v ")) {
      ++obj->v_count;
    } else if (starts_with(line, "f ")) {
      ++obj->f_count;
    }
  }

  fseek(f, pos, SEEK_SET);
}

size_t get_vertices_count_in_f(const char *line) {
  char *next_lf = strchr(line, ' ');
  long long int temp = 0;
  size_t v_count = 0;
  while (next_lf != NULL && sscanf(next_lf, "%lld", &temp) > 0) {
    next_lf = strchr(next_lf + 1, ' ');
    ++v_count;
  }

  return v_count;
}

size_t fill_face(const char *line, object_t *obj, size_t last_v_index,
                 size_t last_f_index) {
  face_t *face = &obj->f_array[last_f_index];
  if (face->v_array == NULL) {
    return memory_allocation_error;
  }

  char *next_lf = strchr(line, ' ');
  long long int v_number = 0;
  size_t v_count = 0;
  while (next_lf != NULL && sscanf(next_lf, "%lld", &v_number) > 0) {
    if (v_number < 0) {
      v_number = last_v_index + v_number;
    } else {
      v_number -= 1;
    }
    face->v_array[v_count] = v_number;
    next_lf = strchr(next_lf + 1, ' ');
    ++v_count;
  }

  return status_ok;
}

static int fill_vertices_and_faces(FILE *f, object_t *obj) {
  if (obj->v_array == NULL || (obj->f_array == NULL && obj->f_count != 0)) {
    return memory_allocation_error;
  }

  int result = status_ok;
  long pos = ftell(f);

  char *line = NULL;
  size_t linecapp = 0;
  ssize_t character_count = 0;

  size_t last_v_index = 0;
  size_t last_f_index = 0;
  while (
      result == status_ok &&
      ((character_count = getline(&line, &linecapp, f)), character_count > 0)) {
    if (starts_with(line, "v ")) {
      sscanf(line + strlen("v "), "%lf %lf %lf", &obj->v_array[last_v_index].x,
             &obj->v_array[last_v_index].y,
             &obj->v_array[last_v_index].z);  // maybe < 3
      check_minmax(obj, last_v_index);
      ++last_v_index;
    } else if (starts_with(line, "f ")) {
      size_t v_count = get_vertices_count_in_f(line);
      obj->f_array[last_f_index].v_count = v_count;
      obj->f_array[last_f_index].v_array =
          (uint64_t *)malloc(v_count * sizeof(uint64_t));

      result = fill_face(line, obj, last_v_index, last_f_index);
      // result can be equal memory_allocation_error
      // we need to free all allocated memory!!!
      ++last_f_index;
    }
  }

  fseek(f, pos, SEEK_SET);

  return result;
}

void check_minmax(object_t *obj, size_t last_v_index) {
  if (obj->v_array[last_v_index].x < obj->x_min) {
    obj->x_min = obj->v_array[last_v_index].x;
  }
  if (obj->v_array[last_v_index].y < obj->y_min) {
    obj->y_min = obj->v_array[last_v_index].y;
  }
  if (obj->v_array[last_v_index].x > obj->x_max) {
    obj->x_max = obj->v_array[last_v_index].x;
  }
  if (obj->v_array[last_v_index].y > obj->y_max) {
    obj->y_max = obj->v_array[last_v_index].y;
  }
}

int parse_obj_file(const char *path, object_t *obj) {
  enum parse_error result = status_ok;
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    return cannot_open_file;
  }

  set_vertices_and_faces_count(f, obj);
  if (obj->v_count > 0) {
    obj->v_array = (point_t *)calloc(obj->v_count, sizeof(point_t));
    if (obj->f_count == 0) {
      obj->f_array = NULL;
    } else {
      obj->f_array = (face_t *)malloc(obj->f_count * sizeof(face_t));
    }
    result = fill_vertices_and_faces(f, obj);
  }
  fclose(f);

  return result;
}
