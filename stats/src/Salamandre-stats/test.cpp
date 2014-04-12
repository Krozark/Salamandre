#include <ORM/backends/Sqlite3.hpp>
orm::Sqlite3Bdd def("./datas/test.db");

orm::Bdd& orm::Bdd::Default = def;

#include <ORM/fields.hpp>
#include <ORM/models/SqlObject.hpp>

#include <Salamandre-stats/stats.hpp>

#include <iostream>

int main()
{
    Stats stats_module = Stats();
    stats_module.setup();
    //orm::Tables::create();
    stats_module.add_node(std::string("test"), 1234);

	auto results = stats_module.get_nodes();

	for(auto u : results)
		std::cout<<*u<<std::endl;

    orm::Bdd::Default.disconnect();
    return 0;
}
