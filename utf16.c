#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <assert.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int retval = EXIT_SUCCESS;

  if (argc != 2) {
    fprintf(stderr, "%s", "Usage: file path");
    retval = EXIT_FAILURE;
    return retval;
  }

  FILE* f = fopen(argv[1], "r");

  if (f == NULL) {
    //print_error(argv[1], errno);
    int retval = EXIT_SUCCESS;
    return retval;
  }

  unsigned char c[2];
  if ((fread(&c, sizeof(char), sizeof(c), f) != 0) != 0) {
    if (c[0] == 0xFE && c[1] == 0xFF) {
      printf("%s\n", "Okay det kan man");
    }
    else {
      if (c[0] == 0xFF && c[1] == 0xFE) {
        printf("%s\n", "Yess");
      }
    }
  }



  return retval;
}
