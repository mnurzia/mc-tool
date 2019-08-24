#include "Annotator.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "../../../ImGuiEx.hpp"
#include "../../../Icons.hpp"
#include "../../../../nbt/TagByte.hpp"
#include "../../../../nbt/TagShort.hpp"
#include "../../../../nbt/TagInt.hpp"
#include "../../../../nbt/TagLong.hpp"
#include "../../../../nbt/TagFloat.hpp"
#include "../../../../nbt/TagDouble.hpp"
#include "../../../../nbt/TagString.hpp"
#include "../../../../nbt/TagByteArray.hpp"
#include "../../../../nbt/TagIntArray.hpp"
#include "../../../../nbt/TagLongArray.hpp"
#include "../../hexeditor/HexEditor.hpp"

namespace App::Components::NBTView::Schema {

	Annotator::Annotator(App* parent_) {
		parent = parent_;
	}

	void Annotator::begin() {
		ImGui::Columns(2, "nbt_view");
		ImVec2 fpad = ImGui::GetStyle().FramePadding;
		fpad.y = 0;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, fpad);
	}

	void Annotator::end() {
		ImGui::PopStyleVar();
		ImGui::Columns(1);
	}

	bool Annotator::renderHeaderIcon(const std::string& name, const char* type, ImGuiEx::Icon icon, ImGuiEx::Icon aux, bool arr) {
		float px = ImGui::GetCursorPosX() + 18.0f;
		static const char spaces[51] = {
			"                                                 \0"
		};
		const char* spcptr = &(
			spaces[50 - int(32.0f / (ImGui::CalcTextSize(" ").x))]
		);
		bool open = false;
		bool show_tooltip = false;
		if (arr) {
			open = ImGui::TreeNodeEx((const char*)name.c_str(), 0, "%s%s", spcptr, name.c_str());
		} else {
			open = ImGui::TreeNodeEx((const char*)name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s%s", spcptr, name.c_str());
		}
		ImGui::SameLine(px);
		ImGuiEx::AddIcon(icon);
		show_tooltip = ImGui::IsItemHovered();
		ImGuiEx::AddIconLayer(aux);
		if (show_tooltip) {
			ImGui::BeginTooltip();
			ImGuiEx::AddIcon(icon);
			ImGuiEx::AddIconLayer(aux);
			ImGui::Text("%s   " ICON_PASTEL_ARROW_RIGHT, name.c_str());
			ImGui::SameLine();
			ImGui::TextDisabled("%s", type);
			ImGui::EndTooltip();
		}
		return open;
	}

	void Annotator::renderTag(const std::string& name, NBT::Tag* in) {
		NBT::Tag::Type current_type = in->getType();
		switch (current_type) {
			case NBT::Tag::Type::BYTE: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("B", NULL, Util::Color(220, 101, 152, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				int8_t val = ((NBT::TagByte*)in)->getValue();
				if (ImGui::InputScalar("##input", ImGuiDataType_S8, &val)) {
					((NBT::TagByte*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::SHORT: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("S", NULL, Util::Color(157, 174, 89, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				int16_t val = ((NBT::TagShort*)in)->getValue();
				if (ImGui::InputScalar("##input", ImGuiDataType_S16, &val)) {
					((NBT::TagShort*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::INT: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("I", NULL, Util::Color(118, 101, 120, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				int32_t val = ((NBT::TagInt*)in)->getValue();
				if (ImGui::InputScalar("##input", ImGuiDataType_S32, &val)) {
					((NBT::TagInt*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::LONG: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("L", NULL, Util::Color(87, 138, 206, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				int64_t val = ((NBT::TagLong*)in)->getValue();
				if (ImGui::InputScalar("##input", ImGuiDataType_S64, &val)) {
					((NBT::TagLong*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::FLOAT: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("F", NULL, Util::Color(189, 87, 206, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				float val = ((NBT::TagFloat*)in)->getValue();
				if (ImGui::InputFloat("##input", &val)) {
					((NBT::TagFloat*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::DOUBLE: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE), ImGuiEx::Icon("D", NULL, Util::Color(206, 87, 189, 255)));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				double val = ((NBT::TagDouble*)in)->getValue();
				if (ImGui::InputDouble("##input", &val)) {
					((NBT::TagDouble*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::STRING: {
				renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PAGE_WHITE_TEXT));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				std::string val(((NBT::TagString*)in)->getValue());
				if (ImGui::InputText("##input", &val)) {
					((NBT::TagString*)in)->setValue(val);
				}
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::COMPOUND: {
				bool need_open = renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_PACKAGE), ImGuiEx::Icon(), true);
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				ImGui::Text("%i entries", ((NBT::TagCompound*)in)->entries());
				ImGui::PopID();
				ImGui::NextColumn();
				if (need_open) {
					renderCompound((NBT::TagCompound*)in);
					ImGui::TreePop();
				}
				break;
			}
			case NBT::Tag::Type::LIST: {
				NBT::TagList* val = (NBT::TagList*)in;
				std::string type = std::string(NBT::TagNames[int8_t(current_type)]);
				type += "<";
				type += std::string(NBT::TagNames[int8_t(val->getListType())]);
				type += ">";
				bool need_open = renderHeaderIcon(name, type.c_str(), ImGuiEx::Icon(ICON_PASTEL_TABLE), ImGuiEx::Icon(), true);
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				if (val->alloc() && val->getListType() == NBT::Tag::Type::END) {
					ImGui::Text("%i items (%i allocated)", val->items(), val->alloc());
				} else {
					ImGui::Text("%i items", val->items());
				}
				ImGui::PopID();
				ImGui::NextColumn();
				if (need_open) {
					renderList(val);
					ImGui::TreePop();
				}
				break;
			}
			case NBT::Tag::Type::BYTE_ARRAY: {
				bool need_open = renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_TABLE_LIGHTNING));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				if (ImGui::Button("Hex")) {
					getParent()->addComponent(
						std::make_unique<HexEditor::HexEditor<int8_t>>(
							getParent(), 
							name,
							((NBT::TagByteArray*)in)->data()
						)
					);
				}
				ImGui::SameLine();
				ImGui::Text("%i items", ((NBT::TagByteArray*)in)->items());
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::INT_ARRAY: {
				bool need_open = renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_TABLE_LIGHTNING));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				if (ImGui::Button("Hex")) {
					getParent()->addComponent(
						std::make_unique<HexEditor::HexEditor<int32_t>>(
							getParent(),
							name,
							((NBT::TagIntArray*)in)->data()
						)
					);
				}
				ImGui::SameLine();
				ImGui::Text("%i items", ((NBT::TagIntArray*)in)->items());
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
			case NBT::Tag::Type::LONG_ARRAY: {
				bool need_open = renderHeaderIcon(name, NBT::TagNames[int8_t(current_type)], ImGuiEx::Icon(ICON_PASTEL_TABLE_LIGHTNING));
				ImGui::NextColumn();
				ImGui::PushID(name.c_str());
				if (ImGui::Button("Hex")) {
					getParent()->addComponent(
						std::make_unique<HexEditor::HexEditor<int64_t>>(
							getParent(),
							name,
							((NBT::TagLongArray*)in)->data()
							)
					);
				}
				ImGui::SameLine();
				ImGui::Text("%i items", ((NBT::TagLongArray*)in)->items());
				ImGui::PopID();
				ImGui::NextColumn();
				break;
			}
		}
	}

	void Annotator::renderCompound(NBT::TagCompound* in) {
		for (auto& el : *in) {
			renderTag(el.first, el.second.get());
		}
	}

	void Annotator::renderList(NBT::TagList* in) {
		size_t i = 0;
		for (auto& el : *in) {
			const std::string& temp = "[" + std::to_string(i) + "]";
			renderTag(temp, el.get());
			i++;
		}
	}

	App* Annotator::getParent() {
		return parent;
	}

}