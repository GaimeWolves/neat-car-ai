#include "connection.h"

std::ostream& operator<<(std::ostream &strm, const Connection &connection)
{
	return strm << "{ IN: " << connection.in.index << "; OUT: " << connection.out.index << "; Weight: "<< connection.weight << "; Enabled: " << (connection.enabled ? "true" : "false") << "; Innovation: " << connection.innovation << " }";
}