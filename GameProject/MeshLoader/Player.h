#pragma once
#include "Mesh.h"
#include "Model.h"
#include "GameObject.h"
#include "Object.h"

#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum LRU{
    GO_LEFT,
    GO_RIGHT,
    GO_UP,
    GO_BACKWARD,
    GO_FORWARD
};

class Player : public GameObject{
    
public:
    Player(const Model &m, glm::vec3 s):GameObject(m,s), size(s){}
    
    glm::vec3 GetPosition() const{return this->position;}
    
    void Collect(vector<Object> &objects){
        for(int i = 0; i < objects.size(); i++){
            if(objects[i].GetVisible()){
//            cout << objects[i].IsWall() << endl;
                std::vector<float> mins = this->GetModel().GetMins();
                std::vector<float> maxs = this->GetModel().GetMaxs();
                std::vector<float> minsObj = objects[i].GetModel().GetMins();
                std::vector<float> maxsObj = objects[i].GetModel().GetMaxs();
                glm::vec3 pos = this->GetPosition();
                glm::vec3 obj_pos = objects[i].GetPosition();
                glm::vec3 obj_size = objects[i].GetSize();

                this->box = AABB(mins[0]*this->size[0]+pos.x,
                                 mins[1]*this->size[1]+pos.y,
                                 mins[2]*this->size[2]+pos.z,
                                 maxs[0]*this->size[0]+pos.x,
                                 maxs[1]*this->size[1]+pos.y,
                                 maxs[2]*this->size[2]+pos.z);
                AABB object = AABB(minsObj[0]*obj_size[0]+obj_pos.x,
                                   minsObj[1]*obj_size[1]+obj_pos.y,
                                   minsObj[2]*obj_size[2]+obj_pos.z,
                                   maxsObj[0]*obj_size[0]+obj_pos.x,
                                   maxsObj[1]*obj_size[1]+obj_pos.y,
                                   maxsObj[2]*obj_size[2]+obj_pos.z);
                if (this->box.overlaps(object)){
                    if (!objects[i].IsWall()){
                        if(!objects[i].GetCollected()){
                            objects[i].SetVisible(false);
                            score += 100;
                            objects[i].SetCollected(true);
                        }
                    }
                    else{
//                        cout << this->bump_left << " " << this->bump_right << endl;
                        if(!objects[i].GetHit()){
                            this->life -= 1;
                            objects[i].SetHit(true);
//                            objects[i].SetVisible(false);
                        }
                        this->position.z += this->real_velocity;

                        if(this->moving_left)
                            this->bump_left = true;
                        else
                            this->bump_left = false;
                        if(this->moving_right)
                            this->bump_right = true;
                        else
                            this->bump_right = false;
                    }
                }
                else{
                    this->bump_left = false;
                    this->bump_right = false;
                }
            }
        }
    }
    
    void Draw(Shader shader, glm::vec3 position, glm::vec3 camera_position) {
        if(this->visible == true){
            this->position = position;
            translation = glm::mat4(1.0f);
            translation = glm::translate( translation, position); // Translate to position
            translation = glm::scale(translation, size);    //Size of the object
            glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE,glm::value_ptr(translation)  );
            this->GetModel().Draw(shader);
        }
    }
    
    void RunPhysics(Object surface, float deltatime){
//        if(!onSurface){
        cout << this->bump_left << " " << this->bump_right << endl;

            this->position.y -= this->gravity;
            this->gravity += 0.09*deltatime;
            std::vector<float> mins = surface.GetModel().GetMins();
            std::vector<float> maxs = surface.GetModel().GetMaxs();
            std::vector<float> minsObj = surface.GetModel().GetMins();
            std::vector<float> maxsObj = surface.GetModel().GetMaxs();
            glm::vec3 pos = this->GetPosition();
            glm::vec3 obj_pos = surface.GetPosition();
            glm::vec3 obj_size = surface.GetSize();
            //To adjust the surface maximum
            maxsObj[1] += 1.0f;
        
            bool stat = this->box.overlaps(AABB(minsObj[0]*obj_size[0]+obj_pos.x,
                                                minsObj[1]*obj_size[1]+obj_pos.y,
                                                minsObj[2]*obj_size[2]+obj_pos.z,
                                                maxsObj[0]*obj_size[0]+obj_pos.x,
                                                maxsObj[1]*obj_size[1]+obj_pos.y,
                                                maxsObj[2]*obj_size[2]+obj_pos.z));

            if(stat){
//                onSurface = true;
                this->gravity -= this->gravity;
            }
        }
//    }
    
    void Move(GLfloat deltaTime){
        this->real_velocity = this->velocity*deltaTime;
        this->position.z -= this->real_velocity;
    }

    void Move(LRU lr, GLfloat deltaTime){
//        GLfloat velocity = this->movementSpeed * deltaTime;
        if(lr == GO_LEFT && !this->bump_left){
            this->position.x -= this->velocity*deltaTime;//*0.5;
        }
        if(lr == GO_RIGHT && !this->bump_right){
            this->position.x += this->velocity*deltaTime;//*0.5;
        }
        if(lr == GO_UP){
            this->position.y += this->real_velocity/3;
        }
        if(lr == GO_BACKWARD){
//            GLfloat temp = 1.3*this->velocity*deltaTime;
//            this->real_velocity = temp;
            this->position.z += this->real_velocity;
        }
        if(lr == GO_FORWARD){
            GLfloat temp = 1.3*this->velocity*deltaTime;
            this->real_velocity = temp;
            this->position.z -= this->real_velocity;
        }
    }
    
    
    glm::mat4 GetTranslation() const{return this->translation;}
    void SetVisible(bool v) {this->visible=v;}
    void SetPosition(glm::vec3 pos){this->position=pos;}
    short GetScore(){return this->score;}
    short GetLife(){return this->life;}
    void setMovingR(bool m){ this->moving_right = m; }
    void setMovingL(bool m){this->moving_left = m; }
    
private:
    
    float velocity = 7.0f;
    float real_velocity;
    float gravity = 0.002f;
//    bool onSurface = false;
    
    AABB box;
    glm::vec3 position;
    glm::vec3 size;
    glm::mat4 translation;
    
    short score = 0;
    short life = 3;
    bool visible = true;
    bool moving_left = false, moving_right = false;
    bool bump_left = false, bump_right = false;
};
