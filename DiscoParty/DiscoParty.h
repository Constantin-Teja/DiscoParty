#pragma once

#include <string>
#include <vector>
#include <map>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "components/simple_scene.h"
#include "components/transform.h"
#include "Settings.h"
#include "ObjectLight.h"

using namespace std;
namespace m1
{
    class DiscoParty : public gfxc::SimpleScene
    {
    public:
        DiscoParty();
        ~DiscoParty();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderStructuresAndDancers(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,const Object & obj);
        void RenderSimpleMeshSpotLights(const glm::mat4& modelMatrix, const SpotLight& spotlight);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void InitSpotlights();
        void InitWalls();
        void InitCeilingAndFloor();
        void InitDancers();

        void DrawDancers(float deltaTimeSeconds);
        void DrawSpotlights(float deltaTimeSeconds);
        void DrawCeilingAndFloor();
        void DrawWalls();

        Mesh* CreateCone(const std::string& name, float nr);
        vector<Object> objVecCeilingAndFloor;
        vector<Object> objVecLeftWall, objVecBackWall, objVecRightWall;
        vector<Object> dancers;
        vector<SpotLight> spotlightsVec;
        vector<glm::vec3> lightPosition, lightDirection, lightColor, dancersDirection;

        int lightningMode = 0;
        int stateDancerPos = 0;
        int spotlightDirection = 0;
        float spotlightDistance = 0;
        float dancersTraveledDistance = 0;
        float dancersRotation = 0;
    };
}   // namespace m1
