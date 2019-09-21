#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <assert.h>
#include <errno.h>
#include <string.h>

// Macros as defined in the assignment:
#define UTF8_4B(n) ((((n)>>3)&1) == 0 && (((n)>>4)&1) == 1 && (((n)>>5)&1) == 1 && (((n)>>6)&1) == 1 &&  (((n)>>7)&1) == 1) ? 1 : 0
#define UTF8_3B(n) ((((n)>>4)&1) == 0 && (((n)>>5)&1) == 1 && (((n)>>6)&1) == 1 &&  (((n)>>7)&1) == 1) ? 1 : 0
#define UTF8_2B(n) ((((n)>>5)&1) == 0 && (((n)>>6)&1) == 1 &&  (((n)>>7)&1) == 1) ? 1 : 0
#define UTF8_CONT(n) ((((n)>>6)&1) == 0 && (((n)>>7)&1) == 1) ? 1 : 0
// This macro returns true if the byte ends with a 0:
#define UTF8_1b(n) (((n>>7)&1) == 0) ? 1 : 0

enum file_type {
  DATA,
  EMPTY,
  ASCII,
  ISO,
  UTF8,
  UTF16BE,
  UTF16LE,
};


// "with no line terminators" added to UTF16LE and UTF16BE for testing reasons.
char* FILE_TYPE_STRINGS[] = {
  "data",
  "empty",
  "ASCII text",
  "ISO-8859 text",
  "UTF-8 Unicode text",
  "Big-endian UTF-16 Unicode text, with no line terminators",
  "Little-endian UTF-16 Unicode text, with no line terminators",
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
  if (ft == ISO) {
    return FILE_TYPE_STRINGS[3];
  }
  if (ft == UTF8) {
    return FILE_TYPE_STRINGS[4];
  }
  if (ft == UTF16BE) {
    return FILE_TYPE_STRINGS[5];
  }
  if (ft == UTF16LE) {
    return FILE_TYPE_STRINGS[6];
  }
  return "ERROR WRONG ENUM";
}

int print_result(const char* path, int max_length, enum file_type ft) {
    return fprintf(stdout, "%s:%*s%s\n",
    path, (int) (max_length - strlen(path) + 1), " ", enumToString(ft));
}


int print_error(const char* path, int max_length, int errnum) {
  return fprintf(stdout, "%s:%*scannot determine (%s)\n",
  path, max_length - (int) strlen(path) + 1, " ", strerror(errnum));
}

// Checks the BOM of a file, returns 1 if its UTF16LE encoded, 2 if its uTF16BE encoded, otherwise returns 0:
int check_UTF16(FILE* f) {
  rewind(f);
  unsigned char c[2];
  int isUTF16 = 0;
  if ((fread(&c, sizeof(char), sizeof(c), f) != 0) != 0) {
    if (c[0] == 0xFE && c[1] == 0xFF) {
      isUTF16 = 2;
    }
    else {
      if (c[0] == 0xFF && c[1] == 0xFE) {
        isUTF16 = 1;
      }
    }
  }
  return isUTF16;
}

// Runs through file, returns 1 if its UTF8 Encoded, otherwise returns 0:
int check_UTF8(FILE* f) {
  rewind(f);
  // Int keeping track, whether to expect a UTF8 - 2byte,3byte,4byte or a continuation byte.
  int nBytes = 0;
  int isUTF8 = 1;
  //is 0 if there are no UTF8 2byte,3byte or 4bytes in the entire file.
  int UTFEncoded = 0;
  int retValue;
  signed char buffer = 0;
  while (1) {
    if (fread(&buffer, 1, 1, f) != 0) {
      if (nBytes == 0) { // If we are not expecting UTF8_CONT bytes:
        if (UTF8_4B(buffer) == 1)
        {
            nBytes = 3;
            UTFEncoded = 1;
        }
        else
        {
          if (UTF8_3B(buffer) == 1)
          {
              nBytes = 2;
              UTFEncoded = 1;
          }
          else
          {
            if (UTF8_2B(buffer) == 1)
            {
                nBytes = 1;
                UTFEncoded = 1;
            }
            else
            {
              if (UTF8_1b(buffer) == 1)
              {
                  nBytes = 0;
              }
            }
          }
        }
      }
      else //If we are expecting UTF8_CONT bytes:
      {
        if (UTF8_4B(buffer) == 1)
        {
            isUTF8 = 0;
        }
        else
        {
          if (UTF8_3B(buffer) == 1)
          {
              isUTF8 = 0;
          }
          else
          {
            if (UTF8_2B(buffer) == 1)
            {
                isUTF8 = 0;
            }
            else
            {
              if (UTF8_1b(buffer) == 1)
              {
                  isUTF8 = 0;
              }
              else
              {
                  if (UTF8_CONT(buffer) == 1)
                  {
                    nBytes--;
                  }
              }
            }
          }
        }
      }
    }
    else
    {
      // If we are not expecting continuation bytes, the UTF8 format was not broken,
      // and we encountered at least one 2byte, 3byte or 4byte.
      if (nBytes == 0 && isUTF8 == 1 && UTFEncoded == 1) {
        retValue = 1;
      }
      else
      {
        retValue = 0;
      }
      break;
    }
  }
  return retValue;
}

// Returns 1 if n is within the ASCII set as defined in the assignment.
// Otherwise returns 0
int isASCIIChar(int n) {
  if (!(n >= 0x07 && n <= 0x0D) && (n != 0x1B) && !(n >= 0x20 && n <= 0x7E)) {
    return 0;
  } else {
    return 1;
  }
}
// Returns 1 if every byte in f is within the ASCII set
// Oterwise returns 0
int check_ASCIIOrEmpty(FILE* f) {
  rewind(f);
  int i = 0;
  int buffer = 1;
  while(1) {
    if (fread(&buffer, sizeof(char), 1, f) != 0) {
      i++;
      if (isASCIIChar(buffer) == 0) {
        return 0;
      }
    }
    else {
      if (i == 0) {
        return -1;
      }
      break;
    }
  }
  return 1;
}

// Returns 1 if n is within the ASCII or the ISO set as defined in the assignment
// Otherwise returns 0
int isISOChar(int n) {
  if (!(n >= 160 && n <= 255) && isASCIIChar(n) == 0) {
    return 0;
  }
  return 1;
}

// Returns 1 if every byte in f is within the ISO or ASCII set.
// Otherwise returns 0
int check_ISO(FILE* f) {
  rewind(f);
  int buffer = 1;
  while(1) {
    if (fread(&buffer, sizeof(char), 1, f) != 0) {
      if (isISOChar(buffer) == 0) {
        return 0;
      }
    }
    else {
      break;
    }
  }
  return 1;
}


int main(int argc, char* argv[])
{
  int retval = EXIT_SUCCESS;
  if (argc < 2) {
    fprintf(stderr, "%s", "Usage: file path");
    retval = EXIT_FAILURE;
    return retval;
  }

  int longestPath = 0;
  for (int i = 1; i < (argc); i++) {
    if ((int) strlen(argv[i]) > longestPath) {
      longestPath = strlen(argv[i]);
    }
  }

  for (int j = 1; j < argc; j++) {
    FILE* f = fopen(argv[j], "r");
    if (f == NULL) {
      print_error(argv[j], longestPath, errno);
      int retval = EXIT_SUCCESS;
      return retval;
    }
    if (check_ASCIIOrEmpty(f) == 1) {
      print_result(argv[j], longestPath, ASCII);
    }
    else
    {
      if(check_ASCIIOrEmpty(f) == -1) {
        print_result(argv[j], longestPath, EMPTY);
      }
      else
      {
        if (check_UTF8(f) == 1) {
          print_result(argv[j], longestPath, UTF8);
        }
        else
        {
          if (check_UTF16(f) == 1) {
            print_result(argv[j], longestPath, UTF16LE);
          }
          else
          {
            if (check_UTF16(f) == 2) {
              print_result(argv[j], longestPath, UTF16BE);
            }
            else {
              if (check_ISO(f) == 1) {
                print_result(argv[j], longestPath, ISO);
              }
              else {
                print_result(argv[j], longestPath, DATA);
              }
            }
          }
        }
      }
    }
    fclose(f);
  }
  return retval;
}
