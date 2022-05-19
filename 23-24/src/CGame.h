#ifndef __CGAME__
#define __CGAME__

#include <glad/glad.h>
#include "shader_s.h"

#include "Model.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Model;
class Mesh;

struct GfxContext
{
    glm::mat4 worldMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 mvpMatrix;
    glm::vec3 eyePos;
    glm::vec3 lightDirection;
    glm::mat4 lightViewProjectionMatrix;
    float time;
};

class CGame
{
public:
    CGame() {};

    void Initialize(unsigned int screenWidth, unsigned int screenHeight);
    void Shutdown();
    void Render();

    void OnResize(int screenWidth, int screenHeight);

    void ReloadShaders();

private:
    void SetupGPUProgram();
    void SetupModel();
    void DrawModel(Model& model, Shader& shader, GfxContext& context);
    void DrawScene(Shader* replacementShader);

    void RenderShadowMap();
    void RenderScene();
    void ResolveToScreen();

    void CreateMSAAFBO();
    void CreateShadowMapFBO();

    Shader* m_pShaderPass0;
    Shader* m_pShaderPass1;
    Shader* m_ShadowMapShader;

    Model* m_pManModel;
    Model* m_pQuad;
    Model* m_pTorus;
    GLuint m_ColorTextureId;
    GLuint m_DepthStencilTextureId;
    GLuint m_IntermediateFBOId;

    GLuint m_ShadowMapNullColorTextureId;
    GLuint m_ShadowMapDepthStencilTextureId;
    GLuint m_ShadowMapFBOId;

    unsigned int m_Width;
    unsigned int m_Height;
};

#endif