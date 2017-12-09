
#ifndef _R2D_IMAGE_H_
#define _R2D_IMAGE_H_

#include "R2D_Object.h"


class R2D_Image : public R2D_Object
{
	protected:

		bool					_autoDeleteTexture;
		ITexture*				_texture;

		Vector2					_uvStart;
		Vector2					_uvSize;

	public:

		R2D_Image(bool autoDeleteTexture=false) : R2D_Object(R2D_IMG), _texture(0), _uvStart(Vector2::NullVector), _uvSize(Vector2::UnitVector),
			_autoDeleteTexture(autoDeleteTexture) {}
		R2D_Image(const Vector2& pos, const Vector2& size, const Color& c, const Vector2& uvStart, const Vector2& uvSize, ITexture *t, bool autoDeleteTexture) :
			R2D_Object(R2D_IMG,pos,size,c), _texture(t), _uvStart(uvStart), _uvSize(uvSize), _autoDeleteTexture(autoDeleteTexture)			 {}
		~R2D_Image()
		{
			if(_autoDeleteTexture && _texture)			delete _texture;
		}

		void					setTexture(ITexture* tex)				{ _texture=tex; }
		ITexture*				getTexture() const						{ return _texture; }

		void					setUVStart(const Vector2& uv)			{ _uvStart=uv; }
		void					setUVSize(const Vector2& uv)			{ _uvSize=uv; }

		const Vector2&			getUVStart() const						{ return _uvStart; }
		const Vector2&			getUVSize() const						{ return _uvSize; }
};


#endif