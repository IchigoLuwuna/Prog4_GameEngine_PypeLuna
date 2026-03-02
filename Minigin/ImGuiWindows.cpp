#include "ImGuiWindows.h"
#include <cmath>
#include <vector>
#include <imgui.h>
#include "Timer.h"
#include "imgui_plot.h"

void dae::ImGuiWindows::Exercise1()
{
	static int sampleCount{ 10 };
	static std::vector<float> results{};

	if ( ImGui::Begin( "Exercise1" ) )
	{
		ImGui::InputInt( "# of samples", &sampleCount );

		if ( ImGui::Button( "Thrash the Cache" ) )
		{
			results.clear();

			std::vector<int> bigBuffer( std::pow( 2, 26 ) );
			std::vector<float> sampledMeasurements( sampleCount );

			for ( auto& element : bigBuffer )
			{
				element = 1;
			}

			auto iterateOverBuffer{ [&]( int stepSize ) {
				for ( size_t index{}; index < bigBuffer.size(); index += stepSize )
				{
					bigBuffer[index] *= 2;
				}
			} };

			for ( int iteration{}; iteration < 11; ++iteration )
			{
				sampledMeasurements.clear();
				for ( int sample{}; sample < sampleCount; ++sample )
				{
					dae::Timer::GetInstance().Lap();
					iterateOverBuffer( std::pow( 2, iteration ) );
					dae::Timer::GetInstance().Lap();
					sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
				}

				float sum{};
				for ( auto& sample : sampledMeasurements )
				{
					sum += sample;
				}
				sum /= sampleCount;
				results.push_back( sum * 1000.f );
			}
		}

		if ( !results.empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = results.data();
			conf.values.count = results.size();
			conf.values.color = 0xFF007FFF;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( results.begin(), results.end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "results", conf );
		}
	}
	ImGui::End();
}
