
#ifndef _TRENDERER_H_
#define _TRENDERER_H_

#include <MediaCommon.h>
#include <algorithm>
#include "Material.h"

template<typename T>
class TRenderer
{
protected:

	IVideoDevice*				_device;

	IShader*					_gbufferShader;
	IShader*					_addShader;
	IShader*					_shadowShader;

	typedef vector<T*>			InstanceVector;
	InstanceVector				_instances;

	static bool					materialSort(const T* i1, const T* i2)
	{
		return i1->getModel()->getMaterial() < i2->getModel()->getMaterial();
	}

	static bool					modelSort(const T* i1, const T* i2)
	{
		return i1->getModel() < i2->getModel();
	}

public:

	TRenderer(IVideoDevice* device, const string& gbufferShader, const string& addShader, const string& shadowShader)
		: _device(device)
	{
		_gbufferShader=gbufferShader.empty() ? 0 : device->createShader(gbufferShader);
		_addShader=addShader.empty() ? 0 : device->createShader(addShader);
		_shadowShader=shadowShader.empty() ? 0 : device->createShader(shadowShader);
	}
	virtual ~TRenderer()
	{
		if(_gbufferShader)		delete _gbufferShader;
		if(_addShader)			delete _addShader;
		if(_shadowShader)		delete _shadowShader;
	}

	void						addInstance(T* i)					{ _instances.push_back(i); }

	void						prepareRender()
	{
		sort(_instances.begin(),_instances.end(),materialSort);
		InstanceVector::iterator ibegin,iend,i;
		i=ibegin=_instances.begin();
		while(i!=_instances.end())
		{
			Material* m=(*ibegin)->getModel()->getMaterial();
			while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m)
				++i;

			sort(ibegin,i,modelSort);
			ibegin=i;
		}
	}

	void						endRender()							{ _instances.clear(); }

	virtual void				render(bool addPass)=0;
	virtual void				renderShadows(const ViewFrustum& vf)=0;

};



#endif