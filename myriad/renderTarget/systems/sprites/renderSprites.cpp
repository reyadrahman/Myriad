#include "renderSprites.h"
#include "renderer/renderer.h"

myr::RenderSprites::RenderSprites()
:RenderSystem(sizeof(SpriteAttributes))
{
	vaoBind();

	configureBufferAttribs();

	bufferBind();

	configureInstanceAttribs();

	vaoRelease();
}

void myr::RenderSprites::render(Shader *shader)
{
	RenderSystem::render(shader);
	
	glUniform1i(shader->getUniformLocation(UNIFORM_ATLAS), Renderer::TextureChannels::ATLAS);
	
	vaoBind();
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, (GLsizei)instanceCount);
	vaoRelease();	

	instanceCount = 0;
}

void myr::RenderSprites::configureBufferAttribs() const
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector), NULL);
}

void myr::RenderSprites::configureInstanceAttribs() const
{
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteAttributes),
		(GLvoid*)offsetof(SpriteAttributes, attributeAtlas));

	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteAttributes),
		(GLvoid*)offsetof(SpriteAttributes, attributeScaleRotate));

	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteAttributes),
		(GLvoid*)offsetof(SpriteAttributes, attributeTransform));
}