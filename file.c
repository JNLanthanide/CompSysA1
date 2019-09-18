#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <assert.h>
#include <errno.h>
#include <string.h>

enum file_type {
  DATA,
  EMPTY,
  ASCII,
};

char* FILE_TYPE_STRINGS[] = {
  "data",
  "empty",
  "ASCII text",
};

char* enumToString(enum file_type ft) {
  if (ft == DATA) {
    return FILE_TYPE_STRINGS[0];
  }
  if (ft == EMPTY) {
    return FILE_TYPE_STRINGS[1];
  }
  if (ft == ASCII) {
    return FILE_TYPE_STRINGS[2];
  }
  return "ERROR WRONG ENUM";
}


int print_result(char* path, enum file_type ft) {
  return fprintf(stdout, "%s: %s\n", path, enumToString(ft));
}

int print_error(char* path, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n",
    path, strerror(errnum));
}

// Returns 1 if n is contained within the ASCII set (as defined in the assignment),
//otherwise returns 0
int check_ASCII(int n) {
  if (!(n >= 0x07 && n <= 0x0D) && (n != 0x1B) && !(n >= 0x20 && n <= 0x7E)) {
    return 0;
  } else {
    return 1;
  }
}

void DetermineFileType(FILE* f, char* path) {
  int buffer = 0;

  int i = 0;

  int isASCII = 1;

  while(1) {
    if (fread(&buffer, 1, 1, f) != 0) {
      if (check_ASCII(buffer) == 0) {
        isASCII = 0;
      }
    }
    else {
      if (i == 0) {
        print_result(path, EMPTY);
      } else {
        if (isASCII == 1) {
          print_result(path, ASCII);
        }
        else {
          print_result(path, DATA);
        }
      }
      break;
    }
    i++;
  }
}

int main(int argc, char *argv[])
{
  int retval = EXIT_SUCCESS;

  if (argc < 2) {
    fprintf(stderr, "%s", "Usage: file path");
    retval = EXIT_FAILURE;
    return retval;
  }
  for (i=1;i <= argc; i++;){
    FILE* f = fopen(argv[i], "r");

    if (f == NULL) {
      print_error(argv[i], errno);
      int retval = EXIT_SUCCESS;
      return retval;
    }

    DetermineFileType(f, argv[i]);
  }

  return retval;
}
