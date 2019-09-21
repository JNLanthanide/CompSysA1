#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <assert.h>
#include <errno.h>
#include <string.h>

int print_error(char* path, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n",
    path, strerror(errnum));
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

int main(int argc, char *argv[])
{
int retval = EXIT_SUCCESS;

if (argc != 2) {
  fprintf(stderr, "%s", "Usage: file path");
  retval = EXIT_FAILURE;
  return retval;
}

FILE* f = fopen(argv[1], "r");

if (f == NULL) {
  print_error(argv[1], errno);
  int retval = EXIT_SUCCESS;
  return retval;
}

int nBytes = 0;
int isUTF8 = 1;
int test = 0;
signed char buffer = 0;
while (1) {
  if (fread(&buffer, 1, 1, f) != 0) {
    for(int i = 0; i < 8; i++)
    {
        printf("%d", (buffer>>i)&1);
    }
    if (nBytes == 0) {
      if (is4Byte(buffer) == 1)
      {
          test = 1;
          nBytes = 3;
          printf("%s%d%s%d", "  4 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
      }
      else
      {
        if (is3Byte(buffer) == 1)
        {
            nBytes = 2;
            test = 1;
            printf("%s%d%s%d", "  3 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
        }
        else
        {
          if (is2Byte(buffer) == 1)
          {
              nBytes = 1;
              test = 1;
              printf("%s%d%s%d", "  2 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
          }
          else
          {
            if (is1Byte(buffer) == 1)
            {
                nBytes = 0;
                printf("%s%d%s%d", "  1 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
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
          printf("%s%d%s%d", "  4 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
      }
      else
      {
        if (is3Byte(buffer) == 1)
        {
            isUTF8 = 0;
            printf("%s%d%s%d", "  3 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
        }
        else
        {
          if (is2Byte(buffer) == 1)
          {
              isUTF8 = 0;
              printf("%s%d%s%d", "  2 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
          }
          else
          {
            if (is1Byte(buffer) == 1)
            {
                isUTF8 = 0;
                printf("%s%d%s%d", "  1 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
            }
            else
            {
                if (isFollowByte(buffer) == 1)
                {
                  nBytes--;
                    printf("%s%d%s%d", "  1 Bytes, nBytes = ", nBytes, "  isUTF8 = ", isUTF8);
                }
            }
          }
        }
      }
    }
    printf("\n");
  }
  else // DID NOT READ CHAR
  {
    printf("%s%d\n", "TEST = ", test);
    if (nBytes == 0 && isUTF8 == 1 && test == 1) {
      printf("%s\n", "ITS FUCKING UTF8");
    }
    else
    {
      printf("%s\n", "ØØØV MAND");
    }
    break;
  }
}
}


    /*for(int i = 0; i < 8; i++)
    {
        printf("%d", (buffer>>i)&1);
    }
    printf("\n");
    printf("%s %d\n", "Bitshift 5", (buffer>>5)&1);
    */
