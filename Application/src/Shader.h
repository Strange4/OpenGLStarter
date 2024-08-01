#pragma once
#include <string>

int create_shader(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
std::string read_from_file(const char* file_name);