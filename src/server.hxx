#ifndef UPDATER_SERVER
#define UPDATER_SERVER

#include <dbus/dbus.h>

#include <map>
#include <string>

namespace updater {
    class Server {
       private:
        typedef DBusHandlerResult (Server::*Handler)(DBusMessage*, DBusMessage**);

        std::string _error;
        const std::string _app_id = "dev.rlxos.Updater";
        const std::string _object_path = "/dev/rlxos/Updater";

        const std::map<std::string, Handler> _callings;

       public:
        Server();
        ~Server();

        bool run();

        DBusHandlerResult handle(DBusMessage* mesg, DBusMessage** reply);

        std::string const& error() const { return _error; };
        std::string const& app_id() const { return _app_id; };
        std::string const& object_path() const { return _object_path; };

        DBusHandlerResult ping(DBusMessage* mesg, DBusMessage** reply);
        DBusHandlerResult quit(DBusMessage* mesg, DBusMessage** reply);

        DBusHandlerResult check(DBusMessage* mesg, DBusMessage** reply);
    };
}

#endif