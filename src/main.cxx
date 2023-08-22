#include <iostream>

#include "server.hxx"
using std::cerr;
using std::cout;
using std::endl;

#include <gio/gio.h>

int main(int argc, char** argv) {
    updater::Server server;
    cout << "running " << server.app_id() << " / " << server.object_path() << endl;
    if (!server.run()) {
        cerr << "ERROR: " << server.error() << endl;
        return 1;
    }
    return 0;
}