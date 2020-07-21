#include "Node.h"

Node::Node()
{
	m_indexColor.r = 0;
	m_indexColor.g = 0;
	m_indexColor.b = 0;
}

ofColor Node::getIndexColor()
{
	return m_indexColor;
}

void Node::setIndexColor(ofColor indexColor)
{
	m_indexColor = indexColor;
}

void Node::setRadius(float radius)
{
	m_radius = radius;
}
float Node::getRadius()
{
	return m_radius;
}

void Node::setParentNode(Node* node)
{
	m_parentNode = node;
}

Node* Node::getParentNode()
{
	return m_parentNode;
}
