#include "renderTarget.h"
#include "../renderer/renderer.h"
#include "systems/renderTargets/renderTargetAttributes.h"

myr::RenderTarget::RenderTarget(const Color &clearColor, const Rect &rect, Renderer *renderer)
	:DefaultRenderTarget(clearColor, rect, renderer), Quad(rect, Vector(0, 0))
{
	createRenderSystems();

	generateFBO();
}

myr::RenderTarget::RenderTarget(const Color &clearColor, const Rect &rect)
	:RenderTarget(clearColor, rect, myr::Renderer::getCurrent())
{

}

myr::RenderTarget::~RenderTarget()
{
	deleteFBO();
}

void myr::RenderTarget::draw(
	const Transform &transform) const
{
	RenderTarget::getCurrent()->render(RENDER_SYSTEM_RENDER_TARGETS, &RenderTargetAttributes(
		SpriteAttributes(
			Atlas::Location(Vector(0, 1), Vector(1, -1)),
			getSize().toVector(),
			getOriginFactor(),
			transform),
		texture));
}

void myr::RenderTarget::draw(
	const Transform &transform,
	const Vector &scale) const
{
	RenderTarget::getCurrent()->render(RENDER_SYSTEM_RENDER_TARGETS, &RenderTargetAttributes(
		SpriteAttributes(
			Atlas::Location(Vector(0, 1), Vector(1, -1)),
			getSize().toVector() * scale,
			getOriginFactor(),
			transform),
		texture));
}

void myr::RenderTarget::draw(
	const int x,
	const int y) const
{
	RenderTarget::getCurrent()->render(RENDER_SYSTEM_RENDER_TARGETS, &RenderTargetAttributes(
		SpriteAttributes(
			Atlas::Location(Vector(0, 1), Vector(1, -1)),
			Vector(x, y),
			getSize().toVector(),
			getOriginFactor()),
		texture));
}

void myr::RenderTarget::draw(
	const int x,
	const int y,
	const Vector &scale) const
{
	RenderTarget::getCurrent()->render(RENDER_SYSTEM_RENDER_TARGETS, &RenderTargetAttributes(
		SpriteAttributes(
			Atlas::Location(Vector(0, 1), Vector(1, -1)),
			Vector(x, y),
			getSize().toVector() * scale,
			getOriginFactor()),
		texture));
}

void myr::RenderTarget::draw(
	const int x,
	const int y,
	const Vector &scale,
	const float angle) const
{
	RenderTarget::getCurrent()->render(RENDER_SYSTEM_RENDER_TARGETS, &RenderTargetAttributes(
		SpriteAttributes(
			Atlas::Location(Vector(0, 1), Vector(1, -1)),
			Vector(x, y),
			getSize().toVector() * scale,
			getOriginFactor(),
			angle),
		texture));
}

void myr::RenderTarget::setSize(const Rect &size)
{
	Quad::setSize(size);

	resize();
}

void myr::RenderTarget::generateFBO()
{
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &texture);

	resize();

	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glDrawBuffers(1, drawBuffers);
}

void myr::RenderTarget::deleteFBO()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);
}

myr::Rect myr::RenderTarget::getResolution() const
{
	return getSize();
}

void myr::RenderTarget::resize()
{
	glActiveTexture(GL_TEXTURE0 + Renderer::TextureChannels::INTERMEDIATE);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		getSize().getWidth(),
		getSize().getHeight(),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}