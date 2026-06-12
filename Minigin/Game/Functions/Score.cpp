#include "Score.h"
#include <fstream>
#include <array>

std::pair<uint32_t, std::array<char, 8>> dae::functions::GetHighScore()
{
	std::ifstream highScoreFile{ "score.bin", std::ios::binary };
	if ( !highScoreFile.is_open() )
	{
		return {};
	}

	uint32_t score{};
	std::array<char, 8> name{};

	highScoreFile.read( reinterpret_cast<char*>( &score ), sizeof( score ) );
	highScoreFile.read( reinterpret_cast<char*>( name.data() ), name.size() );

	return { score, name };
}
void dae::functions::SetHighScore( uint32_t score, const std::array<char, 8>& name )
{
	std::ofstream highScoreFile{ "score.bin", std::ios::binary };

	highScoreFile.write( reinterpret_cast<const char*>( &score ), sizeof( score ) );
	highScoreFile.write( reinterpret_cast<const char*>( name.data() ), name.size() );
}
