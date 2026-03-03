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

	sampleCount = std::max( 1, sampleCount );

	if ( ImGui::Begin( "Exercise 1" ) )
	{
		ImGui::InputInt( "# of samples", &sampleCount );

		if ( ImGui::Button( "Thrash the cache" ) )
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
				// Take Measurements
				sampledMeasurements.clear();
				for ( int sample{}; sample < sampleCount; ++sample )
				{
					dae::Timer::GetInstance().Lap();
					iterateOverBuffer( std::pow( 2, iteration ) );
					dae::Timer::GetInstance().Lap();
					sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
				}

				// Remove Outliers
				if ( sampleCount > 2 )
				{
					sampledMeasurements.erase(
						std::min_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
					sampledMeasurements.erase(
						std::max_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
				}

				// Get Average
				float sum{};
				for ( auto& sample : sampledMeasurements )
				{
					sum += sample;
				}
				sum /= sampledMeasurements.size();
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
			conf.values.color = 0xFF'00'7F'FF;
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

void dae::ImGuiWindows::Exercise2()
{
	static int sampleCount{ 10 };
	static std::array<std::vector<float>, 2> results{};

#ifndef __EMSCRIPTEN__
	constexpr float bigBufferSize{ 26.f };
#else
	constexpr float bigBufferSize{ 24.5f }; // Must be small enough to comply with browser RAM limits
#endif

	sampleCount = std::max( 1, sampleCount );

	struct Transform
	{
		float matrix[4][4]{
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, 1.f },
		};
	};

	struct GameObject3D
	{
		Transform transform{};
		int id{ 1 };
	};

	struct GameObject3DAlt
	{
		Transform* transform{};
		int id{ 1 };
	};

	if ( ImGui::Begin( "Exercise 2" ) )
	{
		ImGui::InputInt( "# of samples", &sampleCount );

		if ( ImGui::Button( "Thrash the cache with GameObject3D" ) )
		{
			results[0].clear();

			std::vector<GameObject3D> bigBuffer( std::pow( 2, bigBufferSize ) );
			std::vector<float> sampledMeasurements( sampleCount );

			for ( auto& element : bigBuffer )
			{
				element.id = 1;
			}

			auto iterateOverBuffer{ [&]( int stepSize ) {
				for ( size_t index{}; index < bigBuffer.size(); index += stepSize )
				{
					bigBuffer[index].id *= 2;
				}
			} };

			std::vector<float> measurements{};

			for ( int iteration{}; iteration < 11; ++iteration )
			{
				// Take Measurements
				sampledMeasurements.clear();
				for ( int sample{}; sample < sampleCount; ++sample )
				{
					dae::Timer::GetInstance().Lap();
					iterateOverBuffer( std::pow( 2, iteration ) );
					dae::Timer::GetInstance().Lap();
					sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
				}

				// Remove Outliers
				if ( sampleCount > 2 )
				{
					sampledMeasurements.erase(
						std::min_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
					sampledMeasurements.erase(
						std::max_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
				}

				// Get Average
				float sum{};
				for ( auto& sample : sampledMeasurements )
				{
					sum += sample;
				}
				sum /= sampledMeasurements.size();
				results[0].push_back( sum * 1000.f );
			}
		}

		if ( ImGui::Button( "Thrash the cache with GameObject3DAlt" ) )
		{
			results[1].clear();

			std::vector<GameObject3DAlt> bigBuffer( std::pow( 2, bigBufferSize ) );
			std::vector<float> sampledMeasurements( sampleCount );

			for ( auto& element : bigBuffer )
			{
				element.id = 1;
			}

			auto iterateOverBuffer{ [&]( int stepSize ) {
				for ( size_t index{}; index < bigBuffer.size(); index += stepSize )
				{
					bigBuffer[index].id *= 2;
				}
			} };

			std::vector<float> measurements{};

			for ( int iteration{}; iteration < 11; ++iteration )
			{
				// Take Measurements
				sampledMeasurements.clear();
				for ( int sample{}; sample < sampleCount; ++sample )
				{
					dae::Timer::GetInstance().Lap();
					iterateOverBuffer( std::pow( 2, iteration ) );
					dae::Timer::GetInstance().Lap();
					sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
				}

				// Remove Outliers
				if ( sampleCount > 2 )
				{
					sampledMeasurements.erase(
						std::min_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
					sampledMeasurements.erase(
						std::max_element( sampledMeasurements.begin(), sampledMeasurements.end() ) );
				}

				// Get Average
				float sum{};
				for ( auto& sample : sampledMeasurements )
				{
					sum += sample;
				}
				sum /= sampledMeasurements.size();
				results[1].push_back( sum * 1000.f );
			}
		}

		if ( !results[0].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = results[0].data();
			conf.values.count = results[0].size();
			conf.values.color = 0xFF'FF'7F'00;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( results[0].begin(), results[0].end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "p1Results", conf );
		}

		if ( !results[1].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = results[1].data();
			conf.values.count = results[1].size();
			conf.values.color = 0xFF'7F'FF'00;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( results[1].begin(), results[1].end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "p2Results", conf );
		}

		if ( !results[0].empty() && !results[1].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };
			const float* combinedData[2]{ results[0].data(), results[1].data() };

			ImU32 colors[]{ 0xFF'FF'7F'00, 0xFF'7F'FF'00 };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.count = results[0].size();
			conf.values.ys_list = combinedData;
			conf.values.ys_count = 2;
			conf.values.colors = colors;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( results[0].begin(), results[0].end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "combined", conf );
		}
	}
	ImGui::End();
}
