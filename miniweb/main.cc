//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hh"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace miniweb;

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("ipaddress", po::value<std::string>()->default_value("0.0.0.0"),
             "set the ip address")
        ("port", po::value<std::string>()->default_value("8000"),
             "set the port");

    po::variables_map config;
    po::store(po::parse_command_line(argc, argv, desc), config);
    po::notify(config);

    if (config.count("help")) {
        std::cerr << desc << "\n";
        return 1;
    }

    routes routes(&config);
    function_handler* osHandler = new function_handler([](const_req req)
		{ return "\"0.1\""; }, "json");

    routes.put(GET,"/os/version",osHandler);

    try {
        http::server::server s(&config, &routes);
        s.run();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
