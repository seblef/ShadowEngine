
#pragma once

#include "G_PreviewCamera.h"
#include "G_EditorApp.h"
#include <ParticleLib.h>

// CModelPreview

class CPreviewParticles : public CStatic
{
	DECLARE_DYNAMIC(CPreviewParticles)

private:

	bool				_refreshing;

	bool				_dragging;
	int					_lastX, _lastY;

	ParticleSystem*		_system;
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
	CPreviewParticles(IMultiVideoDevice* device);
	virtual ~CPreviewParticles();

	void				setParticleSystem(ParticleSystem *ps)			{ _system=ps; }
	void				initDialog();
	void				update(float time);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


