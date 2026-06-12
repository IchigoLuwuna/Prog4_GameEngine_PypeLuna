#ifndef SCORE_FUNCTIONS_H
#define SCORE_FUNCTIONS_H
#include <cstdint>
#include <utility>

/*
 Score entry uses the following binary format:
 0x00 - [score 0]
 0x01 - [score 1]
 0x02 - [score 2]
 0x03 - [score 3]
 0x04 - [char 0]
 0x05 - [char 1]
 0x06 - [char 2]
 0x07 - [char 3]
 0x08 - [char 4]
 0x09 - [char 5]
 0x0a - [char 6]
 0x0b - [char 7]
 */

namespace dae::functions
{
std::pair<uint32_t, std::array<char, 8>> GetHighScore();
void SetHighScore( uint32_t score, const std::array<char, 8>& name );
} // namespace dae::functions
#endif
