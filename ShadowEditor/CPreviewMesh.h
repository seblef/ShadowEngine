
#pragma once

#include "G_PreviewCamera.h"
#include <GameLib.h>

// CModelPreview

class CPreviewMesh : public CStatic
{
	DECLARE_DYNAMIC(CPreviewMesh)

private:

	bool				_refreshing;

	bool				_dragging;
	int					_lastX, _lastY;

	Geometry*			_geo;
	Material*			_mat;
	G_PreviewCamera		_cam;

	IMultiVideoDevice*	_device;
	IView*				_view;
	IShader*			_backShader;
	IShader*			_shader;
	ITexture*			_backTexture;
	IConstantBuffer*	_cBuffer;
	BlendState			_backBlendState;
	DepthStencilState	_backDepthState;
	DepthStencilState	_depthState;

	IVertexBuffer*		_sphereVBuf;
	IIndexBuffer*		_sphereIBuf;

public:
	CPreviewMesh(Material* _mat, Geometry* geo, IMultiVideoDevice* device);
	virtual ~CPreviewMesh();

	void				setMaterial(Material *m)			{ _mat=m; }
	void				initDialog();
	void				update(float time);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint()									{ update(0); }
};


