#include "CGame.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "Mesh.h"
#include <chrono>

GfxContext gfxContext;

void CGame::Initialize(unsigned int screenWidth, unsigned int screenHeight)
{
    m_Width = screenWidth;
    m_Height = screenHeight;

    SetupGPUProgram();
    SetupModel();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);

    CreateMSAAFBO();
    CreateShadowMapFBO();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CGame::CreateMSAAFBO()
{
    glGenFramebuffers(1, &m_IntermediateFBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFBOId);

    glGenTextures(1, &m_ColorTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB8, m_Width, m_Height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTextureId, 0);

    glGenTextures(1, &m_DepthStencilTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, m_Width, m_Height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilTextureId, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        int a = 0;
        ++a;
    }
}

void CGame::CreateShadowMapFBO()
{
    int shadowTextureWidth = 1024;
    int shadowTextureHeight = 1024;

    glGenFramebuffers(1, &m_ShadowMapFBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);

    glGenTextures(1, &m_ShadowMapDepthStencilTextureId);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        shadowTextureWidth, shadowTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        // Отлично!
    }
}


void CGame::Shutdown()
{
    
}

void CGame::OnResize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, m_Width, m_Height);
    glScissor(0, 0, m_Width, m_Height);
}

void CGame::DrawModel(Model& model, Shader& shader, GfxContext& context)
{
    gfxContext.mvpMatrix = gfxContext.projectionMatrix * gfxContext.viewMatrix * gfxContext.worldMatrix;

    shader.setMatrix4x4("modelMatrix", glm::value_ptr(gfxContext.worldMatrix));
    shader.setMatrix4x4("viewMatrix", glm::value_ptr(gfxContext.viewMatrix));
    shader.setMatrix4x4("projectionMatrix", glm::value_ptr(gfxContext.projectionMatrix));
    shader.setMatrix4x4("mvpMatrix", glm::value_ptr(gfxContext.mvpMatrix));
    shader.setVector3("eyePos", glm::value_ptr(gfxContext.eyePos));
    shader.setVector3("lightDirection", glm::value_ptr(gfxContext.lightDirection));
    shader.setMatrix4x4("lightViewProjMatrix", glm::value_ptr(gfxContext.lightViewProjectionMatrix));
    shader.setFloat("time", gfxContext.time);
    model.meshes[0].Draw(shader);
}

void CGame::RenderShadowMap()
{
    const char* debugGroupName = "Render Shadow Maps";
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, strlen(debugGroupName), debugGroupName);
    glDisable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);

    glViewport(0, 0, 1024, 1024);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    DrawScene(m_ShadowMapShader);
    glCullFace(GL_BACK);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glPopDebugGroup();
}

void CGame::RenderScene()
{
    const char* debugGroupName = "Render Scene";
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, strlen(debugGroupName), debugGroupName);
    glEnable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFBOId);

    glViewport(0, 0, m_Width, m_Height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    DrawScene(NULL);
    glPopDebugGroup();
}

void CGame::ResolveToScreen()
{
    const char* debugGroupName = "Resolve To Screen";
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, strlen(debugGroupName), debugGroupName);

    // Blit промежуточного FBO в FBO обратного буфера
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_IntermediateFBOId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glPopDebugGroup();
}

void CGame::DrawScene(Shader* replacementShader)
{
    Shader* shaderToUse = replacementShader != NULL ? replacementShader : m_pShaderPass0;

    // Световые матрицы
    glm::vec3 lightPosition = glm::vec3(-5, 5, 10);
    gfxContext.lightDirection = glm::normalize(glm::vec3(0, 0, 0) - lightPosition);
    glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    float orthoProjScale = 5.0f;
    glm::mat4 lightProjectionMatrix = glm::ortho<float>(-orthoProjScale, orthoProjScale, -orthoProjScale, orthoProjScale, 0.01f, 100.0f);
    gfxContext.lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;

    // Матрица просмотра настроек
    const float degreesPerSecond = 45.0f;
    glm::mat4 viewRotation = glm::rotate(glm::mat4(), glm::radians(glm::radians(25.0f) * gfxContext.time), glm::vec3(0, 1, 0));
    float x = cos(glm::radians(degreesPerSecond) * gfxContext.time) * 4.5f;
    float z = sin(glm::radians(degreesPerSecond) * gfxContext.time) * 4.5f;
    gfxContext.eyePos = glm::vec4(x, 3.0f, z, 0.0f);
    gfxContext.viewMatrix = glm::lookAt(gfxContext.eyePos, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    gfxContext.projectionMatrix = glm::perspective(glm::radians(60.0f), m_Width / static_cast<float>(m_Height), 0.03f, 100.0f);

    // Рисованный человек
    float scaleFactor = 0.4f;
    gfxContext.worldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    gfxContext.worldMatrix = glm::rotate(gfxContext.worldMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    glm::vec3 diffuseColor = glm::vec3(1, 0, 0);

    shaderToUse->use();
    shaderToUse->setVector3("diffuseColor", glm::value_ptr(diffuseColor));
    DrawModel(*m_pManModel, *shaderToUse, gfxContext);

    // Нарисовать торус
    scaleFactor = 0.3f;
    gfxContext.worldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    gfxContext.worldMatrix = glm::translate(gfxContext.worldMatrix, glm::vec3(-5, 7, -10));
    gfxContext.worldMatrix = glm::rotate(gfxContext.worldMatrix, glm::radians(360.0f) * gfxContext.time, glm::vec3(0, 1, 0));

    diffuseColor = glm::vec3(1, 1, 0);
    shaderToUse->use();
    shaderToUse->setVector3("diffuseColor", glm::value_ptr(diffuseColor));
    DrawModel(*m_pTorus, *shaderToUse, gfxContext);


    //  Нарисовать квадрат пола
    diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);

    if (replacementShader == NULL)
    {
        shaderToUse = m_pShaderPass1;
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);
    }
        shaderToUse->use();

        shaderToUse->setVector3("diffuseColor", glm::value_ptr(diffuseColor));
        gfxContext.worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f));
        gfxContext.worldMatrix = glm::scale(gfxContext.worldMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
        gfxContext.worldMatrix = glm::rotate(gfxContext.worldMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
        DrawModel(*m_pQuad, *shaderToUse, gfxContext);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void CGame::Render()
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    gfxContext.time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    RenderShadowMap();
    RenderScene();
    ResolveToScreen();
}

void CGame::SetupModel()
{
    m_pManModel = new Model();
    m_pManModel->Load("man.obj");

    m_pTorus = new Model();
    m_pTorus->Load("torus.obj");

    m_pQuad = new Model();
    m_pQuad->Load("quad.obj");
}

void CGame::SetupGPUProgram()
{
    m_pShaderPass0 = new Shader("vertexPass0.glsl", "fragmentPass0.glsl");
    m_pShaderPass1 = new Shader("vertexPass1.glsl", "fragmentPass1.glsl");
    m_ShadowMapShader = new Shader("shadowMapVertex.glsl", "shadowMapFragment.glsl");
}

void CGame::ReloadShaders()
{
    OutputDebugStringA("Reloading Shaders!\n");
    SetupGPUProgram();
}