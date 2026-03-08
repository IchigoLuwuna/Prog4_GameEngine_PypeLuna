#include "ImGuiComponents.h"
#include <cmath>
#include <numeric>
#include "Timer.h"
#include "imgui_plot.h"

void dae::ImGuiComponents::Exercise1Component::Update()
{
	m_SampleCount = std::max( 1, m_SampleCount );

	if ( m_TakeMeasurement )
	{
		Measure();
		m_TakeMeasurement = false;
	}
}

void dae::ImGuiComponents::Exercise1Component::Render() const
{
	if ( ImGui::Begin( "Exercise 1" ) )
	{
		ImGui::InputInt( "# of samples", &m_SampleCount );

		if ( ImGui::Button( "Thrash the cache" ) )
		{
			m_TakeMeasurement = true;
		}

		if ( !m_Results.empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = m_Results.data();
			conf.values.count = m_Results.size();
			conf.values.color = 0xFF'00'7F'FF;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( m_Results.begin(), m_Results.end() );
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

void dae::ImGuiComponents::Exercise1Component::Measure()
{
	m_Results.clear();

	std::vector<int> bigBuffer( std::pow( 2, 26 ) );
	std::vector<float> sampledMeasurements( m_SampleCount );

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
		for ( int sample{}; sample < m_SampleCount; ++sample )
		{
			dae::Timer::GetInstance().Lap();
			iterateOverBuffer( std::pow( 2, iteration ) );
			dae::Timer::GetInstance().Lap();
			sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
		}

		// Remove Outliers
		if ( m_SampleCount > 2 )
		{
			auto sampledMeasurementsMinMax{ std::minmax_element( sampledMeasurements.begin(),
																 sampledMeasurements.end() ) };
			sampledMeasurements.erase( sampledMeasurementsMinMax.first );
			sampledMeasurements.erase( sampledMeasurementsMinMax.second );
		}

		// Get Average
		float sum{ std::accumulate( sampledMeasurements.begin(), sampledMeasurements.end(), 0.f ) };
		sum /= sampledMeasurements.size();
		m_Results.push_back( sum * 1000.f );
	}
}

void dae::ImGuiComponents::Exercise2Component::Render() const
{
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
		ImGui::InputInt( "# of samples", &m_SampleCount );

		if ( ImGui::Button( "Thrash the cache with GameObject3D" ) )
		{
			m_TakeMeasurement[0] = true;
		}

		if ( ImGui::Button( "Thrash the cache with GameObject3DAlt" ) )
		{
			m_TakeMeasurement[1] = true;
		}

		if ( !m_Results[0].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = m_Results[0].data();
			conf.values.count = m_Results[0].size();
			conf.values.color = 0xFF'FF'7F'00;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( m_Results[0].begin(), m_Results[0].end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "p1Results", conf );
		}

		if ( !m_Results[1].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.ys = m_Results[1].data();
			conf.values.count = m_Results[1].size();
			conf.values.color = 0xFF'7F'FF'00;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( m_Results[1].begin(), m_Results[1].end() );
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2( 200, 100 );
			conf.line_thickness = 2.f;

			ImGui::Plot( "p2Results", conf );
		}

		if ( !m_Results[0].empty() && !m_Results[1].empty() )
		{
			constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };
			const float* combinedData[2]{ m_Results[0].data(), m_Results[1].data() };

			ImU32 colors[]{ 0xFF'FF'7F'00, 0xFF'7F'FF'00 };

			ImGui::PlotConfig conf{};
			conf.values.xs = xValues;
			conf.values.count = m_Results[0].size();
			conf.values.ys_list = combinedData;
			conf.values.ys_count = 2;
			conf.values.colors = colors;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element( m_Results[0].begin(), m_Results[0].end() );
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

void dae::ImGuiComponents::Exercise2Component::Update()
{
	m_SampleCount = std::max( 1, m_SampleCount );

	if ( m_TakeMeasurement[0] || m_TakeMeasurement[1] )
	{
		Measure();

		m_TakeMeasurement[0] = false;
		m_TakeMeasurement[1] = false;
	}
}

void dae::ImGuiComponents::Exercise2Component::Measure()
{
#ifndef __EMSCRIPTEN__
	constexpr float bigBufferSize{ 26.f };
#else
	constexpr float bigBufferSize{ 24.5f }; // Must be small enough to comply with browser RAM limits
#endif

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

	if ( m_TakeMeasurement[0] )
	{
		m_Results[0].clear();

		std::vector<GameObject3D> bigBuffer( std::pow( 2, bigBufferSize ) );
		std::vector<float> sampledMeasurements( m_SampleCount );

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
			for ( int sample{}; sample < m_SampleCount; ++sample )
			{
				dae::Timer::GetInstance().Lap();
				iterateOverBuffer( std::pow( 2, iteration ) );
				dae::Timer::GetInstance().Lap();
				sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
			}

			// Remove Outliers
			if ( m_SampleCount > 2 )
			{
				auto sampledMeasurementsMinMax{ std::minmax_element( sampledMeasurements.begin(),
																	 sampledMeasurements.end() ) };
				sampledMeasurements.erase( sampledMeasurementsMinMax.first );
				sampledMeasurements.erase( sampledMeasurementsMinMax.second );
			}

			// Get Average
			float sum{ std::accumulate( sampledMeasurements.begin(), sampledMeasurements.end(), 0.f ) };
			sum /= sampledMeasurements.size();
			m_Results[0].push_back( sum * 1000.f );
		}
	}

	if ( m_TakeMeasurement[1] )
	{
		m_Results[1].clear();

		std::vector<GameObject3DAlt> bigBuffer( std::pow( 2, bigBufferSize ) );
		std::vector<float> sampledMeasurements( m_SampleCount );

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
			for ( int sample{}; sample < m_SampleCount; ++sample )
			{
				dae::Timer::GetInstance().Lap();
				iterateOverBuffer( std::pow( 2, iteration ) );
				dae::Timer::GetInstance().Lap();
				sampledMeasurements.push_back( dae::Timer::GetInstance().GetElapsed() );
			}

			// Remove Outliers
			if ( m_SampleCount > 2 )
			{
				auto sampledMeasurementsMinMax{ std::minmax_element( sampledMeasurements.begin(),
																	 sampledMeasurements.end() ) };
				sampledMeasurements.erase( sampledMeasurementsMinMax.first );
				sampledMeasurements.erase( sampledMeasurementsMinMax.second );
			}

			// Get Average
			float sum{ std::accumulate( sampledMeasurements.begin(), sampledMeasurements.end(), 0.f ) };
			sum /= sampledMeasurements.size();
			m_Results[1].push_back( sum * 1000.f );
		}
	}
}
