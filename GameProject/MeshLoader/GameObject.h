#pragma once
#include "Mesh.h"
#include "Model.h"
//#include "Player.h"
//#include "Object.h"
//#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class GameObject{
public:
    explicit
    GameObject(const Model &m, glm::vec3 s):model(m), size(s), id(current_id++) {}//, AABB(t1,t2){}
    
//    virtual bool CheckCollision(const std::vector<float>,const std::vector<float>,const std::vector<float>,const std::vector<float>, const glm::vec3, const glm::vec3, const glm::vec3) const = 0;
    virtual void Draw(Shader shader, glm::vec3 position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 camera_position=glm::vec3(0.0f,0.0f,0.0f)) = 0;
    virtual glm::vec3 GetPosition() const = 0;
    virtual glm::mat4 GetTranslation() const = 0;
    virtual void SetVisible(bool) = 0;

    Model GetModel() const {return model;}

    
private:
    Model model;
    glm::vec3 size;
    
    int id;
    static int current_id;
};


int GameObject::current_id;
