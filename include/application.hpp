
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <app_input.hpp>
#include <shader.h>
#include <obj_loader.hpp>
#include <filesystem>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <tile.hpp>
#include <perlin_noise_2d.hpp>
#include <utils.hpp>
#include <resource.hpp>
#include <menu.hpp>


class App {
public:
    App()
        : //m_object(RESOURCES_PATH"objects/grass/hex1.obj"),
        m_shader(RESOURCES_PATH"object.vert", RESOURCES_PATH"object1.frag"),
        m_noise(Global::seed, 2048, 12, 0.9f, 0.1f),
        m_heightNoise(Global::seed * 3, 2048, 12, 0.2f, 0.5f),
        m_textRenderer(RESOURCES_PATH"font1.ttf", 16, RESOURCES_PATH"text.vert", RESOURCES_PATH"text.frag"),
        m_menu()
    {

        m_models[0] = Model((RESOURCES_PATH"objects/grass/hexagon_grass.obj"));
        m_models[1] = Model((RESOURCES_PATH"objects/forest/hexagon_forest.obj"));

        TILETYPE type;
        for (int z = 0; z < Settings::MAP_HEIGHT; z++) {
            for (int x = 0; x < Settings::MAP_WIDTH; x++) {
                if (m_noise.noise(x * 0.1f, z * 0.1f) > 0.8) {
                    type = FOREST;
                }
                else {
                    type = GRASS;
                }
                int rawNoise = (int)(m_heightNoise.noise(z * 0.01f, x * 0.01f) * 2.f);
                float height = (float)rawNoise / 10.f;
                height = 0;
                if (z % 2 == 0) {
                    m_tiles[z][x] = Tile(glm::vec3(x * Settings::TILE_APOTHEM * 2.f, height, z * Settings::TILE_RADIUS * 1.5f), type);
                }
                else {
                    m_tiles[z][x] = Tile(glm::vec3(x * Settings::TILE_APOTHEM * 2.f + Settings::TILE_APOTHEM, height, z * Settings::TILE_RADIUS * 1.5f), type);
                }
                
            }
        }


    }

	void run() {

        float currentFrame = 0.0f;
        float lastFrame = 0.0f;

        float timer = 0.f;
        int frameCounter = 0;

        while (!glfwWindowShouldClose(m_window)) {

            currentFrame = static_cast<float>(glfwGetTime());
            Global::deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            Global::processInput(m_window);
            getTile();

            glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mainGameLoop();

            glfwSwapBuffers(m_window);
            glfwPollEvents();


           // std::cout << std::round(Global::deltaTime * 1000.f * 100.f) / 100.f << "ms\n";
            timer += Global::deltaTime;
            frameCounter++;
            if (timer >= 1) {
                m_fps = (float)frameCounter / timer;
                timer = 0.f;
                frameCounter = 0;
            }
            
        }

	}


private:
	Camera& m_camera = Global::camera;
    GLFWwindow*& m_window = Global::window;
    unsigned int VAO, VBO, EBO;
    Shader m_shader;
    glm::mat4 m_projection = glm::perspective(glm::radians(m_camera.Zoom), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 0.1f, 200.0f);
    std::array<Model, 2> m_models;
    std::array<std::array<Tile, Settings::MAP_WIDTH>, Settings::MAP_HEIGHT> m_tiles;
    PerlinNoise2D m_noise;
    PerlinNoise2D m_heightNoise;
    bool m_mouseOverTile = false;
    glm::vec3 m_mouseIntersectPoint;
    glm::ivec2 m_mouseOverTilePos;
    TextRenderer m_textRenderer;
    int m_fps;
    UI::Menu m_menu;

    void mainGameLoop() {

        draw();
        m_textRenderer.RenderText(std::to_string(m_fps), 1800.f, Settings::SCR_HEIGHT - 60.f, 12, glm::vec3(1, 1, 1));
        m_menu.update();
   
        if (m_mouseOverTile && Global::leftClick) {
            Global::leftClick = false;
            if (m_tiles[m_mouseOverTilePos.x][m_mouseOverTilePos.y].type == FOREST) {
                
            }
        }
        
    }

    void draw() {
        m_camera.ExtractFrustumPlanes(m_projection, m_camera.GetViewMatrix());

        m_shader.use();
        m_shader.setMat4("projection", m_projection);
        m_shader.setMat4("view", m_camera.GetViewMatrix());

        m_shader.setVec3("lightPos", glm::vec3(-1000.0f, 10000.0f, 1000.0f));
        m_shader.setVec3("viewPos", m_camera.Position);
        m_shader.setVec3("lightColor", glm::vec3(1.05f, 0.95f, 0.95f) * 1.f);

        std::vector<glm::mat4> visibleInstances;

        for (int z = 0; z < Settings::MAP_HEIGHT; z++) {
            for (int x = 0; x < Settings::MAP_WIDTH; x++) {
                Tile& tile = m_tiles[z][x];

                if (m_camera.IsSphereInFrustum(tile.pos, Settings::TILE_RADIUS + 0.25f)) {
                    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), tile.pos);

                    if (m_mouseOverTilePos.x == z && m_mouseOverTilePos.y == x) {
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.125f, 0.f));
                    }

                    visibleInstances.push_back(modelMatrix);
                }
            }
        }

        if (!visibleInstances.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferData(GL_ARRAY_BUFFER, visibleInstances.size() * sizeof(glm::mat4), visibleInstances.data(), GL_DYNAMIC_DRAW);
        }

        // Draw all instances in one call
        for (auto& model : m_models) {
            model.DrawInstanced(m_shader, visibleInstances.size());
        }

        m_menu.draw();

        glBindVertexArray(0);
    }

    void getTile() {
        glm::vec3 rayOrigin;
        glm::vec3 rayDirection;
        Utils::screenToWorldRay(Global::mouseLastX, Global::mouseLastY, Settings::SCR_WIDTH, Settings::SCR_HEIGHT, m_projection, m_camera.GetViewMatrix(), rayOrigin, rayDirection);
        m_mouseIntersectPoint = Utils::rayPlaneIntersection(rayOrigin, rayDirection, 0.25f);

        m_mouseOverTilePos = glm::ivec2(-1, -1);
        m_mouseOverTile = false;
        
        if (Global::rightClick) return;
        if (Global::mouseLastY > Settings::SCR_HEIGHT - 120.f) return;

        float closestDistance = 1000.f;
        float dx, dz, distance;
        for (int z = 0; z < Settings::MAP_HEIGHT; z++) {
            for (int x = 0; x < Settings::MAP_WIDTH; x++) {
                glm::vec3 pos = m_tiles[z][x].pos;
                dz = pos.x - m_mouseIntersectPoint.x;
                dx = pos.z - m_mouseIntersectPoint.z;
                distance = std::sqrt(dz * dz + dx * dx);
                if (distance < Settings::TILE_RADIUS && distance < closestDistance) {
                    closestDistance = distance;
                    m_mouseOverTilePos = glm::ivec2(z, x);
                    m_mouseOverTile = true;
                }
                
            }
        }

    }
};