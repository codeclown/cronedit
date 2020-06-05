#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void make_dir_if_not_exists(std::string path);

std::string read_file(std::string path);

bool write_file(std::string path, std::string contents);

bool remove_file(std::string path);

#endif
