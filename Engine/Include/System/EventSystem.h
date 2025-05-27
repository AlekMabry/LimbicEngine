#pragma once

#include <LimbicTypes.h>
//#include <eventpp/eventqueue.h>

//namespace Event = eventpp;

enum EEvent
{
	eResourceSystemRequestStaticMesh,
	eResourceSystemRequestTexture,
	eResourceSystemRequestMaterial,
};

class EventSystem
{
public:

protected:
	//Event::EventQueue<EEvent, void()> queue;
};
