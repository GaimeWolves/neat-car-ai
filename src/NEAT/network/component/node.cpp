#include "node.hpp"

#include <string>

bool operator==(const Node& a, const Node& b)
{
	return a.index == b.index && a.name == b.name && a.type == b.type && a.value == b.value;
}

std::ostream& operator<<(std::ostream &strm, const Node &node)
{
	std::string type;
	switch (node.type)
	{
	case Sensor:
		type = "Sensor";
		break;
	case Hidden:
		type = "Hidden";
		break;
	case Output:
		type = "Output";
		break;
	
	default:
		type = "<Error type>";
		break;
	}

	if (node.name.length() != 0)
		return strm << "{ Index: " << node.index << "; Type: " << type << "; Name: " << node.name << " }";
	else
		return strm << "{ Index: " << node.index << "; Type: " << type << " }";
}