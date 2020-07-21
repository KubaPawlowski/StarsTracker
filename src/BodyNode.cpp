#include "BodyNode.h"

BodyNode::BodyNode() 
{
	// Adding object to register at construction time
	ProgramMode::getInstance()->addToRegister(this);
}

void BodyNode::drawVisible()
{
	if (m_materialEnabled)
	{
		BodyTexture.bind();
		BodyMaterial.begin();
	}
	draw();
	if (m_materialEnabled)
	{
		BodyMaterial.end();
		BodyTexture.unbind();
	}
}

void BodyNode::drawForFbo()
{
	ofSetColor(getIndexColor());
	draw();
}

void BodyNode::setTexture(std::string path)
{
	m_TexturePath = path;
	if (!m_TexturePath.empty())
	{
		setMaterialEnabled(true);
		ofDisableArbTex();
		ofLoadImage(BodyTexture, m_TexturePath);
		if (properName == "Sun")
			BodyMaterial.setEmissiveColor({ 1,1,1,1 });
		else
			BodyMaterial.setEmissiveColor({ 0.1,0.1,0.1,1 });
		BodyMaterial.setAmbientColor({ 0.2,0.2,0.2,0.2 });

		BodyMaterial.setShininess(1);
		enableColors();
	}
}

void BodyNode::setMaterialEnabled(bool enabled)
{
	m_materialEnabled = enabled;
}