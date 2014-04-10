#include <Salamandre-stats/stats.hpp>

Stats::Stats()
{
}

void Stats::setup()
{
    orm::Bdd::Default.connect();
    Node::create(orm::Bdd::Default);
    //orm::Bdd::Default.disconnect();
}

void Stats::add_node(std::string host, int port)
{
    Node node;
    //std::list<Cache<Node>::type_ptr> results;
    Node::query().filter(Node::_host, "exact", host).filter(port, "exact", Node::_port).get(node);
    node.host = host;
    node.port = port;
    node.last_seen_time = time(NULL);
    node.save();
}

void Stats::delete_node(std::string host, int port)
{
    Node node;
    Node::query().filter(Node::_host, "exact", host).filter(port, "exact", Node::_port).get(node);
    node.del();
}

std::list<std::shared_ptr<Node>> Stats::get_nodes()
{
    std::list<std::shared_ptr<Node>> list = Node::all();
    return list;
}
