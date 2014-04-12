#include <ORM/backends/Sqlite3.hpp>
orm::Sqlite3Bdd def("./datas/test.db");

orm::Bdd& orm::Bdd::Default = def;

#include <Salamandre-stats/stats.hpp>

//#include <iostream>

int main()
{
    orm::Bdd::Default.connect();
    orm::Tables::create();

    Stats::add_node(std::string("test"), 1234);

    for(auto& u : Stats::get_nodes(12))
        std::cout<<*u<<std::endl;

	auto results = stats_module.get_nodes();

	for(auto u : results)
		std::cout<<*u<<std::endl;

    orm::Bdd::Default.disconnect();
    return 0;
}
