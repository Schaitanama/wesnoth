/*
   Copyright (C) 2009 - 2016 by Mark de Wever <koraq@xs4all.nl>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/dialogs/campaign_selection.hpp"

#include "game_preferences.hpp"
#include "gui/auxiliary/find_widget.hpp"
#include "gui/dialogs/helper.hpp"
#include "gui/dialogs/campaign_settings.hpp"
#include "gui/widgets/button.hpp"
#include "gui/widgets/image.hpp"
#ifdef GUI2_EXPERIMENTAL_LISTBOX
#include "gui/widgets/list.hpp"
#else
#include "gui/widgets/listbox.hpp"
#endif
#include "gui/widgets/multi_page.hpp"
#include "gui/widgets/scroll_label.hpp"
#include "gui/widgets/settings.hpp"
#include "gui/widgets/tree_view.hpp"
#include "gui/widgets/tree_view_node.hpp"
#include "gui/widgets/toggle_button.hpp"
#include "gui/widgets/window.hpp"
#include "serialization/string_utils.hpp"

#include "utils/functional.hpp"
#include "video.hpp"

namespace gui2
{
namespace dialogs
{

/*WIKI
 * @page = GUIWindowDefinitionWML
 * @order = 2_campaign_selection
 *
 * == Campaign selection ==
 *
 * This shows the dialog which allows the user to choose which campaign to
 * play.
 *
 * @begin{table}{dialog_widgets}
 *
 * campaign_list & & listbox & m &
 *         A listbox that contains all available campaigns. $
 *
 * -icon & & image & o &
 *         The icon for the campaign. $
 *
 * -name & & styled_widget & o &
 *         The name of the campaign. $
 *
 * -victory & & image & o &
 *         The icon to show when the user finished the campaign. The engine
 *         determines whether or not the user has finished the campaign and
 *         sets the visible flag for the widget accordingly. $
 *
 * campaign_details & & multi_page & m &
 *         A multi page widget that shows more details for the selected
 *         campaign. $
 *
 * -image & & image & o &
 *         The image for the campaign. $
 *
 * -description & & styled_widget & o &
 *         The description of the campaign. $
 *
 * @end{table}
 */

REGISTER_DIALOG(campaign_selection)

void campaign_selection::campaign_selected(window& window)
{
	if(new_widgets || true) {
		tree_view& tree
				= find_widget<tree_view>(&window, "campaign_tree", false);

		if(tree.empty()) {
			return;
		}

		assert(tree.selected_item());
		if(tree.selected_item()->id() != "") {
			const unsigned choice
				= lexical_cast<unsigned>(tree.selected_item()->id());

			multi_page& pages
				= find_widget<multi_page>(&window, "campaign_details", false);
			pages.select_page(choice);
			engine_.set_current_level(choice);
		}
	} else {
		const int selected_row
				= find_widget<listbox>(&window, "campaign_list", false)
						  .get_selected_row();

		multi_page& pages
				= find_widget<multi_page>(&window, "campaign_details", false);

		pages.select_page(selected_row);
		engine_.set_current_level(selected_row);
	}
}

void campaign_selection::show_settings(CVideo& video) {
	campaign_settings settings_dlg(engine_);
	settings_dlg.show(video);
}

void campaign_selection::pre_show(window& window)
{
	if(new_widgets || true) {
		/***** Setup campaign tree. *****/
		tree_view& tree
				= find_widget<tree_view>(&window, "campaign_tree", false);

		tree.set_selection_change_callback(
				std::bind(&campaign_selection::campaign_selected,
							this,
							std::ref(window)));

		window.keyboard_capture(&tree);

		string_map tree_group_field;
		std::map<std::string, string_map> tree_group_item;
		/***** Setup campaign details. *****/
		multi_page& pages
				= find_widget<multi_page>(&window, "campaign_details", false);

		unsigned id = 0;
		for(const auto & level : engine_.get_levels_by_type_unfiltered(ng::level::TYPE::SP_CAMPAIGN))
		{
			const config& campaign = level->data();

			/*** Add tree item ***/
			tree_group_field["label"] = campaign["icon"];
			tree_group_item["icon"] = tree_group_field;

			tree_group_field["label"] = campaign["name"];
			tree_group_item["name"] = tree_group_field;

			tree_group_field["label"] = campaign["completed"].to_bool() ? "misc/laurel.png" : "misc/blank-hex.png";
			tree_group_item["victory"] = tree_group_field;

			tree.add_node("campaign", tree_group_item).set_id(std::to_string(id++));

			/*** Add detail item ***/
			string_map detail_item;
			std::map<std::string, string_map> detail_page;

			detail_item["label"] = campaign["description"];
			detail_item["use_markup"] = "true";

			if(!campaign["description_alignment"].empty()) {
				detail_item["text_alignment"] = campaign["description_alignment"];
			}

			detail_page.emplace("description", detail_item);

			detail_item["label"] = campaign["image"];
			detail_page.emplace("image", detail_item);

			pages.add_page(detail_page);
		}
		if (!engine_.get_const_extras_by_type(ng::create_engine::MOD).empty()) {

			tree_group_field["label"] = "Modifications";
			tree_group_item["tree_view_node_label"] = tree_group_field;
			//tree_group_item["tree_view_node_label"] = tree_group_field;
			tree_view_node& mods_node = tree.add_node("campaign_group", tree_group_item);
			std::vector<std::string> enabled = engine_.active_mods();

			id = 0;
			tree_group_item.clear();
			for(const auto& mod : engine_.get_const_extras_by_type(ng::create_engine::MOD))
			{
				bool active = std::find(enabled.begin(), enabled.end(), mod->id) != enabled.end();
				/*** Add tree item ***/
				tree_group_field["label"] = mod->name;
				tree_group_item["checkb"] = tree_group_field;

				tree_view_node & node = mods_node.add_child("modification", tree_group_item);
				toggle_button* checkbox = dynamic_cast<toggle_button*>(node.find("checkb", true));
				VALIDATE(checkbox, missing_widget("checkb"));
				checkbox->set_value(active);
				checkbox->set_label(mod->name);
				checkbox->set_callback_state_change(std::bind(&campaign_selection::mod_toggled, this, id, _1));
				++id;
			}
		}
	} else {
		/***** Hide the tree view. *****/
		if(tree_view* tree
		   = find_widget<tree_view>(&window, "campaign_tree", false, false)) {

			tree->set_visible(widget::visibility::invisible);
		}

		/***** Setup campaign list. *****/
		listbox& list = find_widget<listbox>(&window, "campaign_list", false);
#ifdef GUI2_EXPERIMENTAL_LISTBOX
		connect_signal_notify_modified(
				list,
				std::bind(&campaign_selection::campaign_selected,
							this,
							std::ref(window)));
#else
		list.set_callback_value_change(
				dialog_callback<campaign_selection,
								&campaign_selection::campaign_selected>);
#endif
		window.keyboard_capture(&list);

		/***** Setup campaign details. *****/
		multi_page& pages
				= find_widget<multi_page>(&window, "campaign_details", false);

		for(const auto & level : engine_.get_levels_by_type_unfiltered(ng::level::TYPE::SP_CAMPAIGN))
		{
			const config& campaign = level->data();

			/*** Add list item ***/
			string_map list_item;
			std::map<std::string, string_map> list_item_item;

			list_item["label"] = campaign["icon"];
			list_item_item.emplace("icon", list_item);

			list_item["label"] = campaign["name"];
			list_item_item.emplace("name", list_item);

			grid* grid = &list.add_row(list_item_item);
			assert(grid);

			widget* widget = grid->find("victory", false);
			if(widget && !campaign["completed"].to_bool()) {
				widget->set_visible(widget::visibility::hidden);
			}

			/*** Add detail item ***/
			string_map detail_item;
			std::map<std::string, string_map> detail_page;

			detail_item["use_markup"] = "true";

			detail_item["label"] = campaign["description"];
			detail_page.emplace("description", detail_item);

			detail_item["label"] = campaign["image"];
			detail_page.emplace("image", detail_item);

			pages.add_page(detail_page);
		}
	}
	campaign_selected(window);

	/***** Setup advanced settings button *****/
	button* advanced_settings_button =
			find_widget<button>(&window, "advanced_settings", false, false);
	if(advanced_settings_button) {
		advanced_settings_button->connect_click_handler(
			std::bind(&campaign_selection::show_settings, this, std::ref(window.video())));
	}
}

void campaign_selection::post_show(window& window)
{
	if(new_widgets || true) {
		tree_view& tree
				= find_widget<tree_view>(&window, "campaign_tree", false);

		if(tree.empty()) {
			return;
		}

		assert(tree.selected_item());
		if(tree.selected_item()->id() != "") {
			choice_ = lexical_cast<unsigned>(tree.selected_item()->id());
		}
		deterministic_ = find_widget<toggle_button>(&window,
													 "checkbox_deterministic",
													 false).get_value_bool();

		preferences::set_modifications(engine_.active_mods(), false);
	} else {
		choice_ = find_widget<listbox>(&window, "campaign_list", false)
						  .get_selected_row();
		deterministic_ = find_widget<toggle_button>(&window,
													 "checkbox_deterministic",
													 false).get_value_bool();
	}
}

void campaign_selection::mod_toggled(int id, widget &)
{
	engine_.set_current_mod_index(id);
	engine_.toggle_current_mod();
}

} // namespace dialogs
} // namespace gui2
