#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct student {
  int sid;
  char *sname;
};

void *print_student_name(void *stu) {
  struct student *s = (struct student *)stu;
  printf("Student %d -> %s\n", s->sid, s->sname);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[2];

  struct student students[2] = {
      {.sid = 1, .sname = "david"},
      {.sid = 2, .sname = "john"},
  };

  for (int i = 0; i < 2; ++i) {
    pthread_create(&threads[i], NULL, print_student_name, (void *)&students[i]);
  }

  pthread_exit(NULL);
}