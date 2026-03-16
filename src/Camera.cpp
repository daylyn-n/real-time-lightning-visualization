#include "../include/Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Eye_, Eye_ + ViewDirection_, UpVector_);
}
void Camera::MouseLook(int mouseX, int mouseY)
{
    static bool firstLook = true;
    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
    if(firstLook)
    {
        oldMousePos_ = currentMouse;
    
        firstLook = false;
    }
    glm::vec2 mouseDelta = oldMousePos_ - currentMouse;
    ViewDirection_ = glm::rotate(ViewDirection_, glm::radians(mouseDelta.x) * -1, UpVector_);
    ViewDirection_ = glm::rotate(ViewDirection_, glm::radians(mouseDelta.y) * -1, glm::cross(ViewDirection_, UpVector_)); 
    oldMousePos_ = currentMouse;
}
// move forward and backward
void Camera::MoveForward(float speed)
{
    // simple but not correct
    Eye_ +=  (ViewDirection_ * speed);

}
void Camera::MoveBackward(float speed)
{
    Eye_ -= (ViewDirection_*speed);
}
void Camera::MoveLeft(float speed)
{
    // update x position based on the mouse
    Eye_ -= (glm::cross(ViewDirection_, UpVector_) * speed);
}
void Camera::MoveRight(float speed)
{
    Eye_ += (glm::cross(ViewDirection_, UpVector_) * speed);
}
   
void Camera::MoveUp(float speed)
{
   Eye_ += (UpVector_ * speed); 
}
void Camera::MoveDown(float speed)
{
    Eye_ -= (UpVector_ * speed);
}
