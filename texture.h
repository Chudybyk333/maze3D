#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>

unsigned int LoadTexture(const char* path);
unsigned int LoadCubemap(const std::vector<std::string>& faces);

#endif