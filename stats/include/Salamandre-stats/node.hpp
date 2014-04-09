#ifndef NODE_HPP
#define NODE_HPP

#include <ORM/fields.hpp>
#include <ORM/models/SqlObject.hpp>

class Node : public orm::SqlObject<Node>
{
public:
    Node();
    orm::CharField<255> host;
    orm::IntegerField port;
    orm::IntegerField last_seen_time;

    MAKE_STATIC_COLUMN(host, port, last_seen_time);
};



#endif // NODE_HPP
