#pragma once

#include "MyRenderer.h"
#include "ShadowMapRender.h"
#include "PostEffect.h"


class IRenderer;

/// <summary>
/// �����_�����O�G���W���B
/// </summary>
class RenderingEngine
{
public:
    // �f�B���N�V�������C�g
    struct DirectionalLight
    {
        Vector3 direction;  // ���C�g�̕���
        int castShadow;     // �e���L���X�g����H
        Vector4 color;      // ���C�g�̃J���[
    };

    // ���C�g�\����
    struct Light
    {
        std::array<DirectionalLight,
            NUM_DEFERRED_LIGHTING_DIRECTIONAL_LIGHT>
            directionalLight; // �f�B���N�V�������C�g
        Vector3 eyePos;         // �J�����̈ʒu
        float pad;
        Vector3 ambinetLight;   // ����
    };

    //���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
    enum class EnMainRTSnapshot
    {
        enDrawnOpacity,     // �s�����I�u�W�F�N�g�̕`�抮�����_
        enNum,              // �X�i�b�v�V���b�g�̐�
    };

    // �����_�����O�p�X
    enum class EnRenderingPass
    {
        enRenderToShadowMap,    //�V���h�E�}�b�v�ւ̕`��p�X
        enZPrepass,             //ZPrepass
        enRenderToGBuffer,      //G-Buffer�ւ̕`��p�X
        enForwardRender,        //�t�H���[�h�����_�����O�̕`��p�X
        enPostEffect,           //�|�X�g�G�t�F�N�g
        enRender2D,             //2D�`��B
    };
    /// <summary>
    /// �����_�����O�p�C�v���C����������
    /// </summary>
    void Init();
    
    /// <summary>
    /// �`��I�u�W�F�N�g��ǉ��B
    /// </summary>
    /// <param name="renderObject"></param>
    void AddRenderObject(IRenderer* renderObject)
    {
        m_renderObjects.push_back(renderObject);
    }
    /// <summary>
    /// ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����擾
    /// </summary>
    /// <returns></returns>
    Texture& GetZPrepassDepthTexture()
    {
        return m_zprepassRenderTarget.GetRenderTargetTexture();
    }
    /// <summary>
    /// GBuffer�̃A���x�h�e�N�X�`�����擾
    /// </summary>
    /// <returns></returns>
    Texture& GetGBufferAlbedoTexture()
    {
        return m_gBuffer[enGBufferAlbedo].GetRenderTargetTexture();
    }
    /// <summary>
    /// GBuffer�̖@���e�N�X�`�����擾�B
    /// </summary>
    /// <returns></returns>
    Texture& GetGBufferNormalTexture()
    {
        return m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
    }
    /// <summary>
    /// �s�����I�u�W�F�N�g�̕`�抮�����̃��C�������_�����O�^�[�Q�b�g��
    /// �X�i�b�v�V���b�g���擾
    /// </summary>
    /// <returns></returns>
    Texture& GetMainRenderTargetSnapshotDrawnOpacity()
    {
        return m_mainRTSnapshots[(int)EnMainRTSnapshot::enDrawnOpacity].GetRenderTargetTexture();
    }
    /// <summary>
    /// �����_�����O�p�C�v���C�������s
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
    void Execute(RenderContext& rc);
    /// <summary>
    /// �f�B���N�V�������C�g�̃p�����[�^��ݒ�
    /// </summary>
    /// <param name="lightNo"></param>
    /// <param name="direction"></param>
    /// <param name="color"></param>
    void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
    {
        m_deferredLightingCB.m_light.directionalLight[lightNo].direction = direction;
        m_deferredLightingCB.m_light.directionalLight[lightNo].color = color;
    }
    void SetMainRenderTargetAndDepthStencilBuffer(RenderContext& rc)
    {
        rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_gBuffer[enGBufferAlbedo].GetDSVCpuDescriptorHandle());
    }
private:
    /// <summary>
    /// G-Buffer��������
    /// </summary>
    void InitGBuffer();
    /// <summary>
    /// �f�B�t�@�[�h���C�e�B���O�̏�����
    /// </summary>
    void InitDeferredLighting();
    /// <summary>
    /// �V���h�E�}�b�v�ɕ`��
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void RenderToShadowMap(RenderContext& rc);
    /// <summary>
    /// ZPrepass
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void ZPrepass(RenderContext& rc);
    /// <summary>
    /// G-Buffer�ւ̕`��
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
    void RenderToGBuffer(RenderContext& rc);
    /// <summary>
    /// �f�B�t�@�[�h���C�e�B���O
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void DeferredLighting(RenderContext& rc);
    /// <summary>
    /// 2D�`��
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void Render2D(RenderContext& rc);
    /// <summary>
    /// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
    /// <summary>
    /// �t�H���[�h�����_�����O
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    void ForwardRendering(RenderContext& rc);
    /// <summary>
    /// ���C�������_�����O�^�[�Q�b�g��������
    /// </summary>
    void InitMainRenderTarget();
    /// <summary>
    /// ���C�������_�����O�^�[�Q�b�g�̂̃X�i�b�v�V���b�g����邽�߂�
    /// �����_�����O�^�[�Q�b�g��������
    /// </summary>
    void InitMainRTSnapshotRenderTarget();
    /// <summary>
    /// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e��
    /// �t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������
    /// </summary>
    void InitCopyMainRenderTargetToFrameBufferSprite();
    /// <summary>
    /// ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
    /// </summary>
    void InitZPrepassRenderTarget();
    /// <summary>
    /// ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g���B�e
    /// </summary>
    void SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot);
    /// <summary>
    /// �V���h�E�}�b�v�ւ̕`�揈����������
    /// </summary>
    void InitShadowMapRender();
private:
    // GBuffer�̒�`
    enum EnGBuffer
    {
        enGBufferAlbedo,        // �A���x�h
        enGBufferNormal,        // �@��
        enGBufferWorldPos,      // ���[���h���W
        enGBufferMetaricSmooth, // ���^���b�N�ƃX���[�X�B���^���b�N��r�A�X���[�X��a�Bgb�͖��g�p�B
        enGBUfferShadowParam,   // �e�p�����[�^
        enGBufferNum,           // G-Buffer�̐�
    };

    // �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
    struct SDeferredLightingCB
    {
        Light m_light;      // ���C�g
        float pad;          // �p�f�B���O
        Matrix mlvp[NUM_DEFERRED_LIGHTING_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];
    };

    ShadowMapRender m_shadowMapRenders[NUM_DEFERRED_LIGHTING_DIRECTIONAL_LIGHT];   // �V���h�E�}�b�v�ւ̕`�揈��
    SDeferredLightingCB m_deferredLightingCB;                       // �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
    Sprite m_copyMainRtToFrameBufferSprite;                         // ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
    Sprite m_diferredLightingSprite;                                // �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g
    RenderTarget m_zprepassRenderTarget;                            // ZPrepass�`��p�̃����_�����O�^�[�Q�b�g
    RenderTarget m_mainRenderTarget;                                // ���C�������_�����O�^�[�Q�b�g
    RenderTarget m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];   // ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
    RenderTarget m_gBuffer[enGBufferNum];                           // G-Buffer
    PostEffect m_postEffect;                                        // �|�X�g�G�t�F�N�g
    /*std::vector< Model* > m_zprepassModels;                         // ZPrepass�̕`��p�X�ŕ`�悳��郂�f���̃��X�g
    std::vector< Model* > m_renderToGBufferModels;                  // G�o�b�t�@�ւ̕`��p�X�ŕ`�悷�郂�f���̃��X�g
    std::vector< Model* > m_forwardRenderModels;                    // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f���̃��X�g
    std::vector< IRender* > m_render2DObject;                       // 2D�`��I�u�W�F�N�g�̃��X�g�B*/
    std::vector< IRenderer* > m_renderObjects;                        // �`��I�u�W�F�N�g�̃��X�g�B
};

extern RenderingEngine* g_renderingEngine;