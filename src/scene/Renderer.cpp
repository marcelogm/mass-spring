#pragma once

#include "scene.hpp"

void Renderer::clear() {
	glFlush();
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (Configuration::getInstance()->getOpenGLConfiguration()->mode)
    {
    case RenderMode::TRIANGLES:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case RenderMode::LINES:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case RenderMode::POINTS:
        glPointSize(5.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }
}

void Renderer::prepare() {
    const auto config = Configuration::getInstance();
    this->aspect = config->getViewport()->width / config->getViewport()->height;
    this->projection = glm::perspective(glm::radians(45.f), aspect, 1.f, 1000.f);
}

void Renderer::render(Entity* entity, Camera* camera, LightConfiguration* ligth) {
    const auto view = camera->getView();
    const auto model = *entity->getModel();
    const auto color = *entity->getColor();
    const auto info = entity->getOpenGLInformation();

    glBindVertexArray(info.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, info.positionVBO);
    glBufferData(GL_ARRAY_BUFFER, entity->getRenderedVertices()->size() * sizeof(vec3), entity->getRenderedVertices()->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, info.normalVBO);
    glBufferData(GL_ARRAY_BUFFER, entity->getRenderedNormals()->size() * sizeof(vec3), entity->getRenderedNormals()->data(), GL_STATIC_DRAW);

    glUseProgram(info.shader);
    glUniformMatrix4fv(glGetUniformLocation(info.shader, "model"), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(info.shader, "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(info.shader, "projection"), 1, GL_FALSE, value_ptr(this->projection));
    glUniform4f(glGetUniformLocation(info.shader, "objectColor"), color.r, color.g, color.b, color.a);
    glUniform4f(glGetUniformLocation(info.shader, "lightPosition"),
        ligth->lightPosition.x, 
        ligth->lightPosition.y, 
        ligth->lightPosition.z, 
        ligth->lightPosition.w);
    glUniform1f(glGetUniformLocation(info.shader, "ambientLightStrength"), ligth->ambientLightStrength);
    glUniform1f(glGetUniformLocation(info.shader, "diffuseLightStrength"), ligth->diffuseLightStrength);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, info.positionVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, info.normalVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, entity->getRenderedVertices()->size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

