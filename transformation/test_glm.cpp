//GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵，而不是单位矩阵。
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
int main(){
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // initialized as an identity matrix
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << std::endl << vec.y << std::endl << vec.z << std::endl;
    return 0;
}