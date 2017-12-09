
#ifndef _SEDITOROBJDRAWER_H_
#define _SEDITOROBJDRAWER_H_

#include <DX11MediaMulti.h>

#define MAX_DRAWER_LINES			256

class G_EditorObjDrawer
{
	protected:

		IVideoDevice*		_device;
		IShader*			_shader;
		IVertexBuffer*		_vBuffer;
		VxBase				_vertices[MAX_DRAWER_LINES*2];		
		int					_vCount;
		Color				_color;
		RenderState			_renderState;
		DepthStencilState	_depthState;
		DepthStencilState	_depthOverState;
		BlendState			_blendState;

		void				execute();

	public:

		G_EditorObjDrawer(IVideoDevice *d);
		~G_EditorObjDrawer();

		void				begin(bool noZBuffer=false);
		void				end()											{ execute(); }

		void				setWorldMatrix(const Matrix4& world);
		void				setColor(const Color& color)					{ _color=color; }

		void				drawLine(const Vector3& v1, const Vector3& v2);
		void				drawDot(const Vector3& v, float fSize);
		void				drawCircle(const Vector3& vCenter, const Vector3& vAxe, float fSize);
		void				drawBox(const Vector3& v1, const Vector3& v2);
		void				drawRect(const Vector3& v1, const Vector3& v2,
			const Vector3& v3, const Vector3& v4);
};

#endif