/*!
 *@brief	�}�b�v�`�b�v
 */
#include "k2EnginePreCompile.h"
#include "MapChipRender.h"
#include "LevelRender.h"


MapChipRender::MapChipRender(const LevelObjectData& objData, const char* filePath)
{
	//���f����ǂݍ��ށB
	m_modelRender.Init(filePath);
	//�F�X�Z�b�g�B
	m_modelRender.SetPosition(objData.position);
	m_modelRender.SetScale(objData.scale);
	m_modelRender.SetRotation(objData.rotation);
	//���f�����X�V�B
	m_modelRender.Update();

	//�ÓI�����I�u�W�F�N�g���쐬�B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void MapChipRender::Draw(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

