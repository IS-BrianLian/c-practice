#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <libubox/ustream.h>
#include <libubox/blobmsg_json.h>
#include "libubus.h"

static struct ubus_context *ctx_listen = NULL;
static struct ubus_event_handler listener;

#define UBUS_LISTEN_OBJ_STR "network.interface"

static void receive_event(struct ubus_context *ctx, struct ubus_event_handler *ev,
    const char *type, struct blob_attr *msg)
{
    char *str;

    str = blobmsg_format_json(msg, true);
    printf("{ \"%s\": %s }\n", type, str);
    free(str);
}

static void listen_main(void) {
    const char *event_listen = NULL;
    int ret = 0;

    memset(&listener, 0, sizeof(listener));
    listener.cb = receive_event;
    event_listen = UBUS_LISTEN_OBJ_STR;
    ret = ubus_register_event_handler(ctx_listen, &listener, event_listen);
    if (ret) {
        fprintf(stderr, "Error while registering for event '%s': %s\n",
            event_listen, ubus_strerror(ret));
        return;
    }

    uloop_init();
    ubus_add_uloop(ctx_listen);
    uloop_run();
    uloop_done();
}

int main(int argc, char **argv) {
    const char *ubus_socket_listen = NULL;

    ctx_listen = ubus_connect(ubus_socket_listen);
    if (!ctx_listen) {
        fprintf(stderr, "Failed to connect to ubus\n");
        return -1;
    }

    listen_main();
    ubus_free(ctx_listen);

    return 0;
}
