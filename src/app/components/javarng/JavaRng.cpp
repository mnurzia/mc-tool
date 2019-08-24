#include "JavaRng.hpp"

#include <imgui/imgui.h>

#include "../../ImGuiEx.hpp"
#include "../../../worldgen/JavaRandom.hpp"

namespace App::Components::JavaRNG {

	JavaRNG::JavaRNG(App* app) : WindowComponent(app, "Java RNG Emulator", "javarng") {
		setSize(ImVec2(460, 240));
	}
	
	static bool setButton(const std::string& name, int64_t* permute_seed, int64_t temp_seed) {
		ImGui::SameLine();
		if (ImGui::Button((std::string("Set") + name).c_str())) {
			*permute_seed = temp_seed;
			ImGui::SameLine();
			return true;
		}
		ImGui::SameLine();
		return false;
	}

	void JavaRNG::renderWin() {
		int64_t permute_seed = 0;
		bool set = false;
		//printf("%lli %lli\n", current_seed, temp_seed);
		ImGui::InputScalar("##seed_in", ImGuiDataType_S64, &current_seed);
		int64_t temp_seed = current_seed;
		WorldGen::JavaRandom::setSeed(&temp_seed);
		set |= setButton("##seed_btn", &permute_seed, temp_seed);
		ImGui::Text("Seed");
		temp_seed = current_seed;

		int32_t nextint = WorldGen::JavaRandom::nextInt(&temp_seed);
		ImGui::InputScalar("##nextint_in", ImGuiDataType_S32, &nextint, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
		set |= setButton("##nextint_btn", &permute_seed, temp_seed);
		ImGui::Text("nextInt()");
		temp_seed = current_seed;

		int32_t nextintb[2] = {WorldGen::JavaRandom::nextInt(&temp_seed, abs(int_bound)), int_bound};
		ImGui::InputScalarN("##nextintb_in", ImGuiDataType_S32, &nextintb, 2, nullptr, nullptr, nullptr);
		int_bound = nextintb[1];
		set |= setButton("##nextintb_btn", &permute_seed, temp_seed);
		ImGui::Text("nextInt() [bounded]");
		temp_seed = current_seed;

		int64_t nextlong = WorldGen::JavaRandom::nextLong(&temp_seed);
		ImGui::InputScalar("##nextlong_in", ImGuiDataType_S64, &nextlong, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
		set |= setButton("##nextlong_btn", &permute_seed, temp_seed);
		ImGui::Text("nextLong()");
		temp_seed = current_seed;

		float nextfloat = WorldGen::JavaRandom::nextFloat(&temp_seed);
		ImGui::InputScalar("##nextfloat_in", ImGuiDataType_Float, &nextfloat, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
		set |= setButton("##nextfloat_btn", &permute_seed, temp_seed);
		ImGui::Text("nextFloat()");
		temp_seed = current_seed;

		double nextdouble = WorldGen::JavaRandom::nextDouble(&temp_seed);
		ImGui::InputScalar("##nextdouble_in", ImGuiDataType_Double, &nextdouble, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
		set |= setButton("##nextdouble_btn", &permute_seed, temp_seed);
		ImGui::Text("nextDouble()");
		temp_seed = current_seed;

		if (set) {
			current_seed = permute_seed;
		}
		ImGuiEx::HelpMarker(
			"How to use:\nEnter initial Java seed into Seed field and view nextXXX() outcomes. "
			"Use the Set buttons to set the seed to the next state after performing its respective "
			"calculation."
		);
	}

}