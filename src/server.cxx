#include "server.hxx"
using namespace updater;

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus.h>
#include <glib.h>

auto const SERVER_INTROSPECTION_XML = DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE R"(
    <node>
    
    <interface name='org.freedesktop.DBus.Introspectable'>
        <method name='Introspect'>
            <arg name='data' type='s' direction='out' />
        </method>
    </interface>
)";

GMainLoop* loop;

Server::Server()
    : _callings{
          {"ping", &Server::ping},
          {"check", &Server::check},
          {"quit", &Server::quit},
      } {
}

Server::~Server() {
}

bool Server::run() {
    DBusConnection* conn;
    DBusError err;

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (conn == nullptr) {
        _error = err.message;
        dbus_error_free(&err);
        return false;
    }

    int status = dbus_bus_request_name(conn, _app_id.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (status != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        _error = err.message;
        dbus_error_free(&err);
        return false;
    }

    const DBusObjectPathVTable server_vtable = {
        .message_function = +[](DBusConnection* conn, DBusMessage* mesg, void* data) -> DBusHandlerResult {
            Server* server = reinterpret_cast<Server*>(data);
            DBusMessage* reply = nullptr;
            auto result = server->handle(mesg, &reply);
            if (result == DBUS_HANDLER_RESULT_NOT_YET_HANDLED) {
                return result;
            }

            if (!dbus_connection_send(conn, reply, nullptr)) {
                result = DBUS_HANDLER_RESULT_NEED_MEMORY;
            }
            dbus_message_unref(reply);
            return result;
        },
    };

    if (!dbus_connection_register_object_path(conn, _object_path.c_str(), &server_vtable, this)) {
        _error = "failed to register object '" + _object_path + "' path for '" + _app_id + "'";
        return false;
    }

    loop = g_main_loop_new(nullptr, false);
    dbus_connection_setup_with_g_main(conn, nullptr);
    g_main_loop_run(loop);

    return true;
}

DBusHandlerResult Server::handle(DBusMessage* mesg, DBusMessage** reply) {
    for (auto const& i : _callings) {
        if (dbus_message_is_method_call(mesg, _app_id.c_str(), i.first.c_str())) {
            try {
                return (*this.*i.second)(mesg, reply);
            } catch (std::exception const& excep) {
                if (*reply)
                    dbus_message_unref(*reply);
                *reply = dbus_message_new_error(mesg, "", excep.what());
                if (*reply == nullptr)
                    return DBUS_HANDLER_RESULT_NEED_MEMORY;
            }
        }
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

DBusHandlerResult Server::quit(DBusMessage* mesg, DBusMessage** reply) {
    *reply = dbus_message_new_method_return(mesg);
    g_main_loop_quit(loop);
    return DBUS_HANDLER_RESULT_HANDLED;
}