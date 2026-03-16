#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../thirdparty/glm-master/glm/glm.hpp"
#include "../thirdparty/glm-master/glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../thirdparty/glm-master/glm/gtx/rotate_vector.hpp"

class Camera
{
    public:

        // default contructor
        Camera()
        {
            Eye_ = glm::vec3(0.0f, 0.0f, 0.0f);
            // assume we are looking out into the world space, RECALL: looking outwards is along the negative Z
            ViewDirection_ = glm::vec3(0.0f, 0.0f,-1.0f);
            // assume we start on a perfect plane
            UpVector_ = glm::vec3(0.0f, 1.0f, 0.0f);

            u_offset = -2.0f;
            u_rotate = 0.0f;
            u_size = 1.0f;
            speed = 0.05f;  
        }

        // view matrix we produce and return
        glm::mat4 GetViewMatrix() const;
        void MouseLook(int mouseX, int mouseY);
        // move forward and backward
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void MoveUp(float speed);
        void MoveDown(float speed);

        glm::vec3 Eye_;
        glm::vec3 ViewDirection_;
        glm::vec3 UpVector_;
        glm::vec2 oldMousePos_;        
        float u_offset;
        float u_rotate;
        float u_size;
        float speed;

};


#endif
