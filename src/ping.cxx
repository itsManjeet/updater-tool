#include "server.hxx"
using namespace updater;

DBusHandlerResult Server::ping(DBusMessage* mesg, DBusMessage** reply) {
    const char* pong = "pong";
    if (!(*reply = dbus_message_new_method_return(mesg))) {
    }

    dbus_message_append_args(
        *reply,
        DBUS_TYPE_STRING, &pong,
        DBUS_TYPE_INVALID);
    return DBUS_HANDLER_RESULT_HANDLED;
}