
#ifndef _Interfaces_Common_Types_h_
#define _Interfaces_Common_Types_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


#ifdef _WIN32
//typedef _W64 int INT_PTR, *PINT_PTR;
//typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;
//typedef _W64 long LONG_PTR, *PLONG_PTR;

//typedef UINT_PTR            WPARAM;
//typedef LONG_PTR            LPARAM;
//typedef LONG_PTR            LRESULT;
#endif

typedef	signed		__int8		sint8;
typedef	unsigned	__int8		uint8;
typedef	signed		__int16		sint16;
typedef	unsigned	__int16		uint16;
typedef	signed		__int32		sint32;
typedef	unsigned	__int32		uint32;
typedef	signed		__int64		sint64;
typedef	unsigned	__int64		uint64;

typedef			int			sint;			// at least 32bits (depend on processor)
typedef	unsigned	int			uint;			// at least 32bits (depend on processor)



typedef unsigned int	AEbitfield;
typedef unsigned char	AEboolean;
typedef signed char	AEbyte;
typedef double		AEclampd;
typedef float		AEclampf;
typedef double		AEdouble;
typedef unsigned int	AEenum;
typedef float		AEfloat;
typedef int		AEint;
typedef short		AEshort;
typedef int		AEsizei;
typedef unsigned char	AEubyte;
typedef unsigned int	AEuint;
typedef unsigned short	AEushort;
typedef void		AEvoid;



namespace Common {


}


#endif

