#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    // user did not supply a filename
    puts("Supply a filename.");
    return 1;
  }

  size_t lines_capacity = 16;
  size_t lines_len = 0;
  char** lines = malloc(lines_capacity * sizeof(char*));

  size_t longest_line_len = 0;
  
  FILE *file;
  
  file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Could not open file");
    return 1;
  }

  while (!feof(file)) {
    char chunk[128] = {0};
    size_t buffer_size = sizeof(chunk);
    char* buffer = malloc(buffer_size);
    // strcat() expects buffer to be a valid C string too,
    // so we initialize it to be the empty string
    *buffer = '\0';

    // read line into `buffer`
    while (fgets(chunk, sizeof(chunk), file) != NULL) {
      if (ferror(file)) {
        // crash and burn
        perror("Could not read file");
        fclose(file);

        free(buffer);
        for (size_t i = 0; i < lines_len; i++) {
          free(lines[i]);
        }
        free(lines);

        return 1;
      }

      strcat(buffer, chunk);

      // if we haven't hit a new line, expand buffer to store more characters.
      // the first condition checks if the buffer is completely filled,
      // the second checks if the second-to-last character is a newline.
      // overall this checks if we have encountered a newline on this line yet.
      if (strlen(buffer) != buffer_size - 1 || buffer[buffer_size - 2] == '\n') {
        break;
      }

      buffer_size = buffer_size + sizeof(chunk) - 1;
      buffer = realloc(buffer, buffer_size);
    }

    if (strlen(buffer) > longest_line_len) {
      longest_line_len = strlen(buffer);
    }

    // store buffer, which now contains a whole line, in lines
    if (lines_len + 1 > lines_capacity) {
      lines = realloc(lines, lines_capacity * 2 * sizeof(char*));
      lines_capacity *= 2;
    }

    lines[lines_len] = buffer;
    lines_len++;
  }

  fclose(file);

  for (size_t line = 0; line < lines_len; line++) {
    size_t len = strlen(lines[line]);
    if (len == 0) {
      printf("\n");
    } else {
      for (size_t i = 0; i < (longest_line_len - len) / 2; i++) {
        printf(" ");
      }
      printf("%s", lines[line]);
    }

    free(lines[line]);
  }
  free(lines);
  return 0;
}
