#include <Salamandre-stats/node.hpp>
// Define constructor

namespace salamandre {
    namespace stats {

    // Replaced macro result by code because of namespace resolution errors
    //REGISTER_AND_CONSTRUCT(Node, "node", host, "host", port, "port", last_seen_time, "last_seen_time");

    template<> const std::string orm::SqlObject<Node>::table = "node";
    template<> orm::Bdd* orm::SqlObject<Node>::default_connection = &orm::Bdd::Default;
    template<> std::vector<const orm::VAttr*> orm::SqlObject<Node>::column_attrs = std::vector<const orm::VAttr*>();
    template<> std::vector<orm::VFK*> orm::SqlObject<Node>::column_fks = std::vector<orm::VFK*>();
    template<> orm::Register<Node> orm::SqlObject<Node>::_register = orm::Register<salamandre::stats::Node>();
    template<> orm::Cache<Node> orm::SqlObject<Node>::cache = orm::Cache<salamandre::stats::Node>();


    const std::string Node::_host = "host";
    const std::string Node::_port = "port";
    const std::string Node::_last_seen_time = "last_seen_time";

    //REGISTER_TABLE(Node, "node");


    Node::Node():host (_host), port(_port), last_seen_time(_last_seen_time) {
        host.registerAttr(*this);
        port.registerAttr(*this);
        last_seen_time.registerAttr(*this);
    }
    }

}
