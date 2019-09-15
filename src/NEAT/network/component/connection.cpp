#include "connection.hpp"

bool operator==(const Connection &a, const Connection &b)
{
	return a.in == b.in && a.out == b.out && a.enabled == b.enabled && a.innovation == b.innovation && a.weight == b.weight;
}

bool operator!=(const Connection &a, const Connection &b)
{
	return !(a == b);
}

std::ostream& operator<<(std::ostream &strm, const Connection &connection)
{
	return strm << "{ IN: " << connection.in->index << "; OUT: " << connection.out->index << "; Weight: "<< connection.weight << "; Enabled: " << (connection.enabled ? "true" : "false") << "; Innovation: " << connection.innovation << " }";
}