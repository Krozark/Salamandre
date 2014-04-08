#include <Salamandre-stats/stats.hpp>

Stats::Stats()
{
}

void Stats::setup()
{
    orm::Bdd::Default.connect();
    Node::create(orm::Bdd::Default);
    orm::Bdd::Default.disconnect();
}

void Stats::add_node(std::string host, int port)
{
    Node new_node;
    new_node.host = host;
    new_node.port = port;
    new_node.last_seen_time = time(NULL);
    new_node.save();
}

std::list<std::shared_ptr<Node>> Stats::get_nodes()
{
    std::list<std::shared_ptr<Node>> list = Node::all();
    return list;
}
