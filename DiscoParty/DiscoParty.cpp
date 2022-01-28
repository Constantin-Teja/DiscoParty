#include "lab_m1/DiscoParty/DiscoParty.h"


#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


int SpotLight::nr;
std::vector<glm::vec3> SpotLight::position;
std::vector<glm::vec3> SpotLight::direction;
std::vector<glm::vec3> SpotLight::color;

DiscoParty::DiscoParty()
{
}


DiscoParty::~DiscoParty()
{
}


void DiscoParty::Init()
{
    GetSceneCamera()->SetPosition(glm::vec3(nrUnitatiPodeaTavan * dimensiuneUnitate / 2.f - dimensiuneUnitate / 2.f, 
        HEIGHT_RING / 2.f,
        nrUnitatiPodeaTavan * dimensiuneUnitate + 3*dimensiuneUnitate));
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("StructuresAndDancersShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DiscoParty", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DiscoParty", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("SpotlightsShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DiscoParty", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DiscoParty", "shaders", "Spotlight_FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    InitCeilingAndFloor();
    InitSpotlights();
    InitWalls();
    InitDancers();
}


void DiscoParty::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void DiscoParty::Update(float deltaTimeSeconds)
{
    DrawCeilingAndFloor();
    DrawWalls();
    DrawSpotlights(deltaTimeSeconds);
    DrawDancers(deltaTimeSeconds);
}

void DiscoParty::FrameEnd()
{

}

void DiscoParty::OnInputUpdate(float deltaTime, int mods)
{
}


void DiscoParty::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T)
    {
        lightningMode = lightningMode ? 0 : 1;
    }
}


void DiscoParty::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void DiscoParty::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void DiscoParty::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void DiscoParty::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void DiscoParty::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void DiscoParty::OnWindowResize(int width, int height)
{
}

void DiscoParty::DrawSpotlights(float deltaTimeSeconds)
{
    if (lightningMode != 1)
    {
        float depl = deltaTimeSeconds;
        spotlightDistance += depl;
        for (auto it = spotlightsVec.begin(); it != spotlightsVec.end(); it++)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            // aceasta directiva este folosita pentru nu se scrie in depth buffer
            glDepthMask(GL_FALSE);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            switch (spotlightDirection)
            {
            case 0:
                it->objectPosition += glm::vec3(depl, 0, 0);
                SpotLight::position[it->index] += glm::vec3(depl, 0, 0);
                break;
            case 1:
                it->objectPosition += glm::vec3(0, 0, depl);
                SpotLight::position[it->index] += glm::vec3(0, 0, depl);
                break;
            case 2:
                it->objectPosition += glm::vec3(-depl, 0, 0);
                SpotLight::position[it->index] += glm::vec3(-depl, 0, 0);
                break;
            case 3:
                it->objectPosition += glm::vec3(0, 0, -depl);
                SpotLight::position[it->index] += glm::vec3(0, 0, -depl);
                break;
            }

            spotlightDirection++;
            spotlightDirection = (spotlightDirection == 4) ? spotlightDirection % 4 : spotlightDirection;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, it->objectPosition);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(0.3) * 4, HEIGHT_RING, tan(0.3) * 4));

            RenderSimpleMeshSpotLights(modelMatrix, *it);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
        }

        if (spotlightDistance > dimensiuneUnitate * nrUnitatiPodeaTavan / 2.f)
        {
            spotlightDirection++;
            spotlightDirection = spotlightDirection % 4;
            if (spotlightDirection == 4)
                spotlightDirection = spotlightDirection % 4;
            spotlightDistance = 0;
        }
    }
}

void DiscoParty::DrawCeilingAndFloor()
{
    // Render ceiling and floor
    for (auto it = objVecCeilingAndFloor.begin(); it != objVecCeilingAndFloor.end(); it++)
    {
        switch (lightningMode)
        {
        case 0:
            it->RefreshComponents(glm::vec3(mKdPT), glm::vec3(mKsPT));
            break;
        case 1:
            it->RefreshComponents(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
            break;
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (*it).objectPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(dimensiuneUnitate, 0.1, dimensiuneUnitate));
        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (*it).objectPosition + glm::vec3(0, HEIGHT_RING, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(dimensiuneUnitate, 0.1, dimensiuneUnitate));

        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
    }
}

void DiscoParty::DrawWalls()
{
    for (auto it = objVecLeftWall.begin(); it != objVecLeftWall.end(); it++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3((*it).objectPosition));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, dimensiuneUnitate, dimensiuneUnitate));
        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
    }

    for (auto it = objVecBackWall.begin(); it != objVecBackWall.end(); it++)
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3((*it).objectPosition));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(dimensiuneUnitate, dimensiuneUnitate, 0.1));
        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
    }
    for (auto it = objVecRightWall.begin(); it != objVecRightWall.end(); it++)
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3((*it).objectPosition));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, dimensiuneUnitate, dimensiuneUnitate));
        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
    }
}

void DiscoParty::DrawDancers(float deltaTimeSeconds)
{
    dancersRotation += cos(deltaTimeSeconds) * dancersSpeed / 100.f;
    dancersTraveledDistance += dancersSpeed * deltaTimeSeconds;
    for (auto it = dancers.begin(); it != dancers.end(); it++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        switch (stateDancerPos)
        {
        case 0:
            it->objectPosition.x += dancersSpeed * deltaTimeSeconds;
            break;
        case 1:
            it->objectPosition.x -= dancersSpeed * deltaTimeSeconds;
            it->objectPosition.z += dancersSpeed * deltaTimeSeconds / 2.f;
            break;
        case 2:
            it->objectPosition.x += dancersSpeed * deltaTimeSeconds;
            it->objectPosition.z += dancersSpeed * deltaTimeSeconds / 2.f;
            break;
        case 3:
            it->objectPosition.x -= dancersSpeed * deltaTimeSeconds;
            break;
        case 4:
            it->objectPosition.x += dancersSpeed * deltaTimeSeconds;
            it->objectPosition.z -= dancersSpeed * deltaTimeSeconds / 2.f;
            break;
        case 5:
            it->objectPosition.x -= dancersSpeed * deltaTimeSeconds;
            it->objectPosition.z -= dancersSpeed * deltaTimeSeconds / 2.f;
            break;
        }
        stateDancerPos = (stateDancerPos + 1) % 6;
        if (dancersTraveledDistance > 7 * nrUnitatiPodeaTavan * dimensiuneUnitate / 10.f)
        {
            dancersTraveledDistance = 0;
            stateDancerPos = (stateDancerPos + 1) % 6;
        }
        modelMatrix = glm::translate(modelMatrix, glm::vec3((*it).objectPosition.x, (*it).objectPosition.y, (*it).objectPosition.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(dancersWidth, dancersHeight, dancersWidth));
        modelMatrix = glm::rotate(modelMatrix, dancersRotation, glm::vec3(0, 1, 0));
        RenderStructuresAndDancers(meshes["box"], shaders["StructuresAndDancersShader"], modelMatrix, *it);
    }
}

void DiscoParty::InitSpotlights()
{
    AddMeshToList(CreateCone("SpotLight", 30));

    glm::vec3 lightPosition, lightDirection;
    glm::vec3 spotlightPosition, spotlightDirection = glm::vec3(0, -1, 0);
    {
        spotlightPosition = glm::vec3(dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0.6, dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        lightPosition = glm::vec3(dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0, dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        spotlightsVec.push_back(SpotLight(spot1Color, mKdSpotlight, mKsSpotlight, spotlightPosition, spotlightDirection, spot1Color, lightPosition, spotMatShin));
    }
    {
        spotlightPosition = glm::vec3(3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0.6, dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        lightPosition = glm::vec3(3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0, dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        spotlightsVec.push_back(SpotLight(spot2Color, mKdSpotlight, mKsSpotlight, spotlightPosition, spotlightDirection, spot2Color, lightPosition, spotMatShin));
    }
    {
        spotlightPosition = glm::vec3(3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0.6, 3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        lightPosition = glm::vec3(3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0, 3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        spotlightsVec.push_back(SpotLight(spot3Color, mKdSpotlight, mKsSpotlight, spotlightPosition, spotlightDirection, spot3Color, lightPosition, spotMatShin));
    }
    {
        spotlightPosition = glm::vec3(dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0.6, 3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        lightPosition = glm::vec3(dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f, 0, 3.5f * dimensiuneUnitate * nrUnitatiPodeaTavan / 5.f);
        spotlightsVec.push_back(SpotLight(spot4Color, mKdSpotlight, mKsSpotlight, spotlightPosition, spotlightDirection, spot4Color, lightPosition, spotMatShin));
    }
}

void DiscoParty::InitWalls()
{
    for (int i = 0; i < nrUnitatiPodeaTavan; i++)
    {
        for (int j = 0; j < HEIGHT_RING / dimensiuneUnitate; ++j)
        {
            glm::vec3 objPosition = glm::vec3(-dimensiuneUnitate / 2.f, dimensiuneUnitate * (j+ 0.5f), dimensiuneUnitate * i);
            objVecLeftWall.push_back(Object(glm::vec3(0), glm::vec3(mKdPStanga), glm::vec3(mKsPStanga), objPosition, matShinPStanga));

            objPosition = glm::vec3(dimensiuneUnitate * i, dimensiuneUnitate * (j + 0.5f), -dimensiuneUnitate / 2.f);
            objVecBackWall.push_back(Object(glm::vec3(0), glm::vec3(mKdPSpate), glm::vec3(mKsPSpate), objPosition, matShinPSpate));

            objPosition = glm::vec3(-dimensiuneUnitate / 2.f + dimensiuneUnitate * nrUnitatiPodeaTavan, dimensiuneUnitate * (j + 0.5f), dimensiuneUnitate * i);
            objVecRightWall.push_back(Object(glm::vec3(0), glm::vec3(mKdPDreapta), glm::vec3(mKsPDreapta), objPosition, matShinPDreapta));
        }
    }
}

void DiscoParty::InitCeilingAndFloor()
{
    for (int i = 0; i < nrUnitatiPodeaTavan; ++i)
    {
        for (int j = 0; j < nrUnitatiPodeaTavan; ++j)
        {
            glm::vec3 objPosition = glm::vec3(dimensiuneUnitate * i, 0, dimensiuneUnitate * j);
            glm::vec3 mKe = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
            objVecCeilingAndFloor.push_back(Object(mKe, glm::vec3(mKdPT), glm::vec3(mKsPT), objPosition, matShinPT));
            lightPosition.push_back(objPosition);
            lightDirection.push_back(glm::vec3(0, 1, 0));
            lightColor.push_back(mKe);
        }
    }
}

void DiscoParty::InitDancers()
{
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(1 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            1 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(5 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            1 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(1 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            3 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(5 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            5 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(1 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            5 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
    dancers.push_back(Object(glm::vec3(0), mKdDancers, mKsDancers,
        glm::vec3(5 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f,
            dancersHeight / 2,
            3 * nrUnitatiPodeaTavan * dimensiuneUnitate / 6.f),
        matShinDancers));
}

Mesh* DiscoParty::CreateCone(const std::string& name, float nr)
{
    Mesh* cone = new Mesh(name);
    std::vector<VertexFormat> vertices_cone{
        VertexFormat(glm::vec3(0,1,0)),
    };
    for (int i = 0; i < nr; i++) {
        float grade = RADIANS((float)360 / nr);
        vertices_cone.push_back(VertexFormat(glm::vec3(cos(grade * i), 0, sin(grade * i))));
    }
    std::vector<unsigned int> indices_cone;
    for (int i = 0; i < nr; i++)
        indices_cone.push_back(i);
    indices_cone.push_back(1);
    cone->SetDrawMode(GL_TRIANGLE_FAN);
    cone->InitFromData(vertices_cone, indices_cone);
    return cone;
}


void DiscoParty::RenderStructuresAndDancers(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const  Object& obj)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);
    glUniform3fv(glGetUniformLocation(shader->program, "SpotLightPosition"), SpotLight::nr, glm::value_ptr(SpotLight::position[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "SpotLightDirection"), SpotLight::nr, glm::value_ptr(SpotLight::direction[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "SpotLightColor"), SpotLight::nr, glm::value_ptr(SpotLight::color[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), lightPosition.size(), glm::value_ptr(lightPosition[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_direction"), lightDirection.size(), glm::value_ptr(lightDirection[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_color"), lightColor.size(), glm::value_ptr(lightColor[0]));
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), obj.materialShininess);
    glUniform1i(glGetUniformLocation(shader->program, "lightsNumber"), lightPosition.size());
    glUniform3f(glGetUniformLocation(shader->program, "material_ke"), obj.materialKe.x, obj.materialKe.y, obj.materialKe.z);
    glUniform3f(glGetUniformLocation(shader->program, "material_kd"), obj.materialKd.x, obj.materialKd.y, obj.materialKd.z);
    glUniform3f(glGetUniformLocation(shader->program, "material_ks"), obj.materialKs.x, obj.materialKs.y, obj.materialKs.z);

    glUniform1i(glGetUniformLocation(shader->program, "lightningMode"), lightningMode);

    // Bind model matrix
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void DiscoParty::RenderSimpleMeshSpotLights(const glm::mat4& modelMatrix, const SpotLight& spotlight)
{
    if (!meshes["SpotLight"] || !shaders["SpotlightsShader"] || !shaders["SpotlightsShader"]->GetProgramID())
        return;

    glUseProgram(shaders["SpotlightsShader"]->program);
    glUniform3fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "SpotLightPosition"), 1U, glm::value_ptr(SpotLight::position[spotlight.index]));
    glUniform3fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "SpotLightDirection"), 1U, glm::value_ptr(SpotLight::direction[spotlight.index]));
    glUniform3fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "SpotLightColor"), 1U, glm::value_ptr(SpotLight::color[spotlight.index]));
    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    glUniform3f(glGetUniformLocation(shaders["SpotlightsShader"]->program, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
    // Set material property uniforms (shininess, kd, ks, object color) 
    glUniform1i(glGetUniformLocation(shaders["SpotlightsShader"]->program, "material_shininess"), spotlight.materialShininess);
    glUniform3f(glGetUniformLocation(shaders["SpotlightsShader"]->program, "material_ke"), spotlight.materialKe.x, spotlight.materialKe.y, spotlight.materialKe.z);
    glUniform3f(glGetUniformLocation(shaders["SpotlightsShader"]->program, "material_kd"), spotlight.materialKd.x, spotlight.materialKd.y, spotlight.materialKd.z);
    glUniform3f(glGetUniformLocation(shaders["SpotlightsShader"]->program, "material_ks"), spotlight.materialKs.x, spotlight.materialKs.y, spotlight.materialKs.z);

    //  ca sa trec prin iluminari                
    glUniform1i(glGetUniformLocation(shaders["SpotlightsShader"]->program, "tranzitie"), lightningMode);

    // Bind model matrix
    glUniformMatrix4fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaders["SpotlightsShader"]->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(meshes["SpotLight"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["SpotLight"]->GetDrawMode(), static_cast<int>(meshes["SpotLight"]->indices.size()), GL_UNSIGNED_INT, 0);
}
