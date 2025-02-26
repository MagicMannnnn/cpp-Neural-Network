#pragma once

#include <UI_button.hpp>
#include <text_renderer.hpp>
#include <vector>
#include <resource.hpp>

namespace UI {

	enum MENU_ITEMS {
		HOUSE = 0,
		CROP,
		MENU_ITEMS_SIZE
	};

	enum RESOURCES {
		WOOD = 0,
		RESOURCES_SIZE
	};


	class Menu {
	public:
		Menu()
		:   m_rectShader(RESOURCES_PATH"rect.vert", RESOURCES_PATH"rect.frag"), 
			m_backround(glm::vec2(200, 20), glm::vec2(1500, 100), RESOURCES_PATH"textures/brown.png", m_rectShader, GL_RGB),
			m_textRenderer_16(RESOURCES_PATH"font1.ttf", 16, RESOURCES_PATH"text.vert", RESOURCES_PATH"text.frag"),
			m_textRenderer_8(RESOURCES_PATH"font1.ttf", 8, RESOURCES_PATH"text.vert", RESOURCES_PATH"text.frag"),
			m_textRenderer_4(RESOURCES_PATH"font1.ttf", 4, RESOURCES_PATH"text.vert", RESOURCES_PATH"text.frag")
		{
			m_menuItems.reserve(static_cast<size_t>(MENU_ITEMS::MENU_ITEMS_SIZE));
			m_resources.reserve(static_cast<size_t>(RESOURCES::RESOURCES_SIZE));

			m_menuItems.push_back(new Button(glm::vec2(210, 45), glm::vec2(50, 50), RESOURCES_PATH"icons/house.png", m_rectShader, GL_RGBA));
			m_menuItems.push_back(new Button(glm::vec2(210 + 70, 45), glm::vec2(50, 50), RESOURCES_PATH"icons/plant.png", m_rectShader, GL_RGBA));

			m_resources.push_back(new Resource(glm::vec2(210, Settings::SCR_HEIGHT - 90), RESOURCES_PATH"icons/wood.png", m_rectShader, 35));
		}

		~Menu() {
			m_menuItems.clear();
			m_resources.clear();
		}

		void draw() {
			glDisable(GL_DEPTH_TEST);
			m_backround.Render();
			
			for (Button* item : m_menuItems) {
				item->Render();
			}
			for (Resource* item : m_resources) {
				item->draw(m_textRenderer_16);
			}

			glEnable(GL_DEPTH_TEST);
		}

		void update() {
			for (Button* item : m_menuItems) {
				item->update(glm::vec2(Global::mouseLastX, Settings::SCR_HEIGHT - Global::mouseLastY));
				if (item->getClicked()) {
					std::cout << "clicked!\n";
				}
			}
		}

	private:
		Rect m_backround;
		Shader m_rectShader;
		std::vector<Button*> m_menuItems;
		std::vector<Resource*> m_resources;
		TextRenderer m_textRenderer_16;
		TextRenderer m_textRenderer_8;
		TextRenderer m_textRenderer_4;
	};
}