#include "TrashTheCacheScene.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <chrono>
#include <numeric>
#include <thread>


void TrashTheCacheScene::OnCreate()
{
	// Constexpr because of warning C4127: conditional expression is constant
	// Checks the size of the pointer to determine 32 or 64 bit system
	if constexpr(sizeof(void*) == 4)
	{
		// 32 bit
		m_BufferSize = static_cast<int>(std::pow(2, 22));
	}
	else
	{
		// 64 bit
		m_BufferSize = static_cast<int>(std::pow(2, 26));
	}
}

void TrashTheCacheScene::OnImGui()
{
	ImGui::PlotConfig conf;
	conf.scale.min = 0;
	conf.tooltip.show = true;
	conf.tooltip.format = "Stepsize:%.0f\nValue: %.0f";
	conf.grid_x.show = false;
	conf.grid_y.show = false;
	conf.frame_size = ImVec2(250, 100);
	conf.line_thickness = 2.0f;


	ImGui::Begin("Exercise 01");
	{
		ImGui::InputInt("# samples", &m_NrOfSamplesExercise01, 1, 100);
		if(!m_LoadingExercise01)
		{
			if(ImGui::Button("Trash The Cache"))
			{
				std::thread thread{ &TrashTheCacheScene::Exercise01, this };
				thread.detach();
			}
		}
		else
		{
			ImGui::Text("Waiting...");
		}

		// Only show if theres data and is no longer loading
		if(m_Exercise01XData.size() > 0 && !m_LoadingExercise01)
		{
			conf.values.color = ImColor(255, 150, 0);
			conf.values.xs = m_Exercise01XData.data();
			conf.values.ys = m_Exercise01YData.data();
			conf.values.count = static_cast<int>(m_Exercise01XData.size());
			conf.scale.max = *std::max_element(begin(m_Exercise01YData), end(m_Exercise01YData));
			ImGui::Plot("Exercise01", conf);
		}
	}
	ImGui::End();

	ImGui::Begin("Exercise 02");
	{
		ImGui::InputInt("# samples", &m_NrOfSamplesExercise02, 1, 100);
		if(!m_LoadingExercise02A)
		{
			if(ImGui::Button("Trash The Cache with GameObject3D"))
			{
				std::thread thread{ &TrashTheCacheScene::Exercise02A, this };
				thread.detach();
			}
		}
		else
		{
			ImGui::Text("Waiting...");
		}

		// Only show if theres data and is no longer loading
		if(m_Exercise02AXData.size() > 0 && !m_LoadingExercise02A)
		{
			conf.values.color = ImColor(50, 255, 50);
			conf.values.xs = m_Exercise02AXData.data();
			conf.values.ys = m_Exercise02AYData.data();
			conf.values.count = static_cast<int>(m_Exercise02AXData.size());
			conf.scale.max = *std::max_element(begin(m_Exercise02AYData), end(m_Exercise02AYData));
			ImGui::Plot("Exercise02A", conf);
		}

		if(!m_LoadingExercise02B)
		{
			if(ImGui::Button("Trash The Cache with GameObject3DAlt"))
			{
				std::thread thread{ &TrashTheCacheScene::Exercise02B, this };
				thread.detach();
			}
		}
		else
		{
			ImGui::Text("Waiting...");
		}

		// Only show if theres data and is no longer loading
		if(m_Exercise02BXData.size() > 0 && !m_LoadingExercise02B)
		{ 
			conf.values.color = ImColor(50, 50, 255);
			conf.values.xs = m_Exercise02BXData.data();
			conf.values.ys = m_Exercise02BYData.data();
			conf.values.count = static_cast<int>(m_Exercise02BXData.size());
			conf.scale.max = *std::max_element(begin(m_Exercise02BYData), end(m_Exercise02BYData));
			ImGui::Plot("Exercise02A", conf);
		}

		// Show combined plot when both exercises have data and are no longer loading
		if(m_Exercise02AXData.size() > 0 && m_Exercise02BXData.size() > 0 && !m_LoadingExercise02A && !m_LoadingExercise02B)
		{
			static const float* y_data[] = { m_Exercise02AYData.data(), m_Exercise02BYData.data() };
			static ImU32 colors[2] = { ImColor(50, 255, 50), ImColor(50, 50, 255) };

			ImGui::Text("Combined:");
			conf.values.xs = m_Exercise02AXData.data();
			conf.values.count = static_cast<int>(m_Exercise02AXData.size());
			conf.values.ys = nullptr; // Clear data
			conf.values.ys_list = y_data;
			conf.values.ys_count = 2;
			conf.values.colors = colors;

			conf.scale.max = *std::max_element(begin(m_Exercise02AYData), end(m_Exercise02AYData));
			ImGui::Plot("Extra", conf);
		}

	}
	ImGui::End();
}

void TrashTheCacheScene::Exercise01()
{
	m_Exercise01XData.clear();
	m_Exercise01YData.clear();

	m_LoadingExercise01 = true;
	// Allocate a large buffer of integers (for example 2 ^ 26 integers)
	// Iterate over the buffer in steps of 1, 2, 4, 8 and so on till 1024
	// Every step you perform some operation on the integer, for example:
	// for(int i = 0; i < arr.length; i += step) arr[i] *= 2;
	// Measure how long it takes for the loop to complete for every step and output these timings in the console, for example:	

	// Create an array of integers with size 2^26, default initialized with 2;
	std::vector<int> arr(m_BufferSize, 2);

	// To store sample data and remove biggest + smallest
	std::map<int, std::vector<std::chrono::microseconds>> sampleData{};

	for(size_t sampleIdx{}; sampleIdx < static_cast<size_t>(m_NrOfSamplesExercise01); sampleIdx++)
	{
		for(int stepSize{ 1 }; stepSize <= m_MaxStepSize; stepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for(size_t i{}; i < arr.size(); i += stepSize)
			{
				arr[i] *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

			// Code below shouldnt affect readings, since we already stop the measurement here.

			if(sampleData.count(stepSize) == 0)
			{
				sampleData[stepSize] = std::vector<std::chrono::microseconds>{};  // Create vector if the key doesnt exist yet.
			}
			sampleData[stepSize].push_back(elapsedTime);
		}
	}

	for(auto data : sampleData)
	{
		// Delete biggest and smallest element from the samples
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		// Calculate average
		auto total{ std::accumulate(begin(data.second), end(data.second), std::chrono::microseconds()) };
		double average{ total.count() / static_cast<double>(m_NrOfSamplesExercise01) };

		m_Exercise01XData.push_back(static_cast<float>(data.first));
		m_Exercise01YData.push_back(static_cast<float>(average));
	}

	m_LoadingExercise01 = false;
}

void TrashTheCacheScene::Exercise02A()
{
	// Do the same exercise as the previous one, but now with this GameObject3D class instead of integers
	// Create an array of integers with size 2^26;

	m_Exercise02AXData.clear();
	m_Exercise02AYData.clear();
	m_LoadingExercise02A = true;

	std::vector<GameObject3D> arr(m_BufferSize);

	// To store sample data and remove biggest + smallest
	std::map<int, std::vector<std::chrono::microseconds>> sampleData{};

	for(size_t sampleIdx{}; sampleIdx < static_cast<size_t>(m_NrOfSamplesExercise02); sampleIdx++)
	{
		for(int stepSize{ 1 }; stepSize <= m_MaxStepSize; stepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for(size_t i{}; i < arr.size(); i += stepSize)
			{
				arr[i].ID *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

			// Code below shouldnt affect readings, since we already stop the measurement here.

			if(sampleData.count(stepSize) == 0)
			{
				sampleData[stepSize] = std::vector<std::chrono::microseconds>{};  // Create vector if the key doesnt exist yet.
			}
			sampleData[stepSize].push_back(elapsedTime);
		}
	}

	for(auto data : sampleData)
	{
		// Delete biggest and smallest element from the samples
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		// Calculate average
		auto total{ std::accumulate(begin(data.second), end(data.second), std::chrono::microseconds()) };
		double average{ total.count() / static_cast<double>(m_NrOfSamplesExercise02) };

		m_Exercise02AXData.push_back(static_cast<float>(data.first));
		m_Exercise02AYData.push_back(static_cast<float>(average));
	}

	m_LoadingExercise02A = false;
}

void TrashTheCacheScene::Exercise02B()
{
	// Do the same exercise as the previous one, but now with this GameObject3Dalt class	
	// Create an array of integers with size 2^26;


	m_Exercise02BXData.clear();
	m_Exercise02BYData.clear();
	m_LoadingExercise02B = true;
	std::vector<GameObject3DAlt> arr(m_BufferSize);

	// To store sample data and remove biggest + smallest
	std::map<int, std::vector<std::chrono::microseconds>> sampleData{};

	for(size_t sampleIdx{}; sampleIdx < static_cast<size_t>(m_NrOfSamplesExercise02); sampleIdx++)
	{
		for(int stepSize{ 1 }; stepSize <= m_MaxStepSize; stepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for(size_t i{}; i < arr.size(); i += stepSize)
			{
				arr[i].ID *= 2;
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

			// Code below shouldnt affect readings, since we already stop the measurement here.

			if(sampleData.count(stepSize) == 0)
			{
				sampleData[stepSize] = std::vector<std::chrono::microseconds>{};  // Create vector if the key doesnt exist yet.
			}
			sampleData[stepSize].push_back(elapsedTime);
		}
	}

	for(auto data : sampleData)
	{
		// Delete biggest and smallest element from the samples
		data.second.erase(std::max_element(begin(data.second), end(data.second)));
		data.second.erase(std::min_element(begin(data.second), end(data.second)));

		// Calculate average
		auto total{ std::accumulate(begin(data.second), end(data.second), std::chrono::microseconds()) };
		double average{ total.count() / static_cast<double>(m_NrOfSamplesExercise02) };

		m_Exercise02BXData.push_back(static_cast<float>(data.first));
		m_Exercise02BYData.push_back(static_cast<float>(average));
	}

	m_LoadingExercise02B = false;

}
