#pragma once

#include "CascadeShadowMapMatrix.h"


class IRenderer;

/// <summary>
/// �V���h�E�}�b�v�ւ̕`�揈��
/// </summary>
class ShadowMapRender
{
public:
    /// <summary>
    /// ������
    /// </summary>
    void Init();
    
    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    /// <param name="lightDirection">���C�g�̕���</param>
    void Render(
        RenderContext& rc, 
        int ligNo, 
        Vector3& lightDirection,
        std::vector< IRenderer* >& renderObjects
    );
    /// <summary>
    /// �V���h�E�}�b�v���擾
    /// </summary>
    /// <param name="areaNo">�G���A�ԍ�</param>
    /// <returns></returns>
    Texture& GetShadowMap(int areaNo)
    {
        return m_shadowMaps[areaNo].GetRenderTargetTexture();
    }
    /// <summary>
    /// ���C�g�r���[�v���W�F�N�V�����s����擾
    /// </summary>
    /// <returns></returns>
    const Matrix& GetLVPMatrix(int areaNo) const
    {
        return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
    }
private:
    CascadeShadowMapMatrix m_cascadeShadowMapMatrix;    //�J�X�P�[�h�V���h�E�}�b�v�̍s��������I�u�W�F�N�g
    RenderTarget m_shadowMaps[NUM_SHADOW_MAP];          //�V���h�E�}�b�v
    std::vector< IRenderer* > m_renderers;              //�V���h�E�}�b�v�ւ̃����_���[�̔z��B
};
   