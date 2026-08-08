#pragma once

#include "editor/base/editor_ui.h"

namespace Lacrima
{
	class ToolUI : public EditorUI
	{
	public:
		virtual void init() override;
		virtual void construct() override;

	private:
		void constructCreateEntityPopup();

		std::vector<std::string> entity_categories;
		std::vector<std::vector<std::string>> entity_typess;
	};
}