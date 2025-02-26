#pragma once

//#include <text_renderer.hpp>
#include <app_input.hpp>
#include <UI_button.hpp>


class Resource {
public:
	Resource() = default;
	Resource(glm::vec2 pos, std::string iconPath, Shader& shader, float yOffset = 0, int startAmount = 0)
		: m_pos(pos), 
		m_amount(startAmount),
		m_icon(glm::vec2(pos.x-55, pos.y), glm::vec2(50, 50), iconPath, shader, GL_RGBA),
		m_yOffset(yOffset)
	{
		//m_textRenderer.setText(std::to_string(m_amount));
	}

	void draw(TextRenderer& textRenderer) {
		textRenderer.RenderText(std::to_string(m_amount), m_pos.x, m_pos.y+m_yOffset, 1.f, m_colour);
		m_icon.Render();
	}

	void add() {
		m_amount++;
	}

	void set(int amount) {
		m_amount = amount;
	}

	bool remove(int amount) {
		if (m_amount - amount >= 0) {
			m_amount -= amount;
			return true;
		}
		return false;
	}

private:
	glm::vec2 m_pos;
	int m_amount;
	glm::vec3 m_colour = glm::vec3(0.8f, 0.8f, 0.8f);
	UI::Rect m_icon;
	float m_yOffset;
};
