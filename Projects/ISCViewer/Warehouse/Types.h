
#ifndef Warehouse_Types_h
#define Warehouse_Types_h


// Library includes
#include <string>

// Project includes

// Forward declarations
namespace Core {
	class RenderObject;
}

// Namespace declarations


namespace WarehouseSpace {


class VertexType
{
public:
	enum E {
		Crossing = 10,
		Flank = 20,
		Place = 50,
		Box = 60,
		Parking = 80
	};
};


class Vertex
{
public:
	int	LevelId;
	int	PosId;
	Core::RenderObject *RenderObject;
	VertexType::E	Type;
};


class Edge
{
public:
	int	Alignment;
	int Angle;
	const Vertex *Back;
	int DestLevelId;
	int DestPosId;
	const Vertex *Front;
	int Length;
	int SourceLevelId;
	int SourcePosId;
};


class Version
{
public:
	std::string Name;
	int Major;
	int Minor;
};


}


#endif
