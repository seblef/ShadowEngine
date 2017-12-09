
#ifndef _VIDEOCOMMON_H_
#define _VIDEOCOMMON_H_

#ifdef _DXMULTI

#define VDEVICE				MVideoDevice
#define VDEVICE_INC			"../DX11MediaMulti/MVideoDevice.h"

#else

#define VDEVICE				VideoDevice
#define VDEVICE_INC			"VideoDevice.h"

#endif

#endif