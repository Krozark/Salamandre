#include <Salamandre-stats/stats.hpp>

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

std::list<std::shared_ptr<Node>> Stats::get_nodes(unsigned int number)
{
    std::list<std::shared_ptr<Node>> list;
    Node::query()\
        //.filter(...)
        //.orderBy(...)
        .limit(number)\
        .get(list);

    const unsigned int _size = list.size();
    if(_size > 0)
    {
        //ajout du nombre de node qu'il manque
        unsigned int diff = number - _size;
        std::list<std::shared_ptr<Node>>::iterator current = list.begin();
        while(diff > 0)
        {
            list.push_back(*current);
            ++current;
            --diff;
        }
    }
    return list;
}

unsigned int Stats::get_duplication_number_for(float robustesse)
{
    //\todo TODO
    return 4;
}

std::list<std::shared_ptr<Node>> Stats::get_nodes()
{
    std::list<std::shared_ptr<Node>> list = Node::all();
    return list;
}
