#include "sprite.h"
#include "renderer/renderer.h"
#include "renderTarget/renderTarget.h"
#include "renderTarget/systems/sprites/spriteAttributes.h"

myr::Sprite::Sprite(
	const std::string name,
	SpriteDecoder *decoder,
	const int originX,
	const int originY)
:flags(0), name(name), decoder(decoder), originX(originX), originY(originY), origin(0, 0) {}

myr::Sprite::~Sprite()
{
	if(flags & LOADED)
		myr::RenderTarget::getCurrent()->getRenderer()->getAtlas().release(name);
}

void myr::Sprite::draw(
	const int x,
	const int y,
	const Vector scale)
{
	if(!(flags & LOADED))
		load();

	SpriteAttributes attributes(
		location.location,
		location.size,
		Vector(x, y),
		Vector(width * scale.x, height * scale.y),
		origin);

	RenderTarget::getCurrent()->render(RENDER_SYSTEM_SPRITES, &attributes);
}

void myr::Sprite::draw(
	const int x,
	const int y,
	const Vector scale,
	const float angle)
{
	if(!(flags & LOADED))
		load();
	
	SpriteAttributes attributes(
		location.location,
		location.size,
		Vector(x, y),
		Vector(width * scale.x, height * scale.y),
		origin,
		angle);

	RenderTarget::getCurrent()->render(RENDER_SYSTEM_SPRITES, &attributes);
}

void myr::Sprite::setOrigin(const int originX, const int originY)
{
	this->originX = originX;
	this->originY = originY;
}

short int myr::Sprite::getWidth() const
{
	return width;
}

short int myr::Sprite::getHeight() const
{
	return height;
}

int myr::Sprite::getOriginX() const
{
	return originX;
}

int myr::Sprite::getOriginY() const
{
	return originY;
}

void myr::Sprite::load()
{
	flags |= LOADED;
	
	width = decoder->getWidth();
	height = decoder->getHeight();
	origin = Vector(float(originX) / width, float(originY) / height);
	location = myr::RenderTarget::getCurrent()->getRenderer()->getAtlas().query(
		name,
		decoder->getWidth(),
		decoder->getHeight(),
		decoder->getPixels());
}