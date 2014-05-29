#include <ORM/backends/Sqlite3.hpp>
orm::Sqlite3DB def("./salamandre.db");
orm::DB& orm::DB::Default = def;

#include <Salamandre-stats/stats.hpp>

using namespace salamandre::stats;

float Stats::default_robustesse = 99.9f;

void Stats::init()
{
    orm::DB::Default.connect();
    orm::Tables::create();
}

void Stats::close()
{
    orm::DB::Default.disconnect();
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

void Stats::flush_nodes()
{
    Node::clear();
}

std::list<std::shared_ptr<Node>> Stats::get_nodes(unsigned int number)
{
    std::list<std::shared_ptr<Node>> list;
    unsigned int one_day_ago = time(NULL) - 86400;
    Node::query()\
        .filter(one_day_ago, "gte", Node::_last_seen_time)
        .orderBy("?")
        //.orderBy("last_seen_time", '-')
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

bool Stats::load(const char * path)
{
    FILE * file = ::fopen(path, "r");
    if (file == NULL)
    {
        perror("Unable to load the node list:");
        return false;
    }

    const size_t size = 256;
    char line[size];
    std::string line_str;
    size_t semicolon_pos;

    std::string host;
    int port;

    while (::fgets(line, size, file) != NULL)
    {
        line_str = std::string(line);
        semicolon_pos = line_str.find(":");

        host = line_str.substr(0, semicolon_pos);
        port = ::atoi(line_str.substr(semicolon_pos + 1).c_str());

        Stats::add_node(host, port);
        std::cout << "Importing node : " << host << ":" << port << std::endl;
    }

    ::fclose(file);

    return true;
}

unsigned int Stats::get_duplication_number_for(float robustesse)
{
    //\todo TODO
    return 4;
}
