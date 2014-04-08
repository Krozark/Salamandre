#ifndef NODE_HPP
#define NODE_HPP

#include <ORM/backends/Sqlite3.hpp>
orm::Sqlite3Bdd def("./datas/bdd.db");

orm::Bdd& orm::Bdd::Default = def;

#include <ORM/fields.hpp>
#include <ORM/models/SqlObject.hpp>

class Node : public orm::SqlObject<Node>
{
public:
    Node();
    orm::IntegerField id;
    orm::CharField<255> host;
    orm::IntegerField port;
    orm::IntegerField last_seen_time;

    MAKE_STATIC_COLUMN(id, host, port, last_seen_time);
};

#endif // NODE_HPP
