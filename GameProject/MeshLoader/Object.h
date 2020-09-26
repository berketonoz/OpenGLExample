
#pragma once
#include "Mesh.h"
#include "Model.h"
#include "GameObject.h"
#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Object : public GameObject{
    
public:
    Object(const Model &m, glm::vec3 s, bool w = true, bool su = false):GameObject(m,s), size(s), is_wall(w), surface(su){}
    
    glm::vec3 GetPosition() const{return this->position;}
    
    void Collect(vector<Object> &objects){}

//    bool CheckCollision(const std::vector<float> mins, std::vector<float> maxs, std::vector<float> minsObj, std::vector<float> maxsObj, glm::vec3 pos, glm::vec3 obj_pos, glm::vec3 size) const {return true;}
    
    void Draw(Shader shader, glm::vec3 position, glm::vec3 camera_position) {
        //View Frustum Culling is handled here
        if(this->surface == false){
           if(position.z > camera_position.z-50.0f && position.z < camera_position.z-0.2f){
               if(this->visible == true){
                   //Initialize the bounding box of the object
                   std::vector<float> minsObj = this->GetModel().GetMins();
                   std::vector<float> maxsObj = this->GetModel().GetMaxs();
                   glm::vec3 obj_pos = this->GetPosition();
                   glm::vec3 obj_size = this->GetSize();
                   this->box = AABB(minsObj[0]*obj_size[0]+obj_pos.x,
                                    minsObj[1]*obj_size[1]+obj_pos.y,
                                    minsObj[2]*obj_size[2]+obj_pos.z,
                                    maxsObj[0]*obj_size[0]+obj_pos.x,
                                    maxsObj[1]*obj_size[1]+obj_pos.y,
                                    maxsObj[2]*obj_size[2]+obj_pos.z);
                   this->position = position;
                   translation = glm::mat4(1.0f);
                   translation = glm::translate( translation, position); // Translate to    position
                   translation = glm::scale(translation, this->size);    //Size of the object
                   glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1,      GL_FALSE,glm::value_ptr(translation)  );
                   this->GetModel().Draw(shader);
               }
           }
        }
        else if(this->surface == true){
            this->position = position;
            translation = glm::mat4(1.0f);
            translation = glm::translate( translation, position); // Translate to position
            translation = glm::scale(translation, size);    //Size of the object
            glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1,     GL_FALSE,glm::value_ptr(translation)  );
            this->GetModel().Draw(shader);
        }
    }
    
   
    glm::mat4 GetTranslation() const{return this->translation;}
    void SetVisible(bool v) {this->visible=v;}
//    void SetSurface(bool s){this->surface=s;}
    void SetPosition(glm::vec3 pos){this->position=pos;}
    void SetHit(bool hit){this->hit=hit;}
    void SetCollected(bool c){this->collected=c;}
    bool GetHit(){return this->hit;}
    bool GetCollected(){return this->collected;}
    bool GetVisible(){return this->visible;}
    bool GetSurface(){return this->surface;}
    short GetScore(){return this->score;}
    glm::vec3 GetSize(){return this->size;}
    bool IsWall(){return this->is_wall;}
    AABB GetAABB(){return this->box;}
//    Model GetModel(){return this->GetModel();}
    
private:
    
    float gravity = 0.002f;
    
    AABB box;
    glm::vec3 position;
    glm::vec3 size;
    glm::mat4 translation;
    
    bool hit = false;
    bool collected = false;
    short score = 0;
    bool visible = true;
    bool is_wall;
    bool surface;
};
