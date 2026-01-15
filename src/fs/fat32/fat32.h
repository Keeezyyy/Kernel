#pragma once

enum FILE_ACCESS {
  READ,
  WRITE,
  READ_WRITE,
};

void *open_file(const char * const path, enum FILE_ACCESS access);
