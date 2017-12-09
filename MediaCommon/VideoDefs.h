
#ifndef _VIDEODEFS_H_
#define _VIDEODEFS_H_


enum CullMode
{
	CULL_NONE=0,
	CULL_FRONT,
	CULL_BACK,

	CULL_COUNT
};

enum BlendMode
{
	BLEND_ZERO=0,
	BLEND_ONE,
	BLEND_SRCCOLOR,
	BLEND_INVSRCCOLOR,
	BLEND_SRCALPHA,
	BLEND_INVSRCALPHA,
	BLEND_DESTALPHA,
	BLEND_INVDESTALPHA,
	BLEND_DESTCOLOR,
	BLEND_INVDESTCOLOR,
	BLEND_FACTOR,

	BLEND_COUNT
};

enum CompareFunc
{
	COMP_NEVER=0,
	COMP_LESS,
	COMP_EQUAL2,
	COMP_LESSEQUAL,
	COMP_GREATER,
	COMP_GREATEREQUAL,
	COMP_ALWAYS,

	COMP_COUNT
};

enum StencilOp
{
	STENCIL_KEEP=0,
	STENCIL_REPLACE,
	STENCIL_INCRSAT,
	STENCIL_DECRSAT,
	STENCIL_INVERT,
	STENCIL_INCR,
	STENCIL_DECR,

	STENCIL_COUNT
};

enum FillMode
{
	FILL_SOLID=0,
	FILL_WIREFRAME,
	FILL_POINT,

	FILL_COUNT
};

enum FilterMode
{
	FILTER_POINT=0,
	FILTER_BILINEAR,
	FILTER_TRILINEAR,

	FILTER_COUNT
};

enum AddressMode
{
	ADDRESS_WRAP=0,
	ADDRESS_MIRROR,
	ADDRESS_CLAMP,
	ADDRESS_BORDER,
	ADDRESS_MIRROROUNCE,
	
	ADDRESS_COUNT
};

#define CLEAR_COLORBUFFER			1
#define CLEAR_ZBUFFER				2
#define CLEAR_STENCILBUFFER			4

enum PrimitiveType
{
	PT_POINTLIST=0,
	PT_LINELIST,
	PT_LINESTRIP,
	PT_TRIANGLELIST,
	PT_TRIANGLESTRIP,
	PT_TRIANGLEFAN,
	PT_3_CTRL_POINT_PATCHLIST,
	PT_4_CTRL_POINT_PATCHLIST,

	PT_COUNT
};

enum IndexType
{
	IDX_16BITS,
	IDX_32BITS,

	IDX_COUNT
};

enum BufferUsage
{
	BU_DEFAULT=0,
	BU_IMMUTABLE,
	BU_DYNAMIC,
	BU_STAGING,

	BU_COUNT
};

#define MAX_RENDER_TARGETS					4

enum ShaderType
{
	ST_VERTEX=0,
	ST_PIXEL,
	ST_GEOMETRY,
	ST_HULL,
	ST_DOMAIN,

	ST_COUNT
};

enum TextureFormat
{
	TEXF_A8R8G8B8=0,
	TEXF_X8R8G8B8,
	TEXF_R16F,
	TEXF_R32F,
	TEXF_R32G32F,
	TEXF_G16R16F,
	TEXF_A16R16G16B16F,
	TEXF_A32R32G32B32F,

	TEXF_COUNT
};


const char * const g_CullModeNames[]={
	"None",
	"Front",
	"Back"
};

inline CullMode getCullModeFromName(const string& cull)
{
	for(int i=0;i<CULL_COUNT;++i)
		if(cull==g_CullModeNames[i])
			return (CullMode)i;

	return CULL_NONE;
}

const char * const g_BlendModeNames[]={
	"Zero",
	"One",
	"Source Color",
	"Inv Source Color",
	"Source Alpha",
	"Inv Source Alpha",
	"Dest Alpha",
	"Inv Dest Alpha",
	"Dest Color",
	"Inv Dest Color",
	"Factor"
};

inline BlendMode getBlendModeFromName(const string& blend)
{
	for(int i=0;i<BLEND_COUNT;++i)
		if(blend==g_BlendModeNames[i])
			return (BlendMode)i;

	return BLEND_ONE;
}

const char * const g_CompareFunctionNames[]={
	"Never",
	"Less",
	"Equal",
	"Less Equal",
	"Greater",
	"Greater Equal",
	"Always"
};

inline CompareFunc getCompareFunctionFromName(const string& comp)
{
	for(int i=0;i<COMP_COUNT;++i)
		if(comp==g_CompareFunctionNames[i])
			return (CompareFunc)i;

	return COMP_ALWAYS;
}

const char * const g_StencilOpNames[]={
	"Keep",
	"Replace",
	"Increase Saturate",
	"Decrease Saturate",
	"Invert",
	"Increase",
	"Decrease"
};

inline StencilOp getStencilOpFromName(const string& op)
{
	for(int i=0;i<STENCIL_COUNT;++i)
		if(op==g_StencilOpNames[i])
			return (StencilOp)i;

	return STENCIL_KEEP;
}

const char * const g_FillModeNames[]={
	"Solid",
	"Wireframe",
	"Point"
};

inline FillMode getFillModeFromName(const string& fill)
{
	for(int i=0;i<FILL_COUNT;++i)
		if(fill==g_FillModeNames[i])
			return (FillMode)i;

	return FILL_SOLID;
}

const char * const g_FilterModeNames[]={
	"Point",
	"Bilinear",
	"Trilinear"
};

inline FilterMode getFilterModeFromName(const string& filter)
{
	for(int i=0;i<FILTER_COUNT;++i)
		if(filter==g_FilterModeNames[i])
			return (FilterMode)i;

	return FILTER_BILINEAR;
}

const char * const g_AddressModeNames[]={
	"Wrap",
	"Mirror",
	"Clamp",
	"Border",
	"Mirror Once"
};

inline AddressMode getAddressModeFromName(const string& add)
{
	for(int i=0;i<ADDRESS_COUNT;++i)
		if(add==g_AddressModeNames[i])
			return (AddressMode)i;

	return ADDRESS_WRAP;
}

const char * const g_PrimitiveTypeNames[]={
	"Point List",
	"Line List",
	"Line Strip",
	"Triangle List",
	"Triangle Strip",
	"Triangle Fan",
	"3 Control Point Patch List",
	"4 Control Point Patch List"
};

inline PrimitiveType getPrimitiveTypeFromName(const string& p)
{
	for(int i=0;i<PT_COUNT;++i)
		if(p==g_PrimitiveTypeNames[i])
			return (PrimitiveType)i;

	return PT_TRIANGLELIST;
}

#endif