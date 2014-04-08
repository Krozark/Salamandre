#include <Salamandre-stats/node.hpp>

// Define constructor
REGISTER_AND_CONSTRUCT(Node, "node", id, "id", host, "host", port, "port", last_seen_time, "last_seen_time");
