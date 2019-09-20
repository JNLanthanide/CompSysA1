#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <assert.h>
#include <errno.h>
#include <string.h>

enum file_type {
  DATA,
  EMPTY,
  ASCII,
  ISO,
  UTF8,
  UTF16BE,
  UTF16LE,
};

char* FILE_TYPE_STRINGS[] = {
  "data",
  "empty",
  "ASCII text",
  "ISO-8859 text",
  "UTF8 text",
  "UTF16 Big Endian text",
  "UTF16 Little Endian text",
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
    path, max_length - (int) strlen(path), " ", enumToString(ft));
}


int print_error(const char* path, int max_length, int errnum) {
  return fprintf(stdout, "%s:%*scannot determine (%s)\n",
  path, max_length - (int) strlen(path), " ", strerror(errnum));
}

int is4Byte(signed char n) {
  if (((n>>3)&1) == 0 && ((n>>4)&1) == 1 && ((n>>5)&1) == 1 && ((n>>6)&1) == 1 &&  ((n>>7)&1) == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

int is3Byte(signed char n) {
  if (((n>>4)&1) == 0 && ((n>>5)&1) == 1 && ((n>>6)&1) == 1 &&  ((n>>7)&1) == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

int is2Byte(signed char n) {
  if (((n>>5)&1) == 0 && ((n>>6)&1) == 1 &&  ((n>>7)&1) == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

int is1Byte(signed char n) {
  if (((n>>7)&1) == 0) {
    return 1;
  }
  else {
    return 0;
  }
}

int isFollowByte (signed char n) {
  if (((n>>6)&1) == 0 && ((n>>7)&1) == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

int check_UTF16(FILE* f) {
  rewind(f);
  unsigned char c[2];
  int isUTF16 = 0;
  if ((fread(&c, sizeof(char), sizeof(c), f) != 0) != 0) {
    if (c[0] == 0xFE && c[1] == 0xFF) {
      isUTF16 = 1;
    }
    else {
      if (c[0] == 0xFF && c[1] == 0xFE) {
        isUTF16 = 1;
      }
    }
  }
  return isUTF16;
}

int check_UTF8(FILE* f) {
  rewind(f);
  int nBytes = 0;
  int isUTF8 = 1;
  int retValue;
  signed char buffer = 0;
  while (1) {
    if (fread(&buffer, 1, 1, f) != 0) {
      if (nBytes == 0) {
        if (is4Byte(buffer) == 1)
        {
            nBytes = 3;
        }
        else
        {
          if (is3Byte(buffer) == 1)
          {
              nBytes = 2;
          }
          else
          {
            if (is2Byte(buffer) == 1)
            {
                nBytes = 1;
            }
            else
            {
              if (is1Byte(buffer) == 1)
              {
                  nBytes = 0;
              }
            }
          }
        }
      }
      else
      {
        if (is4Byte(buffer) == 1)
        {
            isUTF8 = 0;
        }
        else
        {
          if (is3Byte(buffer) == 1)
          {
              isUTF8 = 0;
          }
          else
          {
            if (is2Byte(buffer) == 1)
            {
                isUTF8 = 0;
            }
            else
            {
              if (is1Byte(buffer) == 1)
              {
                  isUTF8 = 0;
              }
              else
              {
                  if (isFollowByte(buffer) == 1)
                  {
                    nBytes--;
                  }
              }
            }
          }
        }
      }
    }
    else // DID NOT READ CHAR
    {
      if (nBytes == 0 && isUTF8 == 1) {
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

int isASCIIChar(int n) {
  if (!(n >= 0x07 && n <= 0x0D) && (n != 0x1B) && !(n >= 0x20 && n <= 0x7E)) {
    return 0;
  } else {
    return 1;
  }
}
int check_ASCII(FILE* f) {
  rewind(f);
  int buffer = 1;
  while(1) {
    if (fread(&buffer, sizeof(char), 1, f) != 0) {
      if (isASCIIChar(buffer) == 0) {
        return 0;
      }
    }
    else {
      break;
    }
  }
  return 1;
}

int isISOChar(int n) {
  if (!(n >= 160 && n <= 255) && isASCIIChar(n) == 0) {
    return 0;
  }
  return 1;
}

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

    FILE* f = fopen(argv[1], "r");

    if (f == NULL) {
      print_error(argv[1], 4, errno);
      int retval = EXIT_SUCCESS;
      return retval;
    }

    if (check_ASCII(f) == 1) {
      print_result(argv[1], 4, ASCII);
    }
    else
    {
      if (check_ISO(f) == 1) {
        print_result(argv[1], 4, ISO);
      }
      else
      {
        if (check_UTF8(f) == 1) {
          print_result(argv[1], 4, UTF8);
        }
        else
        {
          if (check_UTF16(f) == 1) {
            print_result(argv[1], 4, UTF16BE);
          }
        }
      }
    }
  fclose(f);
  return retval;
}
