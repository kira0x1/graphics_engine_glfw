//
// Created by kira on 25/10/2023.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "SHADER.h"

struct Properties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct LightProperties {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Material {
    Shader *shader;
    Properties materialProperties{};
    LightProperties lightProperties{};

    glm::mat4 Projection{};
    glm::mat4 View{};

    void setAmbient(glm::vec3 ambient) {
        materialProperties.ambient = ambient;
        shader->setVec3("material.ambient", ambient);
    }

    void setDiffuse(glm::vec3 diffuse) {
        materialProperties.diffuse = diffuse;
        shader->setVec3("material.diffuse", diffuse);
    }

    void setSpecular(glm::vec3 specular) {
        materialProperties.specular = specular;
        shader->setVec3("material.specular", specular);
    }

    void updateShader() {
        shader->setVec3("material.diffuse", materialProperties.diffuse);
        shader->setVec3("material.specular", materialProperties.specular);
        shader->setVec3("material.ambient", materialProperties.ambient);
        shader->setFloat("material.shininess", materialProperties.shininess);

        shader->setVec3("light.ambient", lightProperties.ambient);
        shader->setVec3("light.diffuse", lightProperties.diffuse);
        shader->setVec3("light.specular", lightProperties.specular);
        shader->setVec3("light.position", lightProperties.position);
    }

public:
    explicit Material(Shader *shader) {
        this->shader = shader;

        glm::vec3 lightColor = glm::vec3(0.8f, 0.5f, 0.7f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease lights influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        lightProperties = LightProperties{glm::vec3(0.0f), ambientColor, diffuseColor, glm::vec3(1.0f)};

        materialProperties = Properties{
                glm::vec3(1.0f, 0.5f, 0.31f),
                glm::vec3(1.0f, 0.5f, 0.31f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                32.0f
        };

        updateShader();
    }
};


#endif //MATERIAL_H
