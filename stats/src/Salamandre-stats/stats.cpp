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
	orm::Cache<Node>::type_ptr node;
    std::list<orm::Cache<Node>::type_ptr> results;
    Node::query().filter(host, "exact", Node::_host).filter(port, "exact", Node::_port).get(results);
    if (results.size() > 0) {
		node = results.front();
	} else {
		node = orm::Cache<Node>::type_ptr(new Node());
		node->host = host;
		node->port = port;
	}
    node->last_seen_time = time(NULL);
    node->save();
}

void Stats::delete_node(std::string host, int port)
{
    Node node;
    Node::query().filter(host, "exact", Node::_host).filter(port, "exact", Node::_port).get(node);
    node.del();
}

std::list<std::shared_ptr<Node>> Stats::get_nodes()
{
    std::list<std::shared_ptr<Node>> list = Node::all();
    return list;
}
