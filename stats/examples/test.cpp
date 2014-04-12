#include <ORM/backends/Sqlite3.hpp>
orm::Sqlite3Bdd def("./datas/test.db");

orm::Bdd& orm::Bdd::Default = def;

#include <Salamandre-stats/stats.hpp>

//#include <iostream>

int main()
{
    orm::Bdd::Default.connect();
    orm::Tables::create();

    Stats stats_module = Stats();
    
    stats_module.add_node(std::string("test"), 1234);

	auto results = stats_module.get_nodes();

	for(auto u : results)
		std::cout<<*u<<std::endl;

    orm::Bdd::Default.disconnect();
    return 0;
}
