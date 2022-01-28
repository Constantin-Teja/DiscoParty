#include <vector>

class Object
    {
    public:
        glm::vec3 objectPosition;
        unsigned int materialShininess;
        glm::vec3 materialKe;
        glm::vec3 materialKd;
        glm::vec3 materialKs;

        Object() {};

        Object(
            glm::vec3 materialKe,
            glm::vec3 materialKd,
            glm::vec3 materialKs,
            glm::vec3 objectPosition,
            unsigned int materialShininess) : objectPosition(objectPosition), materialShininess(materialShininess),
            materialKe(materialKe),
            materialKd(materialKd), materialKs(materialKs) {};

        void  RefreshComponents(glm::vec3 material_kd, glm::vec3 material_ks) {
            materialKd = material_kd;
            materialKs = material_ks;
        };
    };

    class SpotLight : public Object
    {
    public:
        int index;
        static std::vector<glm::vec3> position, direction, color;
        static int nr;

        SpotLight() : Object()
        {}

        SpotLight(
            glm::vec3 materialKe,
            glm::vec3 materialKd,
            glm::vec3 materialKs,
            glm::vec3 position,
            glm::vec3 direction,
            glm::vec3 color,
            glm::vec3 objectPosition,
            unsigned int materialShininess) :
            Object(materialKe, materialKd, materialKs, objectPosition, materialShininess)
        {
            SpotLight::position.push_back(position);
            SpotLight::direction.push_back(direction);
            SpotLight::color.push_back(color);
            index = SpotLight::nr++;
        }
    };